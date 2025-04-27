#include "CommandService.h"

namespace geolocation::service {

CommandService::CommandService(std::unique_ptr<interfaces::ICommandFactory> factory)
    : factory_(std::move(factory)) {}

void CommandService::registerCommand(const std::string& command) {
    handlers_[command] = factory_->createHandler(command);
}

std::string CommandService::handleCommand(const command::ParsedCommand& command) {
    auto it = handlers_.find(command.command);
    if (it == handlers_.end()) {
        return "ERR\n";
    }

    std::string response = it->second->handle(command);

    if (command.command == "EXIT") {
        exit_ = true;
    }

    return response;
}

bool CommandService::shouldExit() const {
    return exit_;
}

} // namespace geolocation::service
