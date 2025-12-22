#include <windows.h>
#include <stdio.h>
#include "../libs/ansi_colors.h"

#define BUFFER_SIZE 4096

void trim_whitespace(const char *input, char *output)
{
    int in_word = 0;
    while (*input)
        {
            if (*input == ' ' || *input == '\t' || *input == '\n' || *input == '\r')
                {
                    if (in_word)
                        {
                            *output++ = ' ';
                            in_word = 0;
                        }
                }
            else
                {
                    *output++ = *input;
                    in_word = 1;
                }
            input++;
        }
    if (output > output - 1 && *(output - 1) == ' ')
        {
            output--;
        }
    *output = '\0';
}

void process_stream(HANDLE hInput)
{
    DWORD bytesRead, bytesWritten;
    char buffer[BUFFER_SIZE];
    char trimmed[BUFFER_SIZE * 2];

    while (ReadFile(hInput, buffer, BUFFER_SIZE - 1, &bytesRead, NULL) && bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            trim_whitespace(buffer, trimmed);
            WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), trimmed, (DWORD)strlen(trimmed), &bytesWritten, NULL);
            WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, &bytesWritten, NULL);
        }
}

int main(int argc, char *argv[])
{
    HANDLE hInput;

    if (argc == 2)
        {
            hInput = CreateFileA(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hInput == INVALID_HANDLE_VALUE)
                {
                    fprintf(stderr, ANSI_BOLD_RED"Error opening file: %s\n"ANSI_RESET, argv[1]);
                    return 1;
                }
        }
    else
        {
            DWORD mode;
            hInput = GetStdHandle(STD_INPUT_HANDLE);
            if (GetConsoleMode(hInput, &mode))
                {
                    fprintf(stderr, ANSI_BOLD_RED"Error: Input must come from a pipe or file.\n"ANSI_RESET);
                    return 1;
                }
        }

    process_stream(hInput);

    if (argc == 2)
        {
            CloseHandle(hInput);
        }

    return 0;
}
