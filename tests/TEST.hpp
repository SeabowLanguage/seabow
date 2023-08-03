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
    std::string name;
    std::string(*test_func)(void);
} Test;
std::vector<Test> TESTS;


#define TEST_FUNC(func_name) std::string func_name()
#define ADD_TEST(test_name, test_function) TESTS.push_back(Test{test_name, test_function})
#define TEST_SUCCEED() return ""
#define LAUNCH_TESTS(group_name) return run_tests(group_name)

#define IS_TRUE(test, message) if (!(test)) return message


inline bool run_test(Test t) {
    std::string msg = t.test_func();
    if (!msg.empty()) {
        std::cout << "\t\033[31mTest `" << t.name << "`: error -> `" << msg << "`" << "\033[0m\n";
        return true;
    }
    else {
        std::cout << "\t\033[32mTest `" << t.name << "`: passed" << "\033[0m\n";
        return false;
    }
}

inline int run_tests(std::string group_name) {
    std::cout << "Starting `" << group_name << "` tests...\n";
    unsigned int failed = 0, succeed = 0;

    // Run tests
    for (size_t i=0; i<TESTS.size(); i++)
        run_test(TESTS.at(i)) ? failed++ : succeed++;

    if (failed > 0) std::cout << "\033[31m"; else std::cout << "\033[32m";
    std::cout << "Ending `" << group_name << "` tests with " << failed << " failures and " << succeed << " successes" << "\033[0m\n";
    return failed > 0 ? 1 : 0;
}

#endif // __SEABOW_TEST_HPP__