//
// Created by wangrl2016 on 2023/1/6.
//

#include "base/strings/utf_string_conversions.h"

namespace base {
    // Function template for generating all UTF conversions.
    template<typename InputString, typename DestString>
    bool UTFConversion(const InputString& str_str, DestString* dest_str) {

    }


#if defined(_WIN32)
    // Windows的wchar_t类型表示utf-16
    bool WideToUTF8(const wchar_t* src, size_t src_len, std::string* output) {
        return UTF16ToUTF8(as_u16cstr(src), src_len, output);
    }

    std::string WideToUTF8(WStringPiece wide) {
        return UTF16ToUTF8(StringPiece16(as_u16cstr(wide), wide.size()));
    }
#else
    bool WideToUTF8(const wchar_t* src, size_t src_len, std::string* output) {
        return UTFConversion(WStringPiece(src, src_len), output);
    }

    std::string WideToUTF8(WStringPiece wide) {
        std::string ret;
        // Ignore the success flag of this call, it will do the best it can for
        // invalid input, which is what we want here.
        WideToUTF8(wide.data(), wide.length(), &ret);
    }
#endif

    // UTF8ToWide is the same code, regardless of whether wide is 16 or 32 bits.
    bool UTF8ToWide(const char* src, size_t src_len, std::string* output) {
        return UTFConversion(StringPiece(src, src_len), output);
    }

    std::wstring UTF8ToWide(StringPiece utf8) {
        std::wstring ret;
        // Ignore the success flag of this call, it will do the best it can for
        // invalid input, which is what we want here.
        UTF8ToWide(utf8.data(), utf8.length(), &ret);
    }
}
