@echo off
setlocal enabledelayedexpansion

rem === Configuration ===
set "SRC=src\commands"
set "BUILD=bettercmd\bin"
set "VBS_SRC=src\scripts"
set "COMMANDS=ls ll now cat pwd touch clear stat head tail basename dirname sleep wc randnum randword uptime uniq env"
set "VBS=install.vbs uninstall.vbs"

rem === Cleanup ===
echo [*] Cleaning previous build...
if exist "bettercmd" (
    rmdir /s /q "bettercmd"
    if errorlevel 1 (
        echo [!] Failed to delete existing bettercmd directory.
        exit /b 1
    )
)
if exist bettercmd.zip (
    del /f /q bettercmd.zip
    if errorlevel 1 (
        echo [!] Failed to delete existing bettercmd.zip file.
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
if not exist "bettercmd" mkdir "bettercmd"
for %%F in (%VBS%) do (
    copy "%VBS_SRC%\%%F" "bettercmd\" >nul
    if errorlevel 1 (
        echo [!] Failed to copy %%F from %VBS_SRC%
        exit /b 1
    )
)

rem === Copy INSTALLATION.html from docs ===
echo [*] Copying INSTALLATION.html...
if exist docs\INSTALLATION.html (
    copy docs\INSTALLATION.html "bettercmd\" /Y >nul
    if errorlevel 1 (
        echo [!] Failed to copy INSTALLATION.html
        exit /b 1
    )
) else (
    echo [!] docs\INSTALLATION.html not found!
    exit /b 1
)

rem === Copy version.info from docs ===
echo [*] Copying version.info...
if exist docs\version.info (
    copy docs\version.info "bettercmd\" /Y >nul
    if errorlevel 1 (
        echo [!] Failed to copy version.info
        exit /b 1
    )
) else (
    echo [!] docs\version.info not found!
    exit /b 1
)

rem === Create ZIP archive using tar ===
echo [*] Creating archive bettercmd.zip using tar...
where tar >nul 2>&1
if errorlevel 1 (
    echo [!] tar command not found, skipping archive creation.
) else (
    tar -a -c -f bettercmd.zip bettercmd
    if errorlevel 1 (
        echo [!] Failed to create archive with tar.
        exit /b 1
    )
)

echo -----------------------------------------
echo [#] Build complete.
exit /b 0
