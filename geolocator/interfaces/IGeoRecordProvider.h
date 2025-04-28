#pragma once

#include "GeoRecord.h"
#include <vector>

namespace geolocation::locator {

/// Interface for providing access to GeoRecords.
/// Allows GeoLocator to operate independently of the underlying storage mechanism.
class IGeoRecordProvider {
public:
    virtual ~IGeoRecordProvider() = default;

    /// Retrieves the collection of GeoRecords.
    /// @return Constant reference to the GeoRecords vector.
    virtual const std::vector<common::GeoRecord>& getRecords() const = 0;
};

} // namespace geolocation::locator
