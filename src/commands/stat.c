#include <windows.h>
#include <stdio.h>
#include <string.h>

#define ANSI_BOLD_WHITE  "\033[1;37m"
#define ANSI_BOLD_YELLOW "\033[1;33m"
#define ANSI_BOLD_RED    "\033[1;31m"
#define ANSI_RESET       "\033[0m"

void setConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
}

void printUsage(const char *progName) {
    printf(ANSI_BOLD_YELLOW
        "Usage: %s <filename>\n"
        "Display detailed information about a file.\n\n"
        "Options:\n"
        "  -h, --help     Show this help message\n"
        ANSI_RESET, progName);
}

void formatFileTime(FILETIME ft, char *out, size_t size) {
    SYSTEMTIME stUTC, stLocal;
    FileTimeToSystemTime(&ft, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    snprintf(out, size, "%04d-%02d-%02d %02d:%02d:%02d",
             stLocal.wYear, stLocal.wMonth, stLocal.wDay,
             stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
}

void printAttributes(DWORD attr, char *out, size_t size) {
    out[0] = 0;
    if (attr & FILE_ATTRIBUTE_DIRECTORY)     strcat(out, "Directory ");
    if (attr & FILE_ATTRIBUTE_HIDDEN)        strcat(out, "Hidden ");
    if (attr & FILE_ATTRIBUTE_SYSTEM)        strcat(out, "System ");
    if (attr & FILE_ATTRIBUTE_ARCHIVE)       strcat(out, "Archive ");
    if (attr & FILE_ATTRIBUTE_READONLY)      strcat(out, "ReadOnly ");
    if (attr & FILE_ATTRIBUTE_TEMPORARY)     strcat(out, "Temporary ");
    if (attr & FILE_ATTRIBUTE_REPARSE_POINT) strcat(out, "Link ");
}

void printField(const char *label, const char *value) {
    printf(ANSI_BOLD_YELLOW "%-10s: " ANSI_BOLD_WHITE "%s\n", label, value);
}

int main(int argc, char *argv[]) {
    setConsoleUTF8();

    if (argc != 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printUsage(argv[0]);
        return 0;
    }

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(argv[1], GetFileExInfoStandard, &data)) {
        fprintf(stderr, ANSI_BOLD_RED "Error: " ANSI_BOLD_WHITE "Cannot access '%s'\n" ANSI_RESET, argv[1]);
        return 1;
    }

    LARGE_INTEGER size;
    size.HighPart = data.nFileSizeHigh;
    size.LowPart = data.nFileSizeLow;

    char created[64], modified[64], accessed[64], attrStr[128], fullpath[MAX_PATH];
    formatFileTime(data.ftCreationTime, created, sizeof(created));
    formatFileTime(data.ftLastWriteTime, modified, sizeof(modified));
    formatFileTime(data.ftLastAccessTime, accessed, sizeof(accessed));
    printAttributes(data.dwFileAttributes, attrStr, sizeof(attrStr));
    GetFullPathNameA(argv[1], MAX_PATH, fullpath, NULL);

    printf("\n");

    printField("Name", argv[1]);
    
    char sizeStr[32];
    snprintf(sizeStr, sizeof(sizeStr), "%lld bytes", size.QuadPart);
    printField("Size", sizeStr);

    printField("Attr", attrStr);
    printField("Created", created);
    printField("Modified", modified);
    printField("Accessed", accessed);
    printField("Path", fullpath);

    printf(ANSI_RESET "\n");
    return 0;
}
