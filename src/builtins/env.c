#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtins/env.h"

// Handle the 'export' command
void handle_export(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "export: missing operand\n");
        return;
    }

    char *var = strtok(args[1], "=");
    char *value = strtok(NULL, "=");

    if (var && value)
    {
        setenv(var, value, 1);
    }
    else
    {
        fprintf(stderr, "export: invalid format\n");
    }
}

// Expand environment variables in the input
void expand_variables(char *input)
{
    char expanded[1024] = "";
    char *start = input;
    char *dollar;

    while ((dollar = strchr(start, '$')) != NULL)
    {
        strncat(expanded, start, dollar - start);
        char *end = strpbrk(dollar + 1, " \t\n$");
        if (end == NULL)
        {
            end = dollar + strlen(dollar);
        }
        char var_name[1024];
        strncpy(var_name, dollar + 1, end - dollar - 1);
        var_name[end - dollar - 1] = '\0';
        char *var_value = getenv(var_name);
        if (var_value)
        {
            strcat(expanded, var_value);
        }
        start = end;
    }
    strcat(expanded, start);
    strcpy(input, expanded);
}
