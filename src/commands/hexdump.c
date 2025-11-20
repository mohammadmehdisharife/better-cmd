#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../libs/ansi_colors.h"

#define BYTES_PER_LINE 16

static void print_usage(const char *prog) {
    printf("Usage: %s <file>\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "rb");

    if (!fp) {
        fprintf(stderr, ANSI_BOLD_RED "Error: cannot open file: %s\n" ANSI_RESET, filename);
        return 1;
    }

    unsigned char buffer[BYTES_PER_LINE];
    size_t offset = 0;

    while (1) {
        size_t n = fread(buffer, 1, BYTES_PER_LINE, fp);
        if (n == 0) break;

        printf(ANSI_BOLD_BLUE "%08zx: " ANSI_RESET, offset);

        for (size_t i = 0; i < BYTES_PER_LINE; ++i) {
            if (i < n)
                printf(ANSI_BOLD_CYAN "%02x " ANSI_RESET, buffer[i]);
            else
                printf("   ");
        }

        printf(" ");

        for (size_t i = 0; i < n; ++i) {
            unsigned char c = buffer[i];
            if (isprint(c))
                printf(ANSI_BOLD_GREEN "%c" ANSI_RESET, c);
            else
                printf(".");
        }

        printf("\n");
        offset += n;
    }

    fclose(fp);
    return 0;
}
