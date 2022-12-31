//
// Created by wangrl2016 on 2022/12/19.
//

#ifndef NEWCUT_ALIGNED_MEMORY_H
#define NEWCUT_ALIGNED_MEMORY_H

#include <cstddef>
#include <memory>
#include <glog/logging.h>

namespace base {
    void* AlignedAlloc(size_t size, size_t alignment) {
        return std::aligned_alloc(alignment, size);
    }

    inline void AlignedFree(void* ptr) {
#if defined(_MSC_VER)
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }

    struct AlignedFreeDeleter {
        inline void operator()(void* ptr) const {
            AlignedFree(ptr);
        }
    };

    inline bool IsAligned(uintptr_t val, size_t alignment) {
        return (val & (alignment - 1)) == 0;
    }

    inline bool IsAligned(const void* val, size_t alignment) {
        return IsAligned(reinterpret_cast<uintptr_t>(val), alignment);
    }
}

#endif //NEWCUT_ALIGNED_MEMORY_H
