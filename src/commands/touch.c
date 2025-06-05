#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <io.h>

#define ANSI_BOLD_RED "\x1b[1;31m"
#define ANSI_BOLD_GREEN "\x1b[1;32m"
#define ANSI_BOLD_YELLOW "\x1b[1;33m"
#define ANSI_RESET "\x1b[0m"

void setConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}

void printUsage() {
    printf(ANSI_BOLD_YELLOW
        "Usage: touch [OPTIONS] FILE...\n"
        "Update the access and modification times of each FILE to the current time.\n\n"
        "Options:\n"
        "  -a          change only the access time\n"
        "  -m          change only the modification time\n"
        "  -c          do not create any files\n"
        "  -t [[CC]YY]MMDDhhmm[.ss]\n"
        "              use the specified timestamp instead of the current time\n"
        "  --help      display this help and exit\n"
        ANSI_RESET);
}

// تبدیل رشته زمان به SYSTEMTIME
// فرمت: [[CC]YY]MMDDhhmm[.ss]
int parseTime(const char* str, SYSTEMTIME* st) {
    // مقداردهی پیش‌فرض به زمان فعلی
    GetSystemTime(st);

    int len = (int)strlen(str);
    if (len < 8) return 0;

    int pos = 0;
    char buf[5];
    memset(st, 0, sizeof(SYSTEMTIME));

    // اگر طول بیشتر باشه، ممکن CC یا YY باشه
    if (len == 12 || len == 10 || len == 8 || len == 15) {
        // حالت‌های مختلف را پشتیبانی می‌کنیم
        // برای سادگی، الان فقط پشتیبانی از YYMMDDhhmm.ss هست
        // می‌تونید این بخش رو گسترش بدید
        // اینجا فقط نمونه ساده از خواندن MMDDhhmm را می‌زنیم

        // اگر طول 15 باشد (CCYYMMDDhhmm.ss)
        if (len == 15) {
            // CCYY
            strncpy(buf, str, 4);
            buf[4] = 0;
            int year = atoi(buf);
            st->wYear = year;
            pos = 4;
        } else if (len == 12) {
            // YYMMDDhhmm.ss
            strncpy(buf, str, 2);
            buf[2] = 0;
            int year = atoi(buf);
            st->wYear = year + 2000;
            pos = 2;
        } else {
            // فقط MMDDhhmm
            st->wYear = 2023; // پیش فرض
        }

        // MM
        strncpy(buf, str + pos, 2);
        buf[2] = 0;
        st->wMonth = atoi(buf);
        pos += 2;

        // DD
        strncpy(buf, str + pos, 2);
        buf[2] = 0;
        st->wDay = atoi(buf);
        pos += 2;

        // hh
        strncpy(buf, str + pos, 2);
        buf[2] = 0;
        st->wHour = atoi(buf);
        pos += 2;

        // mm
        strncpy(buf, str + pos, 2);
        buf[2] = 0;
        st->wMinute = atoi(buf);
        pos += 2;

        // .ss
        if (pos + 3 <= len && str[pos] == '.') {
            strncpy(buf, str + pos + 1, 2);
            buf[2] = 0;
            st->wSecond = atoi(buf);
        } else {
            st->wSecond = 0;
        }

        return 1;
    }

    return 0;
}

int touchFile(const char* filename, int changeAccess, int changeModify, int noCreate, SYSTEMTIME* customTime) {
    int exists = (_access(filename, 0) == 0);

    if (!exists && noCreate) {
        printf(ANSI_BOLD_YELLOW "File does not exist and -c specified, skipping: %s\n" ANSI_RESET, filename);
        return 0;
    }

    if (!exists) {
        HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            printf(ANSI_BOLD_RED "Error creating file: %s\n" ANSI_RESET, filename);
            return 1;
        }
        CloseHandle(hFile);
        printf(ANSI_BOLD_GREEN "Created file: %s\n" ANSI_RESET, filename);
    }

    HANDLE hFile = CreateFileA(filename, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf(ANSI_BOLD_RED "Error opening file to update timestamp: %s\n" ANSI_RESET, filename);
        return 1;
    }

    FILETIME ftCreate, ftAccess, ftModify;
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify)) {
        printf(ANSI_BOLD_RED "Error getting file times: %s\n" ANSI_RESET, filename);
        CloseHandle(hFile);
        return 1;
    }

    FILETIME ftNewAccess = ftAccess;
    FILETIME ftNewModify = ftModify;

    if (customTime) {
        FILETIME ftCustom;
        SystemTimeToFileTime(customTime, &ftCustom);
        if (changeAccess) ftNewAccess = ftCustom;
        if (changeModify) ftNewModify = ftCustom;
    } else {
        // زمان فعلی سیستم
        SYSTEMTIME stNow;
        FILETIME ftNow;
        GetSystemTime(&stNow);
        SystemTimeToFileTime(&stNow, &ftNow);
        if (changeAccess) ftNewAccess = ftNow;
        if (changeModify) ftNewModify = ftNow;
    }

    if (!SetFileTime(hFile,
                     NULL,
                     changeAccess ? &ftNewAccess : &ftAccess,
                     changeModify ? &ftNewModify : &ftModify)) {
        printf(ANSI_BOLD_RED "Error setting file times: %s\n" ANSI_RESET, filename);
        CloseHandle(hFile);
        return 1;
    }

    CloseHandle(hFile);
    printf(ANSI_BOLD_GREEN "Touched file: %s\n" ANSI_RESET, filename);
    return 0;
}

int main(int argc, char* argv[]) {
    setConsoleUTF8();

    if (argc < 2) {
        printUsage();
        return 1;
    }

    int changeAccess = 1;
    int changeModify = 1;
    int noCreate = 0;
    SYSTEMTIME customTime;
    SYSTEMTIME* pCustomTime = NULL;

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "--help") == 0) {
            printUsage();
            return 0;
        } else if (strcmp(argv[i], "-a") == 0) {
            changeModify = 0;
        } else if (strcmp(argv[i], "-m") == 0) {
            changeAccess = 0;
        } else if (strcmp(argv[i], "-c") == 0) {
            noCreate = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            if (i + 1 >= argc) {
                printf(ANSI_BOLD_RED "Option -t requires an argument.\n" ANSI_RESET);
                return 1;
            }
            if (!parseTime(argv[i + 1], &customTime)) {
                printf(ANSI_BOLD_RED "Invalid time format for -t.\n" ANSI_RESET);
                return 1;
            }
            pCustomTime = &customTime;
            i++;
        } else if (argv[i][0] == '-') {
            printf(ANSI_BOLD_RED "Unknown option: %s\n" ANSI_RESET, argv[i]);
            return 1;
        } else {
            break;
        }
        i++;
    }

    if (i >= argc) {
        printf(ANSI_BOLD_RED "No files specified.\n" ANSI_RESET);
        return 1;
    }

    int errorCount = 0;
    for (; i < argc; i++) {
        if (touchFile(argv[i], changeAccess, changeModify, noCreate, pCustomTime) != 0) {
            errorCount++;
        }
    }

    return errorCount == 0 ? 0 : 1;
}
