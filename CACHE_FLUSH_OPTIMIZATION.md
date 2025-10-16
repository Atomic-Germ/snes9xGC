# Cache Flush Optimization for SNES9x GC/Wii

## Problem
The original code flushed the entire texture memory buffer on every rendered frame:
```cpp
DCFlushRange(texturemem, TEXTUREMEM_SIZE);  // Always 532,480 bytes
```

This was wasteful because:
- TEXTUREMEM_SIZE = 512 × 520 × 2 = 532,480 bytes
- Most SNES games use 256 × 224 resolution = 112,896 bytes
- Even with 2× filters, that's only 512 × 448 = 450,560 bytes
- Cache flushes are expensive operations on PowerPC hardware

## Solution
Modified `source/video.cpp` to calculate and flush only the actual texture size:

```cpp
// Determine actual texture dimensions based on filtering
u32 actualTexWidth, actualTexHeight;

if (filterIdLocal != FILTER_NONE && vheight <= 239 && vwidth <= 256 && FilterMethod)
{
    // ... filter code ...
    actualTexWidth = vwidth * fscale;
    actualTexHeight = vheight * fscale;
}
else
{
    // ... no filter code ...
    actualTexWidth = vwidth;
    actualTexHeight = vheight;
}

u32 textureSize = actualTexWidth * actualTexHeight * 2;
DCFlushRange(texturemem, textureSize);
```

## Performance Impact

### Standard Resolution Games (256×224)
- **Before**: 532,480 bytes flushed per frame
- **After**: 112,896 bytes flushed per frame
- **Savings**: 419,584 bytes (79% reduction)
- **At 60 FPS**: ~24 MB/sec less cache traffic

### Hi-Res Games (512×448)
- **Before**: 532,480 bytes flushed per frame
- **After**: 450,560 bytes flushed per frame
- **Savings**: 81,920 bytes (15% reduction)
- **At 60 FPS**: ~4.7 MB/sec less cache traffic

### With 2× Filters (512×448)
- **Before**: 532,480 bytes flushed per frame
- **After**: 450,560 bytes flushed per frame
- **Savings**: Same as hi-res (filter output is capped at display resolution)

## Expected Results
- Reduced CPU usage during emulation
- Potentially improved frame pacing consistency
- Lower power consumption
- More headroom for other optimizations

## Testing Recommendations
1. Test with standard resolution games (Super Mario World, etc.)
2. Test with hi-res games (Seiken Densetsu 3, etc.)
3. Test with various filter settings (None, 2xSaI, etc.)
4. Verify no visual artifacts or corruption
5. Measure frame times with and without optimization

## Safety
This change is safe because:
- We're still flushing all data that gets written to the texture
- The texture object initialization already specifies the actual size
- GX hardware only reads the size specified in GX_InitTexObj()
- No out-of-bounds memory access is possible
