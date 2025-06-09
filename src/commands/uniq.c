#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 4096

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    char line[MAX_LINE_LEN];
    char *prev_line = NULL;

    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
    } else if (argc == 1) {
        fp = stdin;
    } else {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return 1;
    }

    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (!prev_line || strcmp(line, prev_line) != 0) {
            printf("%s\n", line);
            free(prev_line);
            prev_line = strdup(line);
        }
    }

    free(prev_line);

    if (fp != stdin) {
        fclose(fp);
    }

    return 0;
}
