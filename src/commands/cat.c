#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include "../libs/ansi_colors.h"

int main(int argc, char *argv[])
{
    FILE *file = NULL;

    if (argc == 2)
        {
            file = fopen(argv[1], "r");
            if (!file)
                {
                    fprintf(stderr, ANSI_BOLD_RED "cat: cannot open '%s': %s\n" ANSI_RESET, argv[1], strerror(errno));
                    return 1;
                }
        }
    else if (argc == 1)
        {
            if (_isatty(_fileno(stdin)))
                {
                    fprintf(stderr, ANSI_BOLD_RED "Error: Input must come from a pipe or file.\n" ANSI_RESET);
                    fprintf(stderr, ANSI_BOLD_RED "Usage: cat [filename]  OR  somecommand | cat\n" ANSI_RESET);
                    return 1;
                }
            file = stdin;
        }
    else
        {
            fprintf(stderr, ANSI_BOLD_RED "Usage: cat [filename]\n" ANSI_RESET);
            return 1;
        }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file))
        {
            fputs(buffer, stdout);
        }

    if (file != stdin)
        {
            fclose(file);
        }

    return 0;
}
