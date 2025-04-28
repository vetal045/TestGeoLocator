#pragma once

#include "IGeoDatabaseLoader.h"

namespace geolocation::loaders {

/// Loads GeoRecords from a binary .geo file.
/// Binary format:
/// [startIp (4 bytes)][endIp (4 bytes)][countryLen (2 bytes)][cityLen (2 bytes)][countryCode][city]
class BinaryGeoDatabaseLoader : public IGeoDatabaseLoader {
public:
    std::vector<common::GeoRecord> load(const std::string& path) const override;
};

} // namespace geolocation::loaders
