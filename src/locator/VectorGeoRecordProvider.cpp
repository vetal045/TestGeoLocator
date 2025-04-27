#include "locator/VectorGeoRecordProvider.h"

namespace geolocation::locator {

using common::GeoRecord;

VectorGeoRecordProvider::VectorGeoRecordProvider(std::vector<GeoRecord> records)
    : records_(std::move(records)) // Move to avoid unnecessary copies
{}

void VectorGeoRecordProvider::setRecords(std::vector<GeoRecord> records) {
    records_ = std::move(records);
}

const std::vector<GeoRecord>& VectorGeoRecordProvider::getRecords() const {
    return records_;
}

} // namespace geolocation::locator
