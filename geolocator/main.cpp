#include "service/CommandService.h"
#include "factory/ConcreteCommandFactory.h"
#include "command/CommandParser.h"
#include "command/ParsedCommand.h"
#include "locator/VectorGeoRecordProvider.h"
#include "locator/GeoLocator.h"


#include <iostream>
#include <memory>
#include <string>

using namespace geolocation;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_database>" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string databasePath = argv[1];

    auto provider = std::make_shared<locator::VectorGeoRecordProvider>(std::vector<common::GeoRecord>{});
    auto locator = std::make_shared<locator::GeoLocator>(*provider);
    auto commandFactory = std::make_unique<geolocation::factory::ConcreteCommandFactory>(databasePath, provider, locator);

    service::CommandService сommandService(std::move(commandFactory));

    std::cout << "READY" << std::endl;

    сommandService.registerCommand("LOAD");
    сommandService.registerCommand("LOOKUP");
    сommandService.registerCommand("EXIT");

    std::string line;
    while (std::getline(std::cin, line)) {
        auto parsed = command::CommandParser::parse(line);

        if (!parsed.has_value()) {
            std::cout << "ERR\n";
            continue;
        }

        std::cout << сommandService.handleCommand(parsed.value());

        if (сommandService.shouldExit()) {
            break;
        }
    }

    return 0;
}
