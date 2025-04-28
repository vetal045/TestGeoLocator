#include "GeoPreprocessor.h"
#include "GeoRecord.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

namespace geolocation::preprocessor {

int RunGeoPreprocessing(
    const std::string& inputPath,
    const std::string& outputPath,
    const IGeoPreprocessorSource& source
) {
    std::vector<geolocation::common::GeoRecord> records = source.load(inputPath);

    // Sorting by start IP ensures that future binary search over the file is valid and fast
    std::sort(records.begin(), records.end(), [](const geolocation::common::GeoRecord& a, const geolocation::common::GeoRecord& b) {
        return a.startIp < b.startIp;
    });

    std::ofstream ofs(outputPath, std::ios::binary);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open output file: " << outputPath << '\n';
        return EXIT_FAILURE;
    }

    // Write number of record as header
    uint32_t numRecords = static_cast<uint32_t>(records.size());
    ofs.write(reinterpret_cast<const char*>(&numRecords), sizeof(numRecords));

    for (const auto& record : records) {
        // Write IP range (start and end) first
        ofs.write(reinterpret_cast<const char*>(&record.startIp), sizeof(record.startIp));
        ofs.write(reinterpret_cast<const char*>(&record.endIp), sizeof(record.endIp));

        uint16_t countryLen = static_cast<uint16_t>(record.countryCode.size());
        uint16_t cityLen = static_cast<uint16_t>(record.city.size());

        ofs.write(reinterpret_cast<const char*>(&countryLen), sizeof(countryLen));
        ofs.write(reinterpret_cast<const char*>(&cityLen), sizeof(cityLen));

        ofs.write(record.countryCode.data(), countryLen);
        ofs.write(record.city.data(), cityLen);
    }

    return EXIT_SUCCESS;
}

void DebugReadGeoFile(const std::string& filePath) {
    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open the binary file: " << filePath << "\n";
        return;
    }

    uint32_t startIp, endIp;
    uint16_t countryLen, cityLen;
    
    while (ifs.read(reinterpret_cast<char*>(&startIp), sizeof(startIp))) {
        ifs.read(reinterpret_cast<char*>(&endIp), sizeof(endIp));
        ifs.read(reinterpret_cast<char*>(&countryLen), sizeof(countryLen));
        ifs.read(reinterpret_cast<char*>(&cityLen), sizeof(cityLen));

        std::vector<char> countryBuf(countryLen);
        std::vector<char> cityBuf(cityLen);

        ifs.read(countryBuf.data(), countryLen);
        ifs.read(cityBuf.data(), cityLen);

        std::string countryCode(countryBuf.data(), countryLen);
        std::string city(cityBuf.data(), cityLen);

        std::cout << "Start IP: " << startIp << "\n";
        std::cout << "End IP: " << endIp << "\n";
        std::cout << "Country Code: " << countryCode << "\n";
        std::cout << "City: " << city << "\n";
        std::cout << "-----------------------\n";
    }
}

} // namespace geolocation::preprocessor
