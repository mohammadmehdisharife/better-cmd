#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include "../libs/ansi_colors.h"

void precise_sleep(DWORD ms)
{
    LARGE_INTEGER frequency, start, now;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    if (ms > 15)
        {
            Sleep(ms - 10);
        }

    do
        {
            QueryPerformanceCounter(&now);
        }
    while ((now.QuadPart - start.QuadPart) < (LONGLONG)(ms * frequency.QuadPart / 1000));

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        {
            fprintf(stderr, ANSI_BOLD_RED "Usage: %s <milliseconds>\n" ANSI_RESET, argv[0]);
            return 1;
        }
    DWORD ms = (DWORD)strtoul(argv[1], NULL, 10);
    precise_sleep(ms);
    return 0;
}
