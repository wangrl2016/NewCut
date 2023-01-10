//
// Created by wangrl2016 on 2023/1/4.
//

#include <vector>
#include <glog/logging.h>
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
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

        if (a_letter_pos == FilePath::StringType::npos ||
                b_letter_pos == FilePath::StringType::npos) {
            return a == b;
        }

        FilePath::StringPieceType a_letter(a.substr(0, a_letter_pos + 1));
        FilePath::StringPieceType b_letter(b.substr(0, b_letter_pos + 1));
        if (!StartsWith(a_letter, b_letter, CompareCase::kInsensitiveASCII)) {
            return false;
        }

        FilePath::StringPieceType a_rest(a.substr(a_letter_pos + 1));
        FilePath::StringPieceType b_rest(b.substr(b_letter_pos + 1));
        return a_rest == b_rest;
    }
#endif

    bool IsPathAbsolute(FilePath::StringPieceType path) {
#if defined(FILE_PATH_USE_DRIVE_LETTERS)
        StringType::size_type letter = FindDriveLetter(path);
        if (letter != StringType::npos) {
            // Look for a separetor right after the drive specification.
            return path.length() > letter + 1 &&
                    FilePath::IsSeparator(path[letter + 1]);
        }
#else
        // Look for a separator in the first position.
        return path.length() > 0 && FilePath::IsSeparator(path[0]);
#endif
    }

    FilePath::FilePath() = default;

    FilePath::FilePath(const FilePath& that) = default;

    FilePath::FilePath(StringPieceType path) : path_(path) {
        StringType::size_type nul_pos = path.find(kStringTerminator);
        if (nul_pos != StringType::npos) {
            path_.erase(nul_pos, StringType::npos);
        }
    }

    FilePath::~FilePath() = default;

    FilePath::FilePath(FilePath&& that) noexcept = default;

    FilePath& FilePath::operator=(const FilePath& that) = default;

    FilePath& FilePath::operator=(FilePath&& that) noexcept = default;

    bool FilePath::operator==(const FilePath& that) const {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
        return EqualDriveLetterCaseInsensitive(this->path_, that.path_);
#else
        return path_ == that.path_;
#endif
    }

    bool FilePath::operator!=(const FilePath& that) const {
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
        return !EqualDriveLetterCaseInsensitive(this->path_, that.path_);
#else
        return path_ != that.path_;
#endif
    }

    bool FilePath::IsSeparator(CharType character) {
        for (size_t i = 0; i < kSeparatorsLength - 1; i++) {
            if (character == kSeparators[i]) {
                return true;
            }
        }
        return false;
    }

    std::vector<FilePath::StringType> FilePath::GetComponents() const {
        std::vector<StringType> ret_val;
//        if (value().empty()) {
//            return ret_val;
//        }
//
//        FilePath current = *this;
//        FilePath base;
//
//        // Capture path components.
//        while (current != current.DirName()) {
//            base = current.BaseName();
//
//            current = current.DirName();
//        }
        return ret_val;
    }

    bool FilePath::IsParent(const FilePath& child) const {
        return false;
    }

    bool FilePath::AppendRelativePath(const FilePath& child, FilePath* path) const {
        return false;
    }

    FilePath FilePath::DirName() const {
        FilePath new_path(path_);
        new_path.StripTrailingSeparatorsInternal();

        // The drive letter, if any always needs to remain in the output. If there
        // is no drive letter, as will always be the case on platform which do not
        // support drive letters, letter will be npos, or -1, so comparisons and
        // resizes below using letter will still be valid.
        StringType::size_type letter = FindDriveLetter(new_path.path_);
        // 找到字符串中的任意字符
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

    FilePath FilePath::BaseName() const {
        FilePath new_path(path_);
        new_path.StripTrailingSeparatorsInternal();

        // The drive letter, if any, is always stripped.
        StringType::size_type letter = FindDriveLetter(new_path.path_);
        if (letter != StringType::npos) {
            new_path.path_.erase(0, letter + 1);
        }

        // Keep everything after the final separator, but if the pathname is only
        // one character, and it's a separator, leave it alone.
        StringType::size_type last_separator =
                new_path.path_.find_last_of(kSeparators, StringType::npos,
                                            kSeparatorsLength - 1);
        if (last_separator != StringType::npos &&
                last_separator < new_path.path_.length() - 1) {
            new_path.path_.erase(0, last_separator + 1);
        }

        return new_path;
    }

    FilePath FilePath::Append(StringPieceType component) const {
        StringPieceType appended = component;
        StringType without_nul;

        StringType::size_type nul_pos = component.find(kStringTerminator);
        if (nul_pos != StringPieceType::npos) {
            without_nul = StringType(component.substr(0, nul_pos));
            appended = StringPieceType(without_nul);
        }

        DCHECK(!IsPathAbsolute(appended));

        if (path_.compare(kCurrentDirectory) == 0 && !appended.empty()) {
            // Append normally doesn't do any normalization, but as a special case,
            // when appending to kCurrentDirectory, just return a new path for the
            // component argument. Appending component to kCurrentDirectory would
            // serve no purpose other than needlessly lengthening the path, and
            // it's likely in practice to wind up with FilePath objects containing
            // only kCurrentDirectory when call DirName on a single relative path
            // component.
            return FilePath(appended);
        }

        FilePath new_path(path_);
        new_path.StripTrailingSeparatorsInternal();

        // Don't append a separator if the path is empty (indicating the current
        // directory) or if the path component is empty (indicating nothing to
        // append).
        if (!appended.empty() && !new_path.path_.empty()) {
            // Don't append a separator if the path still ends with a trailing
            // separator after stripping (indicating the root directory).
            if (!IsSeparator(new_path.path_.back())) {
                // Don't append a separator if the path is just a drive letter.
                if (FindDriveLetter(new_path.path_) + 1 != new_path.path_.length()) {
                    new_path.path_.append(1, kSeparators[0]);
                }
            }
        }
        new_path.path_.append(appended.data(), appended.size());
        return new_path;
    }

    FilePath FilePath::Append(const FilePath& component) const {
        return Append(component.value());
    }

    FilePath FilePath::AppendASCII(StringPiece component) const {
        DCHECK(base::IsStringASCII(component));
#if defined(OS_WIN)
        return Append(UTF8ToWide(component));
#elif defined(OS_POSIX)
        return Append(component);
#endif
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
