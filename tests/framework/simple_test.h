#ifndef SIMPLE_TEST_H
#define SIMPLE_TEST_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cstring>

// Simple test framework macros and functions
#define TEST(test_name) \
    void test_##test_name(); \
    static TestRegistrar reg_##test_name(#test_name, test_##test_name); \
    void test_##test_name()

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "ASSERTION FAILED: " << #condition << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            TestRunner::instance().recordFailure(); \
            return; \
        } \
    } while(0)

#define ASSERT_FALSE(condition) ASSERT_TRUE(!(condition))

#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::cerr << "ASSERTION FAILED: Expected " << #expected << " (" << (expected) << ")" \
                      << " but got " << #actual << " (" << (actual) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            TestRunner::instance().recordFailure(); \
            return; \
        } \
    } while(0)

#define ASSERT_NE(expected, actual) \
    do { \
        if ((expected) == (actual)) { \
            std::cerr << "ASSERTION FAILED: Expected " << #expected << " (" << (expected) << ")" \
                      << " to not equal " << #actual << " (" << (actual) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            TestRunner::instance().recordFailure(); \
            return; \
        } \
    } while(0)

#define ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != nullptr) { \
            std::cerr << "ASSERTION FAILED: Expected " << #ptr << " to be nullptr" \
                      << " but got " << (ptr) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            TestRunner::instance().recordFailure(); \
            return; \
        } \
    } while(0)

#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == nullptr) { \
            std::cerr << "ASSERTION FAILED: Expected " << #ptr << " to not be nullptr" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            TestRunner::instance().recordFailure(); \
            return; \
        } \
    } while(0)

#define ASSERT_STREQ(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            std::cerr << "ASSERTION FAILED: Expected \"" << (expected) << "\" but got \"" << (actual) \
                      << "\" at " << __FILE__ << ":" << __LINE__ << std::endl; \
            TestRunner::instance().recordFailure(); \
            return; \
        } \
    } while(0)

#define ASSERT_STRNE(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) == 0) { \
            std::cerr << "ASSERTION FAILED: Expected \"" << (expected) << "\" to not equal \"" << (actual) \
                      << "\" at " << __FILE__ << ":" << __LINE__ << std::endl; \
            TestRunner::instance().recordFailure(); \
            return; \
        } \
    } while(0)

class TestRunner {
public:
    struct Test {
        std::string name;
        std::function<void()> func;
    };

    static TestRunner& instance() {
        static TestRunner runner;
        return runner;
    }

    void addTest(const std::string& name, std::function<void()> func) {
        tests.push_back({name, func});
    }

    void recordFailure() {
        currentTestFailed = true;
    }

    int runTests() {
        int passed = 0;
        int failed = 0;

        std::cout << "Running " << tests.size() << " tests..." << std::endl;

        for (const auto& test : tests) {
            currentTestFailed = false;
            std::cout << "Running test: " << test.name << "... ";
            
            try {
                test.func();
                if (!currentTestFailed) {
                    std::cout << "PASSED" << std::endl;
                    passed++;
                } else {
                    std::cout << "FAILED" << std::endl;
                    failed++;
                }
            } catch (const std::exception& e) {
                std::cout << "FAILED (exception: " << e.what() << ")" << std::endl;
                failed++;
            } catch (...) {
                std::cout << "FAILED (unknown exception)" << std::endl;
                failed++;
            }
        }

        std::cout << std::endl;
        std::cout << "Test Results:" << std::endl;
        std::cout << "  Passed: " << passed << std::endl;
        std::cout << "  Failed: " << failed << std::endl;
        std::cout << "  Total:  " << tests.size() << std::endl;

        return failed == 0 ? 0 : 1;
    }

private:
    std::vector<Test> tests;
    bool currentTestFailed = false;
};

class TestRegistrar {
public:
    TestRegistrar(const std::string& name, std::function<void()> func) {
        TestRunner::instance().addTest(name, func);
    }
};

#endif // SIMPLE_TEST_H