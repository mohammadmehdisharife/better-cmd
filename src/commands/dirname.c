#include <stdio.h>
#include <string.h>
#include <libgen.h>

#define COLOR_BOLD_RED  "\x1b[1;31m"
#define COLOR_YELLOW    "\x1b[1;33m"
#define COLOR_RESET     "\x1b[0m"

void printHelp() {
    printf(COLOR_YELLOW
        "Usage: dirname [path]\n"
        "\n"
        "Print the directory part of a given path.\n"
        "Examples:\n"
        "  dirname /usr/bin/ls     # /usr/bin\n"
        "  dirname ./src/main.c    # ./src\n"
        "\n"
        "Options:\n"
        "  -h, --help              Show this help message\n"
        COLOR_RESET);
}

void printUsage() {
    fprintf(stderr, COLOR_BOLD_RED
        "Usage: dirname [path]\n"
        "Try 'dirname -h' for more information.\n"
        COLOR_RESET);
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            printHelp();
            return 0;
        }

        char pathCopy[1024];
        strncpy(pathCopy, argv[1], sizeof(pathCopy) - 1);
        pathCopy[sizeof(pathCopy) - 1] = '\0';

        printf("%s\n", dirname(pathCopy));
        return 0;
    }

    fprintf(stderr, COLOR_BOLD_RED "dirname: missing path operand\n" COLOR_RESET);
    printUsage();
    return 1;
}
