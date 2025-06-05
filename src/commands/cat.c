#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COLOR_BOLD_RED  "\x1b[1;31m"
#define COLOR_RESET     "\x1b[0m"

int showLineNumbers = 0;
int numberNonBlank = 0;
int squeezeBlank = 0;
int showEnds = 0;
int showTabs = 0;

void printLine(const char *line, int *lineNumber, int *lastBlank) {
    int isBlank = (line[0] == '\n');

    if (squeezeBlank && isBlank && *lastBlank) {
        return;
    }

    if (showLineNumbers && !(numberNonBlank && isBlank)) {
        printf("%6d\t", (*lineNumber)++);
    }

    *lastBlank = isBlank;

    for (const char *p = line; *p; ++p) {
        if (*p == '\t' && showTabs) {
            fputs("^I", stdout);
        } else if (*p == '\n' && showEnds) {
            putchar('$');
            putchar('\n');
        } else {
            putchar(*p);
        }
    }
}

void printFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, COLOR_BOLD_RED "cat: cannot open '%s': %s" COLOR_RESET "\n", filename, strerror(errno));
        return;
    }

    char line[4096];
    int lineNumber = 1;
    int lastBlank = 0;

    while (fgets(line, sizeof(line), file)) {
        printLine(line, &lineNumber, &lastBlank);
    }

    fclose(file);
}

void printFromStdin() {
    char line[4096];
    int lineNumber = 1;
    int lastBlank = 0;

    while (fgets(line, sizeof(line), stdin)) {
        printLine(line, &lineNumber, &lastBlank);
    }
}

void printUsage() {
    fprintf(stderr, COLOR_BOLD_RED
        "Usage: cat [options] [file...]\n"
        "Options:\n"
        "  -n    Number all lines\n"
        "  -b    Number non-empty lines\n"
        "  -s    Squeeze blank lines\n"
        "  -E    Show $ at end of line\n"
        "  -T    Show tabs as ^I\n" COLOR_RESET);
}

int main(int argc, char *argv[]) {
    int firstFileIndex = 1;
    int foundFile = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            const char *opt = argv[i] + 1;
            while (*opt) {
                switch (*opt) {
                    case 'n': showLineNumbers = 1; break;
                    case 'b': numberNonBlank = 1; showLineNumbers = 1; break;
                    case 's': squeezeBlank = 1; break;
                    case 'E': showEnds = 1; break;
                    case 'T': showTabs = 1; break;
                    default:
                        fprintf(stderr, COLOR_BOLD_RED "cat: invalid option -- '%c'\n" COLOR_RESET, *opt);
                        printUsage();
                        return 1;
                }
                opt++;
            }
        } else {
            foundFile = 1;
            break;
        }
    }

    if (foundFile) {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                printFile(argv[i]);
            }
        }
    } else {
        printFromStdin();
    }

    return 0;
}
