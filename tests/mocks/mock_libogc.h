#ifndef MOCK_LIBOGC_H
#define MOCK_LIBOGC_H

#include <cstdlib>
#include <cstddef>

// Mock definitions for libogc types and functions used in testing
// This allows us to compile and test code without the actual GameCube/Wii libraries

// Basic types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;

// Mock GX types
#define GX_FALSE 0
#define GX_TRUE 1

typedef u32 GXBool;
typedef u32 GXColor;

// Mock VI types
#define VI_TVMODE_NTSC_INT 0
#define VI_TVMODE_NTSC_DS 1
#define VI_TVMODE_NTSC_PROG 2
#define VI_TVMODE_PAL_INT 3
#define VI_TVMODE_PAL_DS 4
#define VI_TVMODE_EURGB60_INT 5
#define VI_TVMODE_EURGB60_DS 6

#define VI_XFBMODE_SF 0
#define VI_XFBMODE_DF 1

#define VI_NON_INTERLACE 0
#define VI_INTERLACE 1

#define VI_MAX_WIDTH_NTSC 720
#define VI_MAX_HEIGHT_NTSC 480
#define VI_MAX_WIDTH_PAL 720
#define VI_MAX_HEIGHT_PAL 576

typedef struct {
    u32 viTVMode;
    u16 fbWidth;
    u16 efbHeight;
    u16 xfbHeight;
    u16 viXOrigin;
    u16 viYOrigin;
    u16 viWidth;
    u16 viHeight;
    u32 xFBmode;
    u8 field_rendering;
    u8 aa;
    u8 sample_pattern[12][2];
    u8 vfilter[7];
} GXRModeObj;

// Mock attribute for alignment
#define ATTRIBUTE_ALIGN(x)

// Mock matrix type
typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

// Mock vector type
typedef struct {
    f32 x, y, z;
} guVector;

// Mock memory functions
#define MEM_K0_TO_K1(addr) (addr)
static inline void* memalign(size_t alignment, size_t size) {
    (void)alignment;
    return malloc(size);
}

static inline void DCInvalidateRange(void* addr, u32 size) {
    (void)addr;
    (void)size;
}

static inline void DCFlushRange(void* addr, u32 size) {
    (void)addr;
    (void)size;
}

// Mock video functions
static inline void VIDEO_Init() {}
static inline void VIDEO_SetBlack(int black) { (void)black; }
static inline void VIDEO_Flush() {}
static inline void VIDEO_WaitVSync() {}
static inline void VIDEO_Configure(GXRModeObj* mode) { (void)mode; }
static inline void VIDEO_ClearFrameBuffer(GXRModeObj* mode, void* fb, u32 color) {
    (void)mode; (void)fb; (void)color;
}
static inline void VIDEO_SetNextFramebuffer(void* fb) { (void)fb; }
static inline int VIDEO_GetNextField() { return 0; }
static inline GXRModeObj* VIDEO_GetPreferredMode(void* p) { (void)p; return nullptr; }

// Mock GX functions
static inline void GX_Init(void* fifo, u32 size) { (void)fifo; (void)size; }
static inline void GX_AbortFrame() {}
static inline void GX_Flush() {}
static inline void GX_DrawDone() {}
static inline void GX_CopyDisp(void* dest, GXBool clear) { (void)dest; (void)clear; }

// Mock standard video modes
extern GXRModeObj TVNtsc480IntDf;
extern GXRModeObj TVNtsc480Prog;
extern GXRModeObj TVPal576IntDfScale;
extern GXRModeObj TVEurgb60Hz480IntDf;
extern GXRModeObj TVPal576ProgScale;

#endif // MOCK_LIBOGC_H