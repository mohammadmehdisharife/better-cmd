#include <stdio.h>
#include <string.h>

#define COLOR_BOLD_RED "\x1b[1;31m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_YELLOW "\x1b[1;33m"
#define COLOR_BOLD_WHITE "\x1b[1;37m"
void printHelp()
{
    printf(COLOR_YELLOW
           "Usage: better_cmd [-option]\n"
           "\n"
           "Options:\n"
           "  -h, --help          Show this help message\n"
           "  -v, --version       Show version information\n"
           "  -l, --list         List available commands\n" COLOR_RESET);
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, COLOR_BOLD_RED "Usage: %s [-option]\nTry 'bettercmd --help' for more information.\n" COLOR_RESET, argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        printHelp();
        return 0;
    }
    else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
    {
        printf(COLOR_BOLD_WHITE"%s Version 0.0.7 Stable\n"COLOR_RESET, argv[0]);
        return 0;
    }
    else if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--list") == 0)
    {
        printf(COLOR_YELLOW "Available commands:\n" COLOR_RESET COLOR_YELLOW "    cat     :" COLOR_RESET " Print content within a file\n" COLOR_YELLOW "    touch   :" COLOR_RESET " Create file\n" COLOR_YELLOW "    clear   :" COLOR_RESET " Clear screen\n" COLOR_YELLOW "    dirname :" COLOR_RESET " Prints the directory portion of a given path.\n" COLOR_YELLOW "    basename:" COLOR_RESET " Prints the final component of a given path.\n" COLOR_YELLOW "    ls      :" COLOR_RESET " Lists directory contents\n" COLOR_YELLOW "    ll      :" COLOR_RESET " Lists files with detailed information\n" COLOR_YELLOW "    now     :" COLOR_RESET " Display time & date\n" COLOR_YELLOW "    head    :" COLOR_RESET " Prints the first part of a file\n" COLOR_YELLOW "    tail    :" COLOR_RESET " Prints the last part of a file\n" COLOR_YELLOW "    stat    :" COLOR_RESET " Displays detailed information about a file or file system\n" COLOR_YELLOW "    pwd     :" COLOR_RESET " Prints the current working directory\n");

        return 0;
    }

    return 0;
}
