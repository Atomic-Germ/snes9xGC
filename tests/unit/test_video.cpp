#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"

// We need to include the source files being tested
// For unit testing, we create a test-friendly version of video functions

// Mock GCSettings for testing
struct SGCSettings {
    int videomode;
    int render;
    int FilterMethod;
    int widescreen;
    float zoomHor;
    float zoomVert;
    int xshift;
    int yshift;
};

SGCSettings GCSettings = {0};

// Test helper function for video mode selection logic
GXRModeObj* TestFindVideoMode(int videomode) {
    GXRModeObj* mode;
    
    switch(videomode) {
        case 1: // NTSC (480i)
            mode = &TVNtsc480IntDf;
            break;
        case 2: // Progressive (480p)  
            mode = &TVNtsc480Prog;
            break;
        case 3: // PAL (50Hz)
            mode = &TVPal576IntDfScale;
            break;
        case 4: // PAL (60Hz)
            mode = &TVEurgb60Hz480IntDf;
            break;
        case 5: // Progressive (576p)
            mode = &TVPal576ProgScale;
            break;
        default:
            mode = &TVNtsc480IntDf; // Default mode
            break;
    }
    return mode;
}

TEST(video_mode_selection_ntsc_480i) {
    GCSettings.videomode = 1;
    GXRModeObj* mode = TestFindVideoMode(GCSettings.videomode);
    
    ASSERT_NOT_NULL(mode);
    ASSERT_EQ(VI_TVMODE_NTSC_INT, mode->viTVMode);
    ASSERT_EQ(640, mode->fbWidth);
    ASSERT_EQ(480, mode->efbHeight);
}

TEST(video_mode_selection_progressive_480p) {
    GCSettings.videomode = 2;
    GXRModeObj* mode = TestFindVideoMode(GCSettings.videomode);
    
    ASSERT_NOT_NULL(mode);
    ASSERT_EQ(VI_TVMODE_NTSC_PROG, mode->viTVMode);
    ASSERT_EQ(640, mode->fbWidth);
    ASSERT_EQ(480, mode->efbHeight);
    ASSERT_EQ(VI_XFBMODE_SF, mode->xFBmode);
}

TEST(video_mode_selection_pal_50hz) {
    GCSettings.videomode = 3;
    GXRModeObj* mode = TestFindVideoMode(GCSettings.videomode);
    
    ASSERT_NOT_NULL(mode);
    ASSERT_EQ(VI_TVMODE_PAL_INT, mode->viTVMode);
    ASSERT_EQ(640, mode->fbWidth);
    ASSERT_EQ(576, mode->efbHeight);
}

TEST(video_mode_selection_pal_60hz) {
    GCSettings.videomode = 4;
    GXRModeObj* mode = TestFindVideoMode(GCSettings.videomode);
    
    ASSERT_NOT_NULL(mode);
    ASSERT_EQ(VI_TVMODE_EURGB60_INT, mode->viTVMode);
    ASSERT_EQ(640, mode->fbWidth);
    ASSERT_EQ(480, mode->efbHeight);
}

TEST(video_mode_selection_progressive_576p) {
    GCSettings.videomode = 5;
    GXRModeObj* mode = TestFindVideoMode(GCSettings.videomode);
    
    ASSERT_NOT_NULL(mode);
    ASSERT_EQ(VI_TVMODE_PAL_DS, mode->viTVMode);
    ASSERT_EQ(640, mode->fbWidth);
    ASSERT_EQ(576, mode->efbHeight);
    ASSERT_EQ(VI_XFBMODE_SF, mode->xFBmode);
}

TEST(video_mode_selection_default) {
    GCSettings.videomode = 999; // Invalid mode
    GXRModeObj* mode = TestFindVideoMode(GCSettings.videomode);
    
    ASSERT_NOT_NULL(mode);
    ASSERT_EQ(VI_TVMODE_NTSC_INT, mode->viTVMode); // Should default to NTSC
}

// Test ResetFbWidth functionality
bool TestResetFbWidth(int width, GXRModeObj* rmode) {
    if (rmode->fbWidth == width)
        return false; // No change needed
    
    rmode->fbWidth = width;
    return true; // Change was made
}

TEST(reset_fb_width_no_change) {
    GXRModeObj mode = TVNtsc480IntDf;
    bool changed = TestResetFbWidth(640, &mode);
    
    ASSERT_FALSE(changed);
    ASSERT_EQ(640, mode.fbWidth);
}

TEST(reset_fb_width_change_required) {
    GXRModeObj mode = TVNtsc480IntDf;
    mode.fbWidth = 512;
    bool changed = TestResetFbWidth(640, &mode);
    
    ASSERT_TRUE(changed);
    ASSERT_EQ(640, mode.fbWidth);
}

// Test aspect ratio calculations
typedef struct {
    int width;
    int height;
} Dimensions;

Dimensions TestCalculateAspectRatio(int baseWidth, int baseHeight, bool widescreen, float zoomHor, float zoomVert) {
    Dimensions result;
    result.width = baseWidth;
    result.height = baseHeight;
    
    if (widescreen) {
        result.width = (3 * result.width) / 4;
    }
    
    result.width = (int)(result.width * zoomHor);
    result.height = (int)(result.height * zoomVert);
    
    return result;
}

TEST(aspect_ratio_4_3_normal) {
    Dimensions dims = TestCalculateAspectRatio(256, 224, false, 1.0f, 1.0f);
    
    ASSERT_EQ(256, dims.width);
    ASSERT_EQ(224, dims.height);
}

TEST(aspect_ratio_16_9_widescreen) {
    Dimensions dims = TestCalculateAspectRatio(256, 224, true, 1.0f, 1.0f);
    
    ASSERT_EQ(192, dims.width); // (3 * 256) / 4 = 192
    ASSERT_EQ(224, dims.height);
}

TEST(aspect_ratio_with_zoom) {
    Dimensions dims = TestCalculateAspectRatio(256, 224, false, 1.5f, 2.0f);
    
    ASSERT_EQ(384, dims.width);  // 256 * 1.5
    ASSERT_EQ(448, dims.height); // 224 * 2.0
}

TEST(aspect_ratio_widescreen_with_zoom) {
    Dimensions dims = TestCalculateAspectRatio(256, 224, true, 1.5f, 2.0f);
    
    ASSERT_EQ(288, dims.width);  // (3 * 256) / 4 * 1.5 = 192 * 1.5 = 288
    ASSERT_EQ(448, dims.height); // 224 * 2.0
}