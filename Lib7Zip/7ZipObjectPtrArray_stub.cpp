#include "lib7zip.h"
#include "compat.h"
#include <algorithm>
#include <memory>

// Enhanced implementation of C7ZipObjectPtrArray with proper memory safety
// Addresses critical memory management issues identified in code review

C7ZipObjectPtrArray::C7ZipObjectPtrArray(bool auto_release) 
    : m_bAutoRelease(auto_release) {
    // Initialize with reasonable capacity to avoid frequent reallocations
    reserve(16);
}

C7ZipObjectPtrArray::~C7ZipObjectPtrArray() {
    clear();
}

void C7ZipObjectPtrArray::clear() {
    if (m_bAutoRelease) {
        // Enhanced memory safety: validate each pointer before deletion
        for (auto* obj : *this) {
            if (IsValidPointer(obj)) {
                try {
                    // Ensure virtual destructor is called properly
                    delete obj;
                } catch (...) {
                    // Log error if possible, but continue cleanup
                    // Don't let exceptions escape destructor
                }
            }
        }
    }
    
    // Clear the vector after cleanup
    std::vector<C7ZipObject*>::clear();
}

// Additional safety methods
void C7ZipObjectPtrArray::push_back_safe(C7ZipObject* obj) {
    if (IsValidPointer(obj)) {
        push_back(obj);
    }
}

void C7ZipObjectPtrArray::remove_safe(C7ZipObject* obj) {
    if (IsValidPointer(obj)) {
        auto it = std::find(begin(), end(), obj);
        if (it != end()) {
            if (m_bAutoRelease) {
                delete *it;
            }
            erase(it);
        }
    }
}

size_t C7ZipObjectPtrArray::get_valid_count() const {
    return std::count_if(begin(), end(), [](const C7ZipObject* obj) {
        return IsValidPointer(obj);
    });
}