cmake_minimum_required(VERSION 3.16)
project(TestGeoLocatorProject)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)

add_subdirectory(geolocator)
add_subdirectory(preprocessor)
add_subdirectory(tests)

enable_testing()
