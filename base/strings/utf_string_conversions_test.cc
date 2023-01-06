//
// Created by wangrl2016 on 2023/1/7.
//

#include <gtest/gtest.h>
#include "base/strings/utf_string_conversions.h"

namespace base {
    const wchar_t* const kConvertRoundtripCases[] {
        L"Google Video",
    };

    TEST(UTFStringConversionsTest, ConvertUTF8AndWide) {
        // We round-trip all the wide strings through UTF-8 to make sure everything
        // agrees on the conversion. This uses the stream operators to test them
        // simultaneously.
        for (auto* i : kConvertRoundtripCases) {
            std::ostringstream utf8;
            utf8 << WideToUTF8(i);      // wide转换为utf8编码
            std::wostringstream wide;
            wide << UTF8ToWide(utf8.str()); // 从utf8转换到wide

            EXPECT_EQ(i, wide.str());
        }
    }
}
