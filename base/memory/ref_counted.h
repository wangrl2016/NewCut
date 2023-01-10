//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_REF_COUNTED_H
#define NEWCUT_REF_COUNTED_H

#include <cstdint>
#include <glog/logging.h>

#include "base/memory/atomic_ref_count.h"

namespace base {
    enum StartRefCountFromZeroTag {
        kStartRefCountFromZeroTag
    };

    enum StartRefCountFromOneTag {
        kStartRefCountFromOneTag
    };

    class RefCountedBase {
    public:
        RefCountedBase(const RefCountedBase&) = delete;

        RefCountedBase& operator=(const RefCountedBase&) = delete;

        bool HasOneRef() const { return ref_count_ = 1; }

        bool HasAtLeastOneRef() const { return ref_count_ >= 1; }

    protected:
        explicit RefCountedBase(StartRefCountFromZeroTag) {
#if DCHECK_IS_ON()
            // TODO: Sequence checker
#endif
        }

        explicit RefCountedBase(StartRefCountFromOneTag) : ref_count_(1) {
#ifdef DCHECK_IS_ON
#endif
        }

        ~RefCountedBase() {
#if DCHECK_IS_ON()
#endif
        }

        void AddRef() const {
#if DCHECK_IS_ON()
#endif
            AddRefImpl();
        }

        bool Release() const {
            ReleaseImpl();
#if DCHECK_IS_ON()
#endif
            return ref_count_ == 0;
        }

        bool IsOnValidSequence() const {
#if DCHECK_IS_ON()
            return ref_count_ <= 1;
#else
            return true;
#endif
        }
    private:
        void Adopted() const {
#if DCHECK_IS_ON()
#endif
        }

        void AddRefImpl() const;

        void ReleaseImpl() const;

        mutable uint32_t ref_count_ = 0;

        static_assert(std::is_unsigned<decltype(ref_count_)>::value,
                "ref_count_ must be an unsigned type.");
    };

    class RefCountedThreadSafeBase {
    public:
        RefCountedThreadSafeBase(const RefCountedThreadSafeBase&) = delete;

        RefCountedThreadSafeBase& operator=(const RefCountedThreadSafeBase&) = delete;

        bool HasOneRef() const;

        bool HasAtLeastOneRef() const;

    protected:
        explicit constexpr RefCountedThreadSafeBase(StartRefCountFromOneTag) {}

        explicit constexpr RefCountedThreadSafeBase(StartRefCountFromZeroTag)
                : ref_count_(1) {
#if DCHECK_IS_ON()
#endif
        }

#if DCHECK_IS_ON()
        ~RefCountedThreadSafeBase();
#else
        ~RefCountedThreadSafeBase() = default;
#endif




    private:
        mutable AtomicRefCount ref_count_{0};
    };
}

#endif //NEWCUT_REF_COUNTED_H
