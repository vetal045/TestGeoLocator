#include <gtest/gtest.h>
#include "handlers/ExitCommandHandler.h"
#include "ParsedCommand.h"

using namespace geolocation::command::handlers;
using namespace geolocation::command;

TEST(ExitCommandHandlerTest, HandlesExitCorrectly) {
    ExitCommandHandler handler;
    ParsedCommand command;

    auto response = handler.handle(command);

    EXPECT_EQ(response, "OK\n");
}
