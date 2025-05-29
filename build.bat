rmdir /s /q build
mkdir build

@REM compile and move to build folder
nim c -d:release .\src\touch.nim
move .\src\touch.exe .\build

nim c -d:release .\src\ls.nim
move .\src\ls.exe .\build

nim c -d:release .\src\cat.nim
move .\src\cat.exe .\build

copy .\src\clear.bat .\build

copy .\src\pwd.bat .\build