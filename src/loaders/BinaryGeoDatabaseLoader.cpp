#include "loaders/BinaryGeoDatabaseLoader.h"

#include <fstream>
#include <iostream>

namespace geolocation::loaders {

using common::GeoRecord;

std::vector<GeoRecord> BinaryGeoDatabaseLoader::load(const std::string& path) const {
    std::vector<GeoRecord> records;
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open .geo file: " << path << '\n';
        return records;
    }

    while (ifs.peek() != EOF) {
        uint32_t startIp, endIp;
        uint16_t countryLen, cityLen;

        ifs.read(reinterpret_cast<char*>(&startIp), sizeof(startIp));
        ifs.read(reinterpret_cast<char*>(&endIp), sizeof(endIp));
        ifs.read(reinterpret_cast<char*>(&countryLen), sizeof(countryLen));
        ifs.read(reinterpret_cast<char*>(&cityLen), sizeof(cityLen));

        std::string countryCode(countryLen, '\0');
        std::string city(cityLen, '\0');

        ifs.read(countryCode.data(), countryLen);
        ifs.read(city.data(), cityLen);

        records.push_back({startIp, endIp, std::move(countryCode), std::move(city)});
    }

    return records;
}

} // namespace geolocation::loaders
