#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "../libs/ansi_colors.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
        {
            fprintf(stderr, ANSI_BOLD_RED "Usage: runtime <command>\n"ANSI_RESET);
            return 1;
        }

    char cmdline[32768] = {0};
    for (int i = 1; i < argc; i++)
        {
            strcat(cmdline, argv[i]);
            if (i < argc - 1)
                strcat(cmdline, " ");
        }

    LARGE_INTEGER frequency, start, end;
    if (!QueryPerformanceFrequency(&frequency))
        {
            fprintf(stderr, ANSI_BOLD_RED "High-resolution timer not supported.\n" ANSI_RESET);
            return 1;
        }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    QueryPerformanceCounter(&start);

    BOOL success = CreateProcess(
                       NULL,
                       cmdline,
                       NULL,
                       NULL,
                       FALSE,
                       0,
                       NULL,
                       NULL,
                       &si,
                       &pi);

    if (!success)
        {
            fprintf(stderr, ANSI_BOLD_RED "CreateProcess failed (%lu).\n" ANSI_RESET, GetLastError());
            return 1;
        }

    WaitForSingleObject(pi.hProcess, INFINITE);

    QueryPerformanceCounter(&end);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    double elapsed_seconds = (double)(end.QuadPart - start.QuadPart) / (double)frequency.QuadPart;

    printf("-----------------------------\n");
    printf(ANSI_BOLD_YELLOW "Total time:" ANSI_RESET " %.4f s\n", elapsed_seconds);

    return 0;
}
