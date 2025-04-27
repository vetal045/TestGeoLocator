#!/bin/bash

set -euo pipefail

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input.csv> <output.geo>"
    exit 1
fi

input_csv="$1"
output_geo="$2"

# Only make input path absolute
input_csv_abs="$(realpath "$input_csv")"

# output_geo оставляем как есть!

# Ensure build directory exists
if [ ! -d "build" ]; then
    echo "Build directory not found. Creating..."
    mkdir build
fi

cd build

# Configure if necessary
if [ ! -f "Makefile" ]; then
    echo "Configuring project with cmake..."
    cmake ..
fi

# Build preprocessor
echo "Building preprocessor tool..."
make -j preprocessor

# Run the preprocessor binary
echo "Running preprocessor..."
./preprocessor/preprocessor "$input_csv_abs" "$output_geo"

echo "Preprocessing completed successfully."
