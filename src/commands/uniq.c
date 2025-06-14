#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../libs/ansi_colors.h"
#include "../libs/uthsah.h"

#define MAX_LINE_LEN 4096

typedef struct {
    char *line;       
    UT_hash_handle hh;  
} LineEntry;

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    char line[MAX_LINE_LEN];
    LineEntry *seen_lines = NULL; 
    LineEntry *entry = NULL;

    if (argc == 1) {
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        if (GetConsoleMode(hInput, &mode)) {
            fprintf(stderr, ANSI_BOLD_RED "Error: Input must come from a pipe or file.\n" ANSI_RESET);
            return 1;
        }
        fp = stdin;
    } else if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, ANSI_BOLD_RED "Error: Cannot open file %s\n" ANSI_RESET, argv[1]);
            return 1;
        }
    } else {
        fprintf(stderr, ANSI_BOLD_YELLOW "Usage: %s [file]\n" ANSI_RESET, argv[0]);
        return 1;
    }

    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0'; 
        }

        HASH_FIND_STR(seen_lines, line, entry);
        if (!entry) {
            printf("%s\n", line);

            entry = malloc(sizeof(LineEntry));
            entry->line = strdup(line);
            HASH_ADD_KEYPTR(hh, seen_lines, entry->line, strlen(entry->line), entry);
        }
    }

    LineEntry *cur, *tmp;
    HASH_ITER(hh, seen_lines, cur, tmp) {
        HASH_DEL(seen_lines, cur);
        free(cur->line);
        free(cur);
    }

    if (fp != stdin) {
        fclose(fp);
    }

    return 0;
}
