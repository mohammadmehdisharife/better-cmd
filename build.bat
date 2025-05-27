rmdir /s /q build
mkdir build
cd src

@REM compile and move to build folder

nim c -d:release touch.nim
move ./touch.exe ../build



cd ..
cls