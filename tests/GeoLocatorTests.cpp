#include <gtest/gtest.h>
#include "locator/GeoLocator.h"
#include "locator/VectorGeoRecordProvider.h"
#include "common/GeoRecord.h"

using geolocation::locator::GeoLocator;
using geolocation::locator::VectorGeoRecordProvider;
using geolocation::common::GeoRecord;

TEST(GeoLocatorTest, FindsCorrectRecord) {
    std::vector<GeoRecord> records = {
        {16777216, 16777471, "US", "Los Angeles"},
        {16777472, 16777727, "US", "San Diego"},
    };

    VectorGeoRecordProvider provider(std::move(records));
    GeoLocator locator(provider);

    auto result = locator.lookup("1.0.0.8");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->first, "US");
    EXPECT_EQ(result->second, "Los Angeles");
}

TEST(GeoLocatorTest, ReturnsNulloptIfNotFound) {
    std::vector<GeoRecord> records = {
        {16777216, 16777471, "US", "Los Angeles"},
        {16777472, 16777727, "US", "San Diego"},
    };

    VectorGeoRecordProvider provider(std::move(records));
    GeoLocator locator(provider);

    auto result = locator.lookup("5.5.5.5");
    EXPECT_FALSE(result.has_value());
}

TEST(GeoLocatorTest, ReturnsNulloptForInvalidIP) {
    std::vector<GeoRecord> records = {
        {16777216, 16777471, "US", "Los Angeles"}
    };

    VectorGeoRecordProvider provider(std::move(records));
    GeoLocator locator(provider);

    auto result = locator.lookup("invalid_ip");
    EXPECT_FALSE(result.has_value());
}
