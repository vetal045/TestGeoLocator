#pragma once

#include <string>
#include <vector>

namespace geolocation::command {

/// Represents a parsed command with optional arguments.
struct ParsedCommand {
    std::string command;          ///< Command name, e.g., "LOAD", "LOOKUP", "EXIT"
    std::vector<std::string> args; ///< Arguments for the command, if any
};

} // namespace geolocation::command
