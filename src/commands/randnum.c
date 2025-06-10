#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../libs/ansi_colors.h"

int main(int argc, char *argv[])
{
    int max;
    if (argc > 2)
    {
        fprintf(stderr, ANSI_BOLD_RED "Usage: %s max_number\n" ANSI_RESET, argv[0]);
        return 1;
    }
    else if (argc == 1)
    {
        max = 100;
    }
    else
    {
        max = atoi(argv[1]);
        if (max <= 0)
        {
            fprintf(stderr, "max_number must be positive.\n");
            return 1;
        }
    }

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    srand((unsigned int)(counter.QuadPart ^ (counter.QuadPart >> 32)));

    int num = rand() % (max + 1);
    printf("%d\n", num);

    return 0;
}
