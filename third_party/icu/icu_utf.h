//
// Created by wangrl2016 on 2023/1/6.
//

#ifndef NEWCUT_ICU_UTF_H
#define NEWCUT_ICU_UTF_H

// Name: ICU
// URL: https://github.com/unicode-org/icu
// Version: 82
//
// This file has the relevant components from ICU copied to handle basic UTF8/16/32
// conversions. Components are copied from umachine.h, utf.h, utf8.h, and utf16.h
// into icu_utf.h

#include <cstdint>

namespace icu {
    // copy from icu4c/source/common/unicode/umachine.h

    /**
     * The ICU boolean type, a signed-byte integer.
     * ICU-specific for historical reasons: The C and C++ standards used to not define type bool.
     * Also provides a fixed type definition, as opposed to
     * type bool whose details (e.g., sizeof) may vary by compiler and between C and C++.
     *
     * @stable ICU 2.0
     */
    typedef int8_t UBool;

    /**
     * Define UChar32 as a type for single Unicode code points.
     * UChar32 is a signed 32-bit integer (same as int32_t).
     *
     * The Unicode code point range is 0..0x10ffff.
     * All other values (negative or >=0x110000) are illegal as Unicode code points.
     * They may be used as sentinel values to indicate "done", "error"
     * or similar non-code point conditions.
     *
     * Before ICU 2.4 (Jitterbug 2146), UChar32 was defined
     * to be wchar_t if that is 32 bits wide (wchar_t may be signed or unsigned)
     * or else to be uint32_t.
     * That is, the definition of UChar32 was platform-dependent.
     *
     * @see U_SENTINEL
     * @stable ICU 2.4
     */
    typedef int32_t UChar32;



}

#endif //NEWCUT_ICU_UTF_H
