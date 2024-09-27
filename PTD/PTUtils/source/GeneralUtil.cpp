#include "GeneralUtil.h"

/*
* Authors: Lord-Giganticus
*/

namespace pt {
    template<typename R>
    R* accessMember(void* ptr, size_t offset) {
        u8* p = static_cast<u8*>(ptr);
        return static_cast<R*>(&p[offset]);
    } 
    void* accessMember(void* ptr, size_t offset) {
        u8* p = static_cast<u8*>(ptr);
        return static_cast<void*>(&p[offset]);
    }
    template <typename R>
    const R* accessMember(const void* ptr, size_t offset) {
        const u8* p = static_cast<const u8*>(ptr);
        return static_cast<const R*>(&p[offset]);
    }
    const void* accessMember(const void* ptr, size_t offset) {
        const u8* p = static_cast<const u8*>(ptr);
        return static_cast<const void*>(&p[offset]);
    }
};