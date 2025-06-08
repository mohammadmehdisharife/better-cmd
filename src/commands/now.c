#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define DEFAULT_FORMAT "%Y-%m-%d %H:%M:%S"
#define MAX_BUFFER_SIZE 256

#define ANSI_BOLD_RED     "\x1b[1;31m"
#define ANSI_BOLD_YELLOW "\033[1;33m"
#define ANSI_RESET        "\x1b[0m"

void setConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}

int getFormattedTime(char *buffer, size_t bufsize, const char *format) {
    struct tm timeinfo;
    time_t rawtime = time(NULL);

    if (localtime_s(&timeinfo, &rawtime) != 0) return -1;

    if (strftime(buffer, bufsize, format, &timeinfo) == 0) return -1;
    return 0;
}

void printUsage(const char *progName) {
    printf(ANSI_BOLD_YELLOW
        "Usage: %s\n"
        "Print the current local date and time.\n\n"
        "Options:\n"
        "  -h, --help  Show this help message\n"
        ANSI_RESET, progName);
}

int main(int argc, char *argv[]) {
    setConsoleUTF8();

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, ANSI_BOLD_RED "Unknown option: %s\n" ANSI_RESET, argv[i]);
            printUsage(argv[0]);
            return 1;
        }
    }

    char timeBuffer[MAX_BUFFER_SIZE];
    if (getFormattedTime(timeBuffer, sizeof(timeBuffer), DEFAULT_FORMAT) != 0) {
        fprintf(stderr, ANSI_BOLD_RED "Failed to format time\n" ANSI_RESET);
        return 1;
    }

    printf("🕰️" ANSI_BOLD_YELLOW " %s\n" ANSI_RESET, timeBuffer);

    return 0;
}
