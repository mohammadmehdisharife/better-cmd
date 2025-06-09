#include <stdio.h>
#include <windows.h>
#include "../libs/ansi_colors.h"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, ANSI_BOLD_RED "Usage: %s <milliseconds>\n" ANSI_RESET, argv[0]);
        return 1;
    }
    DWORD ms = (DWORD)strtoul(argv[1], NULL, 10);
    Sleep(ms);
    return 0;
}
