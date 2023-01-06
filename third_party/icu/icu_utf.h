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

    // icu/icu4c/source/common/unicode/utf8.h

    /**
     * Internal bit vector for 3-byte UTF-8 validity check, for use in U8_IS_VALID_LEAD3_AND_T1.
     * Each bit indicates whether one lead byte + first trail byte pair starts a valid sequence.
     * Lead byte E0..EF bits 3..0 are used as byte index,
     * first trail byte bits 7..5 are used as bit index into that byte.
     * @see U8_IS_VALID_LEAD3_AND_T1
     * @internal
     */
    #define U8_LEAD3_T1_BITS "\x20\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x10\x30\x30"

    /**
     * Internal bit vector for 4-byte UTF-8 validity check, for use in U8_IS_VALID_LEAD4_AND_T1.
     * Each bit indicates whether one lead byte + first trail byte pair starts a valid sequence.
     * First trail byte bits 7..4 are used as byte index,
     * lead byte F0..F4 bits 2..0 are used as bit index into that byte.
     * @see U8_IS_VALID_LEAD4_AND_T1
     * @internal
     */
    #define U8_LEAD4_T1_BITS "\x00\x00\x00\x00\x00\x00\x00\x00\x1E\x0F\x0F\x0F\x00\x00\x00\x00"

    /**
     * Does this code unit (byte) encode a code point by itself (US-ASCII 0..0x7f)?
     * @param c 8-bit code unit (byte)
     * @return true or false
     * @stable ICU 2.4
     */
    #define U8_IS_SINGLE(c) (((c)&0x80)==0)

    /**
     * Append a code point to a string, overwriting 1 to 4 bytes.
     * The offset points to the current end of the string contents
     * and is advanced (post-increment).
     * "Unsafe" macro, assumes a valid code point and sufficient space in the string.
     * Otherwise, the result is undefined.
     *
     * @param s const uint8_t * string buffer
     * @param i string offset
     * @param c code point to append
     * @see U8_APPEND
     * @stable ICU 2.4
     */
    #define U8_APPEND_UNSAFE(s, i, c) UPRV_BLOCK_MACRO_BEGIN { \
        uint32_t __uc=(c); \
        if(__uc<=0x7f) { \
            (s)[(i)++]=(uint8_t)__uc; \
        } else { \
            if(__uc<=0x7ff) { \
                (s)[(i)++]=(uint8_t)((__uc>>6)|0xc0); \
            } else { \
                if(__uc<=0xffff) { \
                    (s)[(i)++]=(uint8_t)((__uc>>12)|0xe0); \
                } else { \
                    (s)[(i)++]=(uint8_t)((__uc>>18)|0xf0); \
                    (s)[(i)++]=(uint8_t)(((__uc>>12)&0x3f)|0x80); \
                } \
                (s)[(i)++]=(uint8_t)(((__uc>>6)&0x3f)|0x80); \
            } \
            (s)[(i)++]=(uint8_t)((__uc&0x3f)|0x80); \
        } \
    } UPRV_BLOCK_MACRO_END

    /**
     * Get a code point from a string at a code point boundary offset,
     * and advance the offset to the next code point boundary.
     * (Post-incrementing forward iteration.)
     * "Safe" macro, checks for illegal sequences and for string boundaries.
     *
     * The length can be negative for a NUL-terminated string.
     *
     * The offset may point to the lead byte of a multi-byte sequence,
     * in which case the macro will read the whole sequence.
     * If the offset points to a trail byte or an illegal UTF-8 sequence, then
     * c is set to a negative value.
     *
     * @param s const uint8_t * string
     * @param i int32_t string offset, must be i<length
     * @param length int32_t string length
     * @param c output UChar32 variable, set to <0 in case of an error
     * @see U8_NEXT_UNSAFE
     * @stable ICU 2.4
     */
    #define U8_NEXT(s, i, length, c) U8_INTERNAL_NEXT_OR_SUB(s, i, length, c, U_SENTINEL)

    /** @internal */
    #define U8_INTERNAL_NEXT_OR_SUB(s, i, length, c, sub) UPRV_BLOCK_MACRO_BEGIN { \
        (c)=(uint8_t)(s)[(i)++]; \
        if(!U8_IS_SINGLE(c)) { \
            uint8_t __t = 0; \
            if((i)!=(length) && \
                /* fetch/validate/assemble all but last trail byte */ \
                ((c)>=0xe0 ? \
                    ((c)<0xf0 ?  /* U+0800..U+FFFF except surrogates */ \
                        U8_LEAD3_T1_BITS[(c)&=0xf]&(1<<((__t=(s)[i])>>5)) && \
                        (__t&=0x3f, 1) \
                    :  /* U+10000..U+10FFFF */ \
                        ((c)-=0xf0)<=4 && \
                        U8_LEAD4_T1_BITS[(__t=(s)[i])>>4]&(1<<(c)) && \
                        ((c)=((c)<<6)|(__t&0x3f), ++(i)!=(length)) && \
                        (__t=(s)[i]-0x80)<=0x3f) && \
                    /* valid second-to-last trail byte */ \
                    ((c)=((c)<<6)|__t, ++(i)!=(length)) \
                :  /* U+0080..U+07FF */ \
                    (c)>=0xc2 && ((c)&=0x1f, 1)) && \
                /* last trail byte */ \
                (__t=(s)[i]-0x80)<=0x3f && \
                ((c)=((c)<<6)|__t, ++(i), 1)) { \
            } else { \
                (c)=(sub);  /* ill-formed*/ \
            } \
        } \
    } UPRV_BLOCK_MACRO_END

    // icu/icu4c/source/common/unicode/utf16.h

    /**
     * Append a code point to a string, overwriting 1 or 2 code units.
     * The offset points to the current end of the string contents
     * and is advanced (post-increment).
     * "Unsafe" macro, assumes a valid code point and sufficient space in the string.
     * Otherwise, the result is undefined.
     *
     * @param s const UChar * string buffer
     * @param i string offset
     * @param c code point to append
     * @see U16_APPEND
     * @stable ICU 2.4
     */
    #define U16_APPEND_UNSAFE(s, i, c) UPRV_BLOCK_MACRO_BEGIN { \
        if((uint32_t)(c)<=0xffff) { \
            (s)[(i)++]=(uint16_t)(c); \
        } else { \
            (s)[(i)++]=(uint16_t)(((c)>>10)+0xd7c0); \
            (s)[(i)++]=(uint16_t)(((c)&0x3ff)|0xdc00); \
        } \
    } UPRV_BLOCK_MACRO_END
}

#endif //NEWCUT_ICU_UTF_H
