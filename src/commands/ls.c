#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_FILES 1024
#define PADDING 4

typedef struct
{
    char name[MAX_PATH];
    DWORD attr;
} FileEntry;

void setConsoleUTF8()
{
    SetConsoleOutputCP(CP_UTF8);
}

int getTerminalWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80; // fallback
}

int main(int argc, char *argv[])
{
    setConsoleUTF8();

    int showHidden = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
        {
            showHidden = 1;
            break;
        }
    }

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;
    FileEntry files[MAX_FILES];
    int count = 0;

    hFind = FindFirstFileA("*", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("No files found.\n");
        return 1;
    }

    do
    {
        if (count >= MAX_FILES)
            break;

        if (!showHidden && findFileData.cFileName[0] == '.')
        {
            continue;
        }

        strcpy(files[count].name, findFileData.cFileName);
        files[count].attr = findFileData.dwFileAttributes;
        count++;
    } while (FindNextFileA(hFind, &findFileData));
    FindClose(hFind);

    int maxNameLen = 0;
    for (int i = 0; i < count; i++)
    {
        int len = (int)strlen(files[i].name);
        if (len > maxNameLen)
            maxNameLen = len;
    }

    int colWidth = maxNameLen + PADDING;
    int termWidth = getTerminalWidth();
    int columns = (termWidth / colWidth) > 0 ? (termWidth / colWidth) : 1;

    for (int i = 0; i < count; i++)
    {
        const char *ext = strrchr(files[i].name, '.');
        const char *symbol;
        const char *color = (files[i].attr & FILE_ATTRIBUTE_DIRECTORY) ? "\033[1;33m" : "\033[1;37m";
        const char *resetcolor = "\033[0m";

        if (files[i].attr & FILE_ATTRIBUTE_DIRECTORY)
        {
            symbol = "📁";
        }
        else if (ext && (strcmp(ext, ".exe") == 0 ||
                         strcmp(ext, ".bat") == 0 ||
                         strcmp(ext, ".cmd") == 0 ||
                         strcmp(ext, ".ps1") == 0 ||
                         strcmp(ext, ".vbs") == 0 ||
                         strcmp(ext, ".msi") == 0 ||
                         strcmp(ext, ".com") == 0 ||
                         strcmp(ext, ".wsf") == 0 ||
                         strcmp(ext, ".pif") == 0 ||
                         strcmp(ext, ".scr") == 0 ||
                         strcmp(ext, ".msc") == 0))
        {
            symbol = "⚙️";
        }
        else if (ext && (strcmp(ext, ".zip") == 0 ||
                         strcmp(ext, ".rar") == 0 ||
                         strcmp(ext, ".7z") == 0 ||
                         strcmp(ext, ".tar") == 0 ||
                         strcmp(ext, ".gz") == 0 ||
                         strcmp(ext, ".bz2") == 0 ||
                         strcmp(ext, ".xz") == 0))
        {
            symbol = "📦";
        }
        else if (ext && (strcmp(ext, ".lnk") == 0 ||
                         strcmp(ext, ".url") == 0))
        {
            symbol = "🔗";
        }
        else if (ext && (strcmp(ext, ".bmp") == 0 ||
                         strcmp(ext, ".dib") == 0 ||
                         strcmp(ext, ".rle") == 0 ||
                         strcmp(ext, ".png") == 0 ||
                         strcmp(ext, ".gif") == 0 ||
                         strcmp(ext, ".jpg") == 0 ||
                         strcmp(ext, ".jpeg") == 0 ||
                         strcmp(ext, ".jfif") == 0 ||
                         strcmp(ext, ".jpe") == 0 ||
                         strcmp(ext, ".webp") == 0 ||
                         strcmp(ext, ".tif") == 0 ||
                         strcmp(ext, ".tiff") == 0 ||
                         strcmp(ext, ".ico") == 0 ||
                         strcmp(ext, ".heic") == 0 ||
                         strcmp(ext, ".heif") == 0 ||
                         strcmp(ext, ".ppm") == 0 ||
                         strcmp(ext, ".pgm") == 0 ||
                         strcmp(ext, ".pbm") == 0 ||
                         strcmp(ext, ".pnm") == 0 ||
                         strcmp(ext, ".pcx") == 0 ||
                         strcmp(ext, ".raw") == 0 ||
                         strcmp(ext, ".svg") == 0 ||
                         strcmp(ext, ".xcf") == 0 ||
                         strcmp(ext, ".psd") == 0 ||
                         strcmp(ext, ".dds") == 0 ||
                         strcmp(ext, ".tga") == 0 ||
                         strcmp(ext, ".exr") == 0 ||
                         strcmp(ext, ".fits") == 0 ||
                         strcmp(ext, ".svgz") == 0))
        {
            symbol = "🖼️";
        }
        else if (ext && (strcmp(ext, ".mp3") == 0 ||
                         strcmp(ext, ".wav") == 0 ||
                         strcmp(ext, ".flac") == 0 ||
                         strcmp(ext, ".aac") == 0 ||
                         strcmp(ext, ".ogg") == 0 ||
                         strcmp(ext, ".wma") == 0 ||
                         strcmp(ext, ".m4a") == 0 ||
                         strcmp(ext, ".alac") == 0 ||
                         strcmp(ext, ".aiff") == 0 ||
                         strcmp(ext, ".ape") == 0 ||
                         strcmp(ext, ".opus") == 0 ||
                         strcmp(ext, ".amr") == 0 ||
                         strcmp(ext, ".mid") == 0 ||
                         strcmp(ext, ".midi") == 0 ||
                         strcmp(ext, ".xmf") == 0 ||
                         strcmp(ext, ".tta") == 0 ||
                         strcmp(ext, ".wv") == 0 ||
                         strcmp(ext, ".cda") == 0 ||
                         strcmp(ext, ".spx") == 0 ||
                         strcmp(ext, ".caf") == 0 ||
                         strcmp(ext, ".dts") == 0 ||
                         strcmp(ext, ".ac3") == 0 ||
                         strcmp(ext, ".eac3") == 0 ||
                         strcmp(ext, ".mlp") == 0 ||
                         strcmp(ext, ".tta") == 0 ||
                         strcmp(ext, ".voc") == 0 ||
                         strcmp(ext, ".vox") == 0 ||
                         strcmp(ext, ".snd") == 0 ||
                         strcmp(ext, ".au") == 0 ||
                         strcmp(ext, ".aif") == 0))
        {
            symbol = "🎵";
        }

        else
        {
            symbol = "📄";
        }

        printf("%s%s|%-*s%s", symbol, color, colWidth - 2, files[i].name, resetcolor);
        if ((i + 1) % columns == 0)
            printf("\n");
    }

    if (count % columns != 0)
        printf("\n");
    return 0;
}
