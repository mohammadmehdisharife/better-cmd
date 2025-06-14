#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../libs/ansi_colors.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error: Input must come from a pipe or file.\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, ANSI_BOLD_RED "cat: cannot open '%s': %s\n" ANSI_RESET, argv[1], strerror(errno));
        return 1;
    }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file)) {
        fputs(buffer, stdout);
    }

    fclose(file);
    return 0;
}
