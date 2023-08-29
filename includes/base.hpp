/**
 * @file base.hpp
 * @author LucaStarz
 * @brief Description of all basic stuff of the seabow command.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_BASE_HPP__
#define __SEABOW_BASE_HPP__

// C standart libraries
#include <thread>
#include <iostream>
#include <cstdint>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>

// OS specs
#ifdef _WIN32
    #define SEABOW_OS           "windows"
    #define SEABOW_SHARED       "dll"
    #include <windows.h>
    #include <codecvt>
    #include "bits/stdc++.h"
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> SBW_WINDOWS_STRING_CONVERTER;
#elif __APPLE__
    #define SEABOW_OS           "macos"
    #define SEABOW_SHARED       "so"
    #include <pthread.h>
#else
    #define SEABOW_OS           "linux"
    #define SEABOW_SHARED       "so"
    #include <bits/stdc++.h>
#endif

// Seabow types
typedef void        sbw_none;
typedef int8_t      sbw_byte;
typedef uint8_t     sbw_ubyte;
typedef int16_t     sbw_short;
typedef uint16_t    sbw_ushort;
typedef int32_t     sbw_int;
typedef uint32_t    sbw_uint;
typedef int64_t     sbw_long;
typedef uint64_t    sbw_ulong;
typedef __int128_t  sbw_int128;
typedef __uint128_t sbw_uint128;
typedef float_t     sbw_float;
typedef double_t    sbw_double;
typedef __float128  sbw_ldouble;
typedef uint8_t     sbw_bool;
typedef char32_t    sbw_char;
typedef std::string sbw_string;

/**
 * @brief Print multiple seabow strings on console.
 * @param n The number of seabow strings to print.
 * @param ... All seabow strings to print on console. 
*/
inline void sbw_print(sbw_ulong n, ...)
{
    va_list args;
    va_start(args, n);
    for (sbw_ulong i=0; i<n; i++) {
        const char *txt = va_arg(args, const char*);

#ifndef _WIN32
        std::cout << txt;
#else
        wprintf(L"%s", txt);
#endif

    }
    va_end(args);
}

/**
 * @brief Get the user's input on the console.
 * @param txt The text to print before read user's input.
 * @return The seabow string that represents the user's input.
*/
inline sbw_string sbw_input(const char *txt)
{
    sbw_print(1, txt);
    sbw_string result;

#ifndef _WIN32
    std::getline(std::cin, result);
#else
    std::getline(std::cin, result);
#endif
    
    return result;
}

#endif // __SEABOW_BASE_HPP__