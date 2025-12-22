#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "../libs/ansi_colors.h"

#define DEFAULT_LINES 10
#define BUFFER_SIZE 1024

void print_head(FILE *fp, int nlines)
{
    char buffer[BUFFER_SIZE];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            printf("%s", buffer);
            if (++count >= nlines)
                break;
        }
}

int main(int argc, char *argv[])
{
    int nlines = DEFAULT_LINES;
    char *filename = NULL;

    for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
                {
                    nlines = atoi(argv[++i]);
                }
            else
                {
                    filename = argv[i];
                }
        }

    if (filename != NULL)
        {
            FILE *fp = fopen(filename, "r");
            if (fp == NULL)
                {
                    fprintf(stderr, ANSI_BOLD_RED "head: cannot open '%s'\n" ANSI_RESET, filename);
                    return 1;
                }
            print_head(fp, nlines);
            fclose(fp);
        }
    else
        {
            if (_isatty(_fileno(stdin)))
                {
                    fprintf(stderr, ANSI_BOLD_RED "head: no input\n" ANSI_RESET);
                    return 1;
                }
            print_head(stdin, nlines);
        }

    return 0;
}
