//
// Created by wangrl2016 on 2023/1/4.
//

#include "base/strings/string_util.h"
#include "base/files/file_path.h"

namespace base {

    const FilePath::CharType kStringTerminator = FILE_PATH_LITERAL('\0');

    FilePath::StringPieceType::size_type FindDriveLetter(
            FilePath::StringPieceType path) {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
        if (path.length() >= 2 && path[1] == L':' &&
                ((path[0] >= L'A' && path[0] <= L'Z') ||
                (path[0] >= L'a' && path[0] <= L'z'))) {
            return 1;
        }
#endif
        return FilePath::StringType::npos;
    }

#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    // 比较两个路径是否相等，Windows中需要忽略盘的大小写
    bool EqualDriveLetterCaseInsensitive(FilePath::StringPieceType a,
                                         FilePath::StringPieceType b) {
        size_t a_letter_pos = FindDriveLetter(a);
        size_t b_letter_pos = FindDriveLetter(b);

        if (a_letter_pos = FilePath::StringType::npos ||
                b_letter_pos == FilePath::StringType::npos) {
            return a == b;
        }

        StringPieceType a_letter(a.substr(0, a_letter_pos + 1));
        StringPieceType b_letter(b.substr(0, b_letter_pos + 1));
        if (!StartWith(a_letter, b_letter, CompareCase::kInsensitiveASICC)) {
            return false;
        }

        StringPieceType a_rest(a.substr(a_letter_pos + 1));
        StringPieceType b_rest(b.substr(b_letter_pos + 1));
        return a_rest == b_rest;
    }
#endif

    FilePath::FilePath() = default;

    FilePath::FilePath(const FilePath& that) = default;

    FilePath::FilePath(StringPieceType path) : path_(path) {
        StringType::size_type nul_pos = path.find(kStringTerminator);
        if (nul_pos != StringType::npos) {
            path_.erase(nul_pos, StringType::npos);
        }
    }

    FilePath::~FilePath() = default;

    FilePath& FilePath::operator=(const FilePath& that) = default;

    FilePath& FilePath::operator=(FilePath&& that) noexcept = default;

    bool FilePath::operator==(const FilePath& that) const {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
        return EqualDriveLetterCaseInsensitive(this->path_, that->path_);
#else
        return path_ == that.path_;
#endif
    }

    FilePath FilePath::DirName() const {
        FilePath new_path(path_);


        return new_path;
    }

}
