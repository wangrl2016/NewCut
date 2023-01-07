//
// Created by wangrl2016 on 2023/1/6.
//

#include <limits>
#include <cstdint>

#include <ostream>
#include <type_traits>
#include <glog/logging.h>

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
    template<typename SrcChar, typename DestChar>
    struct SizeCoefficient {
        static_assert(sizeof(SrcChar) < sizeof(DestChar),
                      "Default case: from a smaller encoding to the bigger one");

        // ASCII symbols are encoded by one code unit in all encodings.
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
            SizeCoefficient<std::decay_t<SrcChar>, std::decay_t<DestChar>>::value;

    // Function overloads that write code_point to the output string. Output string
    // has to have enough space for the code point.
    //
    // Convenience typedef that checks whether the passed in type is integral (i.e.
    // bool, char, int or their extended versions) and is of the correct size.
    template<typename Char, size_t N>
    using EnableIfBitsAre = std::enable_if_t<std::is_integral<Char>::value &&
                                             CHAR_BIT * sizeof(Char) == N, bool>;

    template<typename Char, EnableIfBitsAre<Char, 8> = true>
    void UnicodeAppendUnsafe(Char* out,
                             size_t* size,
                             icu::UChar32 code_point) {
        U8_APPEND_UNSAFE(reinterpret_cast<uint8_t*>(out), *size, code_point);
    }

    template<typename Char, EnableIfBitsAre<Char, 16> = true>
    void UnicodeAppendUnsafe(Char* out,
                             size_t* size,
                             icu::UChar32 code_point) {
        U16_APPEND_UNSAFE(out, *size, code_point);
    }

    template<typename Char, EnableIfBitsAre<Char, 32> = true>
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

    template<typename DestChar>
    bool DoUTFConversion(const char16_t* src,
                         size_t src_len,
                         DestChar* dest,
                         size_t* dest_len) {
        bool success = true;

        auto ConvertSingleChar = [&success](char16_t in) -> icu::UChar32 {
            if (!U16_IS_SINGLE(in) || !IsValidCodePoint(in)) {
                success = false;
                return kErrorCodePoint;
            }
            return in;
        };

        size_t i = 0;
        // Always have another symbol in order to avoid checking boundaries in the
        // middle of the surrogate pair.
        while (i + 1 < src_len) {
            icu::UChar32 code_point;
            if (U16_IS_LEAD(src[i]) && U16_IS_TRAIL(src[i + 1])) {
                code_point = U16_GET_SUPPLEMENTARY(src[i], src[i + 1]);
                if (!IsValidCodePoint(code_point)) {
                    code_point = kErrorCodePoint;
                    success = false;
                }
                i += 2;
            } else {
                code_point = ConvertSingleChar(src[i]);
                i++;
            }
            UnicodeAppendUnsafe(dest, dest_len, code_point);
        }
        if (i < src_len) {
            UnicodeAppendUnsafe(dest, dest_len, ConvertSingleChar(src[i]));
        }
        return success;
    }

#if defined(WCHAR_T_IS_UTF32)

    template<typename DestChar>
    bool DoUTFConversion(const wchar_t* src,
                         size_t src_len,
                         DestChar* dest,
                         size_t* dest_len) {
        bool success = true;

        for (size_t i = 0; i < src_len; i++) {
            auto code_point = static_cast<icu::UChar32>(src[i]);

            if (!IsValidCodePoint(code_point)) {
                success = false;
                code_point = kErrorCodePoint;
            }
            UnicodeAppendUnsafe(dest, dest_len, code_point);
        }
        return success;
    }

#endif

    // Function template for generating all UTF conversions.
    template<typename InputString, typename DestString>
    bool UTFConversion(const InputString& src_str, DestString* dest_str) {
        if (IsStringASCII(src_str)) {
            dest_str->assign(src_str.begin(), src_str.end());
            return true;
        }

        dest_str->resize(src_str.length() *
                         size_coefficient_v<typename InputString::value_type,
                                 typename DestString::value_type>);

        // Empty string is ASCII => it OK to call operator[].
        auto* dest = &(*dest_str)[0];

        // ICU requires 32 bit numbers.
        size_t src_len = src_str.length();
        size_t dest_len = 0;

        bool res = DoUTFConversion(src_str.data(), src_len, dest, &dest_len);

        dest_str->resize(dest_len);
        dest_str->shrink_to_fit();

        return res;
    }

    // UTF16 <-> UTF8
    bool UTF8ToUTF16(const char* src, size_t src_len,
                     std::u16string* output) {
        return UTFConversion(StringPiece(src, src_len), output);
    }

    std::u16string UTF8ToUTF16(StringPiece utf8) {
        std::u16string ret;
        // Ignore the success flag of this call, it will do the best it can for
        // invalid input, which is what we want here.
        UTF8ToUTF16(utf8.data(), utf8.size(), &ret);
        return ret;
    }

    bool UTF16ToUTF8(const char16_t* src, size_t src_len, std::string* output) {
        return UTFConversion(StringPiece16(src, src_len), output);
    }

    std::string UTF16ToUTF8(StringPiece16 utf16) {
        std::string ret;
        // Ignore the success flag of this call, it will do the best it can for
        // invalid input, which is what we want here.
        UTF16ToUTF8(utf16.data(), utf16.length(), &ret);
        return ret;
    }

#if defined(WCHAR_T_IS_UTF16)
    // When wide == UTF-16 the conversions are a NOP.
    bool WideToUTF16(const wchar_t* src, size_t src_len, std::u16string* output) {
        output->assign(src, src + src_len);
        return true;
    }

    std::u16string WideToUTF16(WStringPiece wide) {
        return {wide.begin(), wide.end()};
    }

    bool UTF16ToWide(const char16_t* src, size_t src_len, std::wstring* output) {
        output->assign(src, src + src_len);
        return true;
    }

    std::wstring UTF16ToWide(StringPiece16 utf16) {
        return {utf16.gegin(), utf16.end()};
    }
#elif defined(WCHAR_T_IS_UTF32)

    bool WideToUTF16(const wchar_t* src, size_t src_len, std::u16string* output) {
        return UTFConversion(WStringPiece(src, src_len), output);
    }

    std::u16string WideToUTF16(WStringPiece wide) {
        std::u16string ret;
        // Ignore the success flag of this call, it will do the best it can for
        // invalid input, which is what we want here.
        WideToUTF16(wide.data(), wide.length(), &ret);
        return ret;
    }

    bool UTF16ToWide(const char16_t* src, size_t src_len, std::wstring* output) {
        return UTFConversion(StringPiece16(src, src_len), output);
    }

    std::wstring UTF16ToWide(StringPiece16 utf16) {
        std::wstring ret;
        // Ignore the success flag of this call, it will do the best it can for
        // invalid input, which is what we want here.
        UTF16ToWide(utf16.data(), utf16.length(), &ret);
        return ret;
    }

#endif

    // UTF-8 <-> Wide
    // UTF8ToWide is the same code, regardless of whether wide is 16 or 32 bits.
    bool UTF8ToWide(const char* src, size_t src_len, std::wstring* output) {
        return UTFConversion(StringPiece(src, src_len), output);
    }

    std::wstring UTF8ToWide(StringPiece utf8) {
        std::wstring ret;
        // Ignore the success flag of this call, it will do the best it can for
        // invalid input, which is what we  want here.
        UTF8ToWide(utf8.data(), utf8.length(), &ret);
        return ret;
    }

#if defined(WCHAR_T_ISUTF16)
    bool WideToUTF8(const wchar_t* src, size_t src_len, std::string* output) {
        return UTF16ToUTF8(as_u16cstr(src), src_len, output);
    }

    std::string WideToUTF8(WStringPiece wide) {
        return UTF16ToUTF8(StringPiece(as_u16cstr(wide), wide.size));
    }
#elif defined(WCHAR_T_IS_UTF32)

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

    std::u16string ASCIIToUTF16(StringPiece ascii) {
        DCHECK(IsStringASCII(ascii)) << ascii;
        return {ascii.begin(), ascii.end()};
    }

    std::string UTF16ToASCII(StringPiece16 utf16) {
        DCHECK(IsStringASCII(utf16)) << UTF16ToUTF8(utf16);
        return {utf16.begin(), utf16.end()};
    }

#if defined(WCHAR_T_IS_UTF16)
    std::wstring ASCIIToWide(StringPiece ascii) {
        DCHECK(IsStringASCII(ascii)) << ascii;
        return {ascii.begin(), ascii.end());
    }

    std::string WideToASCII(WStringPiece wide) {
        DCHECK(IsStringASCII(wide)) << wide;
        return {wide.begin(), wide.end()};
    }
#endif
}
