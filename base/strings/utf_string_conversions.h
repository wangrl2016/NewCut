//
// Created by wangrl2016 on 2023/1/6.
//

#ifndef NEWCUT_UTF_STRING_CONVERSIONS_H
#define NEWCUT_UTF_STRING_CONVERSIONS_H

#include <string>

namespace base {
    bool WideToUTF8(const wchar_t* src, size_t src_len,
                    std::string* output);
}


#endif //NEWCUT_UTF_STRING_CONVERSIONS_H
