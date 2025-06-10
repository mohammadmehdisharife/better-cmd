#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../libs/ansi_colors.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: runtime <command>\n");
        return 1;
    }

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    system(argv[1]);

    QueryPerformanceCounter(&end);

    double elapsed_seconds = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("-----------------------------\n");
    printf(ANSI_BOLD_YELLOW "Total time: " ANSI_RESET "%.2f s\n", elapsed_seconds);

    return 0;
}
