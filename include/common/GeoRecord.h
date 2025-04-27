#pragma once

#include <string>
#include <cstdint>

namespace geolocation::common {

/// Represents a geolocation record linked to an IP address range.
/// Each record includes start and end IPs, a country code, and a city name.
struct GeoRecord {
    uint32_t startIp;       ///< Start of the IP range (inclusive).
    uint32_t endIp;         ///< End of the IP range (inclusive).
    std::string countryCode; ///< Two-letter country code (e.g., "US", "DE").
    std::string city;        ///< City name associated with the IP range.
};

} // namespace geolocation::common