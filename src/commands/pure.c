#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../libs/ansi_colors.h"

int is_emoji(const char *text) {
    if ((unsigned char)text[0] >= 0xF0)
        return 4;
    else if ((unsigned char)text[0] >= 0xE0 && (unsigned char)text[1] >= 0x80)
        return 3;
    else if ((unsigned char)text[0] >= 0xC0 && (unsigned char)text[1] >= 0x80)
        return 2;
    return 0;
}

void remove_ansi_codes(char *text) {
    char *p = text;
    char *q = text;
    
    while (*p) {
        if (*p == '\033') {
            p++;
            if (*p == '[') {
                p++;
                while (*p && (*p == ';' || (*p >= '0' && *p <= '9'))) {
                    p++;
                }
                if (*p == 'm' || *p == 'K' || *p == 'J' || *p == 'H') {
                    p++;
                }
            }
        } else {
            *q++ = *p++;
        }
    }
    *q = '\0';
}

void remove_emojis(char *text) {
    char *p = text;
    char *q = text;
    
    while (*p) {
        int emoji_len = is_emoji(p);
        if (emoji_len > 0) {
            p += emoji_len;
        } else {
            *q++ = *p++;
        }
    }
    *q = '\0';
}

int process_input(FILE *input) {
    char *buffer = NULL;
    size_t buffer_size = 0;
    size_t total = 0;
    
    while (!feof(input)) {
        buffer = realloc(buffer, buffer_size + 4096);
        if (!buffer) {
            perror(ANSI_BOLD_RED"Memory allocation failed"ANSI_RESET);
            return 1;
        }
        
        size_t read = fread(buffer + buffer_size, 1, 4096, input);
        if (read == 0 && feof(input)) {
            break;
        }
        buffer_size += read;
    }
    
    if (buffer) {
        buffer = realloc(buffer, buffer_size + 1);
        buffer[buffer_size] = '\0';
        
        remove_ansi_codes(buffer);
        remove_emojis(buffer);
        
        fwrite(buffer, 1, strlen(buffer), stdout);
        free(buffer);
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    if (argc == 1) {
        return process_input(stdin);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE *file = fopen(argv[i], "rb");
            if (!file) {
                fprintf(stderr, ANSI_BOLD_RED"Error opening file: %s\n"ANSI_RESET, argv[i]);
                continue;
            }
            
            if (process_input(file) != 0) {
                fclose(file);
                return 1;
            }
            
            fclose(file);
        }
    }
    
    return 0;
}