#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/ansi_colors.h"

void printHead(const char *filename, int lines) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, ANSI_BOLD_RED "head: cannot open '%s'\n" ANSI_RESET, filename);
        return;
    }

    char buffer[4096];
    int count = 0;
    while (count < lines && fgets(buffer, sizeof(buffer), file)) {
        fputs(buffer, stdout);
        count++;
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
        fprintf(stderr, ANSI_BOLD_RED "head: missing file operand\n" ANSI_RESET);
        return 1;
    }

    printHead(filename, lines);
    return 0;
}
