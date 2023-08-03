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
#ifdef _WIN32
    SetConsoleOutputCP(1200);
    SetConsoleCP(1200);
#endif

    while (true) {
        std::wcout << L">>> ";
        sbw_string g;
        std::getline(std::wcin, g);
        if (g == L"#exit") break;
        std::wcout << L"input: " << g << L"\n";
    }

    std::wcout << L"\n";
    std::wcout << L"int128: " << sizeof(sbw_int128) << L"\n";
    std::wcout << L"double: " << sizeof(sbw_double) << L"\n";
    std::wcout << L"ldouble: " << sizeof(sbw_ldouble) << L"\n";
    std::wcout << L"char: " << sizeof(sbw_char) << L"\n";

    return 0;
}
