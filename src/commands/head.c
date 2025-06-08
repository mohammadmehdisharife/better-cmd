#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COLOR_BOLD_RED  "\x1b[1;31m"
#define COLOR_YELLOW    "\x1b[1;33m"
#define COLOR_RESET     "\x1b[0m"

void printHead(const char *filename, int lines) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, COLOR_BOLD_RED "head: cannot open '%s': %s" COLOR_RESET "\n", filename, strerror(errno));
        return;
    }

    char buffer[4096];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file) && count < lines) {
        fputs(buffer, stdout);
        count++;
    }

    fclose(file);
}

void printHelp() {
    printf(COLOR_YELLOW
        "Usage: head [OPTION]... FILE\n"
        "\n"
        "Print the first 10 lines of FILE to standard output.\n"
        "\n"
        "Options:\n"
        "  -n NUM      Output the first NUM lines instead of the first 10\n"
        "  -h, --help  Show this help message\n"
        COLOR_RESET);
}

void printUsage() {
    fprintf(stderr, COLOR_BOLD_RED
        "Usage: head [OPTION]... FILE\n"
        "Try 'head -h' for more information.\n"
        COLOR_RESET);
}

int main(int argc, char *argv[]) {
    int lines = 10;
    const char *filename = NULL;

    if (argc == 1) {
        fprintf(stderr, COLOR_BOLD_RED "head: missing file operand\n" COLOR_RESET);
        printUsage();
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
            printHelp();
            return 0;
        } else if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                lines = atoi(argv[++i]);
                if (lines < 0) lines = 10;
            } else {
                fprintf(stderr, COLOR_BOLD_RED "head: option '-n' requires an argument\n" COLOR_RESET);
                return 1;
            }
        } else if (!filename) {
            filename = argv[i];
        } else {
            fprintf(stderr, COLOR_BOLD_RED "head: too many arguments\n" COLOR_RESET);
            printUsage();
            return 1;
        }
    }

    if (!filename) {
        fprintf(stderr, COLOR_BOLD_RED "head: missing file operand\n" COLOR_RESET);
        printUsage();
        return 1;
    }

    printHead(filename, lines);
    return 0;
}
