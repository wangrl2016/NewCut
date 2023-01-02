//
// Created by wangrl2016 on 2022/12/19.
//

#include <glog/logging.h>
#include "base/memory/aligned_memory.h"
#include "base/misc/bits.h"

namespace base {
    void* AlignedAlloc(size_t size, size_t alignment) {
        DCHECK_GT(size, 0U);
        DCHECK(IsPowerOfTwo(alignment));
        DCHECK_EQ(alignment % sizeof(void*), 0U);
        void* ptr = nullptr;
#if defined(_MSC_VER)
        ptr = _aligned_malloc(size, alignment);
#else
        int ret = posix_memalign(&ptr, alignment, size);
        if (ret != 0) {
            DLOG(ERROR) << "posix_memalign() returned with error " << ret;
            ptr = nullptr;
        }
#endif

        // Since aligned allocations may fail for non-memory related reasons, force a
        // crash if we encounter a failed allocation.
        if (!ptr) {
            DLOG(ERROR) << "If you crash here, your aligned allocation is incorrect: "
                     << "size = " << size << ", alignemnt " << alignment;
            CHECK(false);
        }

        // Sanity check alignment just to be safe.
        DCHECK(IsAligned(ptr, alignment));
        return ptr;
    }
}
