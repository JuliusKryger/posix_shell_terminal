#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

// Function to check if a file exists and is executable
int is_executable(const char *path)
{
    return access(path, X_OK) == 0;
}

// Function to search for a command in the PATH
char *find_in_path(const char *command)
{
    char *path_env = getenv("PATH");
    if (path_env == NULL)
    {
        return NULL;
    }

    char *path_copy = strdup(path_env);
    char *dir = strtok(path_copy, ":");
    static char full_path[1024];

    while (dir != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (is_executable(full_path))
        {
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

// Function to parse input into arguments
void parse_input(char *input, char **args, int *argc)
{
    *argc = 0;
    char *token = strtok(input, " ");
    while (token != NULL && *argc < 10)
    {
        args[(*argc)++] = token;
        token = strtok(NULL, " ");
    }
    args[*argc] = NULL;
}
