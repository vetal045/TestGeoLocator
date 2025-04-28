#include "BinaryGeoDatabaseLoader.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace geolocation::loaders {

using common::GeoRecord;

std::vector<GeoRecord> BinaryGeoDatabaseLoader::load(const std::string& path) const {
    std::vector<GeoRecord> records;

#if defined(_WIN32)
    HANDLE hFile = CreateFileA(
        path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return records;
    }

    HANDLE hMapping = CreateFileMappingA(
        hFile,
        nullptr,
        PAGE_READONLY,
        0,
        0,
        nullptr
    );

    if (hMapping == nullptr) {
        std::cerr << "Failed to create file mapping." << std::endl;
        CloseHandle(hFile);
        return records;
    }

    void* mapped = MapViewOfFile(
        hMapping,
        FILE_MAP_READ,
        0,
        0,
        0
    );

    if (mapped == nullptr) {
        std::cerr << "Failed to map view of file." << std::endl;
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return records;
    }

    LARGE_INTEGER size;
    GetFileSizeEx(hFile, &size);
    const char* ptr = static_cast<const char*>(mapped);
    const char* end = ptr + size.QuadPart;

#else
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open file: " << path << '\n';
        return records;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cerr << "Failed to get file size: " << path << '\n';
        close(fd);
        return records;
    }

    size_t fileSize = sb.st_size;
    void* mapped = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        std::cerr << "Failed to mmap file: " << path << '\n';
        close(fd);
        return records;
    }

    const char* ptr = static_cast<const char*>(mapped);
    const char* end = ptr + fileSize;
#endif

    while (ptr < end) {
        if (end - ptr < static_cast<ptrdiff_t>(sizeof(uint32_t) * 2 + sizeof(uint16_t) * 2)) {
            std::cerr << "Corrupted .geo file: not enough data\n";
            break;
        }

        uint32_t startIp = *reinterpret_cast<const uint32_t*>(ptr);
        ptr += sizeof(uint32_t);

        uint32_t endIp = *reinterpret_cast<const uint32_t*>(ptr);
        ptr += sizeof(uint32_t);

        uint16_t countryLen = *reinterpret_cast<const uint16_t*>(ptr);
        ptr += sizeof(uint16_t);

        uint16_t cityLen = *reinterpret_cast<const uint16_t*>(ptr);
        ptr += sizeof(uint16_t);

        if (ptr + countryLen + cityLen > end) {
            std::cerr << "Corrupted .geo file: invalid string lengths\n";
            break;
        }

        std::string countryCode(ptr, countryLen);
        ptr += countryLen;

        std::string city(ptr, cityLen);
        ptr += cityLen;

        records.emplace_back(GeoRecord{startIp, endIp, std::move(countryCode), std::move(city)});
    }

#if defined(_WIN32)
    UnmapViewOfFile(mapped);
    CloseHandle(hMapping);
    CloseHandle(hFile);
#else
    munmap(const_cast<char*>(ptr), fileSize);
    close(fd);
#endif

    return records;
}

} // namespace geolocation::loaders
