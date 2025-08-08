#!/bin/bash

set -e

# 7zip source directory - now using third_party/7zip
SEVENZIP_SOURCE_DIR="${SEVENZIP_SOURCE_DIR:-$(pwd)/third_party/7zip}"

if [ ! -d "$SEVENZIP_SOURCE_DIR" ]; then
    echo "Error: 7zip source directory not found at $SEVENZIP_SOURCE_DIR"
    echo "Please set SEVENZIP_SOURCE_DIR environment variable or place 7zip source in the parent directory"
    exit 1
fi

echo "Using 7zip source directory: $SEVENZIP_SOURCE_DIR"

# Clean previous build
echo "Cleaning previous build..."
make clean || true
rm -rf includes/CPP includes/C
rm -f configure

# Regenerate autotools files
echo "Regenerating autotools files..."
./autogen.sh

# Configure with 7zip source
echo "Configuring build..."
SEVENZIP_SOURCE_DIR="$SEVENZIP_SOURCE_DIR" ./configure

# Build
echo "Building lib7zip..."
make -j$(nproc)

echo "Build completed successfully!"
echo "Static library: Lib7Zip/lib7zip.a"
echo "Test programs in Test7Zip/"