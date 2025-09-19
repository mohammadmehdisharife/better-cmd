#include <windows.h>
#include <stdio.h>

int main(void) {
    if (!OpenClipboard(NULL)) {
        fprintf(stderr, "Cannot open clipboard.\n");
        return 1;
    }

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == NULL) {
        fprintf(stderr, "No text data in clipboard.\n");
        CloseClipboard();
        return 1;
    }

    char *pszText = (char*)GlobalLock(hData);
    if (pszText == NULL) {
        fprintf(stderr, "Failed to lock clipboard data.\n");
        CloseClipboard();
        return 1;
    }

    printf("%s", pszText);

    GlobalUnlock(hData);
    CloseClipboard();
    return 0;
}
