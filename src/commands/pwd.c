#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../libs/ansi_colors.h"

int main(int argc, char *argv[])
{
    char buffer[MAX_PATH];

    DWORD length = GetCurrentDirectoryA(MAX_PATH, buffer);
    if (length == 0 || length >= MAX_PATH) {
        return 1;
    }

    printf(ANSI_BOLD_WHITE"%s\n"ANSI_RESET, buffer);
    return 0;
}
