# Phase 1 Debugging - C-Stick State Save/Load Not Working

## Issue

The C-stick left/right was not triggering state save/load, even though the code appeared correct. The L+R+Start menu access change worked fine.

## Root Causes Discovered

### 1. Hardcoded to Controller Port 1
**Problem:** The original code only checked `userInput[0]` (port 1).
```cpp
s8 substickX = userInput[0].pad.substickX;  // Only port 1!
```

**Symptom:** C-stick worked in port 1, but not ports 2-4.

**Fix:** Loop through all controller ports:
```cpp
for (int i = 0; i < 4; i++) {
    s8 substickX = userInput[i].pad.substickX;
    // Check each controller
}
```

### 2. Test Code Caused Crashes
**Problem:** Debug test code repeatedly set `ScreenshotRequested = 1`.
```cpp
if (substickX != 0) {
    ScreenshotRequested = 1;  // Every frame!
}
```

**Symptom:** Exiting menu with game loaded crashed to Swiss.

**Fix:** Removed test code, restored actual save/load logic.

### 3. ConfigRequested Check Missing (Already Fixed)
**Problem:** Code could run during menu navigation.

**Fix:** Added `!ConfigRequested` check to only run during gameplay.

## Final Working Code

```cpp
// Quick State Save/Load with C-Stick (GameCube Controller)
// Only process if not in menu and a game is loaded
if (Memory.ROMFilename[0] != 0 && !ConfigRequested) {
    // Check all controller ports, not just port 0
    for (int i = 0; i < 4; i++) {
        s8 substickX = userInput[i].pad.substickX;
        s8 substickY = userInput[i].pad.substickY;
        
        // C-Stick LEFT = Load State (threshold: -70)
        if (substickX < -70 && stateLoadCooldown == 0) {
            LoadSnapshotAuto(SILENT);
            stateLoadCooldown = 30;
            break; // Only one controller per frame
        }
        // C-Stick RIGHT = Save State (threshold: 70)
        else if (substickX > 70 && stateSaveCooldown == 0) {
            SaveSnapshotAuto(SILENT);
            stateSaveCooldown = 30;
            break; // Only one controller per frame
        }
    }
    
    // Decrement cooldowns
    if (stateLoadCooldown > 0) stateLoadCooldown--;
    if (stateSaveCooldown > 0) stateSaveCooldown--;
    if (slotChangeCooldown > 0) slotChangeCooldown--;
}
```

## Key Features

1. **Multi-port support** - Works with controller in any port (1-4)
2. **Break on first match** - Prevents multiple controllers triggering simultaneously
3. **Silent operation** - No popup messages
4. **Cooldown protection** - 30 frames (0.5 sec) prevents accidental double-operations
5. **Menu-aware** - Only active during gameplay, not in menus

## Testing Confirmed

✅ Works in all controller ports (1-4)
✅ No crashes when exiting menu  
✅ L+R+Start menu access still works
✅ Silent state save/load
✅ Cooldown prevents accidents

## Lessons Learned

1. **Always test multiple controller ports** - Don't assume port 1
2. **Flag spam causes crashes** - Don't repeatedly set request flags every frame
3. **Simple isolation tests work** - Menu trigger test revealed the port issue
4. **User testing is critical** - Found issues that code review missed
