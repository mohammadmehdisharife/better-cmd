@echo off
rmdir /s /q build
mkdir build

set SRC=src\commands
set BUILD=build

for %%F in (touch ls cat now pwd) do (
    echo Compiling %%F...
    nim c -d:release %SRC%\%%F.nim
    move %SRC%\%%F.exe %BUILD%\
)

copy %SRC%\clear.bat %BUILD%\
echo Done.
