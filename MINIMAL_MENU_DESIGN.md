# Minimal Menu Design - Config-File-Based Approach

## Overview

Replace the complex 312 KB GUI system with:
1. **Config file-based settings** (no in-game configuration)
2. **Minimal ROM selector** (Z button)
3. **Quick save/load states** (C-stick left/right)
4. **No configuration menus** during gameplay

## Current System Analysis

### Code Size
- `menu.cpp`: 150 KB (5,029 lines)
- `source/gui/`: 160 KB (12 files, ~2,500 lines)
- **Total: ~312 KB of menu/GUI code**

### Current Menu Triggers
```cpp
MenuRequested() checks:
1. GCSettings.GamepadMenuToggle == 1: Home button OR C-stick left
2. GCSettings.GamepadMenuToggle == 2: L+R+Start combo
3. Default: All toggle options enabled
```

### Existing State Save/Load Functions
```cpp
// From freeze.h:
int SaveSnapshot(char *filepath, bool silent);
int SaveSnapshotAuto(bool silent);
int LoadSnapshot(char *filepath, bool silent);
int LoadSnapshotAuto(bool silent);
```

## Proposed New System

### 1. Control Scheme

**GameCube Controller:**
```
Z Button                → Open minimal ROM selector
C-Stick Left            → Load state (slot 0)
C-Stick Right           → Save state (slot 0)
C-Stick Up (hold 1s)    → Cycle save slot up
C-Stick Down (hold 1s)  → Cycle save slot down
L+R+Start               → Reset game
L+R+Start+Z             → Exit to loader
```

**Wii Remote + Nunchuk:**
```
Z Button (Nunchuk)      → Open minimal ROM selector
C Button (Nunchuk)      → Quick save/load menu overlay
Home Button             → Exit to loader
1+2+Plus                → Reset game
```

**Classic Controller:**
```
ZL Button               → Open minimal ROM selector
ZR Button               → Quick save/load menu overlay
Home Button             → Exit to loader
L+R+Plus                → Reset game
```

### 2. Minimal ROM Selector

When Z is pressed, show a **simple file list overlay**:

```
┌─────────────────────────────────┐
│  SNES9x GX - ROM Selection      │
├─────────────────────────────────┤
│  ▶ Super Mario World.smc        │
│    Super Metroid.smc            │
│    Zelda - ALTTP.smc            │
│    Chrono Trigger.smc           │
│    Final Fantasy 6.smc          │
│                                 │
│  D-Pad: Navigate                │
│  A: Load    B: Cancel           │
└─────────────────────────────────┘
```

**Features:**
- Simple text list (no thumbnails, no metadata)
- Sorted alphabetically
- Shows ~10 ROMs at a time
- Fast navigation with D-pad
- A to load, B to cancel
- Remembers last position

**Implementation:**
- Reuse existing file browser code (strip out GUI elements)
- Direct text rendering (no GUI framework)
- ~500 lines of code vs current 12K filebrowser

### 3. Quick Save/Load States

**Visual Feedback:**
```
C-Stick Left  → "State Loaded (Slot 0)" (1 second overlay)
C-Stick Right → "State Saved (Slot 0)" (1 second overlay)
```

**Slot Management:**
- Default slot: 0
- Hold C-Stick Up/Down for 1 second to change slot (0-9)
- Shows: "Save Slot: 3" (1 second overlay)
- Slot persists across sessions (saved to config)

### 4. Configuration via File Only

**settings.xml** (existing) handles ALL configuration:

```xml
<settings>
    <!-- Video Settings -->
    <videomode>0</videomode>          <!-- 0=Auto, 1=NTSC, 2=480p, etc. -->
    <render>2</render>                <!-- 0=original, 1=filtered, 2=unfiltered -->
    <FilterMethod>0</FilterMethod>    <!-- 0=none, 1=hq2x, etc. -->
    <widescreen>1</widescreen>        <!-- 0=4:3, 1=16:9 -->
    <zoomHor>1.0</zoomHor>
    <zoomVert>1.0</zoomVert>
    <xshift>0</xshift>
    <yshift>0</yshift>
    
    <!-- Audio Settings -->
    <MusicVolume>80</MusicVolume>
    <SFXVolume>80</SFXVolume>
    <MuteAudio>0</MuteAudio>
    <sfxOverclock>0</sfxOverclock>
    <Interpolation>2</Interpolation>
    
    <!-- Emulation Settings -->
    <HiResolution>1</HiResolution>
    <SpriteLimit>0</SpriteLimit>
    <FrameSkip>0</FrameSkip>
    
    <!-- Control Settings -->
    <Controller>0</Controller>
    <WiimoteOrientation>0</WiimoteOrientation>
    <Rumble>1</Rumble>
    <crosshair>1</crosshair>
    <TurboModeButton>0</TurboModeButton>
    <MapABXYRightStick>0</MapABXYRightStick>
    
    <!-- Button Mappings (already exists) -->
    <buttonmap>...</buttonmap>
    
    <!-- Save/Load Settings -->
    <AutoLoad>1</AutoLoad>
    <AutoSave>1</AutoSave>
    <SaveMethod>0</SaveMethod>        <!-- 0=Auto, 1=SD, 2=USB, etc. -->
    <LoadMethod>0</LoadMethod>
    <DefaultSaveSlot>0</DefaultSaveSlot>
    
    <!-- Paths -->
    <LoadFolder>sd:/roms/snes/</LoadFolder>
    <SaveFolder>sd:/saves/snes9xgx/</SaveFolder>
    <CheatFolder>sd:/cheats/</CheatFolder>
</settings>
```

**Editing:**
- Use text editor on PC/Mac
- Transfer via SD card
- Or edit directly on Wii using WiiXplorer/ftpii
- No in-game configuration needed

### 5. Minimal OSD (On-Screen Display)

Simple text overlays for feedback:

```cpp
void ShowOSD(const char *message, int duration_ms) {
    // Display message in corner for specified duration
    // Fade out after duration
}
```

**Messages:**
- "State Saved (Slot 3)"
- "State Loaded (Slot 3)"
- "ROM Loading..."
- "Save Slot: 5"
- "Reset"
- "Exit to Loader"

### 6. Implementation Plan

#### Phase 1: Direct State Save/Load (2-3 hours)
```cpp
// In input.cpp - add to ReportButtons() or decodepad()

// C-Stick for state save/load
if (userInput[0].pad.substickX < -70) {
    // C-stick left = Load state
    if (!stateLoadCooldown) {
        LoadSnapshotAuto(SILENT);
        ShowOSD("State Loaded (Slot 0)", 1000);
        stateLoadCooldown = 60; // 1 second at 60fps
    }
}
else if (userInput[0].pad.substickX > 70) {
    // C-stick right = Save state
    if (!stateSaveCooldown) {
        SaveSnapshotAuto(SILENT);
        ShowOSD("State Saved (Slot 0)", 1000);
        stateSaveCooldown = 60;
    }
}

// Decrement cooldowns
if (stateLoadCooldown > 0) stateLoadCooldown--;
if (stateSaveCooldown > 0) stateSaveCooldown--;
```

#### Phase 2: Minimal ROM Selector (4-6 hours)
```cpp
// Simplified file browser
void MinimalROMSelector() {
    // 1. Scan ROM directory (reuse existing code)
    // 2. Sort alphabetically
    // 3. Render simple text list
    // 4. Handle input (D-pad, A, B)
    // 5. Load selected ROM
}

// In input.cpp
if (userInput[0].pad.btns_d & PAD_TRIGGER_Z) {
    MinimalROMSelector();
}
```

#### Phase 3: Remove Old Menu System (2-3 hours)
```cpp
// 1. Remove menu.cpp (5,029 lines)
// 2. Remove source/gui/ directory (2,500 lines)
// 3. Keep only:
//    - Settings loading (preferences.cpp)
//    - File operations (fileop.cpp)
//    - Simple text rendering
// 4. Update build system (Makefile)
```

#### Phase 4: OSD System (1-2 hours)
```cpp
// Simple overlay rendering
struct OSDMessage {
    char text[256];
    int framesRemaining;
    int fadeFrames;
};

void RenderOSD() {
    if (osdMessage.framesRemaining > 0) {
        // Calculate fade alpha
        int alpha = 255;
        if (osdMessage.framesRemaining < osdMessage.fadeFrames) {
            alpha = (255 * osdMessage.framesRemaining) / osdMessage.fadeFrames;
        }
        
        // Render text with alpha
        RenderText(osdMessage.text, x, y, alpha);
        osdMessage.framesRemaining--;
    }
}
```

## Benefits

### Code Size Reduction
- **Remove: ~312 KB** (menu.cpp + gui/)
- **Add: ~2-3 KB** (minimal ROM selector + OSD)
- **Net savings: ~309 KB** (99% reduction!)

### Performance Benefits
1. **Faster startup** - No GUI initialization
2. **Less memory usage** - No GUI textures/fonts loaded
3. **Simpler codebase** - Easier to maintain
4. **Faster menu access** - Direct state operations vs menu navigation

### User Experience
1. **Faster workflow** - C-stick for instant save/load
2. **Less distraction** - No complex menus during play
3. **Power user friendly** - Config file for advanced users
4. **Streamlined** - Only essential features

### Comparison

| Feature | Current System | Minimal System |
|---------|----------------|----------------|
| Code size | 312 KB | 3 KB |
| Menu layers | 5-6 levels deep | 1 level (ROM selector) |
| State save/load | 4-5 button presses | 1 button press |
| Configuration | In-game menus | Config file |
| Startup time | ~2-3 seconds | <1 second |
| Memory usage | ~2-3 MB (GUI) | ~100 KB |

## Migration Path

### For End Users
1. **Backup settings.xml** - Preserve configuration
2. **Update to new version** - Install minimal build
3. **Verify settings.xml** - Ensure all options correct
4. **Learn new controls** - C-stick for states, Z for ROMs

### Backward Compatibility
- **Settings file**: Fully compatible (same format)
- **Save states**: Fully compatible (same format)
- **SRAM saves**: Fully compatible (same format)
- **Button mappings**: Fully compatible (same format)

## Optional Enhancements

### Enhancement 1: Multi-Slot States
```cpp
// Hold C-stick up/down to cycle slots
if (userInput[0].pad.substickY > 70 && substickHoldFrames > 60) {
    currentSlot = (currentSlot + 1) % 10;
    ShowOSD("Save Slot: %d", currentSlot, 1000);
}
```

### Enhancement 2: ROM Favorites
```cpp
// favorites.txt - list of favorite ROMs
// Press X to toggle favorite
// Press Y to show only favorites
```

### Enhancement 3: Quick Reset
```cpp
// L+R+Start = soft reset
if (L && R && Start) {
    S9xSoftReset();
    ShowOSD("Reset", 1000);
}
```

### Enhancement 4: Cheat Code Shortcuts
```cpp
// L+R+X = toggle cheats on/off
// No menu needed, just enable/disable all
```

## Testing Plan

### Test Cases
1. **State save/load**: Verify all slots work (0-9)
2. **ROM loading**: Test with various ROM formats
3. **Config parsing**: Verify all settings load correctly
4. **Controller types**: Test GC pad, Wiimote, Classic
5. **Edge cases**: 
   - No ROMs in directory
   - Corrupted save states
   - Invalid config values
   - USB/SD card removal during operation

### Performance Testing
1. **Memory usage**: Verify <100 KB overhead
2. **Startup time**: Measure boot to game
3. **State save/load time**: Should be <100ms
4. **ROM selector responsiveness**: Should be instant

## Conclusion

This minimal menu approach:

1. **Drastically reduces complexity** (7,500 lines → ~500 lines)
2. **Improves performance** (less code, less memory, faster)
3. **Enhances workflow** (direct actions vs menu navigation)
4. **Maintains compatibility** (same save formats, same config)
5. **Empowers users** (config file for full control)

The trade-off is that configuration must be done via file editing rather than in-game menus, but this is acceptable for:
- Power users who prefer config files
- Emulation enthusiasts familiar with editing configs
- Users who rarely change settings after initial setup

For those who need GUI configuration, a separate PC/web-based tool could be created to generate the settings.xml file.

**Estimated implementation time: 10-15 hours total**
**Code size savings: 99% (309 KB)**
**Performance improvement: Significant (faster startup, less memory, cleaner code)**
