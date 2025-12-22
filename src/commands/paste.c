#include <windows.h>
#include <stdio.h>
#include "../libs/ansi_colors.h"

int main(void)
{
    if (!OpenClipboard(NULL))
        {
            fprintf(stderr, ANSI_BOLD_RED "Cannot open clipboard.\n" ANSI_RESET);
            return 1;
        }

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == NULL)
        {
            fprintf(stderr, ANSI_BOLD_RED "No text data in clipboard.\n" ANSI_RESET);
            CloseClipboard();
            return 1;
        }

    char *pszText = (char*)GlobalLock(hData);
    if (pszText == NULL)
        {
            fprintf(stderr, ANSI_BOLD_RED "Failed to lock clipboard data.\n" ANSI_RESET);
            CloseClipboard();
            return 1;
        }

    printf("%s", pszText);

    GlobalUnlock(hData);
    CloseClipboard();
    return 0;
}
