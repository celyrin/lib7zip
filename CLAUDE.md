# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

lib7zip is a C++ wrapper library for programmatically accessing 7-Zip archives. This version has been modernized to work with **7-Zip 25.0** and provides both static and shared library builds.

## Build System & Commands

### Prerequisites
- **Required**: 7-Zip 25.0 source code must be available at `../7zip/` or specified via `SEVENZIP_SOURCE_DIR`
- **Tools**: C++ compiler (GCC 8+/Clang 10+), autotools (autoconf, automake, libtool)
- **Dependencies**: pthread, dl (standard on Linux systems)

### Primary Build Commands

```bash
# Method 1: Automated build (recommended)
export SEVENZIP_SOURCE_DIR=../7zip
./build.sh

# Method 2: Autotools build
export SEVENZIP_SOURCE_DIR=../7zip
./configure --prefix=/usr/local
make -j$(nproc)

# Method 3: Direct Makefile build
cd Lib7Zip
make all      # Build both static and shared libraries
make static   # Build only static library (.a)
make shared   # Build only shared library (.so)
```

### Build Output Locations
- **Static Library**: `Lib7Zip/lib7zip.a` (283KB, optimized) or `Lib7Zip/.libs/lib7zip.a` (3.8MB, with debug symbols)
- **Shared Library**: `Lib7Zip/lib7zip.so` (189KB, optimized) or `Lib7Zip/.libs/lib7zip.so.0.0.0` (1.5MB, versioned)

### Test Programs
```bash
# After building, test programs are available in Test7Zip/:
# - test7zip, test7zip2, test7zipmulti, test7zprops, test7zcrypt, test7zipsig, test7ziprar5
# Note: Test programs may require additional fixes for full compatibility
```

### Clean Commands
```bash
make clean                    # Clean build artifacts
make distclean               # Clean all generated files
rm -rf includes/CPP includes/C  # Remove 7zip source links
```

## Architecture & Code Structure

### Core Components

1. **C7ZipLibrary**: Main library class for initialization and archive opening
2. **C7ZipArchive**: Represents an opened archive, provides item enumeration and extraction
3. **C7ZipArchiveItem**: Represents individual files/folders within an archive
4. **C7ZipInStream/C7ZipOutStream**: Abstract stream interfaces for I/O operations
5. **C7ZipMultiVolumes**: Support for multi-volume archives

### Directory Structure
```
Lib7Zip/           # Core library implementation
├── lib7zip.h      # Main public API header
├── *Archive*.cpp  # Archive handling classes
├── *Stream*.cpp   # Stream wrapper implementations
├── OSFunctions*   # Platform-specific OS abstraction
└── compat.h       # Compatibility definitions

includes/          # Symbolic links to 7-Zip source
├── C/            # → ../7zip/C (7-Zip C headers)
└── CPP/          # → ../7zip/CPP (7-Zip C++ headers)

Test7Zip/         # Test programs and sample archives
```

### Key Modernizations for 7-Zip 25.0
- Updated COM interfaces from `MY_UNKNOWN_IMP*` to `Z7_COM_UNKNOWN_IMP_*` macros
- Fixed data types: `unsigned __int64` → `UInt64`
- Added proper `throw()` exception specifications
- Updated include paths for new 7-Zip 25.0 directory structure

## Development Guidelines

### Adding New Features
- Follow the existing C++ class hierarchy with virtual interfaces
- Use the C7ZipObjectPtrArray for memory management of created objects
- All public API should be exposed through lib7zip.h
- Platform-specific code goes in OSFunctions_*.cpp files

### Error Handling
- Use lib7zip::ErrorCodeEnum for standardized error reporting
- Check C7ZipLibrary::GetLastError() for detailed error information
- Handle password-protected archives via SetArchivePassword() methods

### Building Against lib7zip
```bash
# Static linking
g++ -std=c++11 your_app.cpp -I./includes -L./Lib7Zip -l7zip -ldl -lpthread

# Shared linking  
g++ -std=c++11 your_app.cpp -I./includes -L./Lib7Zip/.libs -l7zip -ldl -lpthread
export LD_LIBRARY_PATH=./Lib7Zip/.libs:$LD_LIBRARY_PATH
```

## Common Development Tasks

### Verification Commands
```bash
# Check libraries exist
ls -la Lib7Zip/lib7zip.* Lib7Zip/.libs/lib7zip.so*

# Check library symbols
nm Lib7Zip/lib7zip.a | grep C7ZipLibrary

# Check shared library dependencies  
ldd Lib7Zip/.libs/lib7zip.so
```

### Environment Variables
- `SEVENZIP_SOURCE_DIR`: Path to 7-Zip 25.0 source directory (required for build)
- `LD_LIBRARY_PATH`: Include lib7zip path when using shared libraries

### Troubleshooting Build Issues
1. **"7zip source invalid"**: Ensure SEVENZIP_SOURCE_DIR points to valid 7-Zip source with C/ and CPP/ subdirectories
2. **Missing COM interfaces**: Library adapted for 7-Zip 25.0 - older applications may need interface updates  
3. **Test program compilation failures**: Core library builds successfully; test programs may need additional interface work
4. **Autotools missing**: Install autoconf, automake, libtool packages

## File Modification Guidelines

When working with this codebase:
- The core library (Lib7Zip/) is production-ready and functional
- Include paths use the pattern `#include "../includes/CPP/..."`  
- Platform-specific code is abstracted through OSFunctions_*.h interfaces
- All COM interface implementations require `throw()` exception specifications
- Use UInt64 for 64-bit integers (not unsigned __int64)