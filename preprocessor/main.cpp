#include "GeoPreprocessor.h"
#include "CsvGeoPreprocessorSource.h"
#include <iostream>

using namespace geolocation::preprocessor;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.csv> <output.geo>\n";
        return EXIT_FAILURE;
    }

    CsvGeoPreprocessorSource source;
    return RunGeoPreprocessing(argv[1], argv[2], source);
}
