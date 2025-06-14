#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../libs/ansi_colors.h"

#define BUFFER_SIZE 8192

void remove_substring(const char *input, const char *sub, char *output) {
    size_t sub_len = strlen(sub);
    const char *p = input;
    char *out = output;

    while (*p) {
        if (strncmp(p, sub, sub_len) == 0) {
            p += sub_len;  
        } else {
            *out++ = *p++;
        }
    }
    *out = '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, ANSI_BOLD_RED"Usage: %s <substring_to_remove>\n"ANSI_RESET, argv[0]);
        return 1;
    }

    const char *sub = argv[1];
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;

    if (GetConsoleMode(hInput, &mode)) {
        fprintf(stderr, ANSI_BOLD_RED"Error: Input must come from a pipe or file.\n"ANSI_RESET);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    char output[BUFFER_SIZE * 2]; 

    DWORD bytesRead;
    while (ReadFile(hInput, buffer, BUFFER_SIZE - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        remove_substring(buffer, sub, output);
        DWORD bytesWritten;
        WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), output, (DWORD)strlen(output), &bytesWritten, NULL);
    }

    return 0;
}
