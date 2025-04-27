#pragma once

#include "interfaces/ICommandFactory.h"
#include "locator/VectorGeoRecordProvider.h"
#include "locator/GeoLocator.h"

#include <memory>
#include <string>

namespace geolocation::factory {

/// @brief Concrete factory for creating command handlers based on command names.
class ConcreteCommandFactory : public geolocation::interfaces::ICommandFactory {
public:
    /**
     * @brief Constructs the command factory with a database path, provider, and locator.
     * 
     * @param databasePath Path to the database file.
     * @param provider Shared pointer to the GeoRecord provider.
     * @param locator Shared pointer to the GeoLocator instance.
     */
    ConcreteCommandFactory(const std::string& databasePath,
                            const std::shared_ptr<geolocation::locator::VectorGeoRecordProvider>& provider,
                            const std::shared_ptr<geolocation::locator::GeoLocator>& locator);

    /**
     * @brief Creates a command handler based on the provided command name.
     * 
     * @param command Name of the command (e.g., "LOAD", "LOOKUP", "EXIT").
     * @return Unique pointer to the corresponding command handler, or nullptr if command is unknown.
     */
    std::unique_ptr<geolocation::interfaces::ICommandHandler> createHandler(const std::string& command) const override;

private:
    std::string databasePath_;
    std::shared_ptr<geolocation::locator::VectorGeoRecordProvider> provider_;
    std::shared_ptr<geolocation::locator::GeoLocator> locator_;
};

} // namespace geolocator::factory