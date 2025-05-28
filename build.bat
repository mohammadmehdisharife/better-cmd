rmdir /s /q build
mkdir build

@REM compile and move to build folder
nim c -d:release .\src\touch.nim
move .\src\touch.exe .\build
copy .\src\clear.bat .\build
copy .\src\ls.bat .\build

cls