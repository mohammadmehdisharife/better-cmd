#include <stdio.h>
#include <ctype.h>
#include "../libs/ansi_colors.h"

int count_file(FILE *fp, unsigned long *lines, unsigned long *words, unsigned long *chars)
{
    int c;
    int in_word = 0;

    *lines = *words = *chars = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        (*chars)++;

        if (c == '\n')
            (*lines)++;

        if (isspace(c))
        {
            if (in_word)
            {
                (*words)++;
                in_word = 0;
            }
        }
        else
        {
            in_word = 1;
        }
    }

    if (in_word)
    {
        (*words)++;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    unsigned long lines, words, chars;

    if (argc == 1)
    {
        fp = stdin;
    }
    else if (argc == 2)
    {
        fp = fopen(argv[1], "r");
        if (!fp)
        {
            perror(ANSI_BOLD_RED "Error opening file" ANSI_RESET);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, ANSI_BOLD_RED "Usage: %s [filename]\n" ANSI_RESET, argv[0]);
        return 1;
    }

    count_file(fp, &lines, &words, &chars);

    if (fp != stdin)
    {
        fclose(fp);
        printf("%lu %lu %lu %s\n", lines, words, chars, argv[1]);
    }
    else
    {
        printf("%lu %lu %lu\n", lines, words, chars);
    }

    return 0;
}
