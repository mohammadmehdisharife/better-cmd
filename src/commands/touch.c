#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>

#define ANSI_BOLD_RED    "\x1b[1;31m"
#define ANSI_BOLD_GREEN  "\x1b[1;32m"
#define ANSI_BOLD_YELLOW "\x1b[1;33m"
#define ANSI_RESET       "\x1b[0m"

void setConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}

void printHelp() {
    printf(ANSI_BOLD_YELLOW
        "Usage: touch [file...]\n"
        "\n"
        "Create each file if it does not exist.\n"
        "Update the access and modification times to the current time.\n"
        "\n"
        "Options:\n"
        "  -h, --help      Show this help message.\n"
        ANSI_RESET);
}

int touchFile(const char* filename) {
    int exists = (_access(filename, 0) == 0);

    if (!exists) {
        HANDLE hNew = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hNew == INVALID_HANDLE_VALUE) {
            printf(ANSI_BOLD_RED "Error creating file: %s\n" ANSI_RESET, filename);
            return 1;
        }
        CloseHandle(hNew);
    }

    HANDLE hFile = CreateFileA(filename, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
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

int main(int argc, char* argv[]) {
    setConsoleUTF8();

    if (argc < 2) {
        printf(ANSI_BOLD_RED "touch: missing file operand\n" ANSI_RESET);
        printHelp();
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printHelp();
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
