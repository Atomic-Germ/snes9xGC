# Docker Build Setup for Snes9x GC/Wii

This directory contains Docker-based build scripts for Snes9x GC/Wii development.

## Prerequisites

- Docker installed and running
- At least 4GB free disk space for build artifacts

## Quick Start

```bash
# Build regular versions for both platforms
./docker-build.sh build all

# For PGO optimization, see PGO_OPTIMIZATION.md
./pgo-workflow.sh instrumented
```

## Available Scripts

### docker-build.sh
Main build script supporting multiple commands and platforms.

```bash
./docker-build.sh [COMMAND] [PLATFORM]

Commands:
  build          - Build regular version
  pgo-generate   - Build PGO instrumented version
  pgo-optimize   - Build PGO optimized version
  clean          - Clean build artifacts
  pgo-clean      - Clean PGO data and builds

Platforms:
  wii            - Nintendo Wii
  gc             - Nintendo GameCube
  all            - Both platforms

Examples:
  ./docker-build.sh build wii
  ./docker-build.sh pgo-generate all
  ./docker-build.sh clean all
```

### pgo-workflow.sh
Complete PGO workflow automation script.

```bash
./pgo-workflow.sh [COMMAND]

Commands:
  instrumented   - Build PGO instrumented versions (Step 1)
  optimized      - Build PGO optimized versions (Step 5)
  clean          - Clean all builds and PGO data
  (no arg)       - Show menu with options
```

## Docker Images

The build scripts use the official devkitPro Docker images:
- `devkitpro/devkitppc:latest` - For Wii and GameCube development

## Manual Docker Usage

```bash
# Interactive build environment
docker-compose run --rm builder

# Direct commands
docker run --rm -v "$(pwd)":/workspace -w /workspace devkitpro/devkitppc:latest make wii
docker run --rm -v "$(pwd)":/workspace -w /workspace devkitpro/devkitppc:latest make gc-pgo-generate
```

## Output Files

Built executables are placed in the `executables/` directory:
- `snes9xgx-wii.dol` - Regular Wii build
- `snes9xgx-gc.dol` - Regular GameCube build
- `snes9xgx-wii_pgo_gen.dol` - PGO instrumented Wii build
- `snes9xgx-gc_pgo_gen.dol` - PGO instrumented GameCube build
- `snes9xgx-wii_pgo_use.dol` - PGO optimized Wii build
- `snes9xgx-gc_pgo_use.dol` - PGO optimized GameCube build

## Troubleshooting

### Build Failures
- Ensure Docker has sufficient memory (4GB recommended)
- Check that all source files are present
- Try cleaning builds: `./docker-build.sh clean all`

### PGO Issues
- Ensure `pgo_data/` directory exists with profile data before optimized builds
- Profile data must be collected on the same platform (Wii profiles for Wii builds)

### Permission Issues
- The scripts handle file permissions automatically
- If you encounter permission errors, ensure Docker can write to the workspace

## Development

To modify the build environment:
1. Edit `Dockerfile` for base image changes
2. Edit `docker-compose.yml` for volume/service configuration
3. Modify build scripts for new commands or platforms

## See Also

- [PGO_OPTIMIZATION.md](PGO_OPTIMIZATION.md) - Complete PGO documentation
- [README.md](../README.md) - Main project documentation