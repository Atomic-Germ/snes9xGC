#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"

// Test button mapping functionality

enum {
    CTRLR_NONE = -1,
    CTRLR_GCPAD,
    CTRLR_WIIMOTE,
    CTRLR_NUNCHUK,
    CTRLR_CLASSIC,
    CTRLR_WUPC,
    CTRLR_WIIDRC
};

struct BtnMap {
    u32 btn;
    const char* name;
};

struct CtrlrMap {
    u16 type;
    int num_btns;
    BtnMap map[15];
};

// Mock controller definitions for testing
const char* TestGetControllerName(int controller) {
    switch(controller) {
        case CTRLR_GCPAD: return "GameCube Controller";
        case CTRLR_WIIMOTE: return "Wiimote";
        case CTRLR_NUNCHUK: return "Nunchuk + Wiimote";
        case CTRLR_CLASSIC: return "Classic Controller";
        case CTRLR_WUPC: return "Wii U Pro Controller";
        case CTRLR_WIIDRC: return "Wii U Gamepad";
        default: return "Unknown Controller";
    }
}

bool TestIsValidController(int controller) {
    return controller >= CTRLR_GCPAD && controller <= CTRLR_WIIDRC;
}

int TestGetControllerButtonCount(int controller) {
    switch(controller) {
        case CTRLR_GCPAD: return 12;
        case CTRLR_WIIMOTE: return 8;
        case CTRLR_NUNCHUK: return 10;
        case CTRLR_CLASSIC: return 14;
        case CTRLR_WUPC: return 14;
        case CTRLR_WIIDRC: return 14;
        default: return 0;
    }
}

bool TestMapButton(CtrlrMap* ctrlr, int buttonIndex, u32 buttonValue, const char* buttonName) {
    if (!ctrlr || buttonIndex < 0 || buttonIndex >= 15) return false;
    if (buttonIndex >= ctrlr->num_btns) return false;
    
    ctrlr->map[buttonIndex].btn = buttonValue;
    ctrlr->map[buttonIndex].name = buttonName;
    return true;
}

// Tests for controller identification
TEST(controller_name_gcpad) {
    const char* name = TestGetControllerName(CTRLR_GCPAD);
    ASSERT_STREQ("GameCube Controller", name);
}

TEST(controller_name_wiimote) {
    const char* name = TestGetControllerName(CTRLR_WIIMOTE);
    ASSERT_STREQ("Wiimote", name);
}

TEST(controller_name_nunchuk) {
    const char* name = TestGetControllerName(CTRLR_NUNCHUK);
    ASSERT_STREQ("Nunchuk + Wiimote", name);
}

TEST(controller_name_classic) {
    const char* name = TestGetControllerName(CTRLR_CLASSIC);
    ASSERT_STREQ("Classic Controller", name);
}

TEST(controller_name_wupc) {
    const char* name = TestGetControllerName(CTRLR_WUPC);
    ASSERT_STREQ("Wii U Pro Controller", name);
}

TEST(controller_name_wiidrc) {
    const char* name = TestGetControllerName(CTRLR_WIIDRC);
    ASSERT_STREQ("Wii U Gamepad", name);
}

TEST(controller_name_invalid) {
    const char* name = TestGetControllerName(999);
    ASSERT_STREQ("Unknown Controller", name);
    
    name = TestGetControllerName(CTRLR_NONE);
    ASSERT_STREQ("Unknown Controller", name);
}

// Tests for controller validation
TEST(controller_validation_valid_controllers) {
    ASSERT_TRUE(TestIsValidController(CTRLR_GCPAD));
    ASSERT_TRUE(TestIsValidController(CTRLR_WIIMOTE));
    ASSERT_TRUE(TestIsValidController(CTRLR_NUNCHUK));
    ASSERT_TRUE(TestIsValidController(CTRLR_CLASSIC));
    ASSERT_TRUE(TestIsValidController(CTRLR_WUPC));
    ASSERT_TRUE(TestIsValidController(CTRLR_WIIDRC));
}

TEST(controller_validation_invalid_controllers) {
    ASSERT_FALSE(TestIsValidController(CTRLR_NONE));
    ASSERT_FALSE(TestIsValidController(-2));
    ASSERT_FALSE(TestIsValidController(999));
}

// Tests for button count
TEST(controller_button_count_gcpad) {
    int count = TestGetControllerButtonCount(CTRLR_GCPAD);
    ASSERT_EQ(12, count);
}

TEST(controller_button_count_wiimote) {
    int count = TestGetControllerButtonCount(CTRLR_WIIMOTE);
    ASSERT_EQ(8, count);
}

TEST(controller_button_count_nunchuk) {
    int count = TestGetControllerButtonCount(CTRLR_NUNCHUK);
    ASSERT_EQ(10, count);
}

TEST(controller_button_count_classic) {
    int count = TestGetControllerButtonCount(CTRLR_CLASSIC);
    ASSERT_EQ(14, count);
}

TEST(controller_button_count_wupc) {
    int count = TestGetControllerButtonCount(CTRLR_WUPC);
    ASSERT_EQ(14, count);
}

TEST(controller_button_count_wiidrc) {
    int count = TestGetControllerButtonCount(CTRLR_WIIDRC);
    ASSERT_EQ(14, count);
}

TEST(controller_button_count_invalid) {
    int count = TestGetControllerButtonCount(CTRLR_NONE);
    ASSERT_EQ(0, count);
    
    count = TestGetControllerButtonCount(999);
    ASSERT_EQ(0, count);
}

// Tests for button mapping
TEST(button_mapping_valid) {
    CtrlrMap ctrlr;
    ctrlr.type = CTRLR_GCPAD;
    ctrlr.num_btns = 3;
    
    bool success = TestMapButton(&ctrlr, 0, 0x0100, "A Button");
    ASSERT_TRUE(success);
    ASSERT_EQ(0x0100, ctrlr.map[0].btn);
    ASSERT_STREQ("A Button", ctrlr.map[0].name);
}

TEST(button_mapping_multiple_buttons) {
    CtrlrMap ctrlr;
    ctrlr.type = CTRLR_GCPAD;
    ctrlr.num_btns = 3;
    
    bool success1 = TestMapButton(&ctrlr, 0, 0x0100, "A Button");
    bool success2 = TestMapButton(&ctrlr, 1, 0x0200, "B Button");
    bool success3 = TestMapButton(&ctrlr, 2, 0x0400, "X Button");
    
    ASSERT_TRUE(success1);
    ASSERT_TRUE(success2);
    ASSERT_TRUE(success3);
    
    ASSERT_EQ(0x0100, ctrlr.map[0].btn);
    ASSERT_EQ(0x0200, ctrlr.map[1].btn);
    ASSERT_EQ(0x0400, ctrlr.map[2].btn);
    
    ASSERT_STREQ("A Button", ctrlr.map[0].name);
    ASSERT_STREQ("B Button", ctrlr.map[1].name);
    ASSERT_STREQ("X Button", ctrlr.map[2].name);
}

TEST(button_mapping_invalid_index) {
    CtrlrMap ctrlr;
    ctrlr.type = CTRLR_GCPAD;
    ctrlr.num_btns = 3;
    
    bool success = TestMapButton(&ctrlr, -1, 0x0100, "Invalid");
    ASSERT_FALSE(success);
    
    success = TestMapButton(&ctrlr, 15, 0x0100, "Invalid");
    ASSERT_FALSE(success);
    
    success = TestMapButton(&ctrlr, 3, 0x0100, "Out of range");
    ASSERT_FALSE(success);
}

TEST(button_mapping_null_controller) {
    bool success = TestMapButton(nullptr, 0, 0x0100, "A Button");
    ASSERT_FALSE(success);
}

// Test SNES controller mapping
enum SNESButtons {
    SNES_A = 0x80,
    SNES_B = 0x8000,
    SNES_X = 0x40,
    SNES_Y = 0x4000,
    SNES_L = 0x20,
    SNES_R = 0x10,
    SNES_START = 0x1000,
    SNES_SELECT = 0x2000,
    SNES_UP = 0x800,
    SNES_DOWN = 0x400,
    SNES_LEFT = 0x200,
    SNES_RIGHT = 0x100
};

bool TestIsSNESButton(u32 button) {
    switch(button) {
        case SNES_A:
        case SNES_B:
        case SNES_X:
        case SNES_Y:
        case SNES_L:
        case SNES_R:
        case SNES_START:
        case SNES_SELECT:
        case SNES_UP:
        case SNES_DOWN:
        case SNES_LEFT:
        case SNES_RIGHT:
            return true;
        default:
            return false;
    }
}

TEST(snes_button_validation) {
    ASSERT_TRUE(TestIsSNESButton(SNES_A));
    ASSERT_TRUE(TestIsSNESButton(SNES_B));
    ASSERT_TRUE(TestIsSNESButton(SNES_X));
    ASSERT_TRUE(TestIsSNESButton(SNES_Y));
    ASSERT_TRUE(TestIsSNESButton(SNES_L));
    ASSERT_TRUE(TestIsSNESButton(SNES_R));
    ASSERT_TRUE(TestIsSNESButton(SNES_START));
    ASSERT_TRUE(TestIsSNESButton(SNES_SELECT));
    ASSERT_TRUE(TestIsSNESButton(SNES_UP));
    ASSERT_TRUE(TestIsSNESButton(SNES_DOWN));
    ASSERT_TRUE(TestIsSNESButton(SNES_LEFT));
    ASSERT_TRUE(TestIsSNESButton(SNES_RIGHT));
    
    ASSERT_FALSE(TestIsSNESButton(0x01));
    ASSERT_FALSE(TestIsSNESButton(0x02));
    ASSERT_FALSE(TestIsSNESButton(0x00));
}