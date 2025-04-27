#pragma once

#include "common/GeoRecord.h"

#include <vector>
#include <cstdint>

namespace geolocation::common {

/// @brief Represents a chunk of geolocation records with IP range information.
struct ChunkInfo {
    uint32_t minStartIp = 0; ///< Minimum start IP in the chunk.
    uint32_t maxEndIp = 0;   ///< Maximum end IP in the chunk.
    std::vector<GeoRecord> records; ///< List of geolocation records in the chunk.
};

} // namespace geolocation::common
