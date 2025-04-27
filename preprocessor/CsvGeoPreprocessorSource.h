#pragma once

#include "IGeoPreprocessorSource.h"
#include <csv/csv.h>

namespace geolocation::preprocessor {

/// CSV-based implementation of IGeoPreprocessorSource.
/// Parses a CSV file to produce a list of GeoRecords.
class CsvGeoPreprocessorSource : public IGeoPreprocessorSource {
public:
    std::vector<geolocation::common::GeoRecord> load(const std::string& inputPath) const override;
};

} // namespace geolocation::preprocessor
