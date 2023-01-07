//
// Created by wangrl2016 on 2023/1/6.
//

#ifndef NEWCUT_UTF_STRING_CONVERSIONS_H
#define NEWCUT_UTF_STRING_CONVERSIONS_H

#include <string>
#include "base/strings/string_piece.h"

namespace base {
    // These convert between UTF-8, -16, and -32 strings. They are potentially slow,
    // so avoid unnecessary conversions. The low-level versions return a boolean
    // indicating whether the conversion was 100% valid. In this case, it will still
    // do the best it can and put the result in the output buffer. The version that
    // return strings ignore this error and just return the base conversion
    // possible.

    // UTF16 <-> UTF8
    bool UTF8ToUTF16(const char* src,
                     size_t src_len,
                     std::u16string* output);

    std::u16string UTF8ToUTF16(StringPiece utf8);

    bool UTF16ToUTF8(const char16_t* src,
                     size_t src_len,
                     std::string* output);

    std::string UTF16ToUTF8(StringPiece16 utf16);

    // Wide <-> UTF16
    bool WideToUTF16(const wchar_t* src,
                     size_t src_len,
                     std::u16string* output);

    std::u16string WideToUTF16(WStringPiece wide);

    bool UTF16ToWide(const char16_t* src,
                     size_t src_len,
                     std::wstring* output);

    std::wstring UTF16ToWide(StringPiece16 utf16);

    // UTF8 <-> Wide
    bool UTF8ToWide(const char* src, size_t src_len,
                    std::wstring* output);

    std::wstring UTF8ToWide(StringPiece utf8);

    bool WideToUTF8(const wchar_t* src, size_t src_len,
                    std::string* output);

    std::string WideToUTF8(WStringPiece wide);

    // This converts an ASCII string, typically a hardcoded constant, to a UTF16
    // string.
    std::u16string ASCIIToUTF16(StringPiece ascii);

    std::string UTF16ToASCII(StringPiece16 utf16);

#if defined(WCHAR_T_IS_UTF16)
    // This converts an ASCII string, typically a hardcoded constant, to a wide
    // strings.
    std::wstring ASCIIToWide(StringPiece ascii);

    // Converts to 7-bit ASCII by truncating. The result must be known to be ASCII
    // beforehand.
    std::string WideToASCII(WStringPiece wide);
#endif
}

#endif //NEWCUT_UTF_STRING_CONVERSIONS_H
