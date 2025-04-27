#include <gtest/gtest.h>
#include "command/handlers/ExitCommandHandler.h"
#include "command/ParsedCommand.h"

using namespace geolocation::command::handlers;
using namespace geolocation::command;

TEST(ExitCommandHandlerTest, HandlesExitCorrectly) {
    ExitCommandHandler handler;
    ParsedCommand command;

    auto response = handler.handle(command);

    EXPECT_EQ(response, "OK\n");
}
