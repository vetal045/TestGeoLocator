#pragma once
#include "IGeoPreprocessorSource.h"
#include <string>

namespace geolocation::preprocessor {

/// Run preprocessing: load records, sort, and write to .geo binary file.
/// @param inputPath Path to input CSV file.
/// @param outputPath Path to output .geo file.
/// @param source Loader to load records.
/// @return Exit code (0 = success, non-zero = error).
int RunGeoPreprocessing(
    const std::string& inputPath,
    const std::string& outputPath,
    const IGeoPreprocessorSource& source
);

/// Debug function to read and print .geo file contents.
/// @param filePath Path to the .geo file.
void DebugReadGeoFile(const std::string& filePath);

} // namespace geolocation::preprocessor
