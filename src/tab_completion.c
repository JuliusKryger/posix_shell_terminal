#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "tab_completion.h"

#define BUILTIN_COUNT 5

const char *builtins[] = {"cd", "pwd", "echo", "type", "history"};

void handle_tab_completion(char *input, int *index)
{
    char *completion = NULL;
    size_t input_len = strlen(input);
    DIR *dir;
    struct dirent *entry;

    // Try to complete built-in commands
    for (int i = 0; i < BUILTIN_COUNT; i++)
    {
        if (strncmp(builtins[i], input, input_len) == 0)
        {
            completion = strdup(builtins[i]);
            break;
        }
    }

    // Try to complete executable commands in PATH
    if (!completion)
    {
        char *path_env = getenv("PATH");
        if (path_env)
        {
            char *path_copy = strdup(path_env);
            char *dir = strtok(path_copy, ":");
            while (dir)
            {
                DIR *d = opendir(dir);
                if (d)
                {
                    while ((entry = readdir(d)))
                    {
                        if (entry->d_type == DT_REG && strncmp(entry->d_name, input, input_len) == 0)
                        {
                            completion = strdup(entry->d_name);
                            break;
                        }
                    }
                    closedir(d);
                }
                if (completion) break;
                dir = strtok(NULL, ":");
            }
            free(path_copy);
        }
    }

    // Try to complete file names in the current directory
    if (!completion)
    {
        dir = opendir(".");
        if (dir)
        {
            while ((entry = readdir(dir)))
            {
                if (strncmp(entry->d_name, input, input_len) == 0)
                {
                    completion = strdup(entry->d_name);
                    break;
                }
            }
            closedir(dir);
        }
    }

    // If a completion was found, update the input
    if (completion)
    {
        strcpy(input, completion);
        *index = strlen(completion);
        printf("\r$ %s", input);
        fflush(stdout);
        free(completion);
    }
}
