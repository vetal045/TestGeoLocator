#pragma once

#include "IGeoRecordProvider.h"
#include "GeoRecord.h"

#include <vector>
#include <optional>
#include <string>

namespace geolocation::locator {

/// @brief Service for efficient IP geolocation lookup.
/// 
/// Uses an efficient data structure for quick mapping of IPv4 addresses
/// to their corresponding country code and city name.
class GeoLocator {
public:
    /// @brief Constructs a GeoLocator using a provided record provider.
    /// @param provider Record provider supplying geolocation records.
    explicit GeoLocator(const IGeoRecordProvider& provider);

    /// @brief Refreshes and rebuilds the internal lookup index from provider records.
    void refresh();

    /// @brief Performs a lookup of the given IP address.
    /// @param ipStr IP address in dotted decimal format (e.g., "192.168.1.1").
    /// @return A pair (country code, city) if found, std::nullopt otherwise.
    std::optional<std::pair<std::string, std::string>> lookup(const std::string& ipStr) const;

private:
    /// @brief Parses an IP address string into a 32-bit unsigned integer.
    /// @param ipStr IP address string.
    /// @return Parsed 32-bit representation of the IP address.
    uint32_t parseIp(const std::string& ipStr) const;

    const IGeoRecordProvider& provider_; ///< The provider that supplies GeoRecords.
    std::vector<common::GeoRecord> records_; ///< Flat list of GeoRecords sorted by startIp.
};

} // namespace geolocation::locator
