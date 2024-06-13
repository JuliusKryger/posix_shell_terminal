#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtins/alias.h"

#define MAX_ALIASES 100

typedef struct {
    char name[50];
    char command[100];
} Alias;

static Alias aliases[MAX_ALIASES];
static int alias_count = 0;

// Handle the 'alias' command
void handle_alias(char **args)
{
    if (args[1] == NULL)
    {
        for (int i = 0; i < alias_count; i++)
        {
            printf("alias %s='%s'\n", aliases[i].name, aliases[i].command);
        }
        return;
    }

    char *name = strtok(args[1], "=");
    char *command = strtok(NULL, "'");

    if (name && command)
    {
        for (int i = 0; i < alias_count; i++)
        {
            if (strcmp(aliases[i].name, name) == 0)
            {
                strcpy(aliases[i].command, command);
                return;
            }
        }
        strcpy(aliases[alias_count].name, name);
        strcpy(aliases[alias_count].command, command);
        alias_count++;
    }
    else
    {
        fprintf(stderr, "alias: invalid format\n");
    }
}

// Handle the 'unalias' command
void handle_unalias(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "unalias: missing operand\n");
        return;
    }

    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(aliases[i].name, args[1]) == 0)
        {
            for (int j = i; j < alias_count - 1; j++)
            {
                aliases[j] = aliases[j + 1];
            }
            alias_count--;
            return;
        }
    }
    fprintf(stderr, "unalias: %s: not found\n", args[1]);
}

// Expand aliases in the input arguments
void expand_aliases(char **args, int *argc)
{
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(aliases[i].name, args[0]) == 0)
        {
            char expanded[1024] = "";
            strcat(expanded, aliases[i].command);
            for (int j = 1; j < *argc; j++)
            {
                strcat(expanded, " ");
                strcat(expanded, args[j]);
            }
            parse_input(expanded, args, argc);
            return;
        }
    }
}
