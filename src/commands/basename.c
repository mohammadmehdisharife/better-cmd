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
        fprintf(stderr, ANSI_BOLD_WHITE "basename: missing operand\n" ANSI_RESET);
        return 1;
    }

    char *path = argv[1];
    size_t len = strlen(path);

    if (len == 0) {
        printf(ANSI_BOLD_WHITE "\n" ANSI_RESET);
        return 0;
    }

    char *copy = malloc(len + 1);
    if (!copy) {
        fprintf(stderr, ANSI_BOLD_WHITE "basename: memory error\n" ANSI_RESET);
        return 1;
    }
    strcpy(copy, path);

    while (len > 1 && (copy[len - 1] == PATH_SEP || copy[len - 1] == '/')) {
        copy[len - 1] = '\0';
        len--;
    }

    char *last_sep = strrchr(copy, PATH_SEP);
    char *base = last_sep ? last_sep + 1 : copy;

    printf(ANSI_BOLD_WHITE "%s\n" ANSI_RESET, base);

    free(copy);
    return 0;
}
