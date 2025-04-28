#pragma once

#include "IGeoRecordProvider.h"

namespace geolocation::locator {

/// Provides in-memory access to GeoRecords stored in a std::vector.
class VectorGeoRecordProvider : public IGeoRecordProvider {
public:
    /// Constructs the provider with a given set of records.
    explicit VectorGeoRecordProvider(std::vector<common::GeoRecord> records);

    /// Sets a new collection of GeoRecords.
    void setRecords(std::vector<common::GeoRecord> records);

    /// Returns a constant reference to the records.
    const std::vector<common::GeoRecord>& getRecords() const override;

private:
    std::vector<common::GeoRecord> records_;
};

} // namespace geolocation::locator
