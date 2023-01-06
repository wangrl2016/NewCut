//
// Created by wangrl2016 on 2023/1/6.
//

#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "third_party/icu/icu_utf.h"

namespace base {
    constexpr icu::UChar32 kErrorCodePoint = 0xFFFD;

    inline bool IsValidCodePoint(icu::UChar32 code_point) {
        // Excludes code points that are not Unicode scalar values, i.e.
        // surrogate code points ([0xD800, 0xDFFF]). Additionally, excludes
        // code points larger than 0x10FFFF (the highest codepoint allowed).
        // Non-characters and unassigned code points are allowed.
        // https://unicode.org/glossary/#unicode_scalar_value
        return (code_point >= 0 && code_point < 0xD800) ||
               (code_point >= 0xE000 && code_point <= 0x10FFFF);
    }

    // Size coefficient
    // The maximum number of code units in the destination encoding corresponding to
    // one code unit in the source encoding.
    template <typename SrcChar, typename DestChar>
    struct SizeCoefficient {
        static_assert(sizeof(SrcChar) < sizeof(DestChar),
                "Default case: from a smaller encoding to the bigger one");

        // ASCII symbols are encoded by one code unit in all encoding.
        static constexpr int value = 1;
    };

    template<>
    struct SizeCoefficient<char16_t, char> {
        // One UTF-16 code unit corresponding to at most 3 code units in UTF-8.
        static constexpr int value = 3;
    };

#if defined(WCHAR_T_IS_UTF32)
    template<>
    struct SizeCoefficient<wchar_t, char> {
        // UTF-8 uses at most 4 code units per character.
        static constexpr int value = 4;
    };

    template<>
    struct SizeCoefficient<wchar_t, char16_t> {
        // UTF16 uses at most 2 code units per character.
        static constexpr int value = 2;
    };
#endif

    // 类型退化
    template<typename SrcChar, typename DestChar>
    constexpr int size_coefficient_v =
            SizeCoefficient<std::decay_t<SrcChar>, std::decay<DestChar>>::value;

    // Function overloads that write code_point to the output string. Output string
    // has to have enough space for the code point.
    //
    // Convenience typedef that checks whether the passed in type is integral (i.e.
    // bool, char, int or their extended versions) and is of the correct size.
    template <typename Char, size_t N>
    using EnableIfBitsAre = std::enable_if_t<std::is_integral<Char>::value &&
                                             CHAR_BIT * sizeof(Char) == N, bool>;

    template<typename Char, EnableIfBitsAre<Char, 8> = true>
    void UnicodeAppendUnsafe(Char* out,
                             size_t* size,
                             icu::UChar32 code_point) {
        U8_APPEND_UNSAFE(reinterpret_cast<uint8_t*>(out), *size, code_point);
    }

    template <typename Char, EnableIfBitsAre<Char, 16> = true>
    void UnicodeAppendUnsafe(Char* out,
                             size_t* size,
                             icu::UChar32 code_point) {
        U16_APPEND_UNSAFE(out, *size, code_point);
    }

    template <typename Char, EnableIfBitsAre<Char, 32> = true>
    void UnicodeAppendUnsafe(Char* out,
                             size_t* size,
                             icu::UChar32 code_point) {
        out[(*size)++] = static_cast<Char>(code_point);
    }

    // Main driver of UTFConversion specialized for different Src encodings.
    // dest has to have enough room for the converted text.
    template<typename DestChar>
    bool DoUTFConversion(const char* src,
                         size_t src_len,
                         DestChar* dest,
                         size_t* dest_len) {
        bool success = true;

        for (size_t i = 0; i < src_len;) {
            icu::UChar32 code_point;
            U8_NEXT(reinterpret_cast<const uint8_t*>(src), i, src_len, code_point);

            if (!IsValidCodePoint(code_point)) {
                success = false;
                code_point = kErrorCodePoint;
            }

            UnicodeAppendUnsafe(dest, dest_len, code_point);
        }
        return success;
    }

    template <typename DestChar>
    bool DoUTFConversion(const char16_t * src,
                         size_t src_len,
                         DestChar* dest,
                         size_t* dest_len) {

    }

    // Function template for generating all UTF conversions.
    template<typename InputString, typename DestString>
    bool UTFConversion(const InputString& src_str, DestString* dest_str) {
        if (IsStringASCII(src_str)) {
            dest_str->assign(src_str.begin(), src_str.end());
            return true;
        }

        return true;
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
        return ret;
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
