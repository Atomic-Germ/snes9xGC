# SNES9x GC/Wii Performance Analysis - Main Loop Bottlenecks

## Overview
This document analyzes potential performance bottlenecks in the main emulation loop of SNES9x for GameCube/Wii.

## Main Loop Structure

The main emulation loop is located in `source/snes9xgx.cpp` lines 572-592:

```cpp
while(1) // emulation loop
{
    S9xMainLoop();      // Core SNES emulation
    ReportButtons();    // Input handling
    
    if(ResetRequested) { ... }
    if(ConfigRequested) { ... }
    if(ShutdownRequested) { ... }
}
```

---

## Performance Bottlenecks Identified

### 1. **Video Synchronization Spin Loops** ⚠️ HIGH IMPACT

**Location:** `source/s9xsupport.cpp:96-99`

```cpp
while (FrameTimer == 0)
{
    usleep(50);
}
```

**Problem:** This is a busy-wait loop that wakes up every 50 microseconds to check if it's time to render the next frame. While it does sleep, it still consumes CPU cycles unnecessarily.

**Additional instances:**
- `source/s9xsupport.cpp:138-141` - Similar busy-wait for PAL timing
- `source/video.cpp:764-765` - Video buffer wait loop

**Impact:** Moderate - These short sleeps add up, especially at 60 FPS (checks ~300 times per frame in worst case)

**Potential fix:** Use proper event-based synchronization instead of polling, or increase sleep duration with better frame prediction.

---

### 2. **Memory Clearing on Video Changes** ⚠️ MEDIUM-HIGH IMPACT

**Location:** `source/video.cpp:783`

```cpp
memset(filtermem, 0, FILTERMEM_SIZE);
```

Where `FILTERMEM_SIZE = (512*MAX_SNES_HEIGHT*4)` = ~491KB

**Problem:** Every time CheckVideo is set (mode changes, resolution changes), the entire filter memory buffer is cleared. This is a large memory operation that stalls the CPU.

**Impact:** High during mode changes, but only occasional during normal gameplay. However, if CheckVideo gets set repeatedly, this becomes significant.

**Potential fix:** Only clear if actually needed, or clear incrementally, or use a smaller clear region.

---

### 3. **WPAD_Probe() Called Every Frame** ⚠️ MEDIUM IMPACT

**Location:** `source/input.cpp:360-361`

```cpp
u32 exp_type;
if ( WPAD_Probe(chan, &exp_type) != 0 )
    exp_type = WPAD_EXP_NONE;
```

**Problem:** This is called in `decodepad()` for each active controller (up to 4) every single frame. WPAD_Probe() is a hardware I/O operation that communicates with the Wii Remote over Bluetooth.

**Impact:** Medium - 4 hardware probes per frame at 60 FPS = 240 probes/second. This is expensive I/O.

**Potential fix:** Cache the expansion type and only re-probe on controller state change or periodically (e.g., every 60 frames).

---

### 4. **Multiple Controller Button Scans** ⚠️ LOW-MEDIUM IMPACT

**Location:** `source/input.cpp:159-168`

```cpp
void UpdatePads()
{
    #ifdef HW_RVL
    WiiDRC_ScanPads();
    Retrode_ScanPads();
    XBOX360_ScanPads();
    Hornet_ScanPads();
    Mayflash_ScanPads();
    WPAD_ScanPads();
    #endif
    PAD_ScanPads();
    ...
}
```

**Problem:** Six different controller scan functions are called every frame, even if those controllers aren't connected or being used.

**Impact:** Low-Medium - Most of these probably have early exits if devices aren't present, but still overhead.

**Potential fix:** Only scan for controllers that are actually detected/enabled.

---

### 5. **Button Mapping Loop Inefficiency** ⚠️ LOW IMPACT

**Location:** `source/input.cpp:463-477`

```cpp
for (i = 0; i < MAXJP; i++)
{
    if ( (jp & btnmap[CTRL_PAD][CTRLR_GCPAD][i])
    #ifdef HW_RVL
    || ( (exp_type == WPAD_EXP_NONE) && (wp & btnmap[CTRL_PAD][CTRLR_WIIMOTE][i]) )
    || ( (exp_type == WPAD_EXP_CLASSIC && !isWUPC) && (wp & btnmap[CTRL_PAD][CTRLR_CLASSIC][i]) )
    || ( (exp_type == WPAD_EXP_CLASSIC && isWUPC) && (wp & btnmap[CTRL_PAD][CTRLR_WUPC][i]) )
    || ( (exp_type == WPAD_EXP_NUNCHUK) && (wp & btnmap[CTRL_PAD][CTRLR_NUNCHUK][i]) )
    || ( (wiidrcp & btnmap[CTRL_PAD][CTRLR_WIIDRC][i]) )
    #endif
    )
        S9xReportButton(offset + i, true);
    else
        S9xReportButton(offset + i, false);
}
```

**Problem:** This loops through all buttons (MAXJP = 12) for each controller every frame, with multiple complex conditional checks. The exp_type checks could be factored out.

**Impact:** Low - The operations are simple bit checks, but done frequently (12 * 4 controllers * 60 FPS = 2,880 iterations/second)

**Potential fix:** Pre-select the correct button map based on controller type once, then do a single loop.

---

### 6. **Texture Conversion Every Frame** ⚠️ HIGH IMPACT

**Location:** `source/video.cpp:854-860`

```cpp
if (fm)
    fm((uint8*) GFX.Screen, EXT_PITCH, (uint8*) filtermem, vwidth*fscale*2, vwidth, vheight);
MakeTexture565((char *) filtermem, (char *) texturemem, vwidth*fscale, vheight*fscale);
// or
MakeTexture((char *) GFX.Screen, (char *) texturemem, vwidth, vheight);
```

**Problem:** Every rendered frame, the SNES video output must be converted to a GX texture format. This involves either:
- Running a filter (fm function pointer) + MakeTexture565 conversion
- Direct MakeTexture conversion

The `MakeTexture` function (lines 698-736) uses assembly but still processes every pixel.

**Impact:** HIGH - This is unavoidable for rendering but is one of the most expensive operations per frame. The filter adds extra overhead.

**Potential fix:** The assembly is already optimized. Possible improvements:
- Use DMA for memory copies where possible
- Consider using GX hardware texture conversion if available
- Minimize filter complexity

---

### 7. **Cache Flushing** ✅ FIXED

**Location:** `source/video.cpp:867`

**Previous Problem:** Flushing the data cache for the entire texture memory every frame was expensive - always flushing 532,480 bytes even though most games only use 256x224 (112,896 bytes).

**Fix Applied:** Modified the code to calculate and flush only the actual texture size being used:
```cpp
u32 textureSize = actualTexWidth * actualTexHeight * 2;
DCFlushRange(texturemem, textureSize);
```

**Performance Improvement:**
- Standard 256x224 games: 112KB flushed vs 520KB (79% reduction!)
- Hi-res 512x448 games: 448KB flushed vs 520KB (14% reduction)
- This optimization saves significant CPU cycles every frame (60 times per second)

---

### 8. **Video Thread Synchronization** ⚠️ LOW IMPACT

**Location:** `source/video.cpp:764-765`

```cpp
while ((LWP_ThreadIsSuspended(vbthread) == 0) || (copynow == GX_TRUE))
    usleep(50);
```

**Problem:** Another spin-wait checking if the video thread is ready. This blocks the emulation thread.

**Impact:** Low - Usually the video thread completes quickly, but this can add latency.

**Potential fix:** Use proper thread synchronization primitives (semaphores/conditions) instead of polling.

---

### 9. **Redundant Controller Type Checks** ⚠️ LOW IMPACT

**Location:** Throughout `decodepad()` function

**Problem:** The controller type (exp_type, isWUPC) is checked repeatedly in multiple locations within the same function call.

**Impact:** Low - Branch prediction should handle this well, but it's still redundant.

**Potential fix:** Determine controller type once at the start, use a switch or function pointer.

---

## Summary of Key Issues by Priority

### Critical (Fix First):
1. **Texture conversion and cache flushing** - Happens every frame, ~40KB-500KB memory operations
   - ✅ **FIXED**: Cache flush size optimized to match actual texture size (79% reduction for typical games)

### High Priority:
2. **WPAD_Probe hardware calls** - 240 hardware I/O operations per second
   - Cache controller type, probe only on changes
   
3. **Video sync spin loops** - Multiple busy-wait loops with usleep(50)
   - Use event-based synchronization

### Medium Priority:
4. **Large memset operations** - 491KB clear on video mode changes
   - Only clear when necessary or in smaller chunks

5. **Multiple controller scans** - 6 scan functions called every frame
   - Skip scans for disconnected devices

### Low Priority (Micro-optimizations):
6. **Button mapping loop structure** - Can be streamlined but impact is minimal
7. **Video thread sync** - Rare blocking, low impact
8. **Redundant controller type checks** - Already fast due to branch prediction

---

## Performance Measurement Recommendations

To validate these bottlenecks and measure improvements:

1. Add cycle counters around critical sections:
   ```cpp
   u64 start = gettime();
   // ... code section ...
   u64 elapsed = diff_usec(start, gettime());
   ```

2. Profile specific areas:
   - Time spent in S9xMainLoop() vs ReportButtons() vs update_video()
   - Frame time variance (jitter)
   - Controller input latency

3. Test with different configurations:
   - Filters ON vs OFF
   - Different controller types
   - Different video resolutions (256x224 vs hi-res modes)

---

## Additional Notes

- The core SNES9x emulation in `S9xMainLoop()` (cpuexec.cpp) is heavily optimized and likely not a bottleneck
- Most slowdowns probably come from the wrapper/integration code (video, input, sync)
- The assembly optimized `MakeTexture` is already well-optimized for the platform
- GameCube CPU (485 MHz) vs Wii CPU (729 MHz) will show different bottlenecks
