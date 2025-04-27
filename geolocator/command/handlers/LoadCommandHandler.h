#pragma once

#include "loaders/IGeoDatabaseLoader.h"
#include "locator/VectorGeoRecordProvider.h"
#include "locator/GeoLocator.h"
#include "interfaces/ICommandHandler.h"
#include "command/ParsedCommand.h"

#include <memory>
#include <string>

namespace geolocation::command::handlers {

/// Handles the LOAD command by loading the database into memory.
class LoadCommandHandler : public interfaces::ICommandHandler {
public:

LoadCommandHandler(std::string databasePath,
    const std::shared_ptr<locator::VectorGeoRecordProvider>& provider,
    const std::shared_ptr<locator::GeoLocator>& locator);

    std::string handle(const ParsedCommand& command) override;

private:
    std::string databasePath_;
    std::shared_ptr<locator::VectorGeoRecordProvider> provider_;
    const std::shared_ptr<geolocation::locator::GeoLocator> locator_;
};
    

} // namespace geolocation::command::handlers
