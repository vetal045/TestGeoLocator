#pragma once

#include "ICommandHandler.h"
#include "GeoLocator.h"
#include "ParsedCommand.h"

#include <memory>

namespace geolocation::command::handlers {

/// Handles the LOOKUP command to perform geolocation lookups.
class LookupCommandHandler : public interfaces::ICommandHandler {
public:
    explicit LookupCommandHandler(const std::shared_ptr<locator::GeoLocator>& locator);

    std::string handle(const ParsedCommand& command) override;

private:
    std::shared_ptr<locator::GeoLocator> locator_;
};

} // namespace geolocation::command::handlers
