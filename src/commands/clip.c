#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include "../libs/ansi_colors.h"

#define BUF_SIZE 8192

int main(int argc, char *argv[]) {
    char *text = NULL;

    if (argc > 1) {
        text = argv[1];
    } else {
        if (_isatty(_fileno(stdin))) {
            fprintf(stderr, ANSI_BOLD_RED "Error: No input provided (need argument or pipe)\n" ANSI_RESET);
            fprintf(stderr, ANSI_BOLD_RED "Usage: cb <text-to-copy>  OR  echo text | cb\n" ANSI_RESET);
            return 1;
        }

        char buffer[BUF_SIZE];
        size_t total = 0;
        size_t n;

        while ((n = fread(buffer + total, 1, BUF_SIZE - total - 1, stdin)) > 0) {
            total += n;
            if (total >= BUF_SIZE - 1) {
                fprintf(stderr, ANSI_BOLD_RED "Error: Input too large\n" ANSI_RESET);
                return 1;
            }
        }
        buffer[total] = '\0';

        if (total == 0) {
            fprintf(stderr, ANSI_BOLD_RED "Error: Empty input from pipe\n" ANSI_RESET);
            return 1;
        }

        text = _strdup(buffer);
    }

    const size_t len = strlen(text) + 1;

    if (!OpenClipboard(NULL)) {
        fprintf(stderr, ANSI_BOLD_RED "Error: Cannot open clipboard\n" ANSI_RESET);
        if (argc == 1) free(text);
        return 1;
    }

    if (!EmptyClipboard()) {
        fprintf(stderr, ANSI_BOLD_RED "Error: Cannot empty clipboard\n" ANSI_RESET);
        CloseClipboard();
        if (argc == 1) free(text);
        return 1;
    }

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    if (!hMem) {
        fprintf(stderr, ANSI_BOLD_RED "Error: Cannot allocate memory\n" ANSI_RESET);
        CloseClipboard();
        if (argc == 1) free(text);
        return 1;
    }

    memcpy(GlobalLock(hMem), text, len);
    GlobalUnlock(hMem);

    if (!SetClipboardData(CF_TEXT, hMem)) {
        fprintf(stderr, ANSI_BOLD_RED "Error: Cannot set clipboard data\n" ANSI_RESET);
        GlobalFree(hMem);
        CloseClipboard();
        if (argc == 1) free(text);
        return 1;
    }

    CloseClipboard();

    if (argc == 1) free(text);
    return 0;
}
