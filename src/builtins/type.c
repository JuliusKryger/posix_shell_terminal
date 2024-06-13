#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtins/type.h"
#include "utils.h"

// Handle the 'type' command
void handle_type(char **args)
{
    char *command = args[1];

    if (command == NULL)
    {
        fprintf(stderr, "type: missing operand\n");
        return;
    }

    // Check for built-in commands
    if (strcmp(command, "echo") == 0 || strcmp(command, "exit") == 0 || strcmp(command, "type") == 0 || strcmp(command, "pwd") == 0 || strcmp(command, "cd") == 0)
    {
        printf("%s is a shell builtin\n", command);
    }
    else
    {
        // Search for the command in the PATH
        char *path = find_in_path(command);
        if (path)
        {
            printf("%s is %s\n", command, path);
        }
        else
        {
            printf("%s: not found\n", command);
        }
    }
}
