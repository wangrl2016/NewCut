//
// Created by wangrl2016 on 2023/1/4.
//

#include <vector>
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

    FilePath::FilePath(FilePath&& that) noexcept {

    }

    FilePath& FilePath::operator=(const FilePath& that) = default;

    FilePath& FilePath::operator=(FilePath&& that) noexcept = default;

    bool FilePath::operator==(const FilePath& that) const {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
        return EqualDriveLetterCaseInsensitive(this->path_, that->path_);
#else
        return path_ == that.path_;
#endif
    }

    bool FilePath::operator!=(const FilePath& that) const {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
        return !EqualDriveLetterCaseInsentitive(this->path_, that_path_);
#else
        return path_ != that.path_;
#endif
    }

    bool FilePath::IsSeparator(char character) {
        for (size_t i = 0; i < kSeparatorsLength - 1; i++) {
            if (character == kSeparators[i]) {
                return true;
            }
        }
        return false;
    }

    std::vector<FilePath::StringType> FilePath::GetComponents() const {
        std::vector<StringType> ret_val;
        if (value().empty()) {
            return ret_val;
        }

        FilePath current = *this;
        FilePath base;

        // Capture path components.
        while (current != current.DirName()) {
            base = current.BaseName();

            current = current.DirName();
        }
        return ret_val;
    }

    FilePath FilePath::DirName() const {
        FilePath new_path(path_);
        new_path.StripTrailingSeparatorsInternal();

        // The drive letter, if any always needs to remain in the output. If there
        // is no drive letter, as will always be the case on platform which do not
        // support drive letters, letter will be npos, or -1, so comparisons and
        // resizes below using letter will still be valid.
        StringType::size_type letter = FindDriveLetter(new_path.path_);
        StringType::size_type last_separator =
                new_path.path_.find_last_of(kSeparators, StringType::npos,
                                            kSeparatorsLength - 1);
        if (last_separator == StringType::npos) {
            // path_ is the current directory.
            new_path.path_.resize(letter + 1);
        } else if (last_separator == letter + 1) {
            // path_ is in the root directory.
            new_path.path_.resize(letter + 2);
        } else if (last_separator == letter + 2 &&
                IsSeparator(new_path.path_[letter + 1])) {
            // path_ is in "//" (possibly with a drive letter); leave the double
            // separator intact indicating alternate root.
            new_path.path_.resize(letter + 3);
        } else if (last_separator != 0) {
            // path_ is somewhere else, trim the basename.
            new_path.path_.resize(last_separator);
        }

        new_path.StripTrailingSeparatorsInternal();
        if (!new_path.path_.length()) {
            new_path.path_ = kCurrentDirectory;
        }
        return new_path;
    }

    void FilePath::StripTrailingSeparatorsInternal() {
        // If there is no drive letter, start will be 1, which will prevent stripping
        // the leading separator if there is only one separator. If there is a drive
        // letter, start will be set appropriately to prevent stripping the first
        // separator following the drive letter, if a separator immediately follows
        // the drive letter.
        StringType::size_type start = FindDriveLetter(path_) + 2;

        StringType::size_type last_stripped = StringType::npos;
        for (StringType::size_type pos = path_.length();
             pos > start && IsSeparator(path_[pos - 1]); pos--) {
            // If the string only has two separators, and they are at the beginning,
            // don't strip them, unless the string began with more than two separators.
            if (pos != start + 1 || last_stripped == start + 2 ||
                !IsSeparator(path_[start - 1])) {
                path_.resize(pos - 1);
                last_stripped = pos;
            }
        }
    }
}
