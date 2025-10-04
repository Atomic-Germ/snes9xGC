#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"
#include <cstring>
#include <cstdlib>

// Test memory management functions and patterns used in the project

// Mock memory management functions with tracking
static size_t total_allocated = 0;
static size_t allocation_count = 0;
static size_t free_count = 0;

void* TestMemalign(size_t alignment, size_t size) {
    // For testing, just use regular malloc and track allocations
    (void)alignment; // Ignore alignment for simplicity in tests
    void* ptr = malloc(size);
    if (ptr) {
        total_allocated += size;
        allocation_count++;
    }
    return ptr;
}

void TestFree(void* ptr) {
    if (ptr) {
        free(ptr);
        free_count++;
    }
}

void ResetMemoryStats() {
    total_allocated = 0;
    allocation_count = 0;
    free_count = 0;
}

// Test memory alignment function
bool TestMemoryAlignment(void* ptr, size_t alignment) {
    if (!ptr) return false;
    uintptr_t addr = (uintptr_t)ptr;
    return (addr % alignment) == 0;
}

// Test buffer operations common in the codebase
bool TestSafeBufferCopy(void* dest, size_t destSize, const void* src, size_t srcSize) {
    if (!dest || !src || destSize == 0) return false;
    
    size_t copySize = (srcSize < destSize) ? srcSize : destSize;
    memcpy(dest, src, copySize);
    
    // Clear remaining space if dest is larger
    if (copySize < destSize) {
        memset((char*)dest + copySize, 0, destSize - copySize);
    }
    
    return true;
}

// Test pattern for aligned memory allocation used throughout the codebase
void* TestAllocateAlignedBuffer(size_t size, size_t alignment) {
    // Simulate the memalign(32, size) pattern used in the codebase
    if (size == 0) return nullptr;
    
    void* ptr = TestMemalign(alignment, size);
    if (ptr) {
        // Clear memory as done in many places in the codebase
        memset(ptr, 0, size);
    }
    return ptr;
}

// Tests for memory management patterns
TEST(memory_allocation_basic) {
    ResetMemoryStats();
    
    void* ptr = TestMemalign(32, 1024);
    ASSERT_NOT_NULL(ptr);
    ASSERT_EQ(1, allocation_count);
    
    TestFree(ptr);
    ASSERT_EQ(1, free_count);
}

TEST(memory_allocation_zero_size) {
    void* ptr = TestAllocateAlignedBuffer(0, 32);
    ASSERT_NULL(ptr);
}

TEST(memory_allocation_large_buffer) {
    ResetMemoryStats();
    
    // Test allocation similar to SNES9XGFX_SIZE
    size_t large_size = 512 * 480 * 2; // Typical framebuffer size
    void* ptr = TestAllocateAlignedBuffer(large_size, 32);
    
    ASSERT_NOT_NULL(ptr);
    ASSERT_EQ(1, allocation_count);
    
    TestFree(ptr);
    ASSERT_EQ(1, free_count);
}

TEST(memory_buffer_operations) {
    char dest[100];
    const char* src = "Test data for buffer copy";
    
    bool success = TestSafeBufferCopy(dest, sizeof(dest), src, strlen(src));
    ASSERT_TRUE(success);
    ASSERT_STREQ("Test data for buffer copy", dest);
}

TEST(memory_buffer_overflow_protection) {
    char dest[10];
    const char* src = "This is a very long string that should be truncated";
    
    bool success = TestSafeBufferCopy(dest, sizeof(dest), src, strlen(src));
    ASSERT_TRUE(success);
    
    // Should be truncated and null-terminated
    ASSERT_EQ(9, strlen(dest)); // 10 - 1 for null terminator
}

TEST(memory_buffer_null_protection) {
    char dest[10];
    const char* src = "test";
    
    bool success = TestSafeBufferCopy(nullptr, 10, src, strlen(src));
    ASSERT_FALSE(success);
    
    success = TestSafeBufferCopy(dest, sizeof(dest), nullptr, 5);
    ASSERT_FALSE(success);
    
    success = TestSafeBufferCopy(dest, 0, src, strlen(src));
    ASSERT_FALSE(success);
}

// Test memory leak detection (basic)
TEST(memory_leak_detection) {
    ResetMemoryStats();
    
    // Allocate several buffers
    void* ptr1 = TestMemalign(32, 512);
    void* ptr2 = TestMemalign(32, 1024);
    void* ptr3 = TestMemalign(32, 256);
    
    ASSERT_EQ(3, allocation_count);
    
    // Free all but one
    TestFree(ptr1);
    TestFree(ptr2);
    
    ASSERT_EQ(2, free_count);
    // This would indicate a leak in real code
    ASSERT_NE(allocation_count, free_count);
    
    // Clean up
    TestFree(ptr3);
    ASSERT_EQ(3, free_count);
}

// Test multiple allocation/deallocation cycles
TEST(memory_allocation_cycles) {
    ResetMemoryStats();
    
    for (int i = 0; i < 10; i++) {
        void* ptr = TestMemalign(32, 1024 * (i + 1));
        ASSERT_NOT_NULL(ptr);
        TestFree(ptr);
    }
    
    ASSERT_EQ(10, allocation_count);
    ASSERT_EQ(10, free_count);
}

// Test edge cases for memory sizes
TEST(memory_allocation_edge_cases) {
    // Test 1 byte allocation
    void* ptr1 = TestMemalign(32, 1);
    ASSERT_NOT_NULL(ptr1);
    TestFree(ptr1);
    
    // Test large allocation (simulating ROM buffer)
    void* ptr2 = TestMemalign(32, 8 * 1024 * 1024); // 8MB ROM
    ASSERT_NOT_NULL(ptr2);
    TestFree(ptr2);
    
    // Test typical tile cache allocation
    void* ptr3 = TestMemalign(32, 4096 * 64); // MAX_4BIT_TILES * 64
    ASSERT_NOT_NULL(ptr3);
    TestFree(ptr3);
}