# Unit Testing Guide for Snes9x GX

This guide explains how to build, run, and contribute to the unit test suite for Snes9x GX.

## Table of Contents

- [Overview](#overview)
- [Test Structure](#test-structure)
- [Building Tests](#building-tests)
- [Running Tests](#running-tests)
- [Writing New Tests](#writing-new-tests)
- [Continuous Integration](#continuous-integration)
- [Troubleshooting](#troubleshooting)

## Overview

The Snes9x GX project includes a comprehensive unit test suite designed to:

- Test core functionality without GameCube/Wii hardware dependencies
- Provide regression testing during development
- Validate settings, file operations, video modes, and controller mappings
- Enable confident refactoring and new feature development

The test suite uses a lightweight custom testing framework that provides familiar assertion macros without external dependencies.

## Test Structure

```
tests/
├── framework/          # Testing framework
│   └── simple_test.h   # Test macros and runner
├── mocks/              # Mock implementations
│   ├── mock_libogc.h   # Mock GameCube/Wii library functions
│   └── mock_libogc.cpp # Mock implementations
├── unit/               # Unit tests
│   ├── test_video.cpp      # Video mode and rendering tests
│   ├── test_fileop.cpp     # File operation tests
│   ├── test_button_mapping.cpp # Controller mapping tests
│   ├── test_preferences.cpp    # Settings validation tests
│   └── test_main.cpp           # Test runner entry point
├── Makefile           # Build configuration
└── README.md          # This file
```

## Building Tests

### Prerequisites

- GCC or Clang with C++11 support
- Make utility
- Optional: cppcheck (for static analysis)
- Optional: valgrind (for memory checking)

### Building

From the `tests/` directory:

```bash
# Build all tests
make tests

# Build and run tests
make run

# Clean and rebuild
make rebuild

# Check dependencies
make check-deps
```

### Available Make Targets

- `make tests` - Build the test executable
- `make run` - Build and run tests with output
- `make run-verbose` - Run tests with detailed output
- `make clean` - Remove build artifacts
- `make rebuild` - Clean and rebuild everything
- `make static-analysis` - Run cppcheck static analysis
- `make memcheck` - Run valgrind memory checking
- `make quality` - Run all quality checks
- `make coverage` - Build with coverage and generate report
- `make help` - Show all available targets

## Running Tests

### Basic Test Run

```bash
cd tests
make run
```

Example output:
```
Running 25 tests...
Running test: video_mode_selection_ntsc_480i... PASSED
Running test: video_mode_selection_progressive_480p... PASSED
Running test: strip_device_with_sd_prefix... PASSED
...

Test Results:
  Passed: 25
  Failed: 0
  Total:  25
```

### Verbose Output

```bash
make run-verbose
```

### Quality Checks

```bash
# Static analysis
make static-analysis

# Memory leak checking (Linux only)
make memcheck

# All quality checks
make quality
```

## Writing New Tests

### Test File Structure

Create new test files in `tests/unit/` following the naming convention `test_<module>.cpp`:

```cpp
#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"

// Include any needed headers or create test helper functions

TEST(test_name) {
    // Test implementation
    ASSERT_TRUE(condition);
    ASSERT_EQ(expected, actual);
}

TEST(another_test) {
    // Another test
}
```

### Available Assertions

- `ASSERT_TRUE(condition)` - Assert condition is true
- `ASSERT_FALSE(condition)` - Assert condition is false
- `ASSERT_EQ(expected, actual)` - Assert values are equal
- `ASSERT_NE(expected, actual)` - Assert values are not equal
- `ASSERT_NULL(ptr)` - Assert pointer is null
- `ASSERT_NOT_NULL(ptr)` - Assert pointer is not null
- `ASSERT_STREQ(expected, actual)` - Assert strings are equal
- `ASSERT_STRNE(expected, actual)` - Assert strings are not equal

### Test Naming Conventions

- Use descriptive test names that explain what is being tested
- Group related tests by prefixing with module name
- Use snake_case for test names
- Examples:
  - `video_mode_selection_ntsc_480i`
  - `strip_device_with_sd_prefix`
  - `button_mapping_valid_controller`

### Testing Guidelines

1. **Test one thing per test** - Keep tests focused and specific
2. **Use descriptive names** - Test names should explain the scenario
3. **Test edge cases** - Include null inputs, boundary values, error conditions
4. **Mock dependencies** - Use the mock framework to avoid hardware dependencies
5. **Keep tests independent** - Tests should not depend on each other
6. **Test both success and failure paths** - Verify error handling

### Adding Mock Functions

When testing code that uses GameCube/Wii-specific functions:

1. Add function declarations to `mocks/mock_libogc.h`
2. Add implementations to `mocks/mock_libogc.cpp`
3. Use inline stubs for simple functions
4. Create proper mock objects for complex interactions

Example:
```cpp
// In mock_libogc.h
static inline void VIDEO_SetBlack(int black) { (void)black; }

// In mock_libogc.cpp for more complex mocks
static bool mockVideoInitialized = false;
void VIDEO_Init() {
    mockVideoInitialized = true;
}
```

## Continuous Integration

The project uses GitHub Actions for automated testing:

### Workflows

1. **Unit Tests** (`unit-tests` job)
   - Builds and runs all unit tests
   - Uploads test results as artifacts

2. **Code Quality** (`code-quality` job)
   - Runs cppcheck static analysis
   - Checks code formatting

3. **Build Verification** (`build-verification` job)
   - Verifies Wii and GameCube builds compile
   - Uses DevkitPro toolchain

### Triggering CI

CI runs automatically on:
- Pushes to `main`, `master`, `develop`, or `unit-tests` branches
- Pull requests to `main`, `master`, or `develop` branches

### Viewing Results

1. Go to the "Actions" tab in your GitHub repository
2. Click on the latest workflow run
3. View job results and download artifacts

## Troubleshooting

### Common Build Issues

**Error: `g++: command not found`**
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# macOS
xcode-select --install

# CentOS/RHEL
sudo yum install gcc-c++
```

**Test compilation fails with missing headers**
- Check that all mock headers are properly included
- Verify test file includes the correct framework header
- Check for typos in include paths

### Test Failures

**Assertion failures**
- Check that expected vs actual values match exactly
- For floating-point comparisons, consider using ranges
- Verify test logic matches the actual implementation

**Segmentation faults**
- Run with valgrind: `make memcheck`
- Check for null pointer dereferences
- Verify array bounds in test data

### Performance Issues

**Slow test builds**
- Use parallel make: `make -j4 tests`
- Clean and rebuild if incremental builds are problematic

**Slow test execution**
- Profile tests to identify bottlenecks
- Consider mocking expensive operations
- Break large tests into smaller, focused tests

### Getting Help

1. Check this documentation first
2. Review existing test files for examples
3. Run `make help` for available targets
4. Check the CI logs for detailed error messages
5. Open an issue on GitHub with:
   - Your operating system and compiler version
   - Full error message and build log
   - Steps to reproduce the issue

## Contributing Tests

When contributing new tests:

1. **Follow the style guide** - Match existing test patterns
2. **Add tests for new features** - All new code should include tests
3. **Update this documentation** - If you add new testing patterns or tools
4. **Run all tests locally** - Ensure your changes don't break existing tests
5. **Check CI results** - Make sure your PR passes all automated checks

### Pull Request Checklist

- [ ] Tests build successfully with `make tests`
- [ ] All tests pass with `make run`
- [ ] Static analysis passes with `make static-analysis`
- [ ] New tests follow naming conventions
- [ ] Test coverage includes edge cases
- [ ] Documentation updated if needed