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
    service/CommandService.cpp
    locator/GeoLocator.cpp
    locator/VectorGeoRecordProvider.cpp
    loaders/BinaryGeoDatabaseLoader.cpp
    ../preprocessor/CsvGeoPreprocessorSource.cpp
)

target_include_directories(geolocator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/locator
    ${CMAKE_CURRENT_SOURCE_DIR}/loaders
    ${CMAKE_CURRENT_SOURCE_DIR}/../preprocessor
    ${CMAKE_CURRENT_SOURCE_DIR}/command
    ${CMAKE_CURRENT_SOURCE_DIR}/command/handlers
    ${CMAKE_CURRENT_SOURCE_DIR}/factory
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/service
)

set_target_properties(geolocator PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)
