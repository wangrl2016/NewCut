//
// Created by wangrl2016 on 2023/1/4.
//

#include "base/strings/string_util.h"

namespace base {
    bool StartsWith(StringPiece str,
                    StringPiece search_for,
                    CompareCase case_sensitivity) {
        return StartsWithT(str, search_for, case_sensitivity);
    }
}
