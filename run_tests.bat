@echo off
cd tests

rem === Run tests ===
echo [*] Running tests in the 'tests' directory...

for %%d in (ls ll now cat pwd touch clear stat head tail basename dirname) do (
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
