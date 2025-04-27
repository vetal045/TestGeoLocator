#pragma once

#include "ParsedCommand.h"

#include <string>
#include <optional>

namespace geolocation::command {

/// Parses a line into a command and its arguments.
class CommandParser {
public:
    /// Parses a raw input line.
    /// @param line Input string line from stdin.
    /// @return ParsedCommand if parsing was successful, std::nullopt otherwise.
    static std::optional<ParsedCommand> parse(const std::string& line);
};

} // namespace geolocation::command
