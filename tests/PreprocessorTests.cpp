#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <vector>

#include "CsvGeoPreprocessorSource.h"
#include "GeoPreprocessor.h"

namespace fs = std::filesystem;
using geolocation::preprocessor::CsvGeoPreprocessorSource;
using geolocation::preprocessor::RunGeoPreprocessing;
using geolocation::preprocessor::DebugReadGeoFile;

TEST(PreprocessorTest, ConvertsSmallCsvToGeoFormatCorrectly) {
    fs::path tempDir = fs::temp_directory_path();
    fs::path inputFile = tempDir / "test_input.csv";
    fs::path outputFile = tempDir / "test_output.geo";

    {
        std::ofstream ofs(inputFile);
        ASSERT_TRUE(ofs.is_open());
        ofs << "16777216,16777471,US,United States of America,California,Los Angeles,34.052230,-118.243680\n";            
    }

    CsvGeoPreprocessorSource source;
    int result = RunGeoPreprocessing(inputFile.string(), outputFile.string(), source);

    ASSERT_EQ(result, EXIT_SUCCESS);
    ASSERT_TRUE(fs::exists(outputFile));
    ASSERT_GT(fs::file_size(outputFile), 0);

    std::ifstream ifs(outputFile, std::ios::binary);
    ASSERT_TRUE(ifs.is_open());

    uint32_t startIp, endIp;
    uint16_t countryLen, cityLen;
    
    ifs.read(reinterpret_cast<char*>(&startIp), sizeof(startIp));
    ifs.read(reinterpret_cast<char*>(&endIp), sizeof(endIp));
    ifs.read(reinterpret_cast<char*>(&countryLen), sizeof(countryLen));
    ifs.read(reinterpret_cast<char*>(&cityLen), sizeof(cityLen));
    
    std::vector<char> countryBuf(countryLen);
    std::vector<char> cityBuf(cityLen);
    
    ifs.read(countryBuf.data(), countryLen);
    ifs.read(cityBuf.data(), cityLen);
    
    std::string countryCode(countryBuf.data(), countryLen);
    std::string city(cityBuf.data(), cityLen);
    
    EXPECT_EQ(startIp, 16777216u);
    EXPECT_EQ(endIp, 16777471u);
    EXPECT_EQ(countryCode, "US");
    EXPECT_EQ(city, "Los Angeles");

    ifs.close();
    fs::remove(inputFile);
    fs::remove(outputFile);
}

TEST(PreprocessorTest, RealDatabaseSampleShouldBeProcessedCorrectly) {
    fs::path input = fs::current_path().parent_path().parent_path() / "tests" / "mockData" / "database_1000.csv";
    fs::path output = fs::current_path().parent_path().parent_path() / "tests" / "mockData" / "test_output_real_sample.geo";

    ASSERT_TRUE(fs::exists(input)) << "Test input database_1000.csv not found at: " << input;

    if (fs::exists(output)) {
        fs::remove(output);
    }

    CsvGeoPreprocessorSource source;
    int result = RunGeoPreprocessing(input.string(), output.string(), source);

    ASSERT_EQ(result, EXIT_SUCCESS);
    ASSERT_TRUE(fs::exists(output));
    ASSERT_GT(fs::file_size(output), 0);

    //DebugReadGeoFile(output.string());
    fs::remove(output);
}

TEST(PreprocessorTest, ParsesAndSortsCorrectly) {
    fs::path tempDir = fs::temp_directory_path();
    fs::path inputFile = tempDir / "unsorted_input.csv";
    fs::path outputFile = tempDir / "sorted_output.geo";

    {
        std::ofstream ofs(inputFile);
        ASSERT_TRUE(ofs.is_open());

        ofs << "16777232,16777247,US,United States,CA,San Diego,32.7157,-117.1611\n";
        ofs << "16777216,16777231,US,United States,CA,Los Angeles,34.0522,-118.2437\n";
        ofs << "16777248,16777263,US,United States,CA,San Francisco,37.7749,-122.4194\n";
    }

    CsvGeoPreprocessorSource source;
    int result = RunGeoPreprocessing(inputFile.string(), outputFile.string(), source);

    ASSERT_EQ(result, EXIT_SUCCESS);
    ASSERT_TRUE(fs::exists(outputFile));
    ASSERT_GT(fs::file_size(outputFile), 0);

    std::ifstream ifs(outputFile, std::ios::binary);
    ASSERT_TRUE(ifs.is_open());

    std::vector<uint32_t> loadedStartIps;

    while (ifs.peek() != EOF) {
        uint32_t startIp, endIp;
        uint16_t countryLen, cityLen;

        ifs.read(reinterpret_cast<char*>(&startIp), sizeof(startIp));
        ifs.read(reinterpret_cast<char*>(&endIp), sizeof(endIp));
        ifs.read(reinterpret_cast<char*>(&countryLen), sizeof(countryLen));
        ifs.read(reinterpret_cast<char*>(&cityLen), sizeof(cityLen));

        std::vector<char> countryBuf(countryLen);
        std::vector<char> cityBuf(cityLen);

        ifs.read(countryBuf.data(), countryLen);
        ifs.read(cityBuf.data(), cityLen);

        loadedStartIps.push_back(startIp);
    }

    ASSERT_TRUE(std::is_sorted(loadedStartIps.begin(), loadedStartIps.end()));

    ifs.close();
    fs::remove(inputFile);
    fs::remove(outputFile);
}
