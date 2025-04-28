include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    PreprocessorTests.cpp
    GeoDataLoaderTests.cpp
    GeoLocatorTests.cpp
    CommandParserTests.cpp
    LoadCommandHandlerTests.cpp
    LookupCommandHandlerTests.cpp
    ExitCommandHandlerTests.cpp
    ConcreteCommandFactoryTests.cpp
    CommandServiceTests.cpp

    ${CMAKE_SOURCE_DIR}/preprocessor/GeoPreprocessor.cpp
    ${CMAKE_SOURCE_DIR}/preprocessor/CsvGeoPreprocessorSource.cpp
    ${CMAKE_SOURCE_DIR}/geolocator/loaders/BinaryGeoDatabaseLoader.cpp
    ${CMAKE_SOURCE_DIR}/geolocator/locator/GeoLocator.cpp
    ${CMAKE_SOURCE_DIR}/geolocator/locator/VectorGeoRecordProvider.cpp
    ${CMAKE_SOURCE_DIR}/geolocator/command/CommandParser.cpp
    ${CMAKE_SOURCE_DIR}/geolocator/command/handlers/LoadCommandHandler.cpp
    ${CMAKE_SOURCE_DIR}/geolocator/command/handlers/LookupCommandHandler.cpp
    ${CMAKE_SOURCE_DIR}/geolocator/command/handlers/ExitCommandHandler.cpp
    ${CMAKE_SOURCE_DIR}/geolocator/factory/ConcreteCommandFactory.cpp
    ${CMAKE_SOURCE_DIR}/geolocator/service/CommandService.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator/common
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator/locator
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator/loaders
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator/command
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator/command/handlers
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator/factory
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator/service
    ${CMAKE_CURRENT_SOURCE_DIR}/../preprocessor
)

target_link_libraries(tests
    gtest_main
)

add_test(NAME tests COMMAND tests)
