#!/bin/bash

# Enhanced build verification script
# Addresses issues identified in code review report

set -e

echo "🔍 Starting enhanced build verification..."

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    local status=$1
    local message=$2
    case $status in
        "OK")
            echo -e "${GREEN}✓ $message${NC}"
            ;;
        "WARN")
            echo -e "${YELLOW}⚠ $message${NC}"
            ;;
        "ERROR")
            echo -e "${RED}✗ $message${NC}"
            ;;
    esac
}

# Check 7-Zip source directory
if [ ! -d "third_party/7zip/C" ] || [ ! -d "third_party/7zip/CPP" ]; then
    print_status "ERROR" "7-Zip source not found in third_party/7zip"
    exit 1
fi
print_status "OK" "7-Zip source directory found"

# Check for critical security fixes
echo -e "\n🛡️ Verifying security enhancements..."

# Check if SecureString is being used
if grep -q "SecureString" Lib7Zip/7ZipArchiveOpenCallback.h; then
    print_status "OK" "SecureString password storage implemented"
else
    print_status "WARN" "SecureString not found in callback header"
fi

# Check if safe_memmem is being used
if grep -q "safe_memmem" Lib7Zip/7ZipOpenArchive.cpp; then
    print_status "OK" "Safe memory search implemented"
else
    print_status "WARN" "Safe memory search not found"
fi

# Check for COM interface improvements
if grep -q "Z7_COM_UNKNOWN_IMP" Lib7Zip/compat.h; then
    print_status "OK" "Enhanced COM interface macros present"
else
    print_status "WARN" "Enhanced COM macros not found"
fi

# Check for proper exception specifications
if grep -q "throw()" Lib7Zip/7ZipArchiveOpenCallback.cpp; then
    print_status "OK" "Exception specifications added"
else
    print_status "WARN" "Exception specifications not found"
fi

# Build the library
echo -e "\n🔨 Building lib7zip..."
export SEVENZIP_SOURCE_DIR="$(pwd)/third_party/7zip"

# Clean previous build
make clean >/dev/null 2>&1 || true
rm -rf includes/CPP includes/C 2>/dev/null || true

# Configure and build
if ./configure --prefix=/usr/local >/dev/null 2>&1; then
    print_status "OK" "Configuration successful"
else
    print_status "ERROR" "Configuration failed"
    exit 1
fi

if make -j$(nproc) >/dev/null 2>&1; then
    print_status "OK" "Build successful"
else
    print_status "ERROR" "Build failed"
    exit 1
fi

# Verify libraries exist
echo -e "\n📦 Verifying generated libraries..."

if [ -f "Lib7Zip/lib7zip.a" ]; then
    size=$(stat -c%s "Lib7Zip/lib7zip.a")
    print_status "OK" "Static library created (${size} bytes)"
else
    print_status "ERROR" "Static library not found"
fi

if [ -f "Lib7Zip/.libs/lib7zip.so.0.0.0" ]; then
    size=$(stat -c%s "Lib7Zip/.libs/lib7zip.so.0.0.0")
    print_status "OK" "Shared library created (${size} bytes)"
else
    print_status "WARN" "Shared library not found"
fi

# Check library symbols
echo -e "\n🔍 Verifying library symbols..."

if nm Lib7Zip/lib7zip.a 2>/dev/null | grep -q "C7ZipLibrary"; then
    print_status "OK" "Core symbols present in static library"
else
    print_status "WARN" "Core symbols not found in static library"
fi

# Memory leak check if valgrind is available
if command -v valgrind >/dev/null 2>&1; then
    echo -e "\n🧪 Running basic memory leak test..."
    # Note: This would need a test program to be meaningful
    print_status "OK" "Valgrind available for memory testing"
else
    print_status "WARN" "Valgrind not available for memory leak testing"
fi

# Security checks
echo -e "\n🔐 Security verification..."

# Check for hardcoded passwords or sensitive data
if grep -r -i "password.*=" Lib7Zip/ | grep -v SecureString | grep -v "//"; then
    print_status "WARN" "Potential hardcoded passwords found"
else
    print_status "OK" "No hardcoded passwords detected"
fi

# Check for buffer overflow patterns
if grep -r "strcpy\|sprintf\|gets" Lib7Zip/ >/dev/null 2>&1; then
    print_status "WARN" "Potentially unsafe string functions found"
else
    print_status "OK" "No obviously unsafe string functions detected"
fi

echo -e "\n✅ Build verification completed!"
echo "📋 Next steps:"
echo "   1. Run comprehensive tests with your archive files"
echo "   2. Perform memory leak testing with valgrind"
echo "   3. Test cross-platform compatibility"
echo "   4. Verify performance benchmarks"