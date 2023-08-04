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
#include "base.hpp"

typedef struct Test {
    sbw_string name;
    sbw_string(*test_func)(void);
} Test;
std::vector<Test> TESTS;


inline void INIT_TEST() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

#define TEST_FUNC(func_name) sbw_string func_name()
#define ADD_TEST(test_name, test_function) TESTS.push_back(Test{test_name, test_function})
#define TEST_SUCCEED() return ""
#define LAUNCH_TESTS(group_name) return run_tests(group_name)

#define IS_TRUE(test, message) if (!(test)) return message


inline bool run_test(Test t) {
    sbw_string msg = t.test_func();
    if (!msg.empty()) {
        sbw_print(5, "\t\033[31mTest `", t.name.c_str(), "`: error -> `", msg.c_str(), "`\033[0m\n");
        return true;
    }
    else {
        sbw_print(3, "\t\033[32mTest `", t.name.c_str(), "`: passed\033[0m\n");
        return false;
    }
}

inline int run_tests(sbw_string group_name) {
    sbw_print(3, "Starting `", group_name.c_str(), "` tests...\n");
    unsigned int failed = 0, succeed = 0;

    // Run tests
    for (size_t i=0; i<TESTS.size(); i++)
        run_test(TESTS.at(i)) ? failed++ : succeed++;

    if (failed > 0) sbw_print(1, "\033[31m"); else sbw_print(1, "\033[32m");
    sbw_print(7, "Ending `", group_name.c_str(), "` tests with ", std::to_string(failed).c_str(), " failures and ", std::to_string(succeed).c_str(), " successes\033[0m\n");
    return failed > 0 ? 1 : 0;
}

#endif // __SEABOW_TEST_HPP__