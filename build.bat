@echo off
setlocal

:: --- Configuration ---
set "NIM=nim"
set "SRC_DIR=src\commands"
set "BUILD_DIR=better-cmd\bin"
set "OUT_DIR=better-cmd"
set "SCRIPT_DIR=%~dp0"
set "FILES=touch ls ll cat now pwd"
set "BAT_FILE=clear.bat"
set "INSTALL_VBS=install.vbs"
set "UNINSTALL_VBS=uninstall.vbs"

:: --- Clean previous build ---
echo Cleaning previous build...
rmdir /s /q "%OUT_DIR%" >nul 2>&1
mkdir "%BUILD_DIR%" >nul

:: --- Compile Nim commands ---
echo.
echo Compiling commands:
for %%F in (%FILES%) do (
    echo   Compiling %%F...
    %NIM% c -d:release "%SRC_DIR%\%%F.nim"
    if exist "%SRC_DIR%\%%F.exe" (
        move /Y "%SRC_DIR%\%%F.exe" "%BUILD_DIR%\" >nul
    ) else (
        echo     [ERROR] Failed to compile %%F.nim
    )
)

:: --- Copy support files ---
echo.
echo Copying support files...
copy /Y "%SRC_DIR%\%BAT_FILE%" "%BUILD_DIR%\" >nul
copy /Y "%SCRIPT_DIR%%INSTALL_VBS%" "%OUT_DIR%\" >nul
copy /Y "%SCRIPT_DIR%%UNINSTALL_VBS%" "%OUT_DIR%\" >nul

:: --- Done ---
echo.
echo [✓] Build complete. Output in: %OUT_DIR%
endlocal
