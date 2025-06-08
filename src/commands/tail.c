#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/ansi_colors.h"

void printTail(const char *filename, int lines) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, ANSI_BOLD_RED "tail: cannot open '%s'\n" ANSI_RESET, filename);
        return;
    }

    int total_lines = 0;
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file)) {
        total_lines++;
    }

    rewind(file);

    int start_line = total_lines - lines;
    if (start_line < 0) start_line = 0;

    int current_line = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (current_line >= start_line) {
            fputs(buffer, stdout);
        }
        current_line++;
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    int lines = 10;
    const char *filename = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            lines = atoi(argv[++i]);
            if (lines < 0) lines = 10;
        } else if (!filename) {
            filename = argv[i];
        }
    }

    if (!filename) {
        fprintf(stderr, ANSI_BOLD_RED "tail: missing file operand\n" ANSI_RESET);
        return 1;
    }

    printTail(filename, lines);
    return 0;
}
