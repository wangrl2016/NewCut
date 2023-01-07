//
// Created by wangrl2016 on 2023/1/7.
//

#ifndef NEWCUT_STRING_UTIL_WIN_H
#define NEWCUT_STRING_UTIL_WIN_H

#include "base/strings/string_piece.h"

namespace base {
    inline const char16_t* as_u16cstr(const wchar_t* str) {
        return reinterpret_cast<const char16_t*>(str);
    }

    inline const char16_t* as_u16cstr(WStringPiece str) {
        return reinterpret_cast<const char16_t*>(str.data());
    }

    bool IsStringASCII(WStringPiece str);
}

#endif //NEWCUT_STRING_UTIL_WIN_H
