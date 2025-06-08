#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "../libs/ansi_colors.h"

#define DEFAULT_FORMAT "%Y-%m-%d %H:%M:%S"
#define MAX_BUFFER_SIZE 256

void setConsoleUTF8()
{
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode))
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

int main()
{
    setConsoleUTF8();

    char timeBuffer[MAX_BUFFER_SIZE];
    time_t rawtime = time(NULL);
    struct tm timeinfo;

    if (localtime_s(&timeinfo, &rawtime) != 0)
        return 1;

    if (strftime(timeBuffer, sizeof(timeBuffer), DEFAULT_FORMAT, &timeinfo) == 0)
        return 1;

    printf("🕰️ " ANSI_BOLD_YELLOW "%s\n" ANSI_RESET, timeBuffer);

    return 0;
}
