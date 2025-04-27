#include "CsvGeoPreprocessorSource.h"

namespace geolocation::preprocessor {

std::vector<geolocation::common::GeoRecord> CsvGeoPreprocessorSource::load(const std::string& inputPath) const {
    std::vector<geolocation::common::GeoRecord> records;

    //io::CSVReader<8, io::trim_chars<' ', '"'> > csvReader(inputPath);
    io::CSVReader<8, io::trim_chars<' ', '"'>, io::double_quote_escape<',', '"'>> csvReader(inputPath);

    uint32_t startIp, endIp;
    std::string countryCode, countryName, region, city, latitude, longitude;

    int rowCounter = 0;
    // Read each row and map relevant fields into GeoRecord objects.
    while (csvReader.read_row(startIp, endIp, countryCode, countryName, region, city, latitude, longitude)) {
        if (countryCode.empty() || city.empty()) {
            continue;
        }
        records.emplace_back(geolocation::common::GeoRecord{startIp, endIp, countryCode, city});
    }

    return records;
}

} // namespace geolocation::preprocessor
