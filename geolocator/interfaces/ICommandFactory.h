#pragma once

#include "ICommandHandler.h"
#include "ParsedCommand.h"

#include <memory>
#include <string>

namespace geolocation::interfaces {

/// @brief Interface for creating command handlers dynamically based on command name.
class ICommandFactory {
public:
    virtual ~ICommandFactory() = default;

    /// @brief Creates a command handler based on the command name.
    /// @param command Command name.
    /// @return Unique pointer to a command handler.
    virtual std::unique_ptr<ICommandHandler> createHandler(const std::string& command) const = 0;
};

} // namespace geolocation::interfaces
