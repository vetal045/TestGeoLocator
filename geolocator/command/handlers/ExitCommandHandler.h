#pragma once

#include "interfaces/ICommandHandler.h"
#include "command/ParsedCommand.h"

namespace geolocation::command::handlers {

/// Handles the EXIT command.
class ExitCommandHandler : public interfaces::ICommandHandler {
public:
    std::string handle(const ParsedCommand& command) override;
};

} // namespace geolocation::command::handlers
