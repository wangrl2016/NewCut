//
// Created by wangrl2016 on 2023/1/7.
//

#include "base/strings/string_util_win.h"
#include "base/strings/string_util_helper.h"

namespace base {
    bool IsStringASCII(WStringPiece str) {
        return DoIsStringASCII(str.data(), str.length());
    }
}