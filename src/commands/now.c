#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEFAULT_FORMAT "%Y-%m-%d %H:%M:%S"
#define MAX_FORMAT_LEN 100
#define MAX_BUFFER_SIZE 256

int getFormattedTime(char *buffer, size_t bufsize, const char *format, int useUTC) {
    time_t rawtime;
    struct tm timeinfo;

    time(&rawtime);

    if (useUTC) {
        if (gmtime_s(&timeinfo, &rawtime) != 0) {
            return -1;
        }
    } else {
        if (localtime_s(&timeinfo, &rawtime) != 0) {
            return -1;
        }
    }

    if (strftime(buffer, bufsize, format, &timeinfo) == 0) {
        return -1;
    }

    return 0;
}

void printUsage(const char *progName) {
    printf("Usage: %s [-u] [-f format]\n", progName);
    printf("  -u           Print time in UTC (default is local time)\n");
    printf("  -f format    Specify strftime format string (default: \"%s\")\n", DEFAULT_FORMAT);
    printf("\nExamples:\n");
    printf("  %s\n", progName);
    printf("  %s -u\n", progName);
    printf("  %s -f \"%%Y/%%m/%%d %%H:%%M:%%S\"\n", progName);
}

int main(int argc, char *argv[]) {
    int useUTC = 0;
    char format[MAX_FORMAT_LEN] = DEFAULT_FORMAT;
    char timeBuffer[MAX_BUFFER_SIZE];

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-u") == 0) {
            useUTC = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                strncpy(format, argv[i + 1], MAX_FORMAT_LEN - 1);
                format[MAX_FORMAT_LEN - 1] = '\0';
                i++;
            } else {
                fprintf(stderr, "\x1b[1;31mError: -f option requires a format string\x1b[0m\n");
                printUsage(argv[0]);
                return 1;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "\x1b[1;31mUnknown option: %s\x1b[0m\n", argv[i]);
            printUsage(argv[0]);
            return 1;
        }
    }

    if (getFormattedTime(timeBuffer, sizeof(timeBuffer), format, useUTC) != 0) {
        fprintf(stderr, "\x1b[1;31mFailed to get formatted time\x1b[0m\n");
        return 1;
    }

    printf("%s\n", timeBuffer);

    return 0;
}
