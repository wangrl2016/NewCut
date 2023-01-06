//
// Created by wangrl2016 on 2023/1/4.
//

#ifndef NEWCUT_STRING_PIECE_H
#define NEWCUT_STRING_PIECE_H

#include <string_view>
#include <ostream>

namespace base {

    using StringPiece = std::basic_string_view<char>;
    using StringPiece16 = std::basic_string_view<char16_t>;
    using WStringPiece = std::basic_string_view<wchar_t>;

    std::ostream& operator<<(std::ostream& o, StringPiece piece);

    // Not in the STL: convenience functions to output non-UTF-8 strings to an
    // 8-bit-width stream.
    std::ostream& operator<<(std::ostream& o, StringPiece16 piece);

    std::ostream& operator<<(std::ostream& o, WStringPiece piece);
}


#endif //NEWCUT_STRING_PIECE_H
