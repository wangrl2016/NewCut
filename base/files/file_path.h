//
// Created by wangrl2016 on 2023/1/4.
//

#ifndef NEWCUT_FILE_PATH_H
#define NEWCUT_FILE_PATH_H

// FilePath is a container for path-names stored in a platform's native string
// type, providing containers for manipulation in according to the
// platform's conventions for path-names. It supports the following path
// types:
//
//                          POSIX               Windows
// fundamental type         char[]              wchar_t[]
// Encoding                 unspecified*        UTF-16
// Separator                /                   \, tolerant of /
// Drive letters            no                  case-insensitive A-Z followed by :
// Alternate root           // (surprise!)      \\ (2 Separators) for UNC paths
//
// The encoding need not be specified on POSIX systems, although some
// POSIX-compliant systems do specify an encoding. Mac OS X uses UTF-8
// Linux does not specify an encoding, but in practice, the locale's
// character set may be used.
//
// For more arcane bits of path trivia, see below.
//
// FilePath objects are intended to be used anywhere paths are. An
// application may pass FilePath objects around internally, masking the
// underlying differences between systems, only differing in implementation
// where interfacing directly with the system. For example, a single
// OpenFile(const FilePath &) function may be made available, allowing all
// callers to operate without regard to the underlying implementation. On
// POSIX-like platforms, OpenFile might wrap fopen, and on Windows, it might
// wrap _wfopen_s, perhaps both by calling file_path.value().c_str(). This
// allows each platform to pass path-names around without requiring conversions
// between encodings, which has an impact on performance, but more importantly,
// has an impact on correctness on platforms that do not have well-defined
// encodings for path-names.
//
// Several methods are available to perform common operations on a FilePath
// object, such as determining the parent directory (DirName), isolating the
// final path component (BaseName), and appending a relative pathname string
// to an existing FilePath object (Append). These methods are highly
// recommended over attempting to split and concatenate strings directly.
// These methods are based purely on string manipulation and knowledge of
// platform-specific pathname conventions, and do not consult the filesystem
// at all, making them safe to use without fear of blocking on I/O operations.
// These methods do not function as mutators but instead return distinct
// instances of FilePath objects, and are therefore safe to use on const
// objects. The objects themselves are safe to share between threads.
//
// To aid in initialization of FilePath objects from string literals, a
// FILE_PATH_LITERAL macro is provided, which accounts for the difference
// between char[]-based path-names on POSIX systems and wchar_t[]-based
// path-names on Windows.
//
// As a precaution against premature truncation, paths can't contain NULs.
//
// Because a FilePath object should not be instantiated at the global scope,
// instead, use a FilePath::CharType[] and initialize it with
// FILE_PATH_LITERAL. At runtime, a FilePath object can be created from the
// character array. Example:
//
// const FilePath::CharType kLogFileName[ = = FILE_PATH_LITERAL("log.txt");
//
// void Function() {
//     FilePath log_file_path(kLogFileName);
//     [...]
// }
//
// WARNING: FilePath should ALWAYS de display with LTR direcionality, even
// when the UI language is RTL. This means you always need to pass filepath
// through WrapPathWithLTRFormatting() before displaying it in the
// TRL UI.
//
// This is a very common source of bugs, please try to keep this in mind.
//
// ARCANE BITS OF PATH TRIVIA
//
// - A double leading slash ia actually part of the POSIX standard. Systems
//   are allowed to treat // as alternate root, as Windows does for UNC
//   (Universal Naming Conversion, \\servername\share-name\directory\filename)
//   paths. Most POSIX systems don't do anything
//   special with two leading slashes, but FilePath handles this case properly
//   in case it ever comes across such a system. FilePath needs this support
//   for Windows UNC paths, anyway.
//
// - Windows treats c:\\ the same way it treats \\. This was intended to
//   allow older applications that require drive letters to support UNC paths
//   like \\server\share\path, by permitting c:\\server\share\path as an
//   equivalent. Since the OS treats these paths specially, FilePath needs
//   to de the same. Since windows can use either / or \ as the separator,
//   FilePath treats C://, C:\\, //, and \\ all equivalently.


#include <cstddef>
#include <string>
#include <string_view>

#include "base/strings/string_piece.h"

// Window-style drive letter support and pathname separator characters can be
// enabled and disabled independently, to aid testing. These #defines are
// here so that the same setting can be used in both implementation and
// in the unit test.
#if defined(_WIN32)
#define FILE_PATH_USES_DRIVE_LETTERS
#define FILE_PATH_USES_WIN_SEPARATORS
#endif

#if defined(_WIN32)
// ##表示粘连
#define FILE_PATH_LITERAL_INTERAL(x) L##x
#define FILE_PATH_LITERAL(x) FILE_PATH_LITERAL_INTERAL(x)
#else
#define FILE_PATH_LITERAL(x) x
#endif

namespace base {
    class FilePath {
    public:
#if defined(OS_WIN)
        // Defined for applications for Win32 and Win64. Always defined.
        // On Windows, for Unicode-aware applications, native pathnames are wchar_t
        // arrays encoded in UTF-16.
        typedef std::wstring StringType;
#else
        // On most platforms, native path-names are char arrays, and the encoding
        // may or may not be specified. On Mac OS X, native path-names are encoded
        // in UTF-8.
        typedef std::string StringType;
#endif

        typedef StringType::value_type CharType;

        typedef std::basic_string_view<CharType> StringPieceType;

        static constexpr CharType kSeparators[] =
#if defined(FILE_PATH_USES_WIN_SEPARATORS)
                FILE_PATH_LITERAL("\\/");
#else
                FILE_PATH_LITERAL("/");
#endif
        // std::size(kSeparators), i.e., the number of separators in kSeparators plus
        // one (the null terminator at the end of kSeparators).
        static constexpr size_t kSeparatorsLength = std::size(kSeparators);

        // The special path component meaning "this directory".
        static constexpr CharType kCurrentDirectory[] = FILE_PATH_LITERAL(".");

        // The special path component meaning "the parent directory".
        static constexpr CharType kParentDirectory[] = FILE_PATH_LITERAL("..");

        // The character used to identify a file extension.
        static constexpr CharType kExtensionSeparator = FILE_PATH_LITERAL('.');

        FilePath();

        FilePath(const FilePath& that);

        explicit FilePath(StringPieceType path);

        FilePath& operator=(const FilePath& that);

        ~FilePath();

        // Constructs FilePath with the contents of |that|, which is left in valid but
        // unspecified state.
        FilePath(FilePath&& that) noexcept;

        // Replaces the contents with those of |that|, which is left in valid but
        // unspecified state.
        FilePath& operator=(FilePath&& that) noexcept;

        bool operator==(const FilePath& that) const;

        bool operator!=(const FilePath& that) const;

        bool operator<(const FilePath& that) const {
            return path_ < that.path_;
        }

        [[nodiscard]] const StringType& value() const { return path_; }

        [[nodiscard]] bool empty() const { return path_.empty(); }

        void clear() { path_.clear(); }

        // Return true if |character| is in kSeparators.
        static bool IsSeparator(CharType character);

        // Return a vector of all the components of the provided path. It is
        // equivalent to calling DirName().value() on the path's root component,
        // and BaseName().value on each child component.
        //
        // To make sure this is lossless, so we can differentiate absolute and
        // relative paths, the root slash will be included even though no other
        // slashes will be. The precise behavior is:
        //
        // Posix: "/foo/bar" -> ["/", "foo", "bar"]
        // Windows: "C:\foo\bar" -> ["C:", "\\", "foo", "bar"]
        [[maybe_unused]] std::vector<FilePath::StringType> GetComponents() const;

        // Returns true if this FilePath is a parent or ancestor of the |child|.
        // Absolute and relative paths are accepted. i.e /ffo is a parent to /foo/bar,
        // and foo is a parent to foo/bar. Any ancestor is considered a parent i.e. /a
        // is a parent to both /a/b and /a/b/c. Does not convert paths to absolute,
        // follow symlinks or directory navigation (e.g. ".."). A path is *NOT* its
        // own parent.
        bool IsParent(const FilePath& child) const;

        // Returns true if this FilePath is a parent or ancestor of the |child|.
        // Absolute and relative paths are accepted i.e. /foo is a parent to /foo/bar,
        // and foo is a parent to foo/bar. Any ancestor is considered a parent i.e. /a
        // is a parent to both /a/b/ and /a/b/c. Does not convert paths to absolute,
        // follow symlinks or directory navigation (e.g. ".."). A path is *NOT* its
        // own parent.
        // If IsParent(child) holds, appends to path (if   non-NULL) the
        // relative path to child and returns true. For example, if parent
        // holds "/Users/john/Library/Application Support", child holds
        // "/Users/john/Library/Application Support/Google/Chrome/Default", and
        // *path holds "/Users/john/Library/Caches", then after
        // parent.AppendRelativePath(child, path) is called *path will hold
        // "/Users/john/Library/Caches/Google/Chrome/Default". Otherwise,
        // return false.
        bool AppendRelativePath(const FilePath& child, FilePath* path) const;

        // Returns a FilePath corresponding to the directory containing the path
        // named by this object, stripping away the file component. If this object
        // only contains one component, return a FilePath identifying
        // kCurrentDirectory. If this object already refers to the root directory,
        // returns a FilePath identifying the root directory. Please note that this
        // doesn't resolve directory navigation, e.g. the result for "../a" is "..".
        [[nodiscard]] FilePath DirName() const;

        // Returns a FilePath corresponding to the last path component of this
        // object, either a file or a directory. If this object already refers to
        // the root directory, returns a FilePath identifying the root directory;
        // this is the only situation in which BaseName will return an absolute path.
        FilePath BaseName() const;

        [[nodiscard]] StringType Extension() const;

        StringType FinalExtension() const;

        FilePath RemoveExtension() const;

        FilePath RemoveFinalExtension() const;

        FilePath InsertBeforeExtension(StringPieceType suffix) const;

        FilePath InsertBeforeExtensionASCII(StringPiece suffix) const;

        FilePath AddExtension(StringPieceType extension) const;

        FilePath AddExtensionASCII(StringPiece extension) const;

        FilePath ReplaceExtension(StringPieceType extension) const;

        bool MatchesExtension(StringPieceType extension) const;

        bool MatchesFinalExtension(StringPieceType extension) const;

        FilePath Append(StringPieceType component) const;

        FilePath Append(const FilePath& component) const;

        FilePath AppendASCII(StringPiece component) const;

        bool IsAbsolute() const;

        bool IsNetwork() const;

        bool EndsWithSeparator() const;

        FilePath StripTrailingSeparators() const;

        bool ReferencesParent() const;

        // Return a Unicode human-readable version of this path.
        // Warning: you can *not*, in general, go from a display name back to a real
        // path. Only use this when displaying paths to uses, not just when you
        // want to stuff a std::u16string into some other API.
        std::u16string LossyDisplayName() const;

        std::string MaybeAsASCII() const;

        // Return the path as UTF-8.
        //
        // This function is *unsafe* as there is no way to tell what encoding is
        // used in file name on POSIX system other than Mac,
        // although UTF-8 is practically used everywhere these days. To mitigate
        // the encoding issue, this function internally calls
        // SysNativeMBToWide() on POSIX systems other than Mac,
        // per assumption that the current locale's encoding is used in file
        // names, but this isn't a perfect solution.
        std::string AsUTF8Unsafe() const;

        std::u16string AsUTF16Unsafe() const;

        static FilePath FromASCII(StringPiece ascii);

        static FilePath FromUTF8Unsafe(StringPiece utf8);

        static FilePath FromUTF16Unsafe(StringPiece16 utf16);

        FilePath NormalizePathSeparators() const;

        FilePath NormalizePathSeparatorsTo(CharType separator) const;

        static int CompareIgnoreCase(StringPieceType string1,
                                     StringPieceType string2);

        static bool CompareEqualIgnoreCase(StringPieceType string1,
                                           StringPieceType string2) {
            return CompareIgnoreCase(string1, string2) == 0;
        }

        static bool CompareLessIgnoreCase(StringPieceType string1,
                                          StringPieceType string2) {
            return CompareIgnoreCase(string1, string2) < 0;
        }
    private:
        // Remove trailing separators from this object. If the path is absolute, it
        // will never be stripped any more than to refer to the absolute root
        // directory, so "////" will become "/", not "", A leading pair of
        // separators is never stripped, to support alternate roots. This is used to
        // support UNC paths on Windows.
        void StripTrailingSeparatorsInternal();

        StringType path_;
    };

    std::ostream& operator<<(std::ostream& out, const FilePath& file_path);
}

#endif //NEWCUT_FILE_PATH_H
