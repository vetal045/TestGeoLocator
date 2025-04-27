#include <gtest/gtest.h>
#include "command/handlers/LookupCommandHandler.h"
#include "locator/GeoLocator.h"
#include "locator/VectorGeoRecordProvider.h"
#include "command/ParsedCommand.h"

using namespace geolocation::command::handlers;
using namespace geolocation::locator;
using namespace geolocation::common;
using namespace geolocation::command;

TEST(LookupCommandHandlerTest, HandlesLookupSuccessfully) {
    std::vector<GeoRecord> records = {
        {16777216, 16777471, "US", "Los Angeles"}
    };

    auto provider = std::make_shared<VectorGeoRecordProvider>(records);
    auto locator = std::make_shared<GeoLocator>(*provider);

    LookupCommandHandler handler(locator);

    ParsedCommand command;
    command.args.push_back("1.0.0.8");

    auto response = handler.handle(command);

    EXPECT_EQ(response, "US,Los Angeles\n");
}

TEST(LookupCommandHandlerTest, ReturnsErrorOnInvalidIP) {
    auto provider = std::make_shared<VectorGeoRecordProvider>(std::vector<GeoRecord>{});
    auto locator = std::make_shared<GeoLocator>(*provider);

    LookupCommandHandler handler(locator);

    ParsedCommand command;
    command.args.push_back("bad_ip");

    auto response = handler.handle(command);

    EXPECT_EQ(response, "ERR\n");
}
