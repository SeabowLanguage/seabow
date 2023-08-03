/**
 * @file base.hpp
 * @author LucaStarz
 * @brief Definition of all seabow's C++ types, includes of standart libraries and OS specifications.
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_BASE_HPP__
#define __SEABOW_BASE_HPP__

// C standart libraries
#include <thread>
#include <iostream>
#include <cstdint>
#include <string>
#include <bits/stdc++.h>

// OS specs
#ifdef _WIN32
    #define SEABOW_OS           "windows"
    #define SEABOW_SHARED       "dll"
    #include <windows.h>
#elif __APPLE__
    #define SEABOW_OS           "macos"
    #define SEABOW_SHARED       "so"
    #include <pthread.h>
#else
    #define SEABOW_OS           "linux"
    #define SEABOW_SHARED       "so"
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

#endif // __SEABOW_BASE_HPP__