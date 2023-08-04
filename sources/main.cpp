/**
 * @file main.cpp
 * @author LucaStarz
 * @brief Main entry point of `seabow` system.
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2023
*/

#include "core/lexer.hpp"

int main(int argc, char **argv)
{
#ifdef _WIN32 // Enable UNICODE on Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    SetConsoleTitleA((LPCSTR)"seabow");
#endif

    while (true) {
        sbw_string g = sbw_input(">>> ");
        if (g == "#exit") break;

        sbw_print(3, "input: ", g.c_str(), "\n");
    }

    sbw_print(3, "\n\n---\nint128: ", std::to_string(sizeof(sbw_int128)).c_str(), "\n");
    sbw_print(3, "double: ", std::to_string(sizeof(sbw_double)).c_str(), "\n");
    sbw_print(3, "ldouble: ", std::to_string(sizeof(sbw_ldouble)).c_str(), "\n");
    sbw_print(3, "char: ", std::to_string(sizeof(sbw_char)).c_str(), "\n");
    sbw_print(3, "char of string: ", std::to_string(sizeof(sbw_string("²").at(0))).c_str(), "\n");
    sbw_print(1, "平仮名, ひらがな  ☠️\n---\n");

    return 0;
}
