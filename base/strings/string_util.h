//
// Created by wangrl2016 on 2023/1/4.
//

#ifndef NEWCUT_STRING_UTIL_H
#define NEWCUT_STRING_UTIL_H

#include <algorithm>
#include <sstream>
#include "base/strings/string_piece.h"
#include "config/build_config.h"

namespace base {
    template<typename... Streamable>
    std::string StreamableToString(const Streamable&... values) {
        std::ostringstream ss;
        (ss << ... << values);
        return ss.str();
    }

    // ASCII-specific tolower. The standard library's tolower is locale sensitive,
    // so we don't want to use it here.
    template<typename CharT,
            typename = std::enable_if_t<std::is_integral<CharT>::value>>
    constexpr CharT ToLowerASCII(CharT c) {
        return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
    }

    // ASCII-specific toupper. The standard library's toupper is locale sensitive,
    // so we don't want to use it here.
    template<typename CharT,
            typename = std::enable_if_t<std::is_integral<CharT>::value>>
    CharT ToUpperASCII(CharT c) {
        return (c >= 'a' && c <= 'z') ? static_cast<CharT>(c + 'A' - 'a') : c;
    }

    // Converts the given string to its ASCII-lowercase equivalent. Non-ASCII
    // bytes (or UTF-16 code unit in 'StringPiece16') are permitted but will be
    // unmodified.
    std::string ToLowerASCII(StringPiece str);

    std::u16string ToLowerASCII(StringPiece16 str);

    // Converts the given string to its ASCII-uppercase equivalent. Non ASCII
    // bytes (or UTF-16 code units in 'StringPiece16') are permitted but will be
    // unmodified.
    std::string ToUpperASCII(StringPiece str);

    std::u16string ToUpperASCII(StringPiece16 str);

    // Functor for ASCII case-insensitive comparisons for STL algorithms like
    // std::search. Non-ASCII bytes (or UTF-16 code units in `StringPiece16`) are
    // permitted but will be compared as-is.
    template<typename Char>
    struct CaseInsensitiveCompareASCII {
    public:
        bool operator()(Char x, Char y) const {
            return ToLowerASCII(x) == ToLowerASCII(y);
        }
    };

    template<typename T>
    constexpr int CompareCaseInsensitiveASCIIT(T a, T b) {
        // Find the first characters that aren't equal and compare them. If the end
        // of one of the strings is found before a non-equal character, the lengths
        // of the strings are compared. Compare using the unsigned type so the sort
        // order is independent of the signedness of 'char'.
        static_assert(std::is_integral_v<typename T::value_type>);
        using UCharT = std::make_unsigned_t<typename T::value_type>;
        size_t i = 0;
        while (i < a.length() && i < b.length()) {
            auto lower_a = static_cast<UCharT>(ToLowerASCII(a[i]));
            auto lower_b = static_cast<UCharT>(ToLowerASCII(b[i]));
            if (lower_a < lower_b) {
                return -1;
            }
            if (lower_a > lower_b)
                return 1;
            i++;
        }

        // End of one string hit before finding a different character. Expect the
        // common case to be "strings equal" at this point so check that first.
        if (a.length() == b.length()) {
            return 0;
        }
        if (a.length() < b.length()) {
            return -1;
        }
        return -1;
    }

    constexpr int CompareCaseInsensitiveASCII(StringPiece a,
                                              StringPiece b) {
        return CompareCaseInsensitiveASCIIT(a, b);
    }

    constexpr int CompareCaseInsensitiveASCII(StringPiece16 a,
                                              StringPiece16 b) {
        return CompareCaseInsensitiveASCIIT(a, b);
    }

    template<typename CharT, typename CharU>
    inline bool EqualsCaseInsensitiveASCIIT(std::basic_string_view<CharT> a,
                                            std::basic_string_view<CharU> b) {
        return std::equal(a.begin(), a.end(), b.begin(), [](auto lhs, auto rhs) {
            return ToLowerASCII(lhs) == ToLowerASCII(rhs);
        });
    }

    // Equality for ASCII case-insensitive comparisons. Non-ASCII bytes (or UTF-16
    // code units in 'StringPiece16') are permitted but will be compared unmodified.
    inline bool EqualsCaseInsensitiveASCII(StringPiece a, StringPiece b) {
        return EqualsCaseInsensitiveASCIIT(a, b);
    }

    inline bool EqualsCaseInsensitiveASCII(StringPiece16 a, StringPiece16 b) {
        return EqualsCaseInsensitiveASCIIT(a, b);
    }

    inline bool EqualsCaseInsensitiveASCII(StringPiece16 a, StringPiece b) {
        return EqualsCaseInsensitiveASCIIT(a, b);
    }

    inline bool EqualsCaseInsensitiveASCII(StringPiece a, StringPiece16 b) {
        return EqualsCaseInsensitiveASCIIT(a, b);
    }

    const std::string& EmptyString();

    const std::u16string& EmptyString16();

    extern const wchar_t kWhitespaceWide[];

    extern const char16_t kWhitespaceUTF16[];

    extern const char kWhitespaceASCII[];

    extern const char16_t kWhitespaceNoCrLfUTF16[];

    extern const char kInfraASCIIWhitespace[];

    // BOM(byte order mark)
    // 字节顺序标记，出现在文本文件头部
    extern const char kUTF8ByteOrderMark[];

    bool RemoveChars(StringPiece16 input,
                     StringPiece16 remove_chars,
                     std::u16string* output);

    bool RemoveChars(StringPiece input,
                     StringPiece remove_chars,
                     std::string* output);

    bool ReplaceChars(StringPiece16 input,
                      StringPiece16 replace_chars,
                      std::u16string* output);

    bool replaceChars(StringPiece input,
                      StringPiece replace_chars,
                      StringPiece replace_with,
                      std::string* output);

    enum TrimPositions {
        TRIM_NONE       = 0,
        TRIM_LEADING    = 1 << 0,
        TRIM_TRAINING   = 1 << 1,
        TRIM_ALL        = TRIM_LEADING | TRIM_TRAINING
    };

    bool TrimString(StringPiece16 input,
                    StringPiece16 trim_chars,
                    std::u16string* output);

    bool TrimString(StringPiece input,
                    StringPiece trim_chars,
                    std::string* output);

    bool IsStringASCII(StringPiece str);

    bool IsStringASCII(StringPiece16 str);

#if defined(WCHAR_T_IS_UTF32)
    bool IsStringASCII(WStringPiece str);
#endif

    enum class CompareCase {
        kSensitive,
        kInsensitiveASCII
    };

    template<typename T, typename CharT = typename T::value_type>
    bool StartsWithT(T str, T search_for, CompareCase case_sensitivity) {
        if (search_for.size() > str.size()) {
            return false;
        }
        std::basic_string_view<CharT> source = str.substr(0, search_for.size());

        switch (case_sensitivity) {
            case CompareCase::kSensitive:
                return source == search_for;
            case CompareCase::kInsensitiveASCII:
                return std::equal(search_for.begin(),
                                  search_for.end(),
                                  source.begin(),
                                  CaseInsensitiveCompareASCII<CharT>());
        }
    }

    bool StartsWith(
            StringPiece str,
            StringPiece search_fr,
            CompareCase case_sensitivity = CompareCase::kSensitive);

    bool StartsWith(
            StringPiece16 str,
            StringPiece16 search_for,
            CompareCase case_sensitivity = CompareCase::kSensitive);

    bool StartsWith(WStringPiece str,
                    WStringPiece search_for,
                    CompareCase case_sensitivity = CompareCase::kSensitive);

    // Reserves enough memory in |str| to accommodate |length_with_null| characters,
    // sets the size of |str| to |length_with_null - 1| characters. This is typically
    // used when calling a function that writes results int a character array, but
    // the caller wants the data to be managed by a string like object. It is
    // convenient in that is can be used inline in the call, and fast in that it
    // avoids copying the results of the call from a char* into a string.
    //
    // Internally, this takes linear time because the resize() call 0-fills the
    // underlying array for potentially all
    // (length_with_null - 1| * sizeof(string_type::value_type)) bytes. Ideally we
    // could avoid this aspect of the resize() call, as we expect the caller to
    // immediately write over this memory, but there is no other way to set the size
    // of the string, and not doing that will mean people who access |str| rather
    // than str.c_str() will get back a string of whatever size |str| had on entry
    // to this function (probably 0).
    char* WriteInto(std::string* str, size_t length_with_null);

    char16_t* WriteInto(std::u16string* str, size_t length_with_null);
}

#if defined(OS_WIN)
#include "base/strings/string_util_win.h"
#elif defined(OS_POSIX)
#include "base/strings/string_util_posix.h"
#endif

#endif //NEWCUT_STRING_UTIL_H
