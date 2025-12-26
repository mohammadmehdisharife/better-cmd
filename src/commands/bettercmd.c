#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char exePath[MAX_PATH];
    char batchPath[MAX_PATH];
    char *lastBackslash;
    
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    
    lastBackslash = strrchr(exePath, '\\');
    if (lastBackslash) *lastBackslash = '\0';
    
    sprintf(batchPath, "%s\\..\\conf\\bettercmd.bat", exePath);
    
    int result = system(batchPath);
    
    return result;
}
