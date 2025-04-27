#include "loaders/BinaryGeoDatabaseLoader.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <vector>

namespace geolocation::loaders {

using common::GeoRecord;

std::vector<GeoRecord> BinaryGeoDatabaseLoader::load(const std::string& path) const {
    std::vector<GeoRecord> records;

    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open .geo file: " << path << '\n';
        return records;
    }

    struct stat sb{};
    if (fstat(fd, &sb) == -1) {
        std::cerr << "Failed to get file size: " << path << '\n';
        close(fd);
        return records;
    }

    const size_t fileSize = sb.st_size;
    if (fileSize == 0) {
        close(fd);
        return records;
    }

    void* mapped = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        std::cerr << "Failed to mmap file: " << path << '\n';
        close(fd);
        return records;
    }

    const char* ptr = static_cast<const char*>(mapped);
    const char* end = ptr + fileSize;

    while (ptr < end) {
        if (static_cast<size_t>(end - ptr) < sizeof(uint32_t) * 2 + sizeof(uint16_t) * 2) {
            std::cerr << "Corrupted .geo file: header too small\n";
            break;
        }

        uint32_t startIp = *reinterpret_cast<const uint32_t*>(ptr); ptr += sizeof(uint32_t);
        uint32_t endIp = *reinterpret_cast<const uint32_t*>(ptr); ptr += sizeof(uint32_t);
        uint16_t countryLen = *reinterpret_cast<const uint16_t*>(ptr); ptr += sizeof(uint16_t);
        uint16_t cityLen = *reinterpret_cast<const uint16_t*>(ptr); ptr += sizeof(uint16_t);

        if (static_cast<size_t>(end - ptr) < countryLen + cityLen) {
            std::cerr << "Corrupted .geo file: invalid strings length\n";
            break;
        }

        std::string countryCode(ptr, countryLen); ptr += countryLen;
        std::string city(ptr, cityLen); ptr += cityLen;

        records.emplace_back(GeoRecord{startIp, endIp, std::move(countryCode), std::move(city)});
    }

    munmap(mapped, fileSize);
    close(fd);

    return records;
}

} // namespace geolocation::loaders
