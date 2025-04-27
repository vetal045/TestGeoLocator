#include "GeoService.h"

namespace geolocation::service {

GeoService::GeoService(std::unique_ptr<interfaces::ICommandFactory> factory)
    : factory_(std::move(factory)) {}

void GeoService::registerCommand(const std::string& command) {
    handlers_[command] = factory_->createHandler(command);
}

std::string GeoService::handleCommand(const command::ParsedCommand& command) {
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

bool GeoService::shouldExit() const {
    return exit_;
}

} // namespace geolocation::service
