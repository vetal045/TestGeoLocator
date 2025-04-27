#include "locator/GeoLocator.h"

#include <algorithm>
#include <stdexcept>

namespace geolocation::locator {

GeoLocator::GeoLocator(const IGeoRecordProvider& provider)
    : provider_(provider) {
    refresh();
}

void GeoLocator::refresh() {
    records_ = provider_.getRecords();
}

std::optional<std::pair<std::string, std::string>> GeoLocator::lookup(const std::string& ipStr) const {
    uint32_t ip;
    try {
        ip = parseIp(ipStr);
    } catch (...) {
        return std::nullopt;
    }

    if (records_.empty()) {
        return std::nullopt;
    }

    auto it = std::lower_bound(
        records_.begin(),
        records_.end(),
        ip,
        [](const common::GeoRecord& record, uint32_t targetIp) {
            return record.endIp < targetIp;
        }
    );

    if (it != records_.end() && it->startIp <= ip && ip <= it->endIp) {
        return std::make_pair(it->countryCode, it->city);
    }

    return std::nullopt;
}

uint32_t GeoLocator::parseIp(const std::string& ipStr) const {
    uint32_t result = 0;
    uint32_t part = 0;
    int shift = 24;

    for (char c : ipStr) {
        if (c >= '0' && c <= '9') {
            part = part * 10 + (c - '0');
        } else if (c == '.') {
            result |= (part << shift);
            shift -= 8;
            part = 0;
        } else {
            throw std::invalid_argument("Invalid IP format");
        }
    }
    result |= part;
    return result;
}

} // namespace geolocation::locator
