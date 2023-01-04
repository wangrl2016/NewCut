//
// Created by wangrl2016 on 2023/1/5.
//

#include <gtest/gtest.h>
#include "base/files/file_path.h"


namespace base {
    // This macro helps avoid wrapped lines in the test structs.
    #define FPL(x) FILE_PATH_LITERAL(x)


    typedef testing::Test FilePathTest;

    struct UnaryTestData {
        FilePath::StringPieceType input;
        FilePath::StringPieceType expected;
    };

    TEST_F(FilePathTest, DirName) {
//        const struct UnaryTestData cases[] = {
//                {FPL(""), FPL(".")},
//        };
//        for (size_t i = 0; i < std::size(cases); i++) {
//            FilePath input(cases[i].input);
//            FilePath observed = input.DirName();
//            EXPECT_EQ(FilePath::StringType(cases[i].expected),
//                      observed.value()) << "i: " << i << ", input " << input.value();
//        }
    }
}