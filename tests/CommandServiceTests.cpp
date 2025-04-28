#include <gtest/gtest.h>
#include "CommandService.h"
#include "ParsedCommand.h"
#include "ICommandHandler.h"
#include "ICommandFactory.h"

using namespace geolocation::service;
using namespace geolocation::command;
using namespace geolocation::interfaces;

// Mock Handler
class MockCommandHandler : public ICommandHandler {
public:
    std::string handle(const ParsedCommand& command) override {
        return "OK\n";
    }
};

// Mock Factory
class MockCommandFactory : public ICommandFactory {
public:
    std::unique_ptr<ICommandHandler> createHandler(const std::string& command) const override {
        return std::make_unique<MockCommandHandler>();
    }
};

TEST(CommandServiceTest, RegistersAndHandlesCommandSuccessfully) {
    auto factory = std::make_unique<MockCommandFactory>();
    CommandService service(std::move(factory));

    service.registerCommand("LOAD");

    ParsedCommand parsed;
    parsed.command = "LOAD";

    auto response = service.handleCommand(parsed);

    EXPECT_EQ(response, "OK\n");
}

TEST(CommandServiceTest, ReturnsErrorForUnknownCommand) {
    auto factory = std::make_unique<MockCommandFactory>();
    CommandService service(std::move(factory));

    ParsedCommand parsed;
    parsed.command = "UNKNOWN";

    auto response = service.handleCommand(parsed);

    EXPECT_EQ(response, "ERR\n");
}

TEST(CommandServiceTest, ShouldExitAfterExitCommand) {
    auto factory = std::make_unique<MockCommandFactory>();
    CommandService service(std::move(factory));

    service.registerCommand("EXIT");

    ParsedCommand parsed;
    parsed.command = "EXIT";

    auto response = service.handleCommand(parsed);

    EXPECT_EQ(response, "OK\n");
    EXPECT_TRUE(service.shouldExit());
}
