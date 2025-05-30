@echo off
rmdir /s /q build
mkdir build

set SRC=src
set BUILD=build

for %%F in (touch ls cat now) do (
    echo Compiling %%F...
    nim c -d:release %SRC%\%%F.nim
    move %SRC%\%%F.exe %BUILD%\
)

copy %SRC%\clear.bat %BUILD%\
copy %SRC%\pwd.bat %BUILD%\
echo Done.