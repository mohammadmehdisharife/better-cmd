#include <windows.h>
#include <stdio.h>
#include <string.h>

void printFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("\033[1;31mThere is no man page for %s\n\033[0m", filename);
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
        printf("\033[1;31mUsage: %s <filename>\n\033[0m", argv[0]);
        return 1;
    }

    char exe_path[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    if (length == 0 || length == MAX_PATH)
    {
        printf("\033[1;31mError getting executable path\n\033[0m");
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
