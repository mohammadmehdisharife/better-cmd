#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
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
        (*words)++;
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    unsigned long lines, words, chars;

    if (argc == 1)
    {
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        if (GetConsoleMode(hInput, &mode))
        {
            fprintf(stderr, ANSI_BOLD_RED "Error: Input must come from a pipe or file.\n" ANSI_RESET);
            return 1;
        }
        fp = stdin;
    }
    else if (argc == 2)
    {
        fp = fopen(argv[1], "r");
        if (!fp)
        {
            fprintf(stderr, ANSI_BOLD_RED "cat: cannot open '%s': %s\n" ANSI_RESET, argv[1], strerror(errno));
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
        fclose(fp);

    printf(ANSI_BOLD_YELLOW "LINE: " ANSI_BOLD_WHITE "%lu\n" ANSI_RESET, lines);
    printf(ANSI_BOLD_YELLOW "WORD: " ANSI_BOLD_WHITE "%lu\n" ANSI_RESET, words);
    printf(ANSI_BOLD_YELLOW "CHAR: " ANSI_BOLD_WHITE "%lu\n" ANSI_RESET, chars);

    return 0;
}
