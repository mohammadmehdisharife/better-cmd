@echo off
setlocal

set BIN_DIR=bettercmd\bin
set TESTS_DIR=tests

echo [+] Starting to copy binaries from %BIN_DIR% to %TESTS_DIR%
for %%d in (ls ll now cat pwd touch clear stat head tail basename dirname sleep) do (
    if exist "%BIN_DIR%\%%d.exe" (
        copy /Y "%BIN_DIR%\%%d.exe" "%TESTS_DIR%\%%d\%%d.exe" >nul
    ) else (
        echo [!] Binary not found: %%d.exe
        exit /b 1
    )
)
echo [+] All binaries copied successfully.

cd %TESTS_DIR%

rem === Run tests ===
echo [*] Running tests in the 'tests' directory...

for %%d in (ls ll now cat pwd touch clear stat head tail basename dirname sleep) do (
    cd %%d
    python "%%d_test.py"
    if errorlevel 1 (
        echo [!] Test %%d_test.py failed. Aborting.
        exit /b 1
    )
    cd ..
)

cd ..
echo -----------------------------------------
echo [✓] All tests completed successfully.
exit /b 0
