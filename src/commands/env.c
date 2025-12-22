#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../libs/ansi_colors.h"

extern char **environ;

int main()
{
    int count = 0;
    for (char **env = environ; *env != NULL; env++)
        {
            count++;
        }

    char **keys = malloc(count * sizeof(char *));
    char **values = malloc(count * sizeof(char *));
    if (!keys || !values)
        {
            fprintf(stderr, ANSI_BOLD_RED "Memory allocation failed\n" ANSI_RESET);
            return 1;
        }

    int max_key_len = 0;
    int i = 0;
    for (char **env = environ; *env != NULL; env++, i++)
        {
            char *equal_sign = strchr(*env, '=');
            if (equal_sign)
                {
                    size_t key_len = equal_sign - *env;
                    keys[i] = malloc(key_len + 1);
                    if (!keys[i])
                        {
                            fprintf(stderr, ANSI_BOLD_RED "Memory allocation failed\n" ANSI_RESET);
                            return 1;
                        }
                    strncpy(keys[i], *env, key_len);
                    keys[i][key_len] = '\0';
                    values[i] = equal_sign + 1;

                    if ((int)key_len > max_key_len)
                        {
                            max_key_len = (int)key_len;
                        }
                }
            else
                {
                    keys[i] = strdup(*env);
                    values[i] = "";
                    int len = (int)strlen(*env);
                    if (len > max_key_len)
                        max_key_len = len;
                }
        }

    for (i = 0; i < count; i++)
        {
            printf(ANSI_BOLD_YELLOW"%-*s :"ANSI_RESET" %s\n", max_key_len, keys[i], values[i]);
            free(keys[i]);
        }

    free(keys);
    free(values);

    return 0;
}
