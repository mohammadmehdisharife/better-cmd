#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include "../libs/ansi_colors.h"

void setConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}

int touchFile(const char *filename) {
    int exists = (_access(filename, 0) == 0);

    if (!exists) {
        HANDLE hNew = CreateFileA(
            filename,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (hNew == INVALID_HANDLE_VALUE) {
            printf(ANSI_BOLD_RED "Error creating file: %s\n" ANSI_RESET, filename);
            return 1;
        }
        CloseHandle(hNew);
    }

    HANDLE hFile = CreateFileA(
        filename,
        FILE_WRITE_ATTRIBUTES,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) {
        printf(ANSI_BOLD_RED "Error opening file to update timestamp: %s\n" ANSI_RESET, filename);
        return 1;
    }

    SYSTEMTIME now;
    FILETIME ft;
    GetSystemTime(&now);
    SystemTimeToFileTime(&now, &ft);

    if (!SetFileTime(hFile, NULL, &ft, &ft)) {
        printf(ANSI_BOLD_RED "Error setting file time: %s\n" ANSI_RESET, filename);
        CloseHandle(hFile);
        return 1;
    }

    CloseHandle(hFile);
    printf(ANSI_BOLD_GREEN "📝 Touched file: %s\n" ANSI_RESET, filename);
    return 0;
}

int main(int argc, char *argv[]) {
    setConsoleUTF8();

    if (argc < 2) {
        printf(ANSI_BOLD_RED "touch: missing file operand\n" ANSI_RESET);
        return 1;
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        system("bettercmd -v");
        return 0;
    }

    int errors = 0;
    for (int i = 1; i < argc; i++) {
        if (touchFile(argv[i]) != 0) {
            errors++;
        }
    }

    return errors == 0 ? 0 : 1;
}
