#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_FILES 1024
#define NAME_WIDTH 20
#define PADDING 5

typedef struct {
    char name[MAX_PATH];
    DWORD attr;
    LARGE_INTEGER size;
    FILETIME lastWriteTime;
} FileEntry;

void setConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
}

void formatSize(LARGE_INTEGER size, char* out) {
    if (size.QuadPart > 1000000)
        sprintf(out, "%lldM", size.QuadPart / 1000000);
    else if (size.QuadPart > 1000)
        sprintf(out, "%lldK", size.QuadPart / 1000);
    else
        sprintf(out, "%lld", size.QuadPart);
}

void formatFileTime(FILETIME ft, char* out) {
    SYSTEMTIME stUTC, stLocal;
    FileTimeToSystemTime(&ft, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    sprintf(out, "%04d-%02d-%02d %02d:%02d",
        stLocal.wYear, stLocal.wMonth, stLocal.wDay,
        stLocal.wHour, stLocal.wMinute);
}

int main() {
    setConsoleUTF8();

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA("*", &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No files found.\n");
        return 1;
    }

    FileEntry files[MAX_FILES];
    int count = 0;

    do {
        if (count >= MAX_FILES) break;

        strcpy(files[count].name, findData.cFileName);
        files[count].attr = findData.dwFileAttributes;
        files[count].size.LowPart = findData.nFileSizeLow;
        files[count].size.HighPart = findData.nFileSizeHigh;
        files[count].lastWriteTime = findData.ftLastWriteTime;
        count++;
    } while (FindNextFileA(hFind, &findData));
    FindClose(hFind);

    for (int i = 0; i < count; i++) {
        char sizeStr[32];
        char dateStr[64];
        char namePadded[NAME_WIDTH + 1];

        const char* symbol = (files[i].attr & FILE_ATTRIBUTE_DIRECTORY) ? "📁" : "📄";
        const char* color = (files[i].attr & FILE_ATTRIBUTE_DIRECTORY) ? "\033[1;33m" : "\033[1;37m";

        if (files[i].attr & FILE_ATTRIBUTE_DIRECTORY)
            strcpy(sizeStr, "-");
        else
            formatSize(files[i].size, sizeStr);

        formatFileTime(files[i].lastWriteTime, dateStr);

        snprintf(namePadded, sizeof(namePadded), "%-*s", NAME_WIDTH, files[i].name);

        printf("%s%s %s %5s %s\033[0m\n", color, symbol, namePadded, sizeStr, dateStr);
    }

    return 0;
}
