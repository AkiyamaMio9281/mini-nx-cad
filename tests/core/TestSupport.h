#pragma once

#if defined(MNX_HAS_GTEST)
#include <gtest/gtest.h>
#else
#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace mnx::test {

struct TestCase {
    std::string name;
    std::function<void()> function;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

class TestRegistrar {
public:
    TestRegistrar(std::string name, std::function<void()> function) {
        registry().push_back({std::move(name), std::move(function)});
    }
};

[[noreturn]] inline void fail(const char* file, int line, const std::string& message) {
    std::ostringstream stream;
    stream << file << ':' << line << ": " << message;
    throw std::runtime_error(stream.str());
}

inline int runAllTests() {
    int failures = 0;
    for(const TestCase& test : registry()) {
        try {
            test.function();
            std::cout << "[  PASSED  ] " << test.name << '\n';
        } catch(const std::exception& error) {
            ++failures;
            std::cerr << "[  FAILED  ] " << test.name << '\n' << error.what() << '\n';
        }
    }

    std::cout << registry().size() - static_cast<std::size_t>(failures) << " / " << registry().size()
              << " tests passed\n";
    return failures == 0 ? 0 : 1;
}

} // namespace mnx::test

#define MNX_TEST_CONCAT_IMPL(lhs, rhs) lhs##rhs
#define MNX_TEST_CONCAT(lhs, rhs) MNX_TEST_CONCAT_IMPL(lhs, rhs)

#define TEST(suiteName, testName)                                                                    \
    static void MNX_TEST_CONCAT(suiteName, MNX_TEST_CONCAT(_, testName))();                          \
    namespace {                                                                                      \
    const ::mnx::test::TestRegistrar MNX_TEST_CONCAT(                                                \
        registrar_,                                                                                  \
        MNX_TEST_CONCAT(suiteName, MNX_TEST_CONCAT(_, testName))                                     \
    ) {                                                                                              \
        #suiteName "." #testName, MNX_TEST_CONCAT(suiteName, MNX_TEST_CONCAT(_, testName))           \
    };                                                                                               \
    }                                                                                                \
    static void MNX_TEST_CONCAT(suiteName, MNX_TEST_CONCAT(_, testName))()

#define EXPECT_TRUE(condition)                                                                       \
    do {                                                                                             \
        if(!(condition)) {                                                                           \
            ::mnx::test::fail(__FILE__, __LINE__, "Expected true: " #condition);                     \
        }                                                                                            \
    } while(false)

#define EXPECT_FALSE(condition) EXPECT_TRUE(!(condition))

#define EXPECT_EQ(lhs, rhs)                                                                          \
    do {                                                                                             \
        if(!((lhs) == (rhs))) {                                                                      \
            ::mnx::test::fail(__FILE__, __LINE__, "Expected equality: " #lhs " == " #rhs);          \
        }                                                                                            \
    } while(false)

#define EXPECT_NE(lhs, rhs)                                                                          \
    do {                                                                                             \
        if(!((lhs) != (rhs))) {                                                                      \
            ::mnx::test::fail(__FILE__, __LINE__, "Expected inequality: " #lhs " != " #rhs);        \
        }                                                                                            \
    } while(false)

#define EXPECT_DOUBLE_EQ(lhs, rhs)                                                                   \
    do {                                                                                             \
        if(std::abs((lhs) - (rhs)) > 1.0e-12) {                                                      \
            ::mnx::test::fail(__FILE__, __LINE__, "Expected double equality: " #lhs " == " #rhs);   \
        }                                                                                            \
    } while(false)

#define EXPECT_NEAR(lhs, rhs, tolerance)                                                             \
    do {                                                                                             \
        if(std::abs((lhs) - (rhs)) > (tolerance)) {                                                  \
            ::mnx::test::fail(__FILE__, __LINE__, "Expected near equality: " #lhs " ~= " #rhs);     \
        }                                                                                            \
    } while(false)

#define EXPECT_THROW(statement, exceptionType)                                                       \
    do {                                                                                             \
        bool caughtExpectedException = false;                                                        \
        try {                                                                                        \
            (void)(statement);                                                                       \
        } catch(const exceptionType&) {                                                              \
            caughtExpectedException = true;                                                          \
        }                                                                                            \
        if(!caughtExpectedException) {                                                               \
            ::mnx::test::fail(__FILE__, __LINE__, "Expected exception: " #exceptionType);            \
        }                                                                                            \
    } while(false)
#endif
