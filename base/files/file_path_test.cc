//
// Created by wangrl2016 on 2023/1/5.
//

#include <gtest/gtest.h>
#include "base/files/file_path.h"
#include "config/build_config.h"

namespace base {
    // This macro helps avoid wrapped lines in the test structs.
    #define FPL(x) FILE_PATH_LITERAL(x)


    typedef testing::Test FilePathTest;

    struct UnaryTestData {
        FilePath::StringPieceType input;
        FilePath::StringPieceType expected;
    };

    TEST_F(FilePathTest, DirName) {
        const struct UnaryTestData cases[] = {
                {FPL(""), FPL(".")},
                {FPL("aa"), FPL(".")},
                {FPL("/aa/bb"), FPL("/aa")},
                {FPL("/aa/bb/"), FPL("/aa")},
                {FPL("/aa/bb//"), FPL("/aa")},
                {FPL("/aa/bb/ccc"), FPL("/aa/bb")},
                {FPL("/aa"), FPL("/")},
                {FPL("/aa/"), FPL("/")},
                {FPL("/"), FPL("/")},
                {FPL("//"), FPL("//")},
                {FPL("///"), FPL("/")},
                {FPL("aa/"), FPL(".")},
                {FPL("aa/bb"), FPL("aa")},
                {FPL("aa/bb/"), FPL("aa")},
                {FPL("aa/bb//"), FPL("aa")},
                {FPL("aa//bb//"), FPL("aa")},
                {FPL("aa//bb/"), FPL("aa")},
                {FPL("aa//bb"), FPL("aa")},
                {FPL("//aa/bb"), FPL("//aa")},
                {FPL("//aa/"), FPL("//")},
                {FPL("//aa"), FPL("//")},
                {FPL("0:"), FPL(".")},
                {FPL("@:"), FPL(".")},
                {FPL("[:"), FPL(".")},
                {FPL("`:"), FPL(".")},
                {FPL("{:"), FPL(".")},
                {FPL("\xB3:"), FPL(".")},
                {FPL("\xC5:"), FPL(".")},
                {FPL("/aa/../bb/cc"), FPL("/aa/../bb")},
#if defined(OS_WIN)
                {FPL("\x0143:"), FPL(".")},
#endif
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
                {FPL("c:"), FPL("c:")},
                {FPL("C:"), FPL("C:")},
                {FPL("A:"), FPL("A:")},
                {FPL("Z:"), FPL("Z:")},
                {FPL("a:"), FPL("a:")},
                {FPL("z:"), FPL("z:")},
                {FPL("c:aa"), FPL("c:")},
                {FPL("c:/"), FPL("c:/")},
                {FPL("c://"), FPL("c://")},
                {FPL("c:///"), FPL("c:/")},
                {FPL("c:/aa"), FPL("c:/")},
                {FPL("c:/aa/"), FPL("c:/")},
                {FPL("c:/aa/bb"), FPL("c:/aa")},
                {FPL("c:aa/bb"), FPL("c:aa")},
#endif
#if defined(FILE_PATH_USES_WIN_SEPARATORS)
                {FPL("\\aa\\bb"), FPL("\\aa")},
                {FPL("\\aa\\bb\\"), FPL("\\aa")},
                {FPL("\\aa\\bb\\\\"), FPL("\\aa")},
                {FPL("\\aa\\bb\\ccc"), FPL("\\aa\\bb")},
                {FPL("\\aa"), FPL("\\")},
                {FPL("\\aa\\"), FPL("\\")},
                {FPL("\\"), FPL("\\")},
                {FPL("\\\\"), FPL("\\\\")},
                {FPL("\\\\\\"), FPL("\\")},
                {FPL("aa\\"), FPL(".")},
                {FPL("aa\\bb"), FPL("aa")},
                {FPL("aa\\bb\\"), FPL("aa")},
                {FPL("aa\\bb\\\\"), FPL("aa")},
                {FPL("aa\\\\bb\\\\"), FPL("aa")},
                {FPL("aa\\\\bb\\"), FPL("aa")},
                {FPL("aa\\\\bb"), FPL("aa")},
                {FPL("\\\\aa\\bb"), FPL("\\\\aa")},
                {FPL("\\\\aa\\"), FPL("\\\\")},
                {FPL("\\\\aa"), FPL("\\\\")},
                {FPL("aa\\..\\bb\\c"), FPL("aa\\..\\bb")},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
                {FPL("c:\\"), FPL("c:\\")},
                {FPL("c:\\\\"), FPL("c:\\\\")},
                {FPL("c:\\\\\\"), FPL("c:\\")},
                {FPL("c:\\aa"), FPL("c:\\")},
                {FPL("c:\\aa\\"), FPL("c:\\")},
                {FPL("c:\\aa\\bb"), FPL("c:\\aa")},
                {FPL("c:aa\\bb"), FPL("c:aa")},
#endif
#endif
        };
        for (size_t i = 0; i < std::size(cases); i++) {
            FilePath input(cases[i].input);
            FilePath observed = input.DirName();
            EXPECT_EQ(FilePath::StringType(cases[i].expected),
                      observed.value()) << "i: " << i << ", input " << input.value();
        }
    }

    TEST_F(FilePathTest, BaseName) {
        const struct UnaryTestData cases[] = {
                {FPL(""), FPL("")},
        };
        for (size_t i = 0; i < std::size(cases); i++) {
            FilePath input(cases[i].input);
            FilePath observed = input.BaseName();
            EXPECT_EQ(FilePath::StringType(cases[i].expected),
                      observed.value()) << "i: " << i << ", input: " << input.value();
        }
    }
}