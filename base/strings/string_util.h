//
// Created by wr on 2023/1/4.
//

#ifndef NEWCUT_STRING_UTIL_H
#define NEWCUT_STRING_UTIL_H

#include "base/strings/string_piece.h"

namespace base {


    enum class CompareCase {
        kSensitive,
        kInsensitiveASCII
    };

    template<typename CharT,
            typename = std::enable_if_t<std::is_integral<CharT>::value>>
    constexpr CharT ToLowerASCII(CharT c) {
        return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
    }

    template<typename Char>
    struct CaseInsensitiveCompareASCII {
    public:
        bool operator()(Char x, Char y) const {
            return ToLowerASCII(x) == ToLowerASCII(y);
        }
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
}


#endif //NEWCUT_STRING_UTIL_H
