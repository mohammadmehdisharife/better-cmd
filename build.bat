@echo off
setlocal enabledelayedexpansion

rem === Configuration ===
set "SRC=src\commands"
set "BUILD=bettercmd\bin"
set "VBS_SRC=src\scripts"
set "COMMANDS=ls ll now cat pwd touch clear stat head tail basename dirname sleep wc randnum randword uptime uniq env pure man runtime trim tr paste"
set "VBS=install.vbs uninstall.vbs"

rem === Cleanup ===
cls
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

rem === Copy all files from docs to bettercmd ===
for %%F in (docs\*.*) do (
    echo [*] Copying %%~nxF...
    copy "%%F" "bettercmd\" /Y >nul
    if errorlevel 1 (
        echo [!] Failed to copy %%~nxF
        exit /b 1
    )
)

echo [*] Copying man pages
xcopy ".\man" ".\bettercmd\man" /E /I /H /C /Y >nul
if %ERRORLEVEL% NEQ 0 (
    echo [!] Failed Copy man page.
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
