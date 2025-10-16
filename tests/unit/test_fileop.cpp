#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"
#include <cstring>
#include <cstdlib>

// Test helper functions for string operations commonly used in the project

// Test path manipulation functions
bool TestStripDevice(const char* path, char* result) {
    if (!path || !result) return false;
    
    // Look for device prefixes like "sd:/", "usb:/", "dvd:/"
    const char* colonPos = strchr(path, ':');
    if (colonPos && colonPos[1] == '/') {
        strcpy(result, colonPos + 1);
        return true;
    }
    
    strcpy(result, path);
    return false;
}

bool TestCreatePath(const char* dir, const char* filename, char* result) {
    if (!dir || !filename || !result) return false;
    
    strcpy(result, dir);
    size_t len = strlen(result);
    
    // Add separator if needed
    if (len > 0 && result[len - 1] != '/') {
        strcat(result, "/");
    }
    
    strcat(result, filename);
    return true;
}

const char* TestGetFileExtension(const char* filename) {
    if (!filename) return nullptr;
    
    const char* lastDot = strrchr(filename, '.');
    if (!lastDot || lastDot == filename) return "";
    
    return lastDot + 1;
}

bool TestValidateFileExtension(const char* filename, const char* expectedExt) {
    const char* ext = TestGetFileExtension(filename);
    return ext && strcasecmp(ext, expectedExt) == 0;
}

// Tests for path manipulation
TEST(strip_device_with_sd_prefix) {
    char result[256];
    bool hasDevice = TestStripDevice("sd:/games/mario.smc", result);
    
    ASSERT_TRUE(hasDevice);
    ASSERT_STREQ("/games/mario.smc", result);
}

TEST(strip_device_with_usb_prefix) {
    char result[256];
    bool hasDevice = TestStripDevice("usb:/roms/zelda.smc", result);
    
    ASSERT_TRUE(hasDevice);
    ASSERT_STREQ("/roms/zelda.smc", result);
}

TEST(strip_device_without_prefix) {
    char result[256];
    bool hasDevice = TestStripDevice("/home/user/game.smc", result);
    
    ASSERT_FALSE(hasDevice);
    ASSERT_STREQ("/home/user/game.smc", result);
}

TEST(strip_device_null_input) {
    char result[256];
    bool hasDevice = TestStripDevice(nullptr, result);
    
    ASSERT_FALSE(hasDevice);
}

TEST(create_path_normal) {
    char result[512];
    bool success = TestCreatePath("/games", "mario.smc", result);
    
    ASSERT_TRUE(success);
    ASSERT_STREQ("/games/mario.smc", result);
}

TEST(create_path_with_trailing_slash) {
    char result[512];
    bool success = TestCreatePath("/games/", "mario.smc", result);
    
    ASSERT_TRUE(success);
    ASSERT_STREQ("/games/mario.smc", result);
}

TEST(create_path_empty_dir) {
    char result[512];
    bool success = TestCreatePath("", "mario.smc", result);
    
    ASSERT_TRUE(success);
    ASSERT_STREQ("mario.smc", result);
}

TEST(create_path_null_inputs) {
    char result[512];
    bool success = TestCreatePath(nullptr, "mario.smc", result);
    
    ASSERT_FALSE(success);
    
    success = TestCreatePath("/games", nullptr, result);
    ASSERT_FALSE(success);
}

// Tests for file extension handling
TEST(get_file_extension_normal) {
    const char* ext = TestGetFileExtension("mario.smc");
    
    ASSERT_STREQ("smc", ext);
}

TEST(get_file_extension_multiple_dots) {
    const char* ext = TestGetFileExtension("super.mario.world.smc");
    
    ASSERT_STREQ("smc", ext);
}

TEST(get_file_extension_no_extension) {
    const char* ext = TestGetFileExtension("mario");
    
    ASSERT_STREQ("", ext);
}

TEST(get_file_extension_hidden_file) {
    const char* ext = TestGetFileExtension(".hidden");
    
    ASSERT_STREQ("", ext);
}

TEST(get_file_extension_null_input) {
    const char* ext = TestGetFileExtension(nullptr);
    
    ASSERT_NULL(ext);
}

TEST(validate_smc_extension) {
    bool valid = TestValidateFileExtension("mario.smc", "smc");
    ASSERT_TRUE(valid);
    
    valid = TestValidateFileExtension("mario.SMC", "smc");
    ASSERT_TRUE(valid);
    
    valid = TestValidateFileExtension("mario.zip", "smc");
    ASSERT_FALSE(valid);
}

TEST(validate_zip_extension) {
    bool valid = TestValidateFileExtension("games.zip", "zip");
    ASSERT_TRUE(valid);
    
    valid = TestValidateFileExtension("games.ZIP", "zip");
    ASSERT_TRUE(valid);
    
    valid = TestValidateFileExtension("games.rar", "zip");
    ASSERT_FALSE(valid);
}

// Test memory buffer operations
bool TestSafeMemcpy(void* dest, const void* src, size_t destSize, size_t srcSize) {
    if (!dest || !src || destSize == 0 || srcSize == 0) return false;
    
    // Reserve space for null terminator if this is a string operation
    size_t maxCopy = destSize - 1;
    size_t copySize = (srcSize < maxCopy) ? srcSize : maxCopy;
    memcpy(dest, src, copySize);
    
    // Always null terminate
    ((char*)dest)[copySize] = '\0';
    
    return true;
}

TEST(safe_memcpy_normal_copy) {
    char dest[100];
    const char* src = "Hello, World!";
    bool success = TestSafeMemcpy(dest, src, sizeof(dest), strlen(src));
    
    ASSERT_TRUE(success);
    ASSERT_STREQ("Hello, World!", dest);
}

TEST(safe_memcpy_truncate_copy) {
    char dest[10];
    const char* src = "This is a very long string";
    bool success = TestSafeMemcpy(dest, src, sizeof(dest), strlen(src));
    
    ASSERT_TRUE(success);
    ASSERT_EQ('\0', dest[9]); // Should be null terminated
}

TEST(safe_memcpy_null_inputs) {
    char dest[100];
    const char* src = "Hello";
    
    bool success = TestSafeMemcpy(nullptr, src, 100, strlen(src));
    ASSERT_FALSE(success);
    
    success = TestSafeMemcpy(dest, nullptr, sizeof(dest), 5);
    ASSERT_FALSE(success);
}

TEST(safe_memcpy_zero_sizes) {
    char dest[100];
    const char* src = "Hello";
    
    bool success = TestSafeMemcpy(dest, src, 0, strlen(src));
    ASSERT_FALSE(success);
    
    success = TestSafeMemcpy(dest, src, sizeof(dest), 0);
    ASSERT_FALSE(success);
}