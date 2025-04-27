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

    ${CMAKE_SOURCE_DIR}/preprocessor/GeoPreprocessor.cpp
    ${CMAKE_SOURCE_DIR}/preprocessor/CsvGeoPreprocessorSource.cpp
    ${CMAKE_SOURCE_DIR}/src/loaders/BinaryGeoDatabaseLoader.cpp
    ${CMAKE_SOURCE_DIR}/src/locator/GeoLocator.cpp
    ${CMAKE_SOURCE_DIR}/src/locator/VectorGeoRecordProvider.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/preprocessor
)

target_link_libraries(tests
    gtest_main
)

add_test(NAME tests COMMAND tests)
