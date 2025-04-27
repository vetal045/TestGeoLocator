#pragma once

#include "interfaces/ICommandFactory.h"
#include "interfaces/ICommandHandler.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace geolocation::service {

/// @brief Handles dispatching commands to appropriate handlers.
class GeoService {
public:
    /// @brief Constructs GeoService with a given command factory.
    GeoService(std::unique_ptr<interfaces::ICommandFactory> factory);

    /// @brief Registers a command by its name.
    /// @param command Name of the command.
    void registerCommand(const std::string& command);

    /// @brief Handles an incoming command.
    /// @param command Parsed command.
    /// @return Response string for the command.
    std::string handleCommand(const command::ParsedCommand& command);

    /// @brief Returns true if the service should terminate.
    bool shouldExit() const;

private:
    std::unordered_map<std::string, std::unique_ptr<interfaces::ICommandHandler>> handlers_;
    std::unique_ptr<interfaces::ICommandFactory> factory_;
    bool exit_ = false;
};

} // namespace geolocation::service
