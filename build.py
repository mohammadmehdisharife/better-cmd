#!/usr/bin/env python3
"""
build.py - Build bettercmd for Windows
"""

import os
import sys
import shutil
import subprocess
from pathlib import Path

# Configuration
SRC_DIR = Path("src")
COMMANDS_DIR = SRC_DIR / "commands"
BUILD_DIR = Path("bettercmd/bin")
CONF_DIR = SRC_DIR / "conf"
SCRIPTS_DIR = SRC_DIR / "scripts"
LIBS_DIR = SRC_DIR / "libs"
DOCS_DIR = Path("docs")
MAN_DIR = Path("man")

# List of commands to compile
COMMANDS = [
    "basename", "cat", "clear", "dirname", "env", "head", "hexdump",
    "kill", "ll", "ls", "man", "now", "paste", "ps", "pure", "pwd",
    "randnum", "randword", "runtime", "sleep", "stat", "tac", "tail",
    "touch", "tr", "trim", "uniq", "uptime", "wc", "bettercmd"
]

def clean():
    """Remove previous build files"""
    print("Cleaning previous build...")
    
    for path in [Path("bettercmd"), Path("bettercmd.zip")]:
        if path.exists():
            if path.is_dir():
                shutil.rmtree(path)
            else:
                path.unlink()

def get_compiler():
    """Get compiler name based on platform"""
    if sys.platform == "linux":
        return "x86_64-w64-mingw32-gcc"
    else:
        return "gcc"

def compile_commands(compiler):
    """Compile C files to Windows executables"""
    print(f"Compiling commands...")
    
    for cmd_name in COMMANDS:
        c_file = COMMANDS_DIR / f"{cmd_name}.c"
        exe_file = BUILD_DIR / f"{cmd_name}.exe"
        
        if not c_file.exists():
            print(f"  Skip: {c_file.name} not found")
            continue
        
        print(f"  {cmd_name}")
        
        # Build compile command
        cmd = [compiler, "-I", str(LIBS_DIR), "-o", str(exe_file), str(c_file)]
        
        if sys.platform == "linux":
            cmd.extend(["-static", "-O2"])
        
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        if result.returncode != 0:
            print(f"Error compiling {cmd_name}:")
            if result.stderr:
                print(result.stderr[:200])
            sys.exit(1)

def copy_files():
    """Copy all required files"""
    print("Copying files...")
    
    build_root = Path("bettercmd")
    
    # Copy conf directory
    if CONF_DIR.exists():
        shutil.copytree(CONF_DIR, build_root / "conf", dirs_exist_ok=True)
        print("  conf/")
    
    # Copy VBS scripts
    if SCRIPTS_DIR.exists():
        for script in ["install.vbs", "uninstall.vbs"]:
            src = SCRIPTS_DIR / script
            if src.exists():
                shutil.copy2(src, build_root)
                print(f"  {script}")
    
    # Copy docs
    if DOCS_DIR.exists():
        shutil.copytree(DOCS_DIR, build_root / "docs", dirs_exist_ok=True)
        print("  docs/")
    
    # Copy man pages
    if MAN_DIR.exists():
        shutil.copytree(MAN_DIR, build_root / "man", dirs_exist_ok=True)
        print("  man/")

def create_zip():
    """Create zip archive"""
    print("Creating zip archive...")
    
    import zipfile
    
    with zipfile.ZipFile("bettercmd.zip", 'w', zipfile.ZIP_DEFLATED) as zf:
        for item in Path("bettercmd").rglob("*"):
            if item.is_file():
                zf.write(item, item.relative_to("."))
    
    size = Path("bettercmd.zip").stat().st_size // 1024
    print(f"  Created bettercmd.zip ({size} KB)")

def show_summary():
    """Show build summary"""
    print("\n" + "=" * 40)
    print("Build complete!")
    
    # Count executables
    if BUILD_DIR.exists():
        exe_files = list(BUILD_DIR.glob("*.exe"))
        print(f"\nCreated {len(exe_files)} executables")
    
    # Show directory tree
    print("\nDirectory structure:")
    for item in sorted(Path("bettercmd").iterdir()):
        if item.is_dir():
            count = len(list(item.rglob("*")))
            print(f"  {item.name}/ ({count} items)")
        else:
            print(f"  {item.name}")

def main():
    """Main build process"""
    compiler = get_compiler()
    
    # Check compiler exists
    if shutil.which(compiler) is None:
        print(f"Error: {compiler} not found in PATH")
        sys.exit(1)
    
    clean()
    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    
    compile_commands(compiler)
    copy_files()
    create_zip()
    show_summary()

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nBuild cancelled")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
