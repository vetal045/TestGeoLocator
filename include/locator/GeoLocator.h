#pragma once

#include "common/ChunkInfo.h"
#include "locator/IGeoRecordProvider.h"

#include <string>
#include <vector>
#include <optional>
#include <utility>

namespace geolocation::locator {

/// @brief Service for efficient geolocation lookup based on IP address.
class GeoLocator {
public:
    /// @brief Constructs GeoLocator using the provided record provider.
    /// @param provider Record provider supplying geolocation records.
    explicit GeoLocator(const IGeoRecordProvider& provider);

    /// @brief Performs IP address lookup.
    /// @param ipStr IP address in dotted string format.
    /// @return Pair of (country code, city) if found, std::nullopt otherwise.
    std::optional<std::pair<std::string, std::string>> lookup(const std::string& ipStr) const;

    /// @brief Rebuilds internal chunk structure from the provider's records.
    void refresh();
private:

    /// @brief Builds chunks from the given list of records.
    /// @param records List of geolocation records.
    void buildChunks(const std::vector<common::GeoRecord>& records);

    const IGeoRecordProvider& provider_; ///< Source of geolocation records.
    std::vector<common::ChunkInfo> chunks_; ///< Chunks of sorted geolocation records.
};

} // namespace geolocation::locator
