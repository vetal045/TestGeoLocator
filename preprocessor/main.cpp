#include "GeoPreprocessor.h"
#include "CsvGeoPreprocessorSource.h"

using namespace geolocation::preprocessor;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return EXIT_FAILURE;
    }

    CsvGeoPreprocessorSource source;
    return RunGeoPreprocessing(argv[1], argv[2], source);
}
