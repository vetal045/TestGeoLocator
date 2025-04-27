#include "locator/GeoLocator.h"
#include "common/ChunkInfo.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>

#include <iostream>

namespace geolocation::locator {

namespace {

/// @brief Converts an IP address string to a 32-bit unsigned integer.
/// @param ipStr IP address string.
/// @return 32-bit representation of the IP address.
/// @throws std::invalid_argument if the format is invalid.
uint32_t ipToUint(const std::string& ipStr) {
    std::istringstream iss(ipStr);
    uint32_t a, b, c, d;
    char dot;
    if (!(iss >> a >> dot >> b >> dot >> c >> dot >> d)) {
        throw std::invalid_argument("Invalid IP format");
    }
    return (a << 24) | (b << 16) | (c << 8) | d;
}

} // unnamed namespace

GeoLocator::GeoLocator(const IGeoRecordProvider& provider)
    : provider_(provider) {
    refresh();
}

void GeoLocator::refresh() {
    buildChunks(provider_.getRecords());
}

void GeoLocator::buildChunks(const std::vector<common::GeoRecord>& records) {
    if (records.empty()) {
        return;
    }

    chunks_.clear();

    size_t n = records.size();
    size_t numChunks = std::max<size_t>(4, static_cast<size_t>(std::sqrt(n)));

    chunks_.resize(numChunks);

    uint64_t totalRange = static_cast<uint64_t>(1) << 32;
    uint64_t chunkRange = totalRange / numChunks;

    for (const auto& record : records) {
        uint64_t avgIp = (static_cast<uint64_t>(record.startIp) + record.endIp) / 2;
        size_t chunkIdx = avgIp / chunkRange;
        if (chunkIdx >= numChunks) {
            chunkIdx = numChunks - 1;
        }

        auto& chunk = chunks_[chunkIdx];
        chunk.records.push_back(record);

        if (chunk.records.size() == 1) {
            chunk.minStartIp = record.startIp;
            chunk.maxEndIp = record.endIp;
        } else {
            chunk.minStartIp = std::min(chunk.minStartIp, record.startIp);
            chunk.maxEndIp = std::max(chunk.maxEndIp, record.endIp);
        }
    }

    for (auto& chunk : chunks_) {
        std::sort(chunk.records.begin(), chunk.records.end(), [](const common::GeoRecord& a, const common::GeoRecord& b) {
            return a.startIp < b.startIp;
        });
    }
}

std::optional<std::pair<std::string, std::string>> GeoLocator::lookup(const std::string& ipStr) const {
    uint32_t ip;
    try {
        ip = ipToUint(ipStr);
    } catch (...) {
        return std::nullopt;
    }

    for (const auto& chunk : chunks_) {
        if (chunk.minStartIp <= ip && ip <= chunk.maxEndIp) {
            auto it = std::lower_bound(
                chunk.records.begin(),
                chunk.records.end(),
                ip,
                [](const common::GeoRecord& record, uint32_t targetIp) {
                    return record.endIp < targetIp;
                }
            );

            if (it != chunk.records.end() && it->startIp <= ip && ip <= it->endIp) {
                return std::make_pair(it->countryCode, it->city);
            }
        }
    }

    return std::nullopt;
}

} // namespace geolocation::locator
