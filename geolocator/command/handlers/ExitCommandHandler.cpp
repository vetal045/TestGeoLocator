#include "ExitCommandHandler.h"

namespace geolocation::command::handlers {

std::string ExitCommandHandler::handle(const ParsedCommand&) {
    return "OK\n";
}

} // namespace geolocation::command::handlers
