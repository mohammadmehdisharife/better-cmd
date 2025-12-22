#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include "../libs/ansi_colors.h"

#define MAX_LINES 100000

void tac_from_stdin(FILE *file)
{
    char **lines = malloc(sizeof(char*) * MAX_LINES);
    if (!lines)
        {
            fprintf(stderr, ANSI_BOLD_RED "tac: memory allocation error\n" ANSI_RESET);
            return;
        }

    size_t count = 0;
    char buffer[4096];

    while (fgets(buffer, sizeof(buffer), file))
        {
            if (count >= MAX_LINES)
                {
                    fprintf(stderr, ANSI_BOLD_RED "tac: too many lines (stdin)\n" ANSI_RESET);
                    break;
                }
            lines[count] = strdup(buffer);
            count++;
        }

    for (long i = count - 1; i >= 0; i--)
        {
            fputs(lines[i], stdout);
            free(lines[i]);
        }

    free(lines);
}

void tac_from_file(FILE *file)
{
    long pos;
    int c;
    char buffer[4096];
    int index = 0;

    fseek(file, 0, SEEK_END);
    pos = ftell(file);

    while (pos > 0)
        {
            fseek(file, --pos, SEEK_SET);
            c = fgetc(file);

            if (c == '\n' && index > 0)
                {
                    buffer[index] = '\0';
                    for (int i = index - 1; i >= 0; i--)
                        fputc(buffer[i], stdout);
                    fputc('\n', stdout);
                    index = 0;
                }
            else if (c != '\n' && c != '\r')
                {
                    if (index < sizeof(buffer)-1)
                        buffer[index++] = (char)c;
                }
        }

    if (index > 0)
        {
            buffer[index] = '\0';
            for (int i = index - 1; i >= 0; i--)
                fputc(buffer[i], stdout);
            fputc('\n', stdout);
        }
}

int main(int argc, char *argv[])
{
    FILE *file = NULL;

    if (argc == 2)
        {
            file = fopen(argv[1], "rb");
            if (!file)
                {
                    fprintf(stderr, ANSI_BOLD_RED "tac: cannot open '%s': %s\n" ANSI_RESET, argv[1], strerror(errno));
                    return 1;
                }
        }
    else if (argc == 1)
        {
            if (_isatty(_fileno(stdin)))
                {
                    fprintf(stderr, ANSI_BOLD_RED "Error: Input must come from a pipe or file.\n" ANSI_RESET);
                    fprintf(stderr, ANSI_BOLD_RED "Usage: tac [filename]  OR  somecommand | tac\n" ANSI_RESET);
                    return 1;
                }
            file = stdin;
        }
    else
        {
            fprintf(stderr, ANSI_BOLD_RED "Usage: tac [filename]\n" ANSI_RESET);
            return 1;
        }

    if (file == stdin)
        {
            tac_from_stdin(stdin);
        }
    else
        {
            tac_from_file(file);
            fclose(file);
        }

    return 0;
}
