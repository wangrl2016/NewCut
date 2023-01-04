//
// Created by wangrl2016 on 2023/1/4.
//

#ifndef NEWCUT_STRING_PIECE_H
#define NEWCUT_STRING_PIECE_H

#include <string_view>

namespace base {

    using StringPiece = std::basic_string_view<char>;
    using StringPiece16 = std::basic_string_view<char16_t>;
    using WStringPiece = std::basic_string_view<wchar_t>;
}


#endif //NEWCUT_STRING_PIECE_H
