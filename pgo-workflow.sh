#!/bin/bash
# Complete PGO Workflow Script using Docker
# This script automates the entire PGO process

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Configuration
PLATFORMS=("wii" "gc")
PGO_DATA_DIR="./pgo_data"

echo -e "${GREEN}Snes9x GC/Wii Complete PGO Workflow${NC}"
echo "======================================"

# Function to run docker commands
docker_exec() {
    docker-compose run --rm -T builder "$@"
}

# Function to run make in docker
docker_make() {
    local target=$1
    echo -e "${YELLOW}Running: make $target${NC}"
    docker_exec make "$target"
}

# Function to check if PGO data exists
check_pgo_data() {
    if [ ! -d "$PGO_DATA_DIR" ]; then
        echo -e "${RED}Error: PGO data directory '$PGO_DATA_DIR' not found!${NC}"
        echo "Please ensure you have collected profile data from the target hardware."
        echo "See PGO_OPTIMIZATION.md for instructions on collecting profile data."
        exit 1
    fi

    local file_count=$(find "$PGO_DATA_DIR" -name "*.gcda" | wc -l)
    if [ "$file_count" -eq 0 ]; then
        echo -e "${RED}Error: No profile data files (*.gcda) found in '$PGO_DATA_DIR'!${NC}"
        exit 1
    fi

    echo -e "${GREEN}Found $file_count profile data files${NC}"
}

# Function to show workflow steps
show_workflow() {
    echo -e "${BLUE}PGO Workflow Steps:${NC}"
    echo "1. Build PGO instrumented versions"
    echo "2. Transfer .dol files to target hardware"
    echo "3. Run instrumented versions and collect profile data"
    echo "4. Transfer pgo_data/ directory back to development machine"
    echo "5. Build optimized versions using profile data"
    echo ""
}

# Function to build PGO instrumented versions
build_instrumented() {
    echo -e "${BLUE}Step 1: Building PGO instrumented versions${NC}"

    for platform in "${PLATFORMS[@]}"; do
        echo -e "${YELLOW}Building instrumented version for $platform...${NC}"
        docker_make "${platform}-pgo-generate"
        echo -e "${GREEN}✓ $platform instrumented build complete${NC}"
    done

    echo ""
    echo -e "${GREEN}Instrumented builds completed!${NC}"
    echo "Transfer these files to your target hardware:"
    echo "  - executables/snes9xgx-wii_pgo_gen.dol"
    echo "  - executables/snes9xgx-gc_pgo_gen.dol"
    echo ""
    echo -e "${YELLOW}Next: Run these on your Wii/GameCube and collect profile data${NC}"
}

# Function to build optimized versions
build_optimized() {
    echo -e "${BLUE}Step 5: Building PGO optimized versions${NC}"

    check_pgo_data

    for platform in "${PLATFORMS[@]}"; do
        echo -e "${YELLOW}Building optimized version for $platform...${NC}"
        docker_make "${platform}-pgo-optimize"
        echo -e "${GREEN}✓ $platform optimized build complete${NC}"
    done

    echo ""
    echo -e "${GREEN}PGO optimization complete!${NC}"
    echo "Optimized builds available:"
    echo "  - executables/snes9xgx-wii_pgo_use.dol"
    echo "  - executables/snes9xgx-gc_pgo_use.dol"
}

# Function to clean everything
clean_all() {
    echo -e "${BLUE}Cleaning all builds and PGO data...${NC}"
    docker_make "pgo-clean"
    docker_make "clean"
    echo -e "${GREEN}✓ All cleaned${NC}"
}

# Main menu
case "${1:-menu}" in
    instrumented|1)
        build_instrumented
        ;;

    optimized|5)
        build_optimized
        ;;

    clean)
        clean_all
        ;;

    workflow|menu)
        show_workflow
        echo "Choose an option:"
        echo "1. Build instrumented versions (Step 1)"
        echo "5. Build optimized versions (Step 5)"
        echo "clean. Clean all builds and data"
        echo ""
        echo "Run: $0 <option>"
        ;;

    *)
        echo -e "${RED}Invalid option: $1${NC}"
        echo "Run '$0 menu' for options"
        exit 1
        ;;
esac