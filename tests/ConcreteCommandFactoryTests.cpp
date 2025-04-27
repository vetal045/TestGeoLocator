#include <gtest/gtest.h>
#include "factory/ConcreteCommandFactory.h"
#include "locator/GeoLocator.h"
#include "locator/VectorGeoRecordProvider.h"

using namespace geolocation::factory;
using namespace geolocation::locator;
using namespace geolocation::common;

TEST(ConcreteCommandFactoryTest, CreatesHandlersCorrectly) {
    auto provider = std::make_shared<VectorGeoRecordProvider>(std::vector<GeoRecord>{});
    auto locator = std::make_shared<GeoLocator>(*provider);

    ConcreteCommandFactory factory("test_database.geo", provider, locator);

    auto loadHandler = factory.createHandler("LOAD");
    EXPECT_TRUE(loadHandler != nullptr);

    auto lookupHandler = factory.createHandler("LOOKUP");
    EXPECT_TRUE(lookupHandler != nullptr);

    auto exitHandler = factory.createHandler("EXIT");
    EXPECT_TRUE(exitHandler != nullptr);

    auto unknownHandler = factory.createHandler("UNKNOWN");
    EXPECT_EQ(unknownHandler, nullptr);
}
