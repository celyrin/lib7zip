#ifndef LIB7ZIP_SECURE_STRING_H
#define LIB7ZIP_SECURE_STRING_H

#include <vector>
#include <string>

// Secure string class for password handling
// Addresses password security issues identified in code review
class SecureString {
private:
    std::vector<wchar_t> data;
    mutable bool locked = false;
    
    // Security: overwrite memory multiple times
    void secure_clear() {
        if (!data.empty()) {
            // Multiple pass clearing to prevent memory recovery
            for (int pass = 0; pass < 3; ++pass) {
                std::fill(data.begin(), data.end(), static_cast<wchar_t>(0xAA + pass));
            }
            // Final zero pass
            std::fill(data.begin(), data.end(), 0);
        }
    }
    
public:
    // Constructor from regular wstring
    explicit SecureString(const std::wstring& str = L"") {
        if (!str.empty()) {
            data.resize(str.length() + 1);  // +1 for null terminator
            std::copy(str.begin(), str.end(), data.begin());
            data[str.length()] = 0;
        }
    }
    
    // Copy constructor with secure handling
    SecureString(const SecureString& other) {
        if (!other.data.empty()) {
            data = other.data;
        }
    }
    
    // Assignment operator
    SecureString& operator=(const SecureString& other) {
        if (this != &other) {
            secure_clear();
            if (!other.data.empty()) {
                data = other.data;
            } else {
                data.clear();
            }
        }
        return *this;
    }
    
    // Destructor with secure memory clearing
    ~SecureString() {
        secure_clear();
        data.clear();
    }
    
    // Safe access methods
    const wchar_t* c_str() const {
        return data.empty() ? L"" : data.data();
    }
    
    size_t length() const {
        return data.empty() ? 0 : data.size() - 1;  // -1 for null terminator
    }
    
    bool empty() const {
        return data.empty() || (data.size() == 1 && data[0] == 0);
    }
    
    // Conversion to regular wstring (use sparingly)
    std::wstring to_wstring() const {
        if (data.empty()) return L"";
        return std::wstring(data.data(), length());
    }
    
    // Clear the password
    void clear() {
        secure_clear();
        data.clear();
    }
    
    // Set new password value
    void set(const std::wstring& str) {
        secure_clear();
        if (!str.empty()) {
            data.resize(str.length() + 1);
            std::copy(str.begin(), str.end(), data.begin());
            data[str.length()] = 0;
        } else {
            data.clear();
        }
    }
};

#endif // LIB7ZIP_SECURE_STRING_H