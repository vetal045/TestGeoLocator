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

namespace {

// Platform-specific RAII for file handles
#if defined(_WIN32)
struct WinHandle {
    HANDLE handle = INVALID_HANDLE_VALUE;
    WinHandle() = default;
    explicit WinHandle(HANDLE h) : handle(h) {}
    ~WinHandle() {
        if (handle != INVALID_HANDLE_VALUE && handle != nullptr) {
            CloseHandle(handle);
        }
    }
    operator HANDLE() const { return handle; }
};
#else
struct FdHandle {
    int fd = -1;
    FdHandle() = default;
    explicit FdHandle(int f) : fd(f) {}
    ~FdHandle() {
        if (fd != -1) {
            close(fd);
        }
    }
    operator int() const { return fd; }
};
#endif

bool readUint32(const char*& ptr, const char* end, uint32_t& out) {
    if (end - ptr < static_cast<ptrdiff_t>(sizeof(uint32_t)))
        return false;
    out = *reinterpret_cast<const uint32_t*>(ptr);
    ptr += sizeof(uint32_t);
    return true;
}

bool readUint16(const char*& ptr, const char* end, uint16_t& out) {
    if (end - ptr < static_cast<ptrdiff_t>(sizeof(uint16_t)))
        return false;
    out = *reinterpret_cast<const uint16_t*>(ptr);
    ptr += sizeof(uint16_t);
    return true;
}

bool readString(const char*& ptr, const char* end, size_t len, std::string& out) {
    if (end - ptr < static_cast<ptrdiff_t>(len))
        return false;
    out.assign(ptr, len);
    ptr += len;
    return true;
}

} // unnamed namespace

#if defined(_WIN32)
std::vector<GeoRecord> BinaryGeoDatabaseLoader::loadWindows(const std::string& path) const {
    std::vector<GeoRecord> records;

    WinHandle hFile(CreateFileA(
        path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    ));

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open file: " << path << '\n';
        return records;
    }

    WinHandle hMapping(CreateFileMappingA(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr));
    if (hMapping == nullptr) {
        std::cerr << "Failed to create file mapping.\n";
        return records;
    }

    const char* mapped = static_cast<const char*>(MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0));
    if (!mapped) {
        std::cerr << "Failed to map view of file.\n";
        return records;
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size)) {
        std::cerr << "Failed to get file size.\n";
        UnmapViewOfFile(mapped);
        return records;
    }

    const char* ptr = mapped;
    const char* end = ptr + size.QuadPart;

    if (!parseRecords(ptr, end, records)) {
        std::cerr << "Failed to parse records.\n";
    }

    UnmapViewOfFile(mapped);
    return records;
}
#else
std::vector<GeoRecord> BinaryGeoDatabaseLoader::loadUnix(const std::string& path) const {
    std::vector<GeoRecord> records;

    FdHandle fd(open(path.c_str(), O_RDONLY));
    if (fd == -1) {
        std::cerr << "Failed to open file: " << path << '\n';
        return records;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cerr << "Failed to stat file: " << path << '\n';
        return records;
    }

    size_t fileSize = sb.st_size;
    const char* mapped = static_cast<const char*>(mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0));
    if (mapped == MAP_FAILED) {
        std::cerr << "Failed to mmap file: " << path << '\n';
        return records;
    }

    const char* ptr = mapped;
    const char* end = ptr + fileSize;

    if (!parseRecords(ptr, end, records)) {
        std::cerr << "Failed to parse records.\n";
    }

    munmap(const_cast<char*>(mapped), fileSize);
    return records;
}
#endif

bool BinaryGeoDatabaseLoader::parseRecords(const char*& ptr, const char* end, std::vector<GeoRecord>& records) const {
    uint32_t numRecords = 0;
    if (!readUint32(ptr, end, numRecords)) {
        std::cerr << "Invalid or corrupted .geo header.\n";
        return false;
    }

    records.reserve(numRecords);

    for (uint32_t i = 0; i < numRecords; ++i) {
        uint32_t startIp, endIp;
        uint16_t countryLen, cityLen;
        std::string countryCode, city;

        if (!readUint32(ptr, end, startIp) ||
            !readUint32(ptr, end, endIp) ||
            !readUint16(ptr, end, countryLen) ||
            !readUint16(ptr, end, cityLen) ||
            !readString(ptr, end, countryLen, countryCode) ||
            !readString(ptr, end, cityLen, city)) {
            return false;
        }

        records.emplace_back(GeoRecord{startIp, endIp, std::move(countryCode), std::move(city)});
    }

    return true;
}

std::vector<GeoRecord> BinaryGeoDatabaseLoader::load(const std::string& path) const {
#if defined(_WIN32)
    return loadWindows(path);
#else
    return loadUnix(path);
#endif
}

} // namespace geolocation::loaders
