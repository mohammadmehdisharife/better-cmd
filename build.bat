@echo off
setlocal enabledelayedexpansion

rem === Configuration ===
set "SRC=src\commands"
set "BUILD=better-cmd\bin"
set "VBS_SRC=src\scripts"
set "COMMANDS=ls ll now cat pwd touch clear"
set "VBS=install.vbs uninstall.vbs"

rem === Cleanup ===
echo [*] Cleaning previous build...
if exist "better-cmd" (
    rmdir /s /q "better-cmd"
    if errorlevel 1 (
        echo [!] Failed to delete existing better-cmd directory.
        exit /b 1
    )
)

rem === Create build directory ===
mkdir "%BUILD%"
if errorlevel 1 (
    echo [!] Failed to create build directory.
    exit /b 1
)

rem === Compile C commands ===
echo [*] Compiling commands...
for %%F in (%COMMANDS%) do (
    echo    - %%F.c
    gcc -o "%BUILD%\%%F.exe" "%SRC%\%%F.c"
    if errorlevel 1 (
        echo [!] Failed to compile %%F.c
        exit /b 1
    )
)

rem === Copy VBS install/uninstall files ===
echo [*] Copying install/uninstall scripts...
rem Make sure the target directory exists
if not exist "better-cmd" mkdir "better-cmd"
for %%F in (%VBS%) do (
    copy "%VBS_SRC%\%%F" "better-cmd\" >nul
    if errorlevel 1 (
        echo [!] Failed to copy %%F from %VBS_SRC%
        exit /b 1
    )
)

rem === Create ZIP archive using tar ===
echo [*] Creating archive better-cmd.zip using tar...
where tar >nul 2>&1
if errorlevel 1 (
    echo [!] tar command not found, skipping archive creation.
) else (
    tar -a -c -f better-cmd.zip better-cmd
    if errorlevel 1 (
        echo [!] Failed to create archive with tar.
    )
)

echo [✓] Build complete.
exit /b 0
