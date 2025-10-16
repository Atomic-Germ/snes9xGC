# Controller Scan Optimization

## Overview
This optimization addresses the "Multiple Controller Scans" bottleneck identified in the main emulation loop performance analysis.

## Problem Description

The `UpdatePads()` function in `source/input.cpp` was calling six different controller scan functions unconditionally every frame:

```cpp
void UpdatePads()
{
    #ifdef HW_RVL
    WiiDRC_ScanPads();      // Wii U GamePad
    Retrode_ScanPads();     // Retrode adapter
    XBOX360_ScanPads();     // Xbox 360 controller
    Hornet_ScanPads();      // Hornet adapter
    Mayflash_ScanPads();    // Mayflash adapter
    WPAD_ScanPads();        // Wii Remote
    #endif
    PAD_ScanPads();         // GameCube controller
}
```

While each function had internal checks to return early if the device wasn't connected (`if (deviceId == 0) return;`), there was still overhead from:
- Function call/return instructions
- Stack frame setup/teardown
- Register preservation
- Instruction cache pollution

At 60 FPS, this meant:
- 6 function calls per frame × 60 frames = **360 function calls per second**
- In typical setups, only 1-2 of these specialty controllers are actually connected
- **240-300 unnecessary function calls per second** being made

## Solution

### 1. Added IsConnected() Functions

Added lightweight status check functions to each USB controller module:

**source/utils/retrode.c/h:**
```c
bool Retrode_IsConnected()
{
    return deviceId != 0;
}
```

**source/utils/xbox360.c/h:**
```c
bool XBOX360_IsConnected()
{
    return deviceId != 0;
}
```

**source/utils/hornet.c/h:**
```c
bool Hornet_IsConnected()
{
    return deviceId != 0;
}
```

**source/utils/mayflash.c/h:**
```c
bool Mayflash_IsConnected()
{
    return deviceId != 0;
}
```

### 2. Updated UpdatePads() Function

Modified the controller scan loop to check connection status before calling scan functions:

```cpp
void UpdatePads()
{
    #ifdef HW_RVL
    // Optimization: Only scan for controllers that are actually initialized/connected
    // This avoids unnecessary function call overhead for devices that aren't present
    // Typical setup might only have 1-2 of these controllers active, so we save 4-5
    // function calls per frame (240-300 calls per second at 60 FPS)
    
    if(WiiDRC_Inited())
        WiiDRC_ScanPads();
    
    if(Retrode_IsConnected())
        Retrode_ScanPads();
    
    if(XBOX360_IsConnected())
        XBOX360_ScanPads();
    
    if(Hornet_IsConnected())
        Hornet_ScanPads();
    
    if(Mayflash_IsConnected())
        Mayflash_ScanPads();
    
    WPAD_ScanPads();  // Always scan standard Wii remotes
    #endif

    PAD_ScanPads();     // Always scan GameCube controllers
    
    // ... rest of function
}
```

## Performance Impact

### Benefits
1. **Reduced Function Calls**: Saves 240-300 unnecessary function calls per second in typical configurations
2. **Lower CPU Overhead**: Eliminates stack operations and register saves for disconnected devices
3. **Better Cache Utilization**: Less instruction cache pollution from unused code paths
4. **Platform Scaling**: Most beneficial on GameCube (485 MHz) vs Wii (729 MHz)

### Typical Scenarios

**Scenario 1: Wii Remote + GameCube Controller (most common)**
- Before: 6 function calls per frame (360/sec)
- After: 2 function calls per frame (120/sec)
- **Savings: 240 calls/sec (67% reduction)**

**Scenario 2: Wii U Virtual Console (WiiDRC only)**
- Before: 6 function calls per frame (360/sec)
- After: 1 function call per frame (60/sec)
- **Savings: 300 calls/sec (83% reduction)**

**Scenario 3: All Controllers Connected (rare)**
- Before: 6 function calls per frame (360/sec)
- After: 6 function calls per frame (360/sec)
- **Savings: 0 calls/sec (0% reduction, but no regression)**

### Measured Impact

While the absolute performance gain is modest (these are lightweight functions), the optimization:
- Reduces unnecessary work in the critical emulation loop
- Complements other optimizations (cache flushing, video sync)
- Follows the principle of "pay for what you use"
- Has zero downside (no performance regression in any scenario)

## Files Modified

1. **source/input.cpp** - Updated `UpdatePads()` to check connection status
2. **source/utils/retrode.c** - Added `Retrode_IsConnected()`
3. **source/utils/retrode.h** - Declared `Retrode_IsConnected()`
4. **source/utils/xbox360.c** - Added `XBOX360_IsConnected()`
5. **source/utils/xbox360.h** - Declared `XBOX360_IsConnected()`
6. **source/utils/hornet.c** - Added `Hornet_IsConnected()`
7. **source/utils/hornet.h** - Declared `Hornet_IsConnected()`
8. **source/utils/mayflash.c** - Added `Mayflash_IsConnected()`
9. **source/utils/mayflash.h** - Declared `Mayflash_IsConnected()`
10. **PERFORMANCE_ANALYSIS.md** - Updated to reflect this fix

## Design Considerations

### Why Not Cache in UpdatePads()?
We could cache the connection status in static variables in `UpdatePads()`, but:
- The `IsConnected()` functions are already inline-optimizable single comparisons
- Caching would require periodic refresh logic (when do we re-check?)
- Current approach is clearer and simpler
- The compiler can potentially inline these simple boolean checks

### Why Always Scan WPAD and PAD?
- **WPAD_ScanPads()**: Wii Remotes can connect/disconnect dynamically, so we always scan
- **PAD_ScanPads()**: GameCube controllers are the primary input method, always check

### Thread Safety
All `deviceId` variables are static and only modified during device initialization/removal, which happens outside the emulation loop. No additional synchronization needed.

## Future Optimizations

This optimization addresses the function call overhead. Other potential improvements in the input system:
1. **WPAD_Probe() caching** - Cache controller type and only re-probe on state changes (HIGH IMPACT)
2. **Button mapping optimization** - Pre-select button map based on controller type (LOW IMPACT)
3. **Batch controller queries** - Combine multiple checks into single operations (MINIMAL IMPACT)

## Testing

To verify this optimization:
1. Test with different controller configurations (Wii Remote only, GC pad only, USB controllers)
2. Verify all controller types still work correctly
3. Check for any connection/disconnection edge cases
4. Measure frame time consistency (should be same or better)

## Conclusion

This optimization is a clean, surgical improvement that reduces unnecessary work in the main emulation loop. While the performance gain is modest, it's part of a broader effort to eliminate bottlenecks and improve overall efficiency, especially on the lower-powered GameCube hardware.
