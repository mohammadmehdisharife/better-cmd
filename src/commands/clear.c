#include <windows.h>

int main(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD cellsWritten;
    DWORD consoleSize;
    COORD home = {0, 0};

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return 1;

    consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, home, &cellsWritten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, home, &cellsWritten);
    SetConsoleCursorPosition(hConsole, home);

    return 0;
}
