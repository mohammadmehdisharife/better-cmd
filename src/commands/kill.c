#include <windows.h>
#include <stdio.h>
#include "../libs/ansi_colors.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(ANSI_BOLD_RED "Usage: kill <PID>\n" ANSI_RESET);
        return 1;
    }

    DWORD pid = (DWORD)atoi(argv[1]);

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        fprintf(stderr, ANSI_BOLD_RED "Error: Cannot open process with PID %lu\n" ANSI_RESET, pid);
        return 1;
    }

    if (!TerminateProcess(hProcess, 0)) {
        fprintf(stderr, ANSI_BOLD_RED "Error: Failed to terminate process %lu\n" ANSI_RESET, pid);
        CloseHandle(hProcess);
        return 1;
    }

    printf(ANSI_BOLD_GREEN "Process %lu terminated successfully.\n" ANSI_RESET, pid);
    CloseHandle(hProcess);
    return 0;
}
