#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COLOR_BOLD_RED  "\x1b[1;31m"
#define COLOR_YELLOW    "\x1b[1;33m"
#define COLOR_RESET     "\x1b[0m"

#define MAX_LINES 10000
#define MAX_LINE_LEN 4096

void printTail(const char *filename, int linesToShow) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, COLOR_BOLD_RED "tail: cannot open '%s': %s" COLOR_RESET "\n", filename, strerror(errno));
        return;
    }

    char *lines[MAX_LINES];
    int count = 0;

    // Allocate and read lines into buffer
    while (1) {
        if (count >= MAX_LINES) break;

        lines[count] = malloc(MAX_LINE_LEN);
        if (!fgets(lines[count], MAX_LINE_LEN, file)) {
            free(lines[count]);
            break;
        }

        count++;
    }

    fclose(file);

    int start = (count > linesToShow) ? count - linesToShow : 0;
    for (int i = start; i < count; i++) {
        fputs(lines[i], stdout);
        free(lines[i]);
    }
}

void printHelp() {
    printf(COLOR_YELLOW
        "Usage: tail [OPTION]... FILE\n"
        "\n"
        "Print the last 10 lines of FILE to standard output.\n"
        "\n"
        "Options:\n"
        "  -n NUM      Output the last NUM lines instead of the last 10\n"
        "  -h, --help  Show this help message\n"
        COLOR_RESET);
}

void printUsage() {
    fprintf(stderr, COLOR_BOLD_RED
        "Usage: tail [OPTION]... FILE\n"
        "Try 'tail -h' for more information.\n"
        COLOR_RESET);
}

int main(int argc, char *argv[]) {
    int lines = 10;
    const char *filename = NULL;

    if (argc == 1) {
        fprintf(stderr, COLOR_BOLD_RED "tail: missing file operand\n" COLOR_RESET);
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
                fprintf(stderr, COLOR_BOLD_RED "tail: option '-n' requires an argument\n" COLOR_RESET);
                return 1;
            }
        } else if (!filename) {
            filename = argv[i];
        } else {
            fprintf(stderr, COLOR_BOLD_RED "tail: too many arguments\n" COLOR_RESET);
            printUsage();
            return 1;
        }
    }

    if (!filename) {
        fprintf(stderr, COLOR_BOLD_RED "tail: missing file operand\n" COLOR_RESET);
        printUsage();
        return 1;
    }

    printTail(filename, lines);
    return 0;
}
