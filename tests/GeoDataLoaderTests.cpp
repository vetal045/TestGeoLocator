#include <gtest/gtest.h>
#include "loaders/BinaryGeoDatabaseLoader.h"
#include "common/GeoRecord.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
using geolocation::loaders::BinaryGeoDatabaseLoader;
using geolocation::common::GeoRecord;

TEST(GeoDataLoaderTest, LoadsRecordsCorrectly) {
    fs::path tempDir = fs::temp_directory_path();
    fs::path geoFile = tempDir / "test_data.geo";

    {
        std::ofstream ofs(geoFile, std::ios::binary);
        ASSERT_TRUE(ofs.is_open());

        uint32_t startIp = 16777216;
        uint32_t endIp = 16777471;
        std::string country = "US";
        std::string city = "Los Angeles";

        uint16_t countryLen = country.size();
        uint16_t cityLen = city.size();

        ofs.write(reinterpret_cast<const char*>(&startIp), sizeof(startIp));
        ofs.write(reinterpret_cast<const char*>(&endIp), sizeof(endIp));
        ofs.write(reinterpret_cast<const char*>(&countryLen), sizeof(countryLen));
        ofs.write(reinterpret_cast<const char*>(&cityLen), sizeof(cityLen));
        ofs.write(country.data(), countryLen);
        ofs.write(city.data(), cityLen);
    }

    BinaryGeoDatabaseLoader loader;
    auto records = loader.load(geoFile.string());

    ASSERT_EQ(records.size(), 1);
    EXPECT_EQ(records[0].startIp, 16777216u);
    EXPECT_EQ(records[0].endIp, 16777471u);
    EXPECT_EQ(records[0].countryCode, "US");
    EXPECT_EQ(records[0].city, "Los Angeles");

    fs::remove(geoFile);
}

TEST(GeoDataLoaderTest, HandlesEmptyFileGracefully) {
    fs::path tempDir = fs::temp_directory_path();
    fs::path geoFile = tempDir / "empty_file.geo";

    {
        std::ofstream ofs(geoFile);
        ASSERT_TRUE(ofs.is_open());
    }

    BinaryGeoDatabaseLoader loader;
    auto records = loader.load(geoFile.string());

    EXPECT_TRUE(records.empty());

    fs::remove(geoFile);
}
