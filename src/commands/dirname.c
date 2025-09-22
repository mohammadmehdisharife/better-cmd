#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../libs/ansi_colors.h"

#define BUFFER_SIZE 1024

int is_stdin_pipe(void) {
    DWORD type = GetFileType(GetStdHandle(STD_INPUT_HANDLE));
    return (type == FILE_TYPE_PIPE || type == FILE_TYPE_DISK);
}

int is_directory(const char *path) {
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES)
        return -1; 
    return (attrs & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
}

void print_dirname(const char *path) {
    char buffer[BUFFER_SIZE];
    strncpy(buffer, path, BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE - 1] = '\0';

    int dir_status = is_directory(buffer);
    if (dir_status == -1) {
        fprintf(stderr, ANSI_BOLD_RED "dirname: cannot access '%s'\n" ANSI_RESET, path);
        return;
    }

    if (dir_status == 1 || buffer[strlen(buffer)-1] == '\\' || buffer[strlen(buffer)-1] == '/') {
        printf("%s\n", buffer);
        return;
    }

    char *last_slash = strrchr(buffer, '\\');
    char *last_fslash = strrchr(buffer, '/');
    char *pos = last_slash > last_fslash ? last_slash : last_fslash;

    if (pos) {
        *pos = '\0';
        printf("%s\n", buffer);
    } else {
        printf(".\n");
    }
}

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];

    if (argc > 1) {
        print_dirname(argv[1]);
    } else if (is_stdin_pipe()) {
        while (fgets(buffer, sizeof(buffer), stdin)) {
            buffer[strcspn(buffer, "\r\n")] = '\0';
            if (strlen(buffer) > 0)
                print_dirname(buffer);
        }
    } else {
        fprintf(stderr, ANSI_BOLD_RED "dirname: missing operand\n" ANSI_RESET);
        return 1;
    }

    return 0;
}
