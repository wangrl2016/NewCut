//
// Created by wangrl2016 on 2022/12/30.
//

#include "base/memory/ref_counted.h"

namespace base {
    bool RefCountedThreadSafeBase::HasOneRef() const {
        return ref_count_.IsOne();
    }

    bool RefCountedThreadSafeBase::HasAtLeastOneRef() const {
        return !ref_count_.IsOne();
    }

}