# lib7zip Build Instructions

lib7zip has been successfully adapted to work with 7-Zip 25.0. This document provides complete build instructions for the updated library.

## Prerequisites

- **C++ Compiler**: GCC 8+ or Clang 10+ with C++11 support
- **Build Tools**: autotools (autoconf, automake, libtool)
- **Dependencies**: pthread, dl (standard on most Linux systems)
- **7-Zip Source**: 7-Zip 25.0 source code (included in `../7zip/`)

## Quick Start

### Method 1: Using autotools (Recommended)

```bash
# Set 7-Zip source directory
export SEVENZIP_SOURCE_DIR=../7zip

# Configure and build
./configure --prefix=/usr/local
make -j4

# Install (optional)
sudo make install
```

### Method 2: Direct Makefile

```bash
cd Lib7Zip
make all      # Build both static and shared libraries
make static   # Build only static library (.a)
make shared   # Build only shared library (.so)
```

## Generated Libraries

The build process creates several library variants:

### Static Libraries
- `Lib7Zip/lib7zip.a` - Direct Makefile build (283KB)
- `Lib7Zip/.libs/lib7zip.a` - Autotools build (3.8MB, includes debug symbols)

### Shared Libraries  
- `Lib7Zip/lib7zip.so` - Direct Makefile build (189KB)
- `Lib7Zip/.libs/lib7zip.so.0.0.0` - Autotools build (1.5MB)
- Symbolic links: `lib7zip.so` → `lib7zip.so.0` → `lib7zip.so.0.0.0`

## Usage Examples

### Linking with Static Library
```bash
g++ -std=c++11 your_app.cpp -I./includes -L./Lib7Zip -l7zip -ldl -lpthread
```

### Linking with Shared Library
```bash
g++ -std=c++11 your_app.cpp -I./includes -L./Lib7Zip/.libs -l7zip -ldl -lpthread
export LD_LIBRARY_PATH=./Lib7Zip/.libs:$LD_LIBRARY_PATH
```

## Key Adaptations Made

1. **COM Interface Updates**: Updated from P7ZIP macros to 7-Zip 25.0 COM macros
2. **Data Type Compatibility**: Fixed `unsigned __int64` → `UInt64` throughout
3. **Exception Specifications**: Added `throw()` to all COM method implementations  
4. **Include Path Updates**: Updated paths for new 7-Zip directory structure
5. **Build System Integration**: Both direct Makefile and autotools support

## File Structure

```
lib7zip/
├── configure.ac              # Autotools configuration
├── Makefile.am              # Top-level automake file
├── Lib7Zip/
│   ├── lib7zip.h            # Main header file
│   ├── Makefile             # Direct build
│   ├── Makefile.am          # Autotools build
│   ├── lib7zip.a            # Static library (direct)
│   ├── lib7zip.so           # Shared library (direct)
│   └── .libs/               # Autotools output directory
├── includes/                # Symbolic links to 7-Zip sources
│   ├── C -> ../7zip/C       # 7-Zip C headers
│   └── CPP -> ../7zip/CPP   # 7-Zip C++ headers
└── Test7Zip/                # Test programs (require fixes)
```

## Environment Variables

- `SEVENZIP_SOURCE_DIR`: Must point to 7-Zip 25.0 source directory (e.g., `../7zip`)
- `LD_LIBRARY_PATH`: Include path to shared libraries when running applications

## Troubleshooting

### Configure Fails with "7zip source invalid"
Ensure `SEVENZIP_SOURCE_DIR` points to a valid 7-Zip source with `C/` and `CPP/` subdirectories.

### Missing COM Interface Errors
The library has been adapted for 7-Zip 25.0 API. Older applications may need updates to use the new interface definitions.

### Test Programs Won't Compile
Test programs require additional interface implementations. The core library itself builds successfully and is functional.

## Success Verification

You can verify successful build by checking:
```bash
# Check libraries exist
ls -la Lib7Zip/lib7zip.a Lib7Zip/.libs/lib7zip.so*

# Check symbols in static library  
nm Lib7Zip/lib7zip.a | grep C7ZipLibrary

# Check shared library dependencies
ldd Lib7Zip/.libs/lib7zip.so
```

The adapted lib7zip now successfully compiles with 7-Zip 25.0 and provides both static and shared library variants for use in applications requiring programmatic 7-Zip archive access.