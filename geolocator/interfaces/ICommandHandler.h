#pragma once

#include "ParsedCommand.h"

#include <string>

namespace geolocation::interfaces {

/// Interface for handling a parsed command.
class ICommandHandler {
public:
    virtual ~ICommandHandler() = default;

    /// Executes the command and returns a response.
    /// @param command Parsed command structure.
    /// @return Response string to be printed to stdout.
    virtual std::string handle(const command::ParsedCommand& command) = 0;
};

} // namespace geolocation::interfaces
