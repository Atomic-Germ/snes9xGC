# fileop.cpp Refactoring Summary

## Overview
Refactored device-specific switch statement in `source/fileop.cpp` to use a data-driven device mapping table, eliminating repetitive sprintf calls and platform-specific switch logic.

## Changes Made

### Before
Large switch statement with platform-specific preprocessing:

```cpp
static bool MountFAT(int device, int silent) {
    bool mounted = false;
    char name[10], name2[10];
    DISC_INTERFACE* disc = NULL;

    switch(device) {
#ifdef HW_RVL
        case DEVICE_SD:
            sprintf(name, "sd");
            sprintf(name2, "sd:");
            disc = sd;
            break;
        case DEVICE_USB:
            sprintf(name, "usb");
            sprintf(name2, "usb:");
            disc = usb;
            break;
#else
        case DEVICE_SD_SLOTA:
            sprintf(name, "carda");
            sprintf(name2, "carda:");
            disc = carda;
            break;
        // ... 3 more GameCube devices
#endif
        default:
            return false;
    }
    
    // Use name, name2, and disc...
}
```

### After
Device mapping table with lookup function:

```cpp
struct DeviceMapping {
    int device;
    const char* name;
    const char* name2;
    DISC_INTERFACE** disc;
};

#ifdef HW_RVL
static DeviceMapping deviceMappings[] = {
    {DEVICE_SD, "sd", "sd:", &sd},
    {DEVICE_USB, "usb", "usb:", &usb}
};
#else
static DeviceMapping deviceMappings[] = {
    {DEVICE_SD_SLOTA, "carda", "carda:", &carda},
    {DEVICE_SD_SLOTB, "cardb", "cardb:", &cardb},
    {DEVICE_SD_PORT2, "port2", "port2:", &port2},
    {DEVICE_SD_GCLOADER, "gcloader", "gcloader:", &gcloader}
};
#endif

static const DeviceMapping* GetDeviceMapping(int device) {
    const int numDevices = sizeof(deviceMappings) / sizeof(deviceMappings[0]);
    for (int i = 0; i < numDevices; i++) {
        if (deviceMappings[i].device == device)
            return &deviceMappings[i];
    }
    return NULL;
}

static bool MountFAT(int device, int silent) {
    const DeviceMapping* mapping = GetDeviceMapping(device);
    if (!mapping)
        return false;
    
    DISC_INTERFACE* disc = *mapping->disc;
    // Use mapping->name, mapping->name2, and disc...
}
```

## Function Refactored

**MountFAT()** - Mounts FAT file system on specified device
- Before: 42-line switch statement with 6 cases (platform-dependent)
- After: Single lookup call + null check (4 lines)

### Code Statistics
- **Lines removed**: 42
- **Lines added**: 38 (includes mapping structure + helper function)
- **Net savings**: 4 lines
- **Switch cases eliminated**: 6
- **sprintf calls eliminated**: 12 (2 per case × 6 cases)

## Benefits

1. **Eliminates String Formatting**:
   - No more sprintf calls to construct device names
   - Const strings in table prevent runtime errors
   - Reduced stack usage (no char buffers needed)

2. **Platform Handling**:
   - Platform differences confined to one table definition
   - Same lookup logic works for both platforms
   - Easy to see all devices for each platform at a glance

3. **Maintainability**:
   - Adding new device: Add one table entry
   - All device info in one struct (name, name2, disc interface)
   - Single source of truth for device properties

4. **Type Safety**:
   - Struct enforces consistent device information
   - Compile-time checking of disc interface pointers
   - No risk of sprintf buffer overflow

5. **Readability**:
   - Device properties visible in table format
   - Clear mapping between device ID and names
   - Lookup logic separated from device configuration

## Implementation Details

### Device Mapping Structure
```cpp
struct DeviceMapping {
    int device;           // Device enum value
    const char* name;     // Short name for fatMountSimple
    const char* name2;    // Mount point (with colon)
    DISC_INTERFACE** disc; // Pointer to disc interface pointer
};
```

### Platform-Specific Tables
- **Wii (HW_RVL)**: 2 devices (SD, USB)
- **GameCube**: 4 devices (SlotA, SlotB, Port2, GCLoader)

The same `GetDeviceMapping()` function works for both platforms.

### Lookup Function
Simple linear search through table:
- O(n) where n is small (2-4 devices)
- Early return on match
- NULL return for unknown devices

## Before/After Comparison

### Switch Statement (42 lines)
```cpp
switch(device) {
#ifdef HW_RVL
    case DEVICE_SD:
        sprintf(name, "sd");
        sprintf(name2, "sd:");
        disc = sd;
        break;
    case DEVICE_USB:
        sprintf(name, "usb");
        sprintf(name2, "usb:");
        disc = usb;
        break;
#else
    case DEVICE_SD_SLOTA:
        sprintf(name, "carda");
        sprintf(name2, "carda:");
        disc = carda;
        break;
    // ... 3 more cases
#endif
    default:
        return false;
}
```

### Lookup Call (4 lines)
```cpp
const DeviceMapping* mapping = GetDeviceMapping(device);
if (!mapping)
    return false;
DISC_INTERFACE* disc = *mapping->disc;
```

## Backward Compatibility

✅ **No behavioral changes**  
✅ Same device mounting logic  
✅ Same error handling  
✅ No API changes  
✅ Platform differences preserved  
✅ Ready for GitHub CI validation

## Code Quality Impact

**Before**:
- Repetitive case statements
- Multiple sprintf calls per device
- Platform logic mixed with device logic
- String construction at runtime

**After**:
- Centralized device configuration
- No runtime string construction
- Clear platform separation
- Data-driven device handling

### Memory Impact
- **Saved**: 20 bytes stack space (2× char[10] buffers removed)
- **Added**: Minimal - const strings in read-only data section
- **Net**: Slightly improved (less stack usage)

### Performance Impact
- **Before**: Switch statement O(log n) or O(1) with jump table
- **After**: Linear search O(n) where n is 2-4
- **Practical**: Negligible difference, called infrequently

The performance is essentially the same, but the code is much more maintainable and less error-prone.

## Future Extensibility

Adding a new storage device:
1. Add device constant to appropriate table (Wii or GameCube)
2. Define the disc interface variable
3. No changes to MountFAT or any other functions

The lookup function automatically handles new devices without code changes.
