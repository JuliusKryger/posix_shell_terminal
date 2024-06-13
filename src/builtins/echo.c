#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtins/echo.h"

// Handle the 'echo' command
void handle_echo(char **args)
{
    for (int i = 1; args[i] != NULL; i++)
    {
        if (args[i][0] == '$')
        {
            char *var = getenv(args[i] + 1);
            if (var)
            {
                printf("%s", var);
            }
        }
        else
        {
            printf("%s", args[i]);
        }
        if (args[i + 1] != NULL)
        {
            printf(" ");
        }
    }
    printf("\n");
}
