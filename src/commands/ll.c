#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../libs/ansi_colors.h"

#define MAX_FILES 1024
#define NAME_WIDTH 20

typedef struct
{
    char name[MAX_PATH];
    DWORD attr;
    LARGE_INTEGER size;
    FILETIME lastWriteTime;
} FileEntry;

void setConsoleUTF8()
{
    SetConsoleOutputCP(CP_UTF8);
}

void formatSize(LARGE_INTEGER size, char *out)
{
    if (size.QuadPart > 1000000)
        sprintf(out, "%lldM", size.QuadPart / 1000000);
    else if (size.QuadPart > 1000)
        sprintf(out, "%lldK", size.QuadPart / 1000);
    else
        sprintf(out, "%lld", size.QuadPart);
}

void formatFileTime(FILETIME ft, char *out)
{
    SYSTEMTIME stUTC, stLocal;
    FileTimeToSystemTime(&ft, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    sprintf(out, "%04d-%02d-%02d %02d:%02d",
            stLocal.wYear, stLocal.wMonth, stLocal.wDay,
            stLocal.wHour, stLocal.wMinute);
}

int main(int argc, char *argv[])
{
    setConsoleUTF8();

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA("*", &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("No files found.\n");
        return 1;
    }

    FileEntry files[MAX_FILES];
    int count = 0;

    do
    {
        if (count >= MAX_FILES)
            break;

        strcpy(files[count].name, findData.cFileName);
        files[count].attr = findData.dwFileAttributes;
        files[count].size.LowPart = findData.nFileSizeLow;
        files[count].size.HighPart = findData.nFileSizeHigh;
        files[count].lastWriteTime = findData.ftLastWriteTime;
        count++;
    } while (FindNextFileA(hFind, &findData));
    FindClose(hFind);

    for (int i = 0; i < count; i++)
    {
        char sizeStr[32];
        char dateStr[64];
        char namePadded[NAME_WIDTH + 1];
        const char *color;
        const char *symbol;
        const char *ext = strrchr(files[i].name, '.');

        if (files[i].attr & FILE_ATTRIBUTE_DIRECTORY)
            color = ANSI_BOLD_YELLOW;
        else
            color = ANSI_BOLD_WHITE;

        if (files[i].attr & FILE_ATTRIBUTE_DIRECTORY)
        {
            symbol = "📁";
        }
        else if (ext && (strcmp(ext, ".exe") == 0 || strcmp(ext, ".msi") == 0 || strcmp(ext, ".com") == 0 ||
                         strcmp(ext, ".wsf") == 0 || strcmp(ext, ".pif") == 0 || strcmp(ext, ".scr") == 0 ||
                         strcmp(ext, ".msc") == 0))
            symbol = "📦";
        else if (ext && (strcmp(ext, ".zip") == 0 || strcmp(ext, ".rar") == 0 || strcmp(ext, ".7z") == 0 ||
                         strcmp(ext, ".tar") == 0 || strcmp(ext, ".gz") == 0 || strcmp(ext, ".bz2") == 0 ||
                         strcmp(ext, ".xz") == 0 || strcmp(ext, ".lz") == 0 || strcmp(ext, ".lzma") == 0 ||
                         strcmp(ext, ".iso") == 0 || strcmp(ext, ".cab") == 0 || strcmp(ext, ".arj") == 0 ||
                         strcmp(ext, ".tgz") == 0 || strcmp(ext, ".tbz2") == 0 || strcmp(ext, ".txz") == 0))
            symbol = "🧰";
        else if (ext && (strcmp(ext, ".lnk") == 0 || strcmp(ext, ".url") == 0))
            symbol = "🔗";
        else if (ext && (strcmp(ext, ".bmp") == 0 || strcmp(ext, ".dib") == 0 || strcmp(ext, ".rle") == 0 ||
                         strcmp(ext, ".png") == 0 || strcmp(ext, ".gif") == 0 || strcmp(ext, ".jpg") == 0 ||
                         strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".jfif") == 0 || strcmp(ext, ".jpe") == 0 ||
                         strcmp(ext, ".webp") == 0 || strcmp(ext, ".tif") == 0 || strcmp(ext, ".tiff") == 0 ||
                         strcmp(ext, ".ico") == 0 || strcmp(ext, ".heic") == 0 || strcmp(ext, ".heif") == 0 ||
                         strcmp(ext, ".ppm") == 0 || strcmp(ext, ".pgm") == 0 || strcmp(ext, ".pbm") == 0 ||
                         strcmp(ext, ".pnm") == 0 || strcmp(ext, ".pcx") == 0 || strcmp(ext, ".raw") == 0 ||
                         strcmp(ext, ".svg") == 0 || strcmp(ext, ".xcf") == 0 || strcmp(ext, ".psd") == 0 ||
                         strcmp(ext, ".dds") == 0 || strcmp(ext, ".tga") == 0 || strcmp(ext, ".exr") == 0 ||
                         strcmp(ext, ".fits") == 0 || strcmp(ext, ".svgz") == 0))
            symbol = "📷";
        else if (ext && (strcmp(ext, ".mp3") == 0 || strcmp(ext, ".wav") == 0 || strcmp(ext, ".flac") == 0 ||
                         strcmp(ext, ".aac") == 0 || strcmp(ext, ".ogg") == 0 || strcmp(ext, ".wma") == 0 ||
                         strcmp(ext, ".m4a") == 0 || strcmp(ext, ".alac") == 0 || strcmp(ext, ".aiff") == 0 ||
                         strcmp(ext, ".ape") == 0 || strcmp(ext, ".opus") == 0 || strcmp(ext, ".amr") == 0 ||
                         strcmp(ext, ".mid") == 0 || strcmp(ext, ".midi") == 0 || strcmp(ext, ".xmf") == 0 ||
                         strcmp(ext, ".tta") == 0 || strcmp(ext, ".wv") == 0 || strcmp(ext, ".cda") == 0 ||
                         strcmp(ext, ".spx") == 0 || strcmp(ext, ".caf") == 0 || strcmp(ext, ".dts") == 0 ||
                         strcmp(ext, ".ac3") == 0 || strcmp(ext, ".eac3") == 0 || strcmp(ext, ".mlp") == 0 ||
                         strcmp(ext, ".voc") == 0 || strcmp(ext, ".vox") == 0 || strcmp(ext, ".snd") == 0 ||
                         strcmp(ext, ".au") == 0 || strcmp(ext, ".aif") == 0))
            symbol = "🎧";
        else if (ext && (strcmp(ext, ".mp4") == 0 || strcmp(ext, ".mkv") == 0 || strcmp(ext, ".avi") == 0 ||
                         strcmp(ext, ".mov") == 0 || strcmp(ext, ".flv") == 0 || strcmp(ext, ".wmv") == 0 ||
                         strcmp(ext, ".webm") == 0 || strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0 ||
                         strcmp(ext, ".3gp") == 0 || strcmp(ext, ".m4v") == 0 || strcmp(ext, ".mts") == 0 ||
                         strcmp(ext, ".m2ts") == 0 || strcmp(ext, ".ts") == 0 || strcmp(ext, ".ogv") == 0 ||
                         strcmp(ext, ".rm") == 0 || strcmp(ext, ".rmvb") == 0 || strcmp(ext, ".divx") == 0 ||
                         strcmp(ext, ".vob") == 0 || strcmp(ext, ".f4v") == 0 || strcmp(ext, ".xvid") == 0 ||
                         strcmp(ext, ".mxf") == 0 || strcmp(ext, ".roq") == 0 || strcmp(ext, ".yuv") == 0 ||
                         strcmp(ext, ".asf") == 0))
            symbol = "🎬";
        else if (ext && (strcmp(ext, ".sh") == 0 || strcmp(ext, ".bash") == 0 || strcmp(ext, ".bat") == 0 ||
                         strcmp(ext, ".cmd") == 0 || strcmp(ext, ".ps1") == 0 || strcmp(ext, ".zsh") == 0 ||
                         strcmp(ext, ".py") == 0 || strcmp(ext, ".pl") == 0 || strcmp(ext, ".rb") == 0 ||
                         strcmp(ext, ".js") == 0 || strcmp(ext, ".ts") == 0 || strcmp(ext, ".groovy") == 0 ||
                         strcmp(ext, ".lua") == 0 || strcmp(ext, ".psm1") == 0 || strcmp(ext, ".tcl") == 0 ||
                         strcmp(ext, ".r") == 0 || strcmp(ext, ".coffee") == 0 || strcmp(ext, ".awk") == 0 ||
                         strcmp(ext, ".sed") == 0))
            symbol = "📜";
        else
            symbol = "📄";

        if (files[i].attr & FILE_ATTRIBUTE_DIRECTORY)
            strcpy(sizeStr, "-");
        else
            formatSize(files[i].size, sizeStr);

        formatFileTime(files[i].lastWriteTime, dateStr);

        snprintf(namePadded, sizeof(namePadded), "%-*s", NAME_WIDTH, files[i].name);

        printf("%s%s|%s %5s %s%s\n",
               color, symbol, namePadded, sizeStr, dateStr, ANSI_RESET);
    }

    return 0;
}
