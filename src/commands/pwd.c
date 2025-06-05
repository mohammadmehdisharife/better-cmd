#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#define COLOR_GREEN "\x1b[1;32m"
#define COLOR_YELLOW "\x1b[1;33m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_RESET "\x1b[0m"

int flagDrive = 0, flagBase = 0, flagQuote = 0, flagColor = 0, flagHelp = 0;
int flagJson = 0, flagJsonCompact = 0, flagJsonNoQuote = 0, flagJsonNoComp = 0;
int flagUnix = 0, flagStrip = 0;

void printHelp() {
    printf(COLOR_YELLOW
        "Usage: pwd [options]\n"
        "Options:\n"
        "  -d             Show drive only\n"
        "  -b             Show basename only\n"
        "  -q             Quote the output\n"
        "  -c             Color output (green)\n"
        "  -h             Show this help\n"
        "  -j             JSON output\n"
        "  --json-compact         Compact JSON in one line\n"
        "  --json-no-quote        Remove \"quoted\" from JSON\n"
        "  --json-no-components   Remove \"components\" from JSON\n"
        "  --unix-style           Use '/' instead of '\\'\n"
        "  --strip-root           Remove drive letter from path\n"
        COLOR_RESET);
}

void escapeJsonString(char *dst, const char *src) {
    while (*src) {
        if (*src == '\\') *dst++ = '\\', *dst++ = '\\';
        else if (*src == '"') *dst++ = '\\', *dst++ = '"';
        else *dst++ = *src;
        src++;
    }
    *dst = 0;
}

void replaceBackslashes(char *s) {
    for (; *s; ++s) if (*s == '\\') *s = '/';
}

void printJson(const char *path, const char *drive, const char *base) {
    char escaped[2 * MAX_PATH];
    char quoted[MAX_PATH + 4];
    char *finalPath = strdup(path);

    if (flagUnix) replaceBackslashes(finalPath);
    if (flagStrip && strlen(finalPath) > 2 && finalPath[1] == ':')
        memmove(finalPath, finalPath + 2, strlen(finalPath) - 1);

    escapeJsonString(escaped, finalPath);
    sprintf(quoted, "\"%s\"", finalPath);

    if (flagJsonCompact)
        printf("{\"path\":\"%s\"", escaped);
    else
        printf("{\n  \"path\": \"%s\"", escaped);

    if (!flagJsonNoQuote)
        flagJsonCompact ? printf(",\"quoted\":\"%s\"", quoted) :
                          printf(",\n  \"quoted\": \"%s\"", quoted);

    flagJsonCompact ? printf(",\"drive\":\"%s\"", drive) :
                      printf(",\n  \"drive\": \"%s\"", drive);

    flagJsonCompact ? printf(",\"basename\":\"%s\"", base) :
                      printf(",\n  \"basename\": \"%s\"", base);

    if (!flagJsonNoComp) {
        if (flagJsonCompact) {
            printf(",\"components\":[");
        } else {
            printf(",\n  \"components\": [\n    ");
        }

        char pathCopy[MAX_PATH];
        strcpy(pathCopy, finalPath);
        char *token = strtok(pathCopy, flagUnix ? "/" : "\\");
        int first = 1;
        while (token) {
            if (!first) flagJsonCompact ? printf(",") : printf(", ");
            printf("\"%s\"", token);
            first = 0;
            token = strtok(NULL, flagUnix ? "/" : "\\");
        }
        flagJsonCompact ? printf("]") : printf("\n  ]");
    }

    flagJsonCompact ? printf("}\n") : printf("\n}\n");

    free(finalPath);
}

void printNormal(const char *out) {
    if (flagColor) printf(COLOR_GREEN);
    if (flagQuote) printf("\"%s\"\n", out);
    else printf("%s\n", out);
    if (flagColor) printf(COLOR_RESET);
}

int main(int argc, char *argv[]) {
    char buffer[MAX_PATH];
    DWORD len = GetCurrentDirectoryA(MAX_PATH, buffer);
    if (len == 0 || len > MAX_PATH) {
        fprintf(stderr, COLOR_RED "pwd: failed to get current directory" COLOR_RESET "\n");
        return 1;
    }

    char drive[4] = {0};
    strncpy(drive, buffer, 2);
    drive[2] = 0;

    char *basename = strrchr(buffer, '\\');
    basename = basename ? basename + 1 : buffer;

    // Arg parsing
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) flagDrive = 1;
        else if (strcmp(argv[i], "-b") == 0) flagBase = 1;
        else if (strcmp(argv[i], "-q") == 0) flagQuote = 1;
        else if (strcmp(argv[i], "-c") == 0) flagColor = 1;
        else if (strcmp(argv[i], "-h") == 0) flagHelp = 1;
        else if (strcmp(argv[i], "-j") == 0) flagJson = 1;
        else if (strcmp(argv[i], "--json-compact") == 0) flagJsonCompact = 1;
        else if (strcmp(argv[i], "--json-no-quote") == 0) flagJsonNoQuote = 1;
        else if (strcmp(argv[i], "--json-no-components") == 0) flagJsonNoComp = 1;
        else if (strcmp(argv[i], "--unix-style") == 0) flagUnix = 1;
        else if (strcmp(argv[i], "--strip-root") == 0) flagStrip = 1;
        else {
            fprintf(stderr, COLOR_RED "pwd: unknown option %s" COLOR_RESET "\n", argv[i]);
            return 1;
        }
    }

    if (flagHelp) {
        printHelp();
        return 0;
    }

    if (flagJson) {
        printJson(buffer, drive, basename);
    } else {
        if (flagDrive) printNormal(drive);
        else if (flagBase) printNormal(basename);
        else printNormal(buffer);
    }

    return 0;
}
