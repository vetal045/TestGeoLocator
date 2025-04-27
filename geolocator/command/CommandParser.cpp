#include "CommandParser.h"

#include <sstream>

namespace geolocation::command {

std::optional<ParsedCommand> CommandParser::parse(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command.empty()) {
        return std::nullopt;
    }

    ParsedCommand parsedCommand;
    parsedCommand.command = command;

    std::string arg;
    while (iss >> arg) {
        parsedCommand.args.push_back(arg);
    }

    return parsedCommand;
}

} // namespace geolocation::command
