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
#if defined(_WIN32)
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

        const StringType& value() const { return path_; }

        bool empty() const { return path_.empty(); }

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
        std::vector<FilePath::StringType> GetComponents() const;

        bool IsParent(const FilePath& child) const;

        [[nodiscard]] FilePath DirName() const;
    private:
        StringType path_;
    };
}

#endif //NEWCUT_FILE_PATH_H
