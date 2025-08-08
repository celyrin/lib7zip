# lib7zip Upgrade Guide - Adapting to 7-Zip 25.0

## Overview

This guide describes how to upgrade lib7zip from the old p7zip dependency to work with the new official 7-Zip 25.0 source code.

## Major Changes

### 1. Directory Structure Changes
- **Old Version**: Relied on `p7zip` source code, used `myWindows` and `include_windows` directories
- **New Version**: Uses official `7-Zip` source code directly with simplified path structure

### 2. Build System Modifications

#### configure.ac Changes
```diff
- P7ZIP_SOURCE_DIR -> SEVENZIP_SOURCE_DIR
- p7zip_src_* -> sevenzip_src_*
```

#### Makefile.am Changes
```diff
- -I../includes/CPP/myWindows
- -I../includes/CPP/include_windows
+ -I../includes/CPP/Common
+ -I../includes/CPP/Windows
+ -I../includes/C
```

### 3. Source Code Modifications

#### Header Path Updates
All source files have been updated with the following include path changes:
```diff
- #include "CPP/myWindows/StdAfx.h"
- #include "CPP/include_windows/windows.h"
+ #include "CPP/Common/StdAfx.h"
+ #include "CPP/Common/MyWindows.h"

- #include "Common/ComTry.h"
- #include "Windows/PropVariant.h"
+ #include "CPP/Common/ComTry.h"
+ #include "CPP/Windows/PropVariant.h"
```

#### Version Compatibility
- Removed conditional compilation for old versions (`#if MY_VER_MAJOR >= 15`)
- Unified usage of `MyBuffer.h`

## Build Methods

### Method 1: Using the Provided Build Script
```bash
cd lib7zip
SEVENZIP_SOURCE_DIR=/path/to/7zip ./build.sh
```

### Method 2: Manual Build
```bash
cd lib7zip

# Create symbolic links
ln -sf /path/to/7zip/CPP includes/CPP
ln -sf /path/to/7zip/C includes/C

# Build using simple Makefile
cd Lib7Zip
make
```

## Known Issues and Solutions

### 1. COM Interface Compatibility Issues
The new version of 7-Zip has stricter COM interface definitions, which may require additional adaptation work. A basic compatibility header file `compat.h` has been provided.

### 2. Exception Specification Issues
The new version requires stricter exception specifications (`throw()`), which may need to be added to implementation classes.

### 3. Build Dependencies
If using autotools, the following need to be installed:
- autoconf
- automake  
- libtool

## File Modification List

### Modified Files:
- `configure.ac` - Updated source directory variables
- `Lib7Zip/Makefile.am` - Updated include paths and source files
- `Lib7Zip/*.cpp` - Updated all header include paths

### New Files:
- `build.sh` - Automated build script
- `Lib7Zip/Makefile` - Simplified build file
- `Lib7Zip/compat.h` - Compatibility header file
- `UPGRADE_GUIDE.md` - This upgrade guide

## Build Verification

Basic version information verification:
```bash
cd lib7zip
g++ simple_test.cpp -o simple_test && ./simple_test
```

Should output:
```
7-Zip version: 25.00
7-Zip date: 2025-07-05
Build successful!
```

## Next Steps

1. **Complete COM Interface Adaptation**: Further work needed to fix COM interface compatibility issues
2. **Test Case Verification**: Run test programs in Test7Zip to verify functionality
3. **Documentation Updates**: Update API documentation to reflect new version changes
4. **Performance Testing**: Verify performance characteristics of the new version

## Important Notes

- Ensure you are using official 7-Zip 25.0 or newer source code
- Thorough testing is recommended before production use
- Complete COM interface compatibility may require additional development work

## Quick Start

1. **Set up the environment**:
   ```bash
   export SEVENZIP_SOURCE_DIR=/path/to/your/7zip/source
   ```

2. **Run the build script**:
   ```bash
   cd lib7zip
   ./build.sh
   ```

3. **Verify the build**:
   ```bash
   ./simple_test
   ```

4. **Use in your project**:
   - Link against `Lib7Zip/lib7zip.a`
   - Include `Lib7Zip/lib7zip.h` in your code

## Troubleshooting

### Build Fails with "No such file or directory"
- Ensure `SEVENZIP_SOURCE_DIR` points to the correct 7-Zip source directory
- Verify that symbolic links are created correctly in `includes/`

### Compilation Errors Related to COM Interfaces
- This is expected with the current state of adaptation
- Further COM interface work is needed for full compatibility
- Consider using the compatibility layer in `compat.h`

### Missing autotools
- Install required packages: `sudo apt-get install autoconf automake libtool` (Ubuntu/Debian)
- Or use the manual build method instead