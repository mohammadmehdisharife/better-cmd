#include <stdio.h>
#include "../libs/ansi_colors.h"

void printFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file %s\n", filename);
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf(ANSI_BOLD_RED "Usage: %s <filename>\n" ANSI_RESET, argv[0]);
        return 1;
    }

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "../man/%s.txt", argv[1]);

    printFile(filepath);
    return 0;
}