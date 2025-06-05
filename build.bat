@echo off
rem --- Configuration ---
set SRC=src\commands
set BUILD=better-cmd\bin

rem --- Cleanup ---
rmdir /s /q better-cmd
mkdir %BUILD%

rem --- Compile C commands ---
for %%F in (ls ll now cat pwd touch) do (
    echo Compiling C command %%F...
    gcc -o %BUILD%\%%F.exe %SRC%\%%F.c
)

rem --- Copy additional files ---
copy %SRC%\clear.bat %BUILD%\
copy install.vbs better-cmd\
copy uninstall.vbs better-cmd\

echo Build complete.
