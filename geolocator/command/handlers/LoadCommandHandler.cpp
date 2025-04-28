#include "LoadCommandHandler.h"
#include "CsvGeoPreprocessorSource.h"
#include "BinaryGeoDatabaseLoader.h"
#include "GeoLocator.h"

#include <algorithm>
#include <filesystem>

namespace geolocation::command::handlers {

namespace fs = std::filesystem;

LoadCommandHandler::LoadCommandHandler(std::string databasePath,
    const std::shared_ptr<locator::VectorGeoRecordProvider>& provider,
    const std::shared_ptr<locator::GeoLocator>& locator)
        : databasePath_(std::move(databasePath)),
        provider_(provider),
        locator_(locator) {}

std::string LoadCommandHandler::handle(const ParsedCommand& command) {
    if (!command.args.empty()) {
        return "ERR\n";
    }

    try {
        if (databasePath_.empty() || !fs::exists(databasePath_)) {
            return "ERR\n";
        }

        std::vector<common::GeoRecord> records;

        if (databasePath_.ends_with(".csv")) {
            geolocation::preprocessor::CsvGeoPreprocessorSource csvSource;
            records = csvSource.load(databasePath_);
        } else if (databasePath_.ends_with(".geo")) {
            geolocation::loaders::BinaryGeoDatabaseLoader loader;
            records = loader.load(databasePath_);
        } else {
            return "ERR\n";
        }

        if (records.empty()) {
            return "ERR\n";
        }

        provider_->setRecords(std::move(records));
        locator_->refresh();
    }
    catch (...) {
        return "ERR\n";
    }

    return "OK\n";
}

} // namespace geolocation::command::handlers
