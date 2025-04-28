#pragma once

#include <optional>
#include <string>

namespace geolocation::locator {

/// @brief Interface for geolocation lookup services.
/// 
/// Defines basic operations for refreshing the internal state and performing IP lookups.
class ILocator {
public:
    virtual ~ILocator() = default;

    /// @brief Refreshes the internal geolocation data.
    /// 
    /// Should be called if the underlying data source has changed.
    virtual void refresh() = 0;

    /// @brief Performs an IP address lookup.
    /// @param ipStr IPv4 address in dotted-decimal notation (e.g., "192.168.1.1").
    /// @return A pair (country code, city) if found, std::nullopt otherwise.
    virtual std::optional<std::pair<std::string, std::string>> lookup(const std::string& ipStr) const = 0;
};

} // namespace geolocation::locator
