#!/bin/bash

# Exit immediately if a command exits with a non-zero status,
# treat unset variables as an error and fail pipelines
set -euo pipefail

#--------------------------------------------------
# Utility function to find the most recently changed .cpp/.h file
#--------------------------------------------------
check_for_changes() {
    find . -name '*.cpp' -o -name '*.h' | xargs stat -f "%m %n" | sort -n | tail -n 1
}

#--------------------------------------------------
# Parse command-line arguments
#--------------------------------------------------
BUILD_ONLY=false
TEST_ONLY=false

while [[ "$#" -gt 0 ]]; do
    case "$1" in
        --build) BUILD_ONLY=true ;;
        --test)  TEST_ONLY=true ;;
        *)
            echo "Usage: $0 [--build] [--test]"
            exit 1
            ;;
    esac
    shift
done

#--------------------------------------------------
# Basic environment checks
#--------------------------------------------------
if ! command -v cmake &> /dev/null; then
    echo "cmake could not be found. Please install cmake."
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo "make could not be found. Please install make."
    exit 1
fi

#--------------------------------------------------
# Build preparation
#--------------------------------------------------
echo "Checking if files changed..."

last_change_time=$(check_for_changes)
cmake_last_change_time=""

if [ -f "CMakeLists.txt" ]; then
    cmake_last_change_time=$(stat -f %m CMakeLists.txt)
fi

if [ "$last_change_time" -gt "$cmake_last_change_time" ] || ! [ -d build ]; then
    echo "Cleaning old build directory..."
    rm -rf build
fi

if ! [ -d build ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

#--------------------------------------------------
# Always rerun cmake
#--------------------------------------------------
echo "Configuring project with cmake..."
cmake ..

#--------------------------------------------------
# Build and/or Test based on arguments
#--------------------------------------------------
if [ "$BUILD_ONLY" = true ]; then
    echo "Building project only..."
    make -j
    echo "Project built successfully."
    exit 0
fi

if [ "$TEST_ONLY" = true ]; then
    echo "Building tests only..."
    make -j tests
    echo "Running tests..."
    if ctest --output-on-failure; then
        echo "Tests passed successfully."
    else
        echo "Tests failed."
        exit 1
    fi
    exit 0
fi

# Default: build and run tests
echo "Building project..."
make -j

echo "Running tests..."
if ctest --output-on-failure; then
    echo "Build and tests completed successfully."
else
    echo "Tests failed."
    exit 1
fi
