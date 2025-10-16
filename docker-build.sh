#!/bin/bash
# Docker-based PGO build script for Snes9x GC/Wii

set -e

IMAGE_NAME="snes9xgc-builder"
CONTAINER_NAME="snes9xgc-build"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Snes9x GC/Wii PGO Build Script${NC}"
echo "================================="

# Function to run make in docker
docker_make() {
    local target=$1
    local platform=$2

    echo -e "${YELLOW}Building ${platform} target: ${target}${NC}"

    docker run --rm \
        -v "$(pwd)":/workspace \
        -w /workspace \
        --name "${CONTAINER_NAME}" \
        devkitpro/devkitppc:latest \
        make "${target}"
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [COMMAND] [PLATFORM]"
    echo ""
    echo "Commands:"
    echo "  build          - Build regular version"
    echo "  pgo-generate   - Build PGO instrumented version"
    echo "  pgo-optimize   - Build PGO optimized version"
    echo "  clean          - Clean build artifacts"
    echo "  pgo-clean      - Clean PGO data and builds"
    echo ""
    echo "Platforms:"
    echo "  wii            - Nintendo Wii"
    echo "  gc             - Nintendo GameCube"
    echo "  all            - Both platforms"
    echo ""
    echo "Examples:"
    echo "  $0 build wii           - Build regular Wii version"
    echo "  $0 pgo-generate all    - Build PGO instrumented versions for both platforms"
    echo "  $0 pgo-optimize gc     - Build PGO optimized GameCube version"
}

# Check arguments
if [ $# -lt 1 ]; then
    show_usage
    exit 1
fi

COMMAND=$1
PLATFORM=${2:-all}

case $PLATFORM in
    wii)
        PLATFORMS=("wii")
        ;;
    gc)
        PLATFORMS=("gc")
        ;;
    all)
        PLATFORMS=("wii" "gc")
        ;;
    *)
        echo -e "${RED}Error: Invalid platform '$PLATFORM'${NC}"
        show_usage
        exit 1
        ;;
esac

# Execute commands
case $COMMAND in
    build)
        for platform in "${PLATFORMS[@]}"; do
            docker_make "$platform" "$platform"
        done
        ;;

    pgo-generate)
        for platform in "${PLATFORMS[@]}"; do
            docker_make "${platform}-pgo-generate" "$platform"
        done
        ;;

    pgo-optimize)
        for platform in "${PLATFORMS[@]}"; do
            docker_make "${platform}-pgo-optimize" "$platform"
        done
        ;;

    clean)
        for platform in "${PLATFORMS[@]}"; do
            docker_make "${platform}-clean" "$platform"
        done
        ;;

    pgo-clean)
        for platform in "${PLATFORMS[@]}"; do
            docker_make "${platform}-pgo-clean" "$platform"
        done
        ;;

    *)
        echo -e "${RED}Error: Invalid command '$COMMAND'${NC}"
        show_usage
        exit 1
        ;;
esac

echo -e "${GREEN}Build completed successfully!${NC}"