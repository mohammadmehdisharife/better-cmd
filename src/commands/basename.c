#include <stdio.h>
#include <string.h>
#include <libgen.h>

#define COLOR_BOLD_RED  "\x1b[1;31m"
#define COLOR_YELLOW    "\x1b[1;33m"
#define COLOR_RESET     "\x1b[0m"

void printHelp() {
    printf(COLOR_YELLOW
        "Usage: basename [path]\n"
        "\n"
        "Print the final component of a given path.\n"
        "Examples:\n"
        "  basename /usr/bin/ls     # ls\n"
        "  basename ./main.c        # main.c\n"
        "\n"
        "Options:\n"
        "  -h, --help               Show this help message\n"
        COLOR_RESET);
}

void printUsage() {
    fprintf(stderr, COLOR_BOLD_RED
        "Usage: basename [path]\n"
        "Try 'basename -h' for more information.\n"
        COLOR_RESET);
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            printHelp();
            return 0;
        }

        char *path = argv[1];
        printf("%s\n", basename(path));
        return 0;
    }

    fprintf(stderr, COLOR_BOLD_RED "basename: missing path operand\n" COLOR_RESET);
    printUsage();
    return 1;
}
