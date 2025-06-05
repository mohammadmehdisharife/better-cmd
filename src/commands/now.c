#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define DEFAULT_FORMAT "%Y-%m-%d %H:%M:%S"
#define MAX_FORMAT_LEN 100
#define MAX_BUFFER_SIZE 256

#define COLOR_RED    "\x1b[1;31m"
#define COLOR_GREEN  "\x1b[1;32m"
#define COLOR_YELLOW "\x1b[1;33m"
#define COLOR_CYAN   "\x1b[1;36m"
#define COLOR_RESET  "\x1b[0m"

int getFormattedTime(char *buffer, size_t bufsize, const char *format, int useUTC, time_t *customTime) {
    struct tm timeinfo;
    time_t rawtime;

    if (customTime) {
        rawtime = *customTime;
    } else {
        time(&rawtime);
    }

    if (useUTC) {
#ifdef _WIN32
        if (gmtime_s(&timeinfo, &rawtime) != 0) return -1;
#else
        struct tm *tmp = gmtime(&rawtime);
        if (!tmp) return -1;
        timeinfo = *tmp;
#endif
    } else {
#ifdef _WIN32
        if (localtime_s(&timeinfo, &rawtime) != 0) return -1;
#else
        struct tm *tmp = localtime(&rawtime);
        if (!tmp) return -1;
        timeinfo = *tmp;
#endif
    }

    if (strftime(buffer, bufsize, format, &timeinfo) == 0) return -1;

    return 0;
}

int parseISO8601(const char *isoStr, time_t *outTime) {
    struct tm timeinfo = {0};
    char *res;

    if (strlen(isoStr) < 19) return -1;

#ifdef _WIN32
    res = NULL; // strptime در ویندوز وجود ندارد
    // یک پیاده‌سازی ساده برای فرمت مشخص:
    int year, month, day, hour, min, sec;
    if (sscanf(isoStr, "%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec) == 6) {
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month -1;
        timeinfo.tm_mday = day;
        timeinfo.tm_hour = hour;
        timeinfo.tm_min = min;
        timeinfo.tm_sec = sec;
        if (strchr(isoStr, 'Z')) {
            *outTime = _mkgmtime(&timeinfo);
        } else {
            *outTime = mktime(&timeinfo);
        }
        return 0;
    }
    return -1;
#else
    res = strptime(isoStr, "%Y-%m-%dT%H:%M:%S", &timeinfo);
    if (!res) return -1;

    if (*res == 'Z') {
        *outTime = timegm(&timeinfo);
    } else {
        *outTime = mktime(&timeinfo);
    }
    return 0;
#endif
}

#ifdef _WIN32
int getMilliseconds() {
    FILETIME ft;
    ULARGE_INTEGER uli;
    GetSystemTimeAsFileTime(&ft);
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    // 100-nanosecond intervals since January 1, 1601 (UTC)
    // تبدیل به میلی‌ثانیه برای زمان فعلی
    ULONGLONG ms = (uli.QuadPart / 10000) % 1000;
    return (int)ms;
}
#else
int getMilliseconds() {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
        return (int)(ts.tv_nsec / 1000000);
    }
    return 0;
}
#endif

void printUsage(const char *progName) {
    printf("Usage: %s [-u] [-f FORMAT] [-t TIMESTAMP] [-I ISO8601] [-i] [-m] [-h]\n", progName);
    printf("  -u           " COLOR_CYAN "Print time in UTC (default local time)" COLOR_RESET "\n");
    printf("  -f FORMAT    " COLOR_CYAN "Specify strftime format string (default: \"%s\")" COLOR_RESET "\n", DEFAULT_FORMAT);
    printf("  -t TIMESTAMP " COLOR_CYAN "Print time for given UNIX timestamp (seconds since epoch)" COLOR_RESET "\n");
    printf("  -I ISO8601   " COLOR_CYAN "Parse ISO8601 time string (e.g. 2025-06-05T14:30:00Z)" COLOR_RESET "\n");
    printf("  -i           " COLOR_CYAN "Output time in ISO8601 format" COLOR_RESET "\n");
    printf("  -m           " COLOR_CYAN "Print milliseconds (only with current time)" COLOR_RESET "\n");
    printf("  -h, --help   " COLOR_CYAN "Show this help message" COLOR_RESET "\n");
}

int main(int argc, char *argv[]) {
    int useUTC = 0;
    int outputISO = 0;
    int printMillis = 0;
    char format[MAX_FORMAT_LEN] = DEFAULT_FORMAT;
    char timeBuffer[MAX_BUFFER_SIZE];
    time_t customTimeValue = 0;
    time_t *customTimePtr = NULL;
    const char *isoInputStr = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-u") == 0) {
            useUTC = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                strncpy(format, argv[i + 1], MAX_FORMAT_LEN - 1);
                format[MAX_FORMAT_LEN - 1] = '\0';
                outputISO = 0;
                i++;
            } else {
                fprintf(stderr, COLOR_RED "Error: -f option requires a format string\n" COLOR_RESET);
                printUsage(argv[0]);
                return 1;
            }
        } else if (strcmp(argv[i], "-t") == 0) {
            if (i + 1 < argc) {
                customTimeValue = (time_t)atoll(argv[i + 1]);
                customTimePtr = &customTimeValue;
                i++;
            } else {
                fprintf(stderr, COLOR_RED "Error: -t option requires a timestamp\n" COLOR_RESET);
                printUsage(argv[0]);
                return 1;
            }
        } else if (strcmp(argv[i], "-I") == 0) {
            if (i + 1 < argc) {
                isoInputStr = argv[i + 1];
                i++;
            } else {
                fprintf(stderr, COLOR_RED "Error: -I option requires an ISO8601 string\n" COLOR_RESET);
                printUsage(argv[0]);
                return 1;
            }
        } else if (strcmp(argv[i], "-i") == 0) {
            outputISO = 1;
        } else if (strcmp(argv[i], "-m") == 0) {
            printMillis = 1;
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

    if (outputISO) {
        const char *isoFormat = useUTC ? "%Y-%m-%dT%H:%M:%SZ" : "%Y-%m-%dT%H:%M:%S%z";
        if (getFormattedTime(timeBuffer, sizeof(timeBuffer), isoFormat, useUTC, customTimePtr) != 0) {
            fprintf(stderr, COLOR_RED "Failed to format time in ISO8601\n" COLOR_RESET);
            return 1;
        }
    } else {
        if (getFormattedTime(timeBuffer, sizeof(timeBuffer), format, useUTC, customTimePtr) != 0) {
            fprintf(stderr, COLOR_RED "Failed to get formatted time\n" COLOR_RESET);
            return 1;
        }
    }

    if (printMillis && !customTimePtr) {
        int ms = getMilliseconds();
        printf(COLOR_GREEN "%s.%03d\n" COLOR_RESET, timeBuffer, ms);
    } else {
        printf(COLOR_GREEN "%s\n" COLOR_RESET, timeBuffer);
    }

    return 0;
}
