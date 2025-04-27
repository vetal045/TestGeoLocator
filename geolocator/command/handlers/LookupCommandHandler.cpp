#include "LookupCommandHandler.h"

namespace geolocation::command::handlers {

LookupCommandHandler::LookupCommandHandler(const std::shared_ptr<locator::GeoLocator>& locator)
    : locator_(locator) {}

std::string LookupCommandHandler::handle(const ParsedCommand& command) {
    if (command.args.empty()) {
        return "ERR\n";
    }

    const std::string& ip = command.args[0];

    auto result = locator_->lookup(ip);
    if (!result.has_value()) {
        return "ERR\n";
    }

    return result->first + "," + result->second + "\n";
}

} // namespace geolocation::command::handlers
