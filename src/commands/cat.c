#include <stdio.h>
#include <stdlib.h>
#include "../libs/ansi_colors.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, ANSI_BOLD_RED "cat: missing file operand\n" ANSI_RESET);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, ANSI_BOLD_RED "cat: cannot open '%s'\n" ANSI_RESET, argv[1]);
        return 1;
    }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file)) {
        fputs(buffer, stdout);
    }

    fclose(file);
    return 0;
}
