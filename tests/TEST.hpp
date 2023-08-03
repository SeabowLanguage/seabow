/**
 * @file TEST.hpp
 * @author LucaStarz
 * @brief Test library for seabow's source code.
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_TEST_HPP__
#define __SEABOW_TEST_HPP__

#include <vector>
#include <string>
#include <iostream>

typedef struct Test {
    std::wstring name;
    std::wstring(*test_func)(void);
} Test;
std::vector<Test> TESTS;


#define TEST_FUNC(func_name) std::wstring func_name()
#define ADD_TEST(test_name, test_function) TESTS.push_back(Test{test_name, test_function})
#define TEST_SUCCEED() return L""
#define LAUNCH_TESTS(group_name) return run_tests(group_name)

#define IS_TRUE(test, message) if (!(test)) return message


inline bool run_test(Test t) {
    std::wstring msg = t.test_func();
    if (!msg.empty()) {
        std::wcout << L"\t\033[31mTest `" << t.name << L"`: error -> `" << msg << L"`" << L"\033[0m\n";
        return true;
    }
    else {
        std::wcout << L"\t\033[32mTest `" << t.name << L"`: passed" << L"\033[0m\n";
        return false;
    }
}

inline int run_tests(std::wstring group_name) {
    std::wcout << L"Starting `" << group_name << L"` tests...\n";
    unsigned int failed = 0, succeed = 0;

    // Run tests
    for (size_t i=0; i<TESTS.size(); i++)
        run_test(TESTS.at(i)) ? failed++ : succeed++;

    if (failed > 0) std::wcout << L"\033[31m"; else std::wcout << L"\033[32m";
    std::wcout << L"Ending `" << group_name << L"` tests with " << failed << L" failures and " << succeed << L" successes\033[0m\n";
    return failed > 0 ? 1 : 0;
}

#endif // __SEABOW_TEST_HPP__