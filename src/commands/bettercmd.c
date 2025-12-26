#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shellapi.h>

int main() {
    char exePath[MAX_PATH];
    char batchPath[MAX_PATH];
    char resolvedPath[MAX_PATH];
    
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    
    char *lastBackslash = strrchr(exePath, '\\');
    if (lastBackslash) {
        *lastBackslash = '\0';
    }
    
    snprintf(batchPath, MAX_PATH, "%s\\..\\conf\\bettercmd.bat", exePath);
    
    if (GetFullPathNameA(batchPath, MAX_PATH, resolvedPath, NULL) == 0) {
        MessageBoxA(NULL, "Failed to resolve batch file path", "Error", MB_ICONERROR);
        return 1;
    }
    
    HINSTANCE hResult = ShellExecuteA(
        NULL,           
        "open",         
        resolvedPath,   
        NULL,           
        NULL,           
        SW_SHOWDEFAULT  
    );
    
    if ((int)hResult <= 32) {
        char errorMsg[256];
        snprintf(errorMsg, sizeof(errorMsg), 
                "Failed to execute batch file\nError code: %d", (int)hResult);
        MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR);
        return 1;
    }
    
    return 0;
}
