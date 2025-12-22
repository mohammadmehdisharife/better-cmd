#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>
#include "../libs/ansi_colors.h"

int main(void)
{
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        {
            fprintf(stderr, ANSI_BOLD_RED "Error: Cannot create snapshot.\n" ANSI_RESET);
            return 1;
        }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32))
        {
            CloseHandle(hSnapshot);
            fprintf(stderr, ANSI_BOLD_RED "Error: Cannot get process info.\n" ANSI_RESET);
            return 1;
        }

    printf(ANSI_BOLD_CYAN "   PID\t   Process Name\n" ANSI_RESET);
    printf(ANSI_BOLD_WHITE "=================================\n" ANSI_RESET);

    do
        {
            printf(ANSI_BOLD_YELLOW "%6lu\t" ANSI_RESET, pe32.th32ProcessID);

            if (_stricmp(pe32.szExeFile, "System") == 0 ||
                    _stricmp(pe32.szExeFile, "smss.exe") == 0 ||
                    _stricmp(pe32.szExeFile, "csrss.exe") == 0 ||
                    _stricmp(pe32.szExeFile, "wininit.exe") == 0 ||
                    _stricmp(pe32.szExeFile, "services.exe") == 0 ||
                    _stricmp(pe32.szExeFile, "lsass.exe") == 0 ||
                    _stricmp(pe32.szExeFile, "svchost.exe") == 0)
                {
                    printf(ANSI_BOLD_BLUE "%s\n" ANSI_RESET, pe32.szExeFile);
                }
            else
                {
                    printf(ANSI_BOLD_GREEN "%s\n" ANSI_RESET, pe32.szExeFile);
                }
        }
    while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return 0;
}
