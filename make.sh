#!/bin/bash

set -euo pipefail

FULL_REBUILD=false
BUILD_ONLY=false
TEST_ONLY=false

while [[ "$#" -gt 0 ]]; do
    case "$1" in
        --full-rebuild) FULL_REBUILD=true ;;
        --build-only) BUILD_ONLY=true ;;
        --test) TEST_ONLY=true ;;
        *)
            echo "Usage: $0 [--full-rebuild] [--build-only] [--test]"
            exit 1
            ;;
    esac
    shift
done

if ! command -v cmake &> /dev/null; then
    echo "Error: cmake is not installed."
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo "Error: make is not installed."
    exit 1
fi

if [ "$FULL_REBUILD" = true ]; then
    echo "Performing full rebuild..."
    rm -rf build
fi

if [ ! -d build ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

echo "Running CMake configuration..."
cmake ..

if [ "$BUILD_ONLY" = true ]; then
    echo "Building all targets..."
    make -j
    exit 0
fi

if [ "$TEST_ONLY" = true ]; then
    echo "Building and running tests..."
    make -j tests
    ctest --output-on-failure
    exit 0
fi

# Default: build and run tests
echo "Building project and running tests..."
make -j
ctest --output-on-failure
