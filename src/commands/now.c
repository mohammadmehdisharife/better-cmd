#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define DEFAULT_FORMAT "%Y-%m-%d %H:%M:%S"
#define MAX_FORMAT_LEN 100
#define MAX_BUFFER_SIZE 256

#define COLOR_RED          "\x1b[1;31m"
#define COLOR_GREEN        "\x1b[1;32m"
#define ANSI_BOLD_YELLOW   "\x1b[1;33m"
#define COLOR_RESET        "\x1b[0m"

int getFormattedTime(char *buffer, size_t bufsize, const char *format, int useUTC, time_t *customTime) {
    struct tm timeinfo;
    time_t rawtime = customTime ? *customTime : time(NULL);

    if (useUTC) {
        if (gmtime_s(&timeinfo, &rawtime) != 0) return -1;
    } else {
        if (localtime_s(&timeinfo, &rawtime) != 0) return -1;
    }

    if (strftime(buffer, bufsize, format, &timeinfo) == 0) return -1;
    return 0;
}

int parseISO8601(const char *isoStr, time_t *outTime) {
    struct tm timeinfo = {0};
    int year, month, day, hour, min, sec;

    if (sscanf(isoStr, "%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec) == 6) {
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = day;
        timeinfo.tm_hour = hour;
        timeinfo.tm_min = min;
        timeinfo.tm_sec = sec;

        *outTime = strchr(isoStr, 'Z') ? _mkgmtime(&timeinfo) : mktime(&timeinfo);
        return 0;
    }
    return -1;
}

int getMilliseconds() {
    FILETIME ft;
    ULARGE_INTEGER uli;
    GetSystemTimeAsFileTime(&ft);
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    ULONGLONG ms = (uli.QuadPart / 10000) % 1000;
    return (int)ms;
}

void printUsage(const char *progName) {
    printf(ANSI_BOLD_YELLOW "Usage: %s [-u] [-f FORMAT] [-t TIMESTAMP] [-I ISO8601] [-i] [-m] [-j|--json] [-h|--help]\n", progName);
    printf("  -u           Print time in UTC (default local time)\n");
    printf("  -f FORMAT    Specify strftime format string (default: \"%s\")\n", DEFAULT_FORMAT);
    printf("  -t TIMESTAMP Print time for given UNIX timestamp (seconds since epoch)\n");
    printf("  -I ISO8601   Parse ISO8601 time string (e.g. 2025-06-05T14:30:00Z)\n");
    printf("  -i           Output time in ISO8601 format\n");
    printf("  -m           Print milliseconds (only with current time)\n");
    printf("  --json       Output result in JSON format\n");
    printf("  -h, --help   Show this help message\n" COLOR_RESET);
}

int main(int argc, char *argv[]) {
    int useUTC = 0, outputISO = 0, printMillis = 0, outputJSON = 0;
    char format[MAX_FORMAT_LEN] = DEFAULT_FORMAT;
    char timeBuffer[MAX_BUFFER_SIZE];
    time_t customTimeValue = 0;
    time_t *customTimePtr = NULL;
    const char *isoInputStr = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-u") == 0) {
            useUTC = 1;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            strncpy(format, argv[++i], MAX_FORMAT_LEN - 1);
            format[MAX_FORMAT_LEN - 1] = '\0';
            outputISO = 0;
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            customTimeValue = (time_t)_atoi64(argv[++i]);
            customTimePtr = &customTimeValue;
        } else if (strcmp(argv[i], "-I") == 0 && i + 1 < argc) {
            isoInputStr = argv[++i];
        } else if (strcmp(argv[i], "-i") == 0) {
            outputISO = 1;
        } else if (strcmp(argv[i], "-m") == 0) {
            printMillis = 1;
        } else if (strcmp(argv[i], "--json") == 0 || strcmp(argv[i], "-j") == 0) {
            outputJSON = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, COLOR_RED "Unknown option: %s\n" COLOR_RESET, argv[i]);
            printUsage(argv[0]);
            return 1;
        }
    }

    if (isoInputStr) {
        if (parseISO8601(isoInputStr, &customTimeValue) != 0) {
            fprintf(stderr, COLOR_RED "Failed to parse ISO8601 string: %s\n" COLOR_RESET, isoInputStr);
            return 1;
        }
        customTimePtr = &customTimeValue;
    }

    const char *fmt = outputISO ? (useUTC ? "%Y-%m-%dT%H:%M:%SZ" : "%Y-%m-%dT%H:%M:%S") : format;
    if (getFormattedTime(timeBuffer, sizeof(timeBuffer), fmt, useUTC, customTimePtr) != 0) {
        fprintf(stderr, COLOR_RED "Failed to format time\n" COLOR_RESET);
        return 1;
    }

    if (outputJSON) {
        int ms = (printMillis && !customTimePtr) ? getMilliseconds() : 0;
        printf("{\n  \"time\": \"%s\"", timeBuffer);
        if (printMillis && !customTimePtr) printf(",\n  \"milliseconds\": %d", ms);
        printf("\n}\n");
    } else {
        if (printMillis && !customTimePtr) {
            printf(COLOR_GREEN "%s.%03d\n" COLOR_RESET, timeBuffer, getMilliseconds());
        } else {
            printf(COLOR_GREEN "%s\n" COLOR_RESET, timeBuffer);
        }
    }

    return 0;
}
