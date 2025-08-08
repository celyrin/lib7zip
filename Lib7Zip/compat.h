#ifndef LIB7ZIP_COMPAT_H
#define LIB7ZIP_COMPAT_H

// Minimal compatibility layer for 7zip 25.0 - Fixed version
// This version avoids conflicts with existing 7-Zip headers

// Safe error handling macros
#define SAFE_COM_CALL(call) \
    do { \
        HRESULT hr = (call); \
        if (FAILED(hr)) { \
            return hr; \
        } \
    } while(0)

#define VALIDATE_POINTER(ptr, retval) \
    if (!(ptr)) { \
        return (retval); \
    }

// Define missing constants if not already defined
#ifndef E_POINTER
#define E_POINTER ((HRESULT)0x80004003L)
#endif

#ifndef E_OUTOFMEMORY
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#endif

// Type safety helpers
template<typename T>
inline bool IsValidPointer(const T* ptr) {
    return ptr != nullptr;
}

// Safe memory operations
template<typename T>
inline void SafeClearMemory(T* ptr, size_t count = 1) {
    if (ptr && count > 0) {
        memset(ptr, 0, sizeof(T) * count);
    }
}

#endif // LIB7ZIP_COMPAT_H