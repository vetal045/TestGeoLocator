add_executable(preprocessor
    main.cpp
    GeoPreprocessor.cpp
    CsvGeoPreprocessorSource.cpp
)

target_include_directories(preprocessor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../geolocator/common
)