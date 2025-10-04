#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"

// Test input system functionality including controller state and button mapping

// SNES controller button definitions (from the actual codebase)
enum SNESButtons {
    SNES_B = 0x8000,
    SNES_Y = 0x4000,
    SNES_SELECT = 0x2000,
    SNES_START = 0x1000,
    SNES_UP = 0x0800,
    SNES_DOWN = 0x0400,
    SNES_LEFT = 0x0200,
    SNES_RIGHT = 0x0100,
    SNES_A = 0x0080,
    SNES_X = 0x0040,
    SNES_L = 0x0020,
    SNES_R = 0x0010
};

// Controller state structure
struct ControllerState {
    u32 buttons_held;
    u32 buttons_pressed;
    u32 buttons_released;
    s16 stick_x;
    s16 stick_y;
    bool connected;
};

// Test functions for input processing
bool TestProcessButtonInput(ControllerState* current, ControllerState* previous) {
    if (!current || !previous) return false;
    
    // Calculate pressed and released buttons
    current->buttons_pressed = current->buttons_held & ~previous->buttons_held;
    current->buttons_released = previous->buttons_held & ~current->buttons_held;
    
    return true;
}

bool TestValidateButtonCombo(u32 buttons, u32 required_combo) {
    return (buttons & required_combo) == required_combo;
}

u32 TestMapGCButtonToSNES(u32 gc_button) {
    // Simplified mapping from GameCube to SNES buttons
    switch (gc_button) {
        case 0x0001: return SNES_A;     // GC A -> SNES A
        case 0x0002: return SNES_B;     // GC B -> SNES B
        case 0x0004: return SNES_X;     // GC X -> SNES X
        case 0x0008: return SNES_Y;     // GC Y -> SNES Y
        case 0x0010: return SNES_L;     // GC L -> SNES L
        case 0x0020: return SNES_R;     // GC R -> SNES R
        case 0x0100: return SNES_START; // GC Start -> SNES Start
        case 0x1000: return SNES_UP;    // GC D-Pad Up -> SNES Up
        case 0x2000: return SNES_DOWN;  // GC D-Pad Down -> SNES Down
        case 0x4000: return SNES_LEFT;  // GC D-Pad Left -> SNES Left
        case 0x8000: return SNES_RIGHT; // GC D-Pad Right -> SNES Right
        default: return 0;
    }
}

bool TestStickDeadzone(s16 x, s16 y, s16 deadzone) {
    // Test if stick input is within deadzone
    return (abs(x) < deadzone && abs(y) < deadzone);
}

u32 TestStickToDigital(s16 x, s16 y, s16 threshold) {
    u32 digital = 0;
    
    if (x < -threshold) digital |= SNES_LEFT;
    if (x > threshold) digital |= SNES_RIGHT;
    if (y < -threshold) digital |= SNES_DOWN;
    if (y > threshold) digital |= SNES_UP;
    
    return digital;
}

// Tests for input system
TEST(input_button_state_basic) {
    ControllerState current = {0};
    ControllerState previous = {0};
    
    // Press A button
    current.buttons_held = SNES_A;
    previous.buttons_held = 0;
    
    bool success = TestProcessButtonInput(&current, &previous);
    ASSERT_TRUE(success);
    ASSERT_EQ(SNES_A, current.buttons_pressed);
    ASSERT_EQ(0, current.buttons_released);
}

TEST(input_button_state_release) {
    ControllerState current = {0};
    ControllerState previous = {0};
    
    // Release A button
    current.buttons_held = 0;
    previous.buttons_held = SNES_A;
    
    bool success = TestProcessButtonInput(&current, &previous);
    ASSERT_TRUE(success);
    ASSERT_EQ(0, current.buttons_pressed);
    ASSERT_EQ(SNES_A, current.buttons_released);
}

TEST(input_button_state_multiple) {
    ControllerState current = {0};
    ControllerState previous = {0};
    
    // Press A and B, was holding Start
    current.buttons_held = SNES_A | SNES_B | SNES_START;
    previous.buttons_held = SNES_START;
    
    bool success = TestProcessButtonInput(&current, &previous);
    ASSERT_TRUE(success);
    ASSERT_EQ(SNES_A | SNES_B, current.buttons_pressed);
    ASSERT_EQ(0, current.buttons_released);
}

TEST(input_button_combo_validation) {
    u32 buttons = SNES_L | SNES_R | SNES_START;
    
    // Test valid combo
    bool valid = TestValidateButtonCombo(buttons, SNES_L | SNES_R);
    ASSERT_TRUE(valid);
    
    // Test invalid combo
    valid = TestValidateButtonCombo(buttons, SNES_A | SNES_B);
    ASSERT_FALSE(valid);
    
    // Test exact combo
    valid = TestValidateButtonCombo(buttons, SNES_L | SNES_R | SNES_START);
    ASSERT_TRUE(valid);
}

TEST(input_gc_to_snes_mapping) {
    // Test basic button mappings
    ASSERT_EQ(SNES_A, TestMapGCButtonToSNES(0x0001));
    ASSERT_EQ(SNES_B, TestMapGCButtonToSNES(0x0002));
    ASSERT_EQ(SNES_X, TestMapGCButtonToSNES(0x0004));
    ASSERT_EQ(SNES_Y, TestMapGCButtonToSNES(0x0008));
    
    // Test D-pad mappings
    ASSERT_EQ(SNES_UP, TestMapGCButtonToSNES(0x1000));
    ASSERT_EQ(SNES_DOWN, TestMapGCButtonToSNES(0x2000));
    ASSERT_EQ(SNES_LEFT, TestMapGCButtonToSNES(0x4000));
    ASSERT_EQ(SNES_RIGHT, TestMapGCButtonToSNES(0x8000));
    
    // Test invalid button
    ASSERT_EQ(0, TestMapGCButtonToSNES(0x9999));
}

TEST(input_stick_deadzone) {
    s16 deadzone = 20;
    
    // Test within deadzone
    ASSERT_TRUE(TestStickDeadzone(10, 15, deadzone));
    ASSERT_TRUE(TestStickDeadzone(-19, 19, deadzone));
    ASSERT_TRUE(TestStickDeadzone(0, 0, deadzone));
    
    // Test outside deadzone
    ASSERT_FALSE(TestStickDeadzone(25, 10, deadzone));
    ASSERT_FALSE(TestStickDeadzone(10, -25, deadzone));
    ASSERT_FALSE(TestStickDeadzone(-30, 30, deadzone));
}

TEST(input_stick_to_digital) {
    s16 threshold = 50;
    
    // Test neutral position
    u32 digital = TestStickToDigital(0, 0, threshold);
    ASSERT_EQ(0, digital);
    
    // Test single directions
    digital = TestStickToDigital(-60, 0, threshold);
    ASSERT_EQ(SNES_LEFT, digital);
    
    digital = TestStickToDigital(60, 0, threshold);
    ASSERT_EQ(SNES_RIGHT, digital);
    
    digital = TestStickToDigital(0, -60, threshold);
    ASSERT_EQ(SNES_DOWN, digital);
    
    digital = TestStickToDigital(0, 60, threshold);
    ASSERT_EQ(SNES_UP, digital);
    
    // Test diagonal
    digital = TestStickToDigital(-60, 60, threshold);
    ASSERT_EQ(SNES_LEFT | SNES_UP, digital);
    
    // Test within threshold
    digital = TestStickToDigital(30, -30, threshold);
    ASSERT_EQ(0, digital);
}

TEST(input_controller_state_null_protection) {
    ControllerState current = {0};
    
    bool success = TestProcessButtonInput(nullptr, &current);
    ASSERT_FALSE(success);
    
    success = TestProcessButtonInput(&current, nullptr);
    ASSERT_FALSE(success);
    
    success = TestProcessButtonInput(nullptr, nullptr);
    ASSERT_FALSE(success);
}

TEST(input_button_sequence) {
    ControllerState states[4] = {0};
    
    // Frame 0: Press A
    states[1].buttons_held = SNES_A;
    TestProcessButtonInput(&states[1], &states[0]);
    ASSERT_EQ(SNES_A, states[1].buttons_pressed);
    
    // Frame 1: Hold A, press B
    states[2].buttons_held = SNES_A | SNES_B;
    TestProcessButtonInput(&states[2], &states[1]);
    ASSERT_EQ(SNES_B, states[2].buttons_pressed);
    ASSERT_EQ(0, states[2].buttons_released);
    
    // Frame 2: Release A, hold B
    states[3].buttons_held = SNES_B;
    TestProcessButtonInput(&states[3], &states[2]);
    ASSERT_EQ(0, states[3].buttons_pressed);
    ASSERT_EQ(SNES_A, states[3].buttons_released);
}

TEST(input_all_snes_buttons) {
    // Test all SNES button constants are defined correctly
    u32 all_buttons = SNES_A | SNES_B | SNES_X | SNES_Y | 
                      SNES_L | SNES_R | SNES_START | SNES_SELECT |
                      SNES_UP | SNES_DOWN | SNES_LEFT | SNES_RIGHT;
    
    // Should have 12 unique bits set
    int bit_count = 0;
    for (int i = 0; i < 16; i++) {
        if (all_buttons & (1 << i)) bit_count++;
    }
    ASSERT_EQ(12, bit_count);
    
    // Test individual button values are powers of 2
    ASSERT_EQ(1, __builtin_popcount(SNES_A));
    ASSERT_EQ(1, __builtin_popcount(SNES_B));
    ASSERT_EQ(1, __builtin_popcount(SNES_X));
    ASSERT_EQ(1, __builtin_popcount(SNES_Y));
}