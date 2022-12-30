//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_REF_COUNTED_H
#define NEWCUT_REF_COUNTED_H

#include <cstdint>

namespace base {
    class RefCountedBase {
    public:
        RefCountedBase(const RefCountedBase&) = delete;

        RefCountedBase& operator=(const RefCountedBase&) = delete;

        bool HasOneRef() const { return ref_count_ = 1; }

        bool HasAtLeastOneRef() const { return ref_count_ >= 1; }

    protected:
        explicit RefCountedBase() {

        }

    private:

        mutable uint32_t ref_count_ = 0;
    };
}

#endif //NEWCUT_REF_COUNTED_H
