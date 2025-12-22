#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "../libs/ansi_colors.h"

void printFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        {
            printf(ANSI_BOLD_RED "There is no man page for %s\n" ANSI_RESET, filename);
            return;
        }
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
        {
            printf("%s", line);
        }
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        {
            printf(ANSI_BOLD_RED "Usage: %s <filename>\n" ANSI_RESET, argv[0]);
            return 1;
        }

    char exe_path[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    if (length == 0 || length == MAX_PATH)
        {
            printf(ANSI_BOLD_RED "Error getting executable path\n" ANSI_RESET);
            return 1;
        }

    for (int i = length - 1; i >= 0; i--)
        {
            if (exe_path[i] == '\\' || exe_path[i] == '/')
                {
                    exe_path[i] = '\0';
                    break;
                }
        }

    char filepath[MAX_PATH];
    snprintf(filepath, sizeof(filepath), "%s\\..\\man\\%s.txt", exe_path, argv[1]);

    printFile(filepath);
    return 0;
}
