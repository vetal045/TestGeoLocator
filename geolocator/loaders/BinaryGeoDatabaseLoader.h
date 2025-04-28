#pragma once

#include "IGeoDatabaseLoader.h"
#include "common/GeoRecord.h"

#include <string>
#include <vector>

namespace geolocation::loaders {

/// Loads GeoRecords from a binary .geo file using memory-mapped I/O.
/// 
/// .geo file format:
/// - uint32_t numRecords
/// - repeated:
///   - uint32_t startIp
///   - uint32_t endIp
///   - uint16_t countryCodeLength
///   - uint16_t cityLength
///   - countryCode bytes
///   - city bytes
class BinaryGeoDatabaseLoader : public IGeoDatabaseLoader {
public:
    std::vector<common::GeoRecord> load(const std::string& path) const override;

private:
#if defined(_WIN32)
    std::vector<common::GeoRecord> loadWindows(const std::string& path) const;
#else
    std::vector<common::GeoRecord> loadUnix(const std::string& path) const;
#endif

    bool parseRecords(const char*& ptr, const char* end, std::vector<common::GeoRecord>& records) const;
};

} // namespace geolocation::loaders
