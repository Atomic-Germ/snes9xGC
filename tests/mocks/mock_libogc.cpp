#include "mock_libogc.h"

// Mock video mode objects
GXRModeObj TVNtsc480IntDf = {
    VI_TVMODE_NTSC_INT,     // viDisplayMode
    640,                    // fbWidth
    480,                    // efbHeight
    480,                    // xfbHeight
    40,                     // viXOrigin
    0,                      // viYOrigin
    640,                    // viWidth
    480,                    // viHeight
    VI_XFBMODE_DF,         // xFBmode
    GX_FALSE,              // field_rendering
    GX_FALSE,              // aa
    {},                    // sample_pattern
    {}                     // vfilter
};

GXRModeObj TVNtsc480Prog = {
    VI_TVMODE_NTSC_PROG,   // viDisplayMode
    640,                   // fbWidth
    480,                   // efbHeight
    480,                   // xfbHeight
    40,                    // viXOrigin
    0,                     // viYOrigin
    640,                   // viWidth
    480,                   // viHeight
    VI_XFBMODE_SF,        // xFBmode
    GX_FALSE,             // field_rendering
    GX_FALSE,             // aa
    {},                   // sample_pattern
    {}                    // vfilter
};

GXRModeObj TVPal576IntDfScale = {
    VI_TVMODE_PAL_INT,    // viDisplayMode
    640,                  // fbWidth
    576,                  // efbHeight
    576,                  // xfbHeight
    40,                   // viXOrigin
    0,                    // viYOrigin
    640,                  // viWidth
    576,                  // viHeight
    VI_XFBMODE_DF,       // xFBmode
    GX_FALSE,            // field_rendering
    GX_FALSE,            // aa
    {},                  // sample_pattern
    {}                   // vfilter
};

GXRModeObj TVEurgb60Hz480IntDf = {
    VI_TVMODE_EURGB60_INT, // viDisplayMode
    640,                   // fbWidth
    480,                   // efbHeight
    480,                   // xfbHeight
    40,                    // viXOrigin
    0,                     // viYOrigin
    640,                   // viWidth
    480,                   // viHeight
    VI_XFBMODE_DF,        // xFBmode
    GX_FALSE,             // field_rendering
    GX_FALSE,             // aa
    {},                   // sample_pattern
    {}                    // vfilter
};

GXRModeObj TVPal576ProgScale = {
    VI_TVMODE_PAL_DS,     // viDisplayMode
    640,                  // fbWidth
    576,                  // efbHeight
    576,                  // xfbHeight
    40,                   // viXOrigin
    0,                    // viYOrigin
    640,                  // viWidth
    576,                  // viHeight
    VI_XFBMODE_SF,       // xFBmode
    GX_FALSE,            // field_rendering
    GX_FALSE,            // aa
    {},                  // sample_pattern
    {}                   // vfilter
};