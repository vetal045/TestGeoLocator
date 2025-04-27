#include "ConcreteCommandFactory.h"
#include "command/handlers/LoadCommandHandler.h"
#include "command/handlers/LookupCommandHandler.h"
#include "command/handlers/ExitCommandHandler.h"

namespace geolocation::factory {

ConcreteCommandFactory::ConcreteCommandFactory(const std::string& databasePath,
                                               const std::shared_ptr<geolocation::locator::VectorGeoRecordProvider>& provider,
                                               const std::shared_ptr<geolocation::locator::GeoLocator>& locator)
    : databasePath_(databasePath),
      provider_(provider),
      locator_(locator) {}

std::unique_ptr<geolocation::interfaces::ICommandHandler> ConcreteCommandFactory::createHandler(const std::string& command) const {
    if (command == "LOAD") {
        return std::make_unique<geolocation::command::handlers::LoadCommandHandler>(databasePath_, provider_, locator_);
    }
    if (command == "LOOKUP") {
        return std::make_unique<geolocation::command::handlers::LookupCommandHandler>(locator_);
    }
    if (command == "EXIT") {
        return std::make_unique<geolocation::command::handlers::ExitCommandHandler>();
    }
    return nullptr;
}

} // namespace geolocator::factory
