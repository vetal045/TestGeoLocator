#pragma once

#include "GeoRecord.h"
#include <string>
#include <vector>

namespace geolocation::preprocessor {

/// Interface for a source that loads GeoRecords.
/// Allows different implementations (e.g., CSV file, database, API).
class IGeoPreprocessorSource {
public:
    virtual ~IGeoPreprocessorSource() = default;

    /// Loads GeoRecords from the specified input path.
    /// @param inputPath Path to the input source.
    /// @return Vector of loaded GeoRecords.
    virtual std::vector<geolocation::common::GeoRecord> load(const std::string& inputPath) const = 0;
};

} // namespace geolocation::preprocessor
