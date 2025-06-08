@echo off
setlocal enabledelayedexpansion

set "BIN_DIR=bettercmd\bin"
set "TESTS_DIR=tests"
set "XCOPY_OPTIONS=/Y"

echo Copying binaries from %BIN_DIR% to %TESTS_DIR%

for %%F in (%BIN_DIR%\*.exe) do (
    set "filename=%%~nF"
    call :copy_binary "%%F" "!filename!"
)

echo Done.
goto :eof

:copy_binary
set "source=%~1"
set "name=%~2"
set "dest_dir=%TESTS_DIR%\%name%"

if not exist "%dest_dir%" (
    echo Creating directory: %dest_dir%
    mkdir "%dest_dir%"
)

echo Copying %source% to %dest_dir%\%name%.exe
xcopy %XCOPY_OPTIONS% "%source%" "%dest_dir%\" >nul
goto :eof
