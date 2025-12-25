#!/bin/bash

# ===== Configuration =====
SRC="src/commands"
BUILD="bettercmd/bin"
VBS_SRC="src/scripts"
COMMANDS="ls ll now cat pwd touch clear stat head tail basename dirname sleep wc randnum randword uptime uniq env pure man runtime trim tr paste ps kill tac hexdump bettercmd"
VBS="install.vbs uninstall.vbs bettercmd.bat"

# ===== Colors for output =====
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# ===== Helper functions =====
error_exit() {
    echo -e "${RED}[!] $1${NC}"
    exit 1
}

info() {
    echo -e "${BLUE}[*] $1${NC}"
}

success() {
    echo -e "${GREEN}[#] $1${NC}"
}

# ===== Cleanup =====
clear
info "Cleaning previous build..."

if [ -d "bettercmd" ]; then
    rm -rf "bettercmd" || error_exit "Failed to delete existing bettercmd directory."
fi

if [ -f "bettercmd.zip" ]; then
    rm -f "bettercmd.zip" || error_exit "Failed to delete existing bettercmd.zip file."
fi

# ===== Create build directory =====
mkdir -p "$BUILD" || error_exit "Failed to create build directory."

# ===== Check for MinGW =====
if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo -e "${YELLOW}[!] MinGW-w64 not found. Installing...${NC}"
    if command -v apt-get &> /dev/null; then
        sudo apt-get update
        sudo apt-get install -y mingw-w64
    elif command -v dnf &> /dev/null; then
        sudo dnf install -y mingw-w64
    elif command -v yum &> /dev/null; then
        sudo yum install -y mingw-w64
    elif command -v pacman &> /dev/null; then
        sudo pacman -S --noconfirm mingw-w64
    else
        error_exit "Please install mingw-w64 manually: x86_64-w64-mingw32-gcc"
    fi
fi

# ===== Compile C commands for Windows =====
info "Compiling commands for Windows..."
for cmd in $COMMANDS; do
    echo -e "    - ${cmd}.c"
    
    # Check if source file exists
    if [ ! -f "$SRC/${cmd}.c" ]; then
        echo -e "${YELLOW}[W] Source file $SRC/${cmd}.c not found, skipping...${NC}"
        continue
    fi
    
    # Compile for Windows using MinGW
    x86_64-w64-mingw32-gcc -o "$BUILD/${cmd}.exe" "$SRC/${cmd}.c" -static -O2
    
    if [ $? -ne 0 ]; then
        error_exit "Failed to compile ${cmd}.c"
    fi
done

# ===== Copy VBS install/uninstall files =====
info "Copying install/uninstall scripts..."
mkdir -p "bettercmd"

for vbs_file in $VBS; do
    if [ -f "$VBS_SRC/$vbs_file" ]; then
        cp "$VBS_SRC/$vbs_file" "bettercmd/" || error_exit "Failed to copy $vbs_file from $VBS_SRC"
    else
        echo -e "${YELLOW}[W] $VBS_SRC/$vbs_file not found${NC}"
    fi
done

# ===== Copy all files from docs to bettercmd =====
if [ -d "docs" ]; then
    info "Copying documentation files..."
    for file in docs/*; do
        if [ -f "$file" ]; then
            filename=$(basename "$file")
            echo "    - $filename"
            cp "$file" "bettercmd/" || error_exit "Failed to copy $filename"
        fi
    done
else
    echo -e "${YELLOW}[W] docs directory not found${NC}"
fi

# ===== Copy man pages =====
info "Copying man pages..."
if [ -d "man" ]; then
    mkdir -p "bettercmd/man"
    cp -r "man/"* "bettercmd/man/" || error_exit "Failed to copy man pages"
else
    echo -e "${YELLOW}[W] man directory not found${NC}"
fi

# ===== Create ZIP archive =====
info "Creating archive bettercmd.zip..."
if command -v zip &> /dev/null; then
    # Create zip file
    if [ -d "bettercmd" ]; then
        zip -r "bettercmd.zip" "bettercmd" || error_exit "Failed to create zip archive"
    else
        error_exit "bettercmd directory not found for archiving"
    fi
elif command -v 7z &> /dev/null; then
    7z a "bettercmd.zip" "bettercmd" || error_exit "Failed to create 7z archive"
else
    echo -e "${YELLOW}[!] zip or 7z command not found. Installing zip...${NC}"
    if command -v apt-get &> /dev/null; then
        sudo apt-get install -y zip
        zip -r "bettercmd.zip" "bettercmd" || error_exit "Failed to create zip archive"
    else
        echo -e "${YELLOW}[W] Skipping archive creation. Package bettercmd/ is ready.${NC}"
    fi
fi

# ===== Finalize =====
echo "-----------------------------------------"
success "Build complete! Windows executable files are in bettercmd/bin/"

# Show file info
if [ -d "bettercmd/bin" ]; then
    echo ""
    echo "Generated Windows executables:"
    ls -la "bettercmd/bin/"*.exe | wc -l | xargs echo "Total .exe files:"
    
    # Test if executables are actually Windows binaries
    if command -v file &> /dev/null && [ -f "bettercmd/bin/ls.exe" ]; then
        echo ""
        echo "Verifying binary format:"
        file "bettercmd/bin/ls.exe"
    fi
fi

exit 0
