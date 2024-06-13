#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "script.h"
#include "utils.h"
#include "builtins/alias.h"
#include "builtins/env.h"
#include "builtins/cd.h"
#include "builtins/pwd.h"
#include "builtins/echo.h"
#include "builtins/type.h"
#include "history.h"
#include "command_execution.h"

// Execute a single line of a script
void execute_line(char *line)
{
    char *args[10];
    int argc;

    expand_variables(line);
    parse_input(line, args, &argc);
    expand_aliases(args, &argc);

    // Handle built-in commands
    if (strcmp(args[0], "cd") == 0)
    {
        handle_cd(args);
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        handle_pwd();
    }
    else if (strcmp(args[0], "echo") == 0)
    {
        handle_echo(args);
    }
    else if (strcmp(args[0], "type") == 0)
    {
        handle_type(args);
    }
    else if (strcmp(args[0], "export") == 0)
    {
        handle_export(args);
    }
    else if (strcmp(args[0], "alias") == 0)
    {
        handle_alias(args);
    }
    else if (strcmp(args[0], "unalias") == 0)
    {
        handle_unalias(args);
    }
    else if (strcmp(args[0], "history") == 0)
    {
        handle_history();
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
    else
    {
        // Execute external command
        execute_command(args);
    }
}

// Execute a script from a file
void execute_script(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("fopen");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0; // Remove the newline character
        execute_line(line);
    }

    fclose(file);
}

// Read the configuration file
void read_config_file()
{
    char *home = getenv("HOME");
    if (!home) return;

    char path[1024];
    snprintf(path, sizeof(path), "%s/.my_shellrc", home);

    FILE *file = fopen(path, "r");
    if (!file) return;

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0; // Remove the newline character
        execute_line(line);
    }

    fclose(file);
}
