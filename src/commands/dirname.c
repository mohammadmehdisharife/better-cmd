#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../libs/ansi_colors.h"

#ifdef _WIN32
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, ANSI_BOLD_WHITE "dirname: missing operand\n" ANSI_RESET);
        return 1;
    }

    char *path = argv[1];
    size_t len = strlen(path);

    if (len == 0) {
        printf(ANSI_BOLD_WHITE ".\n" ANSI_RESET);
        return 0;
    }

    char *copy = malloc(len + 1);
    if (!copy) {
        fprintf(stderr, ANSI_BOLD_WHITE "dirname: memory error\n" ANSI_RESET);
        return 1;
    }
    strcpy(copy, path);

    while (len > 1 && (copy[len - 1] == PATH_SEP || copy[len - 1] == '/')) {
        copy[len - 1] = '\0';
        len--;
    }

    char *last_sep = strrchr(copy, PATH_SEP);
    if (!last_sep) {
        printf(ANSI_BOLD_WHITE ".\n" ANSI_RESET);
    } else if (last_sep == copy) {
        copy[1] = '\0';
        printf(ANSI_BOLD_WHITE "%s\n" ANSI_RESET, copy);
    } else {
        *last_sep = '\0';
        printf(ANSI_BOLD_WHITE "%s\n" ANSI_RESET, copy);
    }

    free(copy);
    return 0;
}