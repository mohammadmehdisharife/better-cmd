#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../libs/ansi_colors.h"

#define WORD_LENGTH 5

char random_char()
{
    return 'a' + rand() % 26;
}

void generate_random_word(char *word, int length)
{
    for (int i = 0; i < length; i++)
        {
            word[i] = random_char();
        }
    word[length] = '\0';
}

unsigned int high_precision_seed()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (unsigned int)(counter.QuadPart);
}

int main(int argc, char *argv[])
{
    int count = 10;

    if (argc == 2)
        {
            count = atoi(argv[1]);
            if (count <= 0)
                {
                    fprintf(stderr, ANSI_BOLD_RED"Invalid count value.\n"ANSI_RESET);
                    return 1;
                }
        }
    else if (argc > 2)
        {
            fprintf(stderr, ANSI_BOLD_RED"Usage: %s [count]\n"ANSI_RESET, argv[0]);
            return 1;
        }

    srand(high_precision_seed());
    char word[WORD_LENGTH + 1];

    for (int i = 0; i < count; i++)
        {
            generate_random_word(word, WORD_LENGTH);
            printf("%s\n", word);
        }

    return 0;
}