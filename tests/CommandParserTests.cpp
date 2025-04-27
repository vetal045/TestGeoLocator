#include <gtest/gtest.h>
#include "command/CommandParser.h"

using namespace geolocation::command;

TEST(CommandParserTest, ParsesValidCommandWithArgs) {
    CommandParser parser;
    auto result = parser.parse("LOAD database.geo");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->command, "LOAD");
    ASSERT_EQ(result->args.size(), 1);
    EXPECT_EQ(result->args[0], "database.geo");
}

TEST(CommandParserTest, ParsesCommandWithoutArgs) {
    CommandParser parser;
    auto result = parser.parse("EXIT");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->command, "EXIT");
    EXPECT_TRUE(result->args.empty());
}

TEST(CommandParserTest, ReturnsNulloptForEmptyInput) {
    CommandParser parser;
    auto result = parser.parse("");

    EXPECT_FALSE(result.has_value());
}
