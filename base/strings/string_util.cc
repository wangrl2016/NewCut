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

    bool StartsWith(StringPiece16 str,
                    StringPiece16 search_str,
                    CompareCase case_sensitivity) {
        return StartsWithT(str, search_str, case_sensitivity);
    }

    bool StartsWith(WStringPiece str,
                    WStringPiece search_for,
                    CompareCase case_sensitivity) {
        return StartsWithT(str, search_for, case_sensitivity);
    }
}
