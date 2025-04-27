#!/bin/bash

# Exit immediately if a command fails
set -euo pipefail

# Doxygen config file
DOXYFILE="Doxyfile"

# Check if Doxyfile exists
if [ ! -f "$DOXYFILE" ]; then
    echo "Error: $DOXYFILE not found. Please run 'doxygen -g' first and configure it."
    exit 1
fi

# Run doxygen
echo "Generating documentation..."
doxygen "$DOXYFILE"

echo "Documentation generated successfully in docs/html/"
