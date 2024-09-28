#pragma once

#include "syati.h"

namespace pt {
    template<typename R>
    R* accessMember(void* ptr, size_t offset);
    void* accessMember(void* ptr, size_t offset);
    template<typename R>
    const R* accessMember(const void* ptr, size_t offset);
    const void* accessMember(const void* ptr, size_t offset);
};