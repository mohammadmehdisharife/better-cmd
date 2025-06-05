#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_FILES 1024
#define PADDING 4

typedef struct {
    char name[MAX_PATH];
    DWORD attr;
} FileEntry;

void setConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
}

int getTerminalWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80; // fallback
}

int main(int argc, char *argv[]) {
    setConsoleUTF8();

    int showHidden = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            showHidden = 1;
            break;
        }
    }

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;
    FileEntry files[MAX_FILES];
    int count = 0;

    hFind = FindFirstFileA("*", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No files found.\n");
        return 1;
    }

    do {
        if (count >= MAX_FILES) break;

        if (!showHidden && findFileData.cFileName[0] == '.') {
            continue;
        }

        strcpy(files[count].name, findFileData.cFileName);
        files[count].attr = findFileData.dwFileAttributes;
        count++;
    } while (FindNextFileA(hFind, &findFileData));
    FindClose(hFind);

    int maxNameLen = 0;
    for (int i = 0; i < count; i++) {
        int len = (int)strlen(files[i].name);
        if (len > maxNameLen) maxNameLen = len;
    }

    int colWidth = maxNameLen + PADDING;
    int termWidth = getTerminalWidth();
    int columns = (termWidth / colWidth) > 0 ? (termWidth / colWidth) : 1;

    for (int i = 0; i < count; i++) {
        const char* symbol = (files[i].attr & FILE_ATTRIBUTE_DIRECTORY) ? "📁" : "📄";
        printf("%s|%-*s", symbol, colWidth - 2, files[i].name);
        if ((i + 1) % columns == 0) printf("\n");
    }

    if (count % columns != 0) printf("\n");
    return 0;
}
