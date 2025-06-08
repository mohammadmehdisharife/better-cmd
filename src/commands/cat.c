#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COLOR_BOLD_RED  "\x1b[1;31m"
#define COLOR_YELLOW    "\x1b[1;33m"
#define COLOR_RESET     "\x1b[0m"

void printFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, COLOR_BOLD_RED "cat: cannot open '%s': %s" COLOR_RESET "\n", filename, strerror(errno));
        return;
    }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file)) {
        fputs(buffer, stdout);
    }

    fclose(file);
}

void printHelp() {
    printf(COLOR_YELLOW
        "Usage: cat [file...]\n"
        "\n"
        "Concatenate and display the content of one or more files.\n"
        "Examples:\n"
        "  cat file.txt        # print a single file\n"
        "  cat a.txt b.txt     # print multiple files in order\n"
        "\n"
        "Options:\n"
        "  -h, --help          Show this help message\n"
        COLOR_RESET);
}

void printUsage() {
    fprintf(stderr, COLOR_BOLD_RED
        "Usage: cat [file...]\n"
        "Try 'cat -h' for more information.\n"
        COLOR_RESET);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, COLOR_BOLD_RED "cat: missing file operand\n" COLOR_RESET);
        printUsage();
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printHelp();
            return 0;
        } else {
            printFile(argv[i]);
        }
    }

    return 0;
}
