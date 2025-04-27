add_executable(preprocessor
    main.cpp
    GeoPreprocessor.cpp
    CsvGeoPreprocessorSource.cpp
)

target_include_directories(preprocessor PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
