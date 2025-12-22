#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "../libs/ansi_colors.h"

#define DEFAULT_LINES 10
#define BUFFER_SIZE 1024
#define MAX_LINES 10000

void print_tail(FILE *fp, int nlines)
{
    char *lines[MAX_LINES];
    int count = 0;

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            if (count < MAX_LINES)
                {
                    lines[count] = strdup(buffer);
                    count++;
                }
        }

    int start = (count > nlines) ? count - nlines : 0;

    for (int i = start; i < count; i++)
        {
            printf("%s", lines[i]);
            free(lines[i]);
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
                    fprintf(stderr, ANSI_BOLD_RED "tail: cannot open '%s'\n" ANSI_RESET, filename);
                    return 1;
                }
            print_tail(fp, nlines);
            fclose(fp);
        }
    else
        {
            if (_isatty(_fileno(stdin)))
                {
                    fprintf(stderr, ANSI_BOLD_RED "tail: no input\n" ANSI_RESET);
                    return 1;
                }
            print_tail(stdin, nlines);
        }

    return 0;
}
