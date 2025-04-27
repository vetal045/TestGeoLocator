#include <gtest/gtest.h>
#include "command/handlers/LoadCommandHandler.h"
#include "locator/GeoLocator.h"
#include "locator/VectorGeoRecordProvider.h"
#include "command/ParsedCommand.h"

#include <fstream>

using namespace geolocation::command::handlers;
using namespace geolocation::locator;
using namespace geolocation::common;
using namespace geolocation::command;

TEST(LoadCommandHandlerTest, HandlesLoadWithFail) {
    auto provider = std::make_shared<VectorGeoRecordProvider>(std::vector<GeoRecord>{});
    auto locator = std::make_shared<GeoLocator>(*provider);

    LoadCommandHandler handler("not_existed_database.geo", provider, locator);

    ParsedCommand command;
    auto response = handler.handle(command);

    EXPECT_EQ(response, "ERR\n");
}

TEST(LoadCommandHandlerTest, HandlesRealGeoDatabaseLoadSuccessfully) {
    const std::string testDatabasePath = "test_real_database.geo";
    {
        std::ofstream out(testDatabasePath, std::ios::binary);
        if (!out) {
            FAIL() << "Failed to create test database file.";
        }

        uint32_t startIp = 16777216; // 1.0.0.0
        uint32_t endIp = 16777471;   // 1.0.0.255
        uint16_t countryLen = 2;     // "US"
        uint16_t cityLen = 11;       // "Los Angeles"
        std::string country = "US";
        std::string city = "Los Angeles";

        out.write(reinterpret_cast<const char*>(&startIp), sizeof(startIp));
        out.write(reinterpret_cast<const char*>(&endIp), sizeof(endIp));
        out.write(reinterpret_cast<const char*>(&countryLen), sizeof(countryLen));
        out.write(reinterpret_cast<const char*>(&cityLen), sizeof(cityLen));
        out.write(country.data(), country.size());
        out.write(city.data(), city.size());
    }

    auto provider = std::make_shared<VectorGeoRecordProvider>(std::vector<GeoRecord>{});
    auto locator = std::make_shared<GeoLocator>(*provider);

    LoadCommandHandler handler(testDatabasePath, provider, locator);

    ParsedCommand command;
    auto response = handler.handle(command);

    EXPECT_EQ(response, "OK\n");

    std::remove(testDatabasePath.c_str());
}
