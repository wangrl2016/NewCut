//
// Created by wangrl2016 on 2023/1/6.
//

#ifndef NEWCUT_BUILD_CONFIG_H
#define NEWCUT_BUILD_CONFIG_H

#define BUILD_FLAG_CAT_INDIRECT(a, b) a ## b
#define BUILD_FLAG_CAT(a, b) BUILD_FLAG_CAT_INDIRECT(a, b)

#define BUILD_FLAG(flag) (BUILD_FLAG_CAT(BUILD_FLAG_INTERNAL_, flag)())

// A set of macros to use for platform detection.
#if defined(ANDROID)
#define OS_ANDROID 1
#elif defined(__APPLE__)
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define OS_IOS 1
#else
#define OS_MAC 1
#endif
#elif defined(__linux__)
#define OS_LINUX 1
#elif defined(_WIN32)
#define OS_WIN 1
#else
#error Please add support for your platform in build/build_config.h
#endif

// For access to standard POSIXish features, use OS_POSIX instead of a
// more specific macro.
#if defined(OS_LINUX) || defined(OS_MAC) || defined(OS_ANDROID) || defined(OS_IOS)
#define OS_POSIX 1
#endif

// Type detection for wchar_t
// 在Windows下wchar_t占2个字节（byte）
#if defined(OS_WIN)
#define WCHAR_T_IS_UTF16
#else
#define WCHAR_T_IS_UTF32
#endif

#endif //NEWCUT_BUILD_CONFIG_H
