#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

BOOL clear_screen_method1(HANDLE hStdOut) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        return FALSE;
    }

    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    COORD homeCoords = {0, 0};
    DWORD count;

    if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count)) {
        return FALSE;
    }
    if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) {
        return FALSE;
    }
    if (!SetConsoleCursorPosition(hStdOut, homeCoords)) {
        return FALSE;
    }

    return TRUE;
}

BOOL clear_screen_method2() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hStdOut, &dwMode)) {
        return FALSE;
    }

    if (!SetConsoleMode(hStdOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        return FALSE;
    }

    const char* clear_ansi = "\x1b[2J\x1b[H";
    DWORD written;
    if (!WriteConsoleA(hStdOut, clear_ansi, (DWORD)strlen(clear_ansi), &written, NULL)) {
        return FALSE;
    }

    if (!SetConsoleMode(hStdOut, dwMode)) {
        return FALSE;
    }

    return TRUE;
}

BOOL clear_screen_method3() {
    if (system("cls") == 0) {
        return TRUE;
    }
    return FALSE;
}

void clear_screen() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        if (clear_screen_method2()) {
            return;
        } else if (clear_screen_method3()) {
            return;
        } else {
            fprintf(stderr, "Error: GetStdHandle failed and all clear methods failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (clear_screen_method1(hStdOut)) {
        return;
    } else if (clear_screen_method2()) {
        return;
    } else if (clear_screen_method3()) {
        return;
    } else {
        fprintf(stderr, "Error: All clear methods failed.\n");
        exit(EXIT_FAILURE);
    }
}

int main() {
    clear_screen();
    return 0;
}
