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

    /**
     * This value is intended for sentinel values for APIs that
     * (take or) return single code points (UChar32).
     * It is outside of the Unicode code point range 0..0x10ffff.
     *
     * For example, a "done" or "error" value in a new API
     * could be indicated with U_SENTINEL.
     *
     * ICU APIs designed before ICU 2.4 usually define service-specific "done"
     * values, mostly 0xffff.
     * Those may need to be distinguished from
     * actual U+ffff text contents by calling functions like
     * CharacterIterator::hasNext() or UnicodeString::length().
     *
     * @return -1
     * @see UChar32
     * @stable ICU 2.4
     */
    #define U_SENTINEL (-1)

    /**
     * \def UPRV_BLOCK_MACRO_BEGIN
     * Defined as the "do" keyword by default.
     * @internal
     */
    #ifndef UPRV_BLOCK_MACRO_BEGIN
    #define UPRV_BLOCK_MACRO_BEGIN do
    #endif

    /**
     * \def UPRV_BLOCK_MACRO_END
     * Defined as "while (false)" by default.
     * @internal
     */
    #ifndef UPRV_BLOCK_MACRO_END
    #define UPRV_BLOCK_MACRO_END while (false)
    #endif


}

#endif //NEWCUT_ICU_UTF_H
