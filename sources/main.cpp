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
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    while (true) {
        std::cout << ">>> ";
        sbw_string g;
        std::getline(std::cin, g);
        if (g == "#exit") break;
        printf("input: %s\n", g.c_str());
    }

    std::cout << "\n";
    std::cout << "int128: " << sizeof(sbw_int128) << "\n";
    std::cout << "double: " << sizeof(sbw_double) << "\n";
    std::cout << "ldouble: œœ" << sizeof(sbw_ldouble) << "\n";

    return 0;
}
