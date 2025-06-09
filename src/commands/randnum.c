#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    else if (argc == 2)
    {
        max = atoi(argv[1]);
        if (max <= 0)
        {
            fprintf(stderr, "max_number must be positive.\n");
            return 1;
        }
    }

    srand((unsigned int)time(NULL));

    int num = rand() % (max + 1);
    printf("%d\n", num);

    return 0;
}
