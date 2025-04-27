cmake_minimum_required(VERSION 3.16)
project(geolocator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(geolocator
    main.cpp
    command/CommandParser.cpp
    command/handlers/LoadCommandHandler.cpp
    command/handlers/LookupCommandHandler.cpp
    command/handlers/ExitCommandHandler.cpp
    factory/ConcreteCommandFactory.cpp
    service/GeoService.cpp
)

target_sources(geolocator PRIVATE
    ../src/locator/GeoLocator.cpp
    ../src/locator/VectorGeoRecordProvider.cpp
    ../src/loaders/BinaryGeoDatabaseLoader.cpp
    ../preprocessor/CsvGeoPreprocessorSource.cpp
)

target_include_directories(geolocator PRIVATE
    ../include
    ../src/locator
    ../src/loaders
    ../preprocessor
    .
    command
    command/handlers
    factory
    interfaces
    service
)

set_target_properties(geolocator PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)
