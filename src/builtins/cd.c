#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtins/cd.h"

// Handle the 'cd' command
void handle_cd(char **args)
{
    char *path = args[1];

    if (path == NULL || strcmp(path, "~") == 0)
    {
        path = getenv("HOME");
    }

    if (chdir(path) != 0)
    {
        perror("cd");
    }
}
