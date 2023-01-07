//
// Created by wangrl2016 on 2023/1/4.
//

#include "base/strings/string_util.h"
#include "base/strings/string_util_helper.h"

namespace base {
    template<typename T, typename CharT = typename T::value_type>
    std::basic_string<CharT> ToLowerASCIIImpl(T str) {
        std::basic_string<CharT> ret;
        ret.reserve(str.size());
        for (size_t i = 0; i < str.size(); i++) {
            ret.push_back(ToLowerASCII(str[i]));
        }
        return ret;
    }

    template<typename T, typename CharT = typename T::value_type>
    std::basic_string<CharT> ToUpperASCIIImpl(T str) {
        std::basic_string<CharT> ret;
        ret.reserve(str.size());
        for (size_t i = 0; i < str.size(); i++) {
            ret.push_back(ToUpperASCII(str[i]));
        }
        return ret;
    }

    std::string ToLowerASCII(StringPiece str) {
        return ToLowerASCIIImpl(str);
    }

    std::u16string ToLowerASCII(StringPiece16 str) {
        return ToLowerASCIIImpl(str);
    }

    std::string ToUpperASCII(StringPiece str) {
        return ToUpperASCIIImpl(str);
    }

    std::u16string ToUpperASCII(StringPiece16 str) {
        return ToUpperASCIIImpl(str);
    }

#define WHITESPACE_ASCII_NO_CR_LF \
        0x09,       /* CHARACTER TABULATION */ \
        0x0B,       /* LINE TABULATION */      \
        0x0C,       /* FROM FEED (FF) */       \
        0x20        /* SPACE */

#define WHITESPACE_ASCII \
        WHITESPACE_ASCII_NO_CR_LF,  /* Comment to make clang-format linebreak */ \
        0x0A,                       /* LINE FEED (LF) */                         \
        0x0D                        /* CARRIAGE RETURN (CR) */

#define WHITESPACE_UNICODE_NON_ASCII            \
        0x0085, /* NEXT LINE (NEL) */           \
        0x00A0, /* NO-BREAK SPACE */            \
        0x1680, /* OGHAM SPACE MARK */          \
        0x2000, /* EN QUAD */                   \
        0x2001, /* EM QUAD */                   \
        0x2002, /* EN SPACE */                  \
        0x2003, /* EM SPACE */                  \
        0x2004, /* THREE-PER-EM SPACE */        \
        0x2005, /* FOUR-PER-EM SPACE */         \
        0x2006, /* SIX-PER-EM SPACE */          \
        0x2007, /* FIGURE SPACE */              \
        0x2008, /* PUNCTUATION SPACE */         \
        0x2009, /* THIN SPACE */                \
        0x200A, /* HAIR SPACE */                \
        0x2028, /* LINE SEPARATOR */            \
        0x2029, /* PARAGRAPH SEPARATOR */       \
        0x202F, /* NARROW NO-BREAK SPACE */     \
        0x205F, /* MEDIUM MATHEMATICAL SPACE */ \
        0x3000  /* IDEOGRAPHIC SPACE */

#define WHITESPACE_UNICODE_NO_CR_LF \
        WHITESPACE_ASCII_NO_CR_LF, WHITESPACE_UNICODE_NON_ASCII

#define WHITESPACE_UNICODE WHITESPACE_ASCII, WHITESPACE_UNICODE_NON_ASCII

    const wchar_t kWhitespaceWide[] = {WHITESPACE_UNICODE, 0};
    const char16_t kWhitespaceUTF16[] = {WHITESPACE_UNICODE, 0};
    const char16_t kWhitespaceNoCrLfUTF16[] = {WHITESPACE_UNICODE_NO_CR_LF, 0};
    const char kWhitespaceASCII[] = {WHITESPACE_ASCII, 0};
    const char16_t kWhitespaceASCIIAs16[] = {WHITESPACE_ASCII, 0};
    const char kInfraAsciiWhitespace[] = {0x09, 0x0A, 0x0C, 0x0D, 0x20, 0};
    const char kUtf8ByteOrderMark[] = "\xEF\xBB\xBF";


    bool IsStringASCII(StringPiece str) {
        return DoIsStringASCII(str.data(), str.length());
    }

    bool IsStringASCII(StringPiece16 str) {
        return DoIsStringASCII(str.data(), str.length());
    }

#if defined(WCHAR_T_IS_UTF32)

    bool IsStringASCII(WStringPiece str) {
        return DoIsStringASCII(str.data(), str.length());
    }

#endif

    bool StartsWith(StringPiece str,
                    StringPiece search_for,
                    CompareCase case_sensitivity) {
        return StartsWithT(str, search_for, case_sensitivity);
    }

    bool StartsWith(StringPiece16 str,
                    StringPiece16 search_str,
                    CompareCase case_sensitivity) {
        return StartsWithT(str, search_str, case_sensitivity);
    }

    bool StartsWith(WStringPiece str,
                    WStringPiece search_for,
                    CompareCase case_sensitivity) {
        return StartsWithT(str, search_for, case_sensitivity);
    }

    char* WriteInto(std::string* str, size_t length_will_null) {
        return WriteIntoT(str, length_will_null);
    }

    char16_t* WriteInto(std::u16string* str, size_t length_with_null) {
        return WriteIntoT(str, length_with_null);
    }
}
