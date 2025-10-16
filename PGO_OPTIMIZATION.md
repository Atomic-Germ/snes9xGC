# Profile-Guided Optimization (PGO) for Snes9x GC/Wii

## Overview

Profile-Guided Optimization (PGO) is a compiler optimization technique that uses runtime profiling data to make better optimization decisions. This can provide 5-15% performance improvements by optimizing code paths that are actually executed during gameplay.

## Prerequisites

- Docker installed and running
- Access to Wii/GameCube hardware for profile collection

## Quick Start with Docker

The easiest way to use PGO is with the provided Docker scripts:

```bash
# Step 1: Build instrumented versions
./pgo-workflow.sh instrumented

# Step 2: Transfer .dol files to your Wii/GameCube and collect profiles
# (See detailed instructions below)

# Step 3: Build optimized versions
./pgo-workflow.sh optimized
```

## Docker-Based Building

### Using the Build Script

```bash
# Build regular versions
./docker-build.sh build all

# Build PGO instrumented versions
./docker-build.sh pgo-generate all

# Build PGO optimized versions
./docker-build.sh pgo-optimize all

# Clean builds
./docker-build.sh clean all
./docker-build.sh pgo-clean all
```

### Using Docker Compose

```bash
# Start interactive build environment
docker-compose run --rm builder

# Then run make commands inside the container
make wii-pgo-generate
make gc-pgo-optimize
```

### Manual Docker Commands

```bash
# Build Wii version
docker run --rm -v "$(pwd)":/workspace -w /workspace devkitpro/devkitppc:latest make wii

# Build with PGO
docker run --rm -v "$(pwd)":/workspace -w /workspace devkitpro/devkitppc:latest make wii-pgo-generate
```

## PGO Workflow Script

For the complete automated workflow, use `pgo-workflow.sh`:

```bash
# Show available options
./pgo-workflow.sh

# Build instrumented versions (Step 1)
./pgo-workflow.sh instrumented

# Build optimized versions (Step 5)
./pgo-workflow.sh optimized

# Clean everything
./pgo-workflow.sh clean
```

## Traditional Building (Without Docker)

If you prefer to build without Docker, ensure devkitPPC is installed and run:

### Wii Targets

```bash
# Generate profile-instrumented Wii build
make wii-pgo-generate

# Build optimized Wii version (after collecting profiles)
make wii-pgo-optimize

# Clean PGO data and builds
make wii-pgo-clean
```

### GameCube Targets

```bash
# Generate profile-instrumented GameCube build
make gc-pgo-generate

# Build optimized GameCube version (after collecting profiles)
make gc-pgo-optimize

# Clean PGO data and builds
make gc-pgo-clean
```

### Step 1: Build Profile-Instrumented Version

```bash
# For Wii
make wii-pgo-generate

# For GameCube
make gc-pgo-generate
```

This creates `snes9xgx-wii_pgo_gen.dol` or `snes9xgx-gc_pgo_gen.dol` in the `executables/` directory.

### Step 2: Transfer to Target Hardware

Copy the instrumented `.dol` file to your Wii/GameCube storage device (SD card, USB drive, etc.).

### Step 3: Run and Collect Profiles

Run the instrumented version on your Wii/GameCube and play various games for 10-15 minutes total. The more representative gameplay, the better the optimization.

**Recommended test scenarios:**
- Standard resolution games (Super Mario World, Zelda: A Link to the Past)
- Hi-resolution games (Star Ocean, Seiken Densetsu 3)
- Mode 7 games (F-Zero, Super Mario Kart)
- Games with different video filters enabled
- Both NTSC and PAL games if available

The instrumented build will be slightly slower due to profiling overhead, but this is normal.

### Step 4: Extract Profile Data

After running the instrumented version, the profile data will be stored in the `pgo_data/` directory on your storage device. Copy this directory back to your development machine.

**Important:** The profile data files are binary and platform-specific. Make sure to collect profiles on the same platform you'll be optimizing for (Wii profiles for Wii builds, GameCube profiles for GameCube builds).

### Step 5: Build Optimized Version

Place the `pgo_data/` directory in the project root, then build the optimized version:

```bash
# For Wii
make wii-pgo-optimize

# For GameCube
make gc-pgo-optimize
```

This creates `snes9xgx-wii_pgo_use.dol` or `snes9xgx-gc_pgo_use.dol` with PGO optimizations applied.

## Expected Performance Improvements

- **5-15% overall performance improvement**
- Better optimization of frequently executed code paths
- Improved branch prediction
- More efficient register allocation
- Reduced instruction cache misses

## Technical Details

### Compiler Flags Used

- **Profile Generation**: `-fprofile-generate=pgo_data`
- **Profile Usage**: `-fprofile-use=pgo_data`

### Build Directories

- Normal builds: `build_wii/`, `build_gc/`
- PGO generation: `build_wii_pgo_gen/`, `build_gc_pgo_gen/`
- PGO optimized: `build_wii_pgo_use/`, `build_gc_pgo_use/`

### Profile Data Files

The `pgo_data/` directory contains binary profile files (`.gcda` files) that are generated during execution of the instrumented build.

## Troubleshooting

### Profile Collection Issues

- **No profile data generated**: Make sure the instrumented version runs long enough (at least 5-10 minutes of gameplay)
- **Profile data corrupted**: Ensure proper transfer of the `pgo_data/` directory
- **Wrong platform profiles**: Wii profiles won't work for GameCube builds and vice versa

### Build Issues

- **PGO build fails**: Make sure profile data exists in `pgo_data/` directory before running optimized build
- **Linker errors**: Clean all builds (`make clean`) and rebuild from scratch

### Performance Issues

- **No improvement**: Try collecting profiles from different games or longer gameplay sessions
- **Worse performance**: This is rare but possible; fall back to regular `-O3` build

## Best Practices

1. **Collect diverse profiles**: Test various game types and settings
2. **Use representative workloads**: Profile actual gameplay, not just menus
3. **Keep profiles current**: Re-collect profiles when making significant code changes
4. **Test thoroughly**: Verify the optimized build works correctly before distribution

## Compatibility

PGO-optimized builds are fully compatible with regular builds. The optimization only affects internal code layout and does not change the emulator's behavior or save file formats.