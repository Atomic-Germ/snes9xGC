# Makefile for Snes9x GX Unit Tests
# This builds and runs unit tests for the project

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -g
INCLUDES = -Iframework -Imocks -I../source
LDFLAGS = 

# Test framework
FRAMEWORK_DIR = framework
FRAMEWORK_SRC = $(FRAMEWORK_DIR)/simple_test.h

# Mock libraries
MOCKS_DIR = mocks
MOCKS_SRC = $(MOCKS_DIR)/mock_libogc.cpp
MOCKS_OBJ = $(MOCKS_SRC:.cpp=.o)

# Unit tests
UNIT_DIR = unit
UNIT_SOURCES = $(wildcard $(UNIT_DIR)/test_*.cpp)
UNIT_OBJECTS = $(UNIT_SOURCES:.cpp=.o)

# Test executable
TEST_EXECUTABLE = run_tests
TEST_MAIN = $(UNIT_DIR)/test_main.cpp
TEST_MAIN_OBJ = $(TEST_MAIN:.cpp=.o)

# Output files
TEST_RESULTS = test-results.txt
BUILD_LOG = build.log

.PHONY: all tests clean run help

# Default target
all: tests

# Build all tests
tests: $(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(MOCKS_OBJ) $(UNIT_OBJECTS) $(TEST_MAIN_OBJ)
	@echo "Linking test executable..."
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "✓ Test executable built successfully"

# Compile mock objects
$(MOCKS_DIR)/%.o: $(MOCKS_DIR)/%.cpp
	@echo "Compiling mock: $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile unit test objects
$(UNIT_DIR)/%.o: $(UNIT_DIR)/%.cpp $(FRAMEWORK_SRC)
	@echo "Compiling test: $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Run tests
run: $(TEST_EXECUTABLE)
	@echo "Running unit tests..."
	@echo "========================================"
	./$(TEST_EXECUTABLE) | tee $(TEST_RESULTS)
	@echo "========================================"
	@echo "Test results saved to $(TEST_RESULTS)"

# Run tests with verbose output
run-verbose: $(TEST_EXECUTABLE)
	@echo "Running unit tests (verbose)..."
	@echo "========================================"
	./$(TEST_EXECUTABLE) 2>&1 | tee $(TEST_RESULTS)
	@echo "========================================"

# Clean build artifacts
clean:
	@echo "Cleaning test build artifacts..."
	rm -f $(MOCKS_OBJ) $(UNIT_OBJECTS) $(TEST_MAIN_OBJ)
	rm -f $(TEST_EXECUTABLE)
	rm -f $(TEST_RESULTS) $(BUILD_LOG)
	@echo "✓ Clean complete"

# Clean and rebuild
rebuild: clean tests

# Check for missing dependencies
check-deps:
	@echo "Checking build dependencies..."
	@which $(CXX) > /dev/null || (echo "ERROR: $(CXX) not found" && exit 1)
	@echo "✓ Build dependencies satisfied"

# Static analysis using cppcheck (if available)
static-analysis:
	@echo "Running static analysis..."
	@if which cppcheck > /dev/null 2>&1; then \
		cppcheck --enable=warning,style,performance,portability \
		         --std=c++11 \
		         --suppress=missingIncludeSystem \
		         --suppress=unusedFunction \
		         --force \
		         --inline-suppr \
		         $(UNIT_SOURCES) $(MOCKS_SRC) 2>&1 | tee static-analysis.log; \
		echo "✓ Static analysis complete (results in static-analysis.log)"; \
	else \
		echo "⚠ cppcheck not found, skipping static analysis"; \
	fi

# Memory leak checking with valgrind (if available and on Linux)
memcheck: $(TEST_EXECUTABLE)
	@echo "Running memory leak detection..."
	@if which valgrind > /dev/null 2>&1; then \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		         --verbose --log-file=valgrind.log ./$(TEST_EXECUTABLE); \
		echo "✓ Memory check complete (results in valgrind.log)"; \
	else \
		echo "⚠ valgrind not found, skipping memory check"; \
	fi

# Run all quality checks
quality: static-analysis memcheck
	@echo "✓ All quality checks complete"

# Show test coverage information
coverage: CXXFLAGS += --coverage
coverage: LDFLAGS += --coverage
coverage: clean tests run
	@echo "Generating coverage report..."
	@if which gcov > /dev/null 2>&1; then \
		gcov $(UNIT_SOURCES) $(MOCKS_SRC); \
		echo "✓ Coverage files generated (.gcov files)"; \
	else \
		echo "⚠ gcov not found, skipping coverage report"; \
	fi

# Help target
help:
	@echo "Snes9x GX Test Suite"
	@echo "===================="
	@echo ""
	@echo "Available targets:"
	@echo "  tests          - Build all unit tests"
	@echo "  run            - Run unit tests"
	@echo "  run-verbose    - Run tests with verbose output"
	@echo "  clean          - Clean build artifacts"
	@echo "  rebuild        - Clean and rebuild"
	@echo "  check-deps     - Check build dependencies"
	@echo "  static-analysis- Run cppcheck static analysis"
	@echo "  memcheck       - Run valgrind memory checking"
	@echo "  quality        - Run all quality checks"
	@echo "  coverage       - Build with coverage and generate report"
	@echo "  help           - Show this help message"
	@echo ""
	@echo "Test files:"
	@echo "  Framework:     $(FRAMEWORK_SRC)"
	@echo "  Mocks:         $(MOCKS_SRC)"
	@echo "  Unit tests:    $(UNIT_SOURCES)"
	@echo "  Executable:    $(TEST_EXECUTABLE)"

# Debug target to show variables
debug:
	@echo "Build Configuration:"
	@echo "  CXX:           $(CXX)"
	@echo "  CXXFLAGS:      $(CXXFLAGS)"
	@echo "  INCLUDES:      $(INCLUDES)"
	@echo "  LDFLAGS:       $(LDFLAGS)"
	@echo "  UNIT_SOURCES:  $(UNIT_SOURCES)"
	@echo "  UNIT_OBJECTS:  $(UNIT_OBJECTS)"
	@echo "  MOCKS_SRC:     $(MOCKS_SRC)"
	@echo "  MOCKS_OBJ:     $(MOCKS_OBJ)"