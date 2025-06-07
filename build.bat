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
if exist better-cmd.zip (
    del /f /q better-cmd.zip
    if errorlevel 1 (
        echo [!] Failed to delete existing better-cmd.zip file.
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
if not exist "better-cmd" mkdir "better-cmd"
for %%F in (%VBS%) do (
    copy "%VBS_SRC%\%%F" "better-cmd\" >nul
    if errorlevel 1 (
        echo [!] Failed to copy %%F from %VBS_SRC%
        exit /b 1
    )
)

rem === Copy INSTALLATION.html from docs ===
echo [*] Copying INSTALLATION.html...
if exist docs\INSTALLATION.html (
    copy docs\INSTALLATION.html "better-cmd\" /Y >nul
    if errorlevel 1 (
        echo [!] Failed to copy INSTALLATION.html
        exit /b 1
    )
) else (
    echo [!] docs\INSTALLATION.html not found!
    exit /b 1
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
        exit /b 1
    )
)

echo [#] Build complete.
exit /b 0
