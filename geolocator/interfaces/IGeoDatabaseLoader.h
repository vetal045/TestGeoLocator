#pragma once

#include "GeoRecord.h"
#include <vector>
#include <string>

namespace geolocation::loaders {

/// Interface for loading GeoRecords from external sources.
class IGeoDatabaseLoader {
public:
    virtual ~IGeoDatabaseLoader() = default;

    /// Loads GeoRecords from the specified file path.
    /// @param path Path to the database file.
    /// @return Vector of loaded GeoRecords.
    virtual std::vector<common::GeoRecord> load(const std::string& path) const = 0;
};

} // namespace geolocation::loaders
