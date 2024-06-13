#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "utils.h"
#include "builtins/cd.h"
#include "builtins/pwd.h"
#include "builtins/echo.h"
#include "builtins/type.h"
#include "builtins/alias.h"
#include "builtins/env.h"
#include "builtins/history.h"
#include "builtins/script.h"
#include "history.h"
#include "tab_completion.h"
#include "redirection.h"
#include "command_execution.h"
#include "job_control.h"

void copy_default_config(const char *default_config, const char *config_path)
{
    int src_fd = open(default_config, O_RDONLY);
    if (src_fd < 0)
    {
        perror("open");
        return;
    }

    int dest_fd = open(config_path, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (dest_fd < 0)
    {
        close(src_fd);
        if (errno == EEXIST)
        {
            return; // File already exists, no need to copy
        }
        perror("open");
        return;
    }

    char buffer[1024];
    ssize_t nread;
    while ((nread = read(src_fd, buffer, sizeof(buffer))) > 0)
    {
        if (write(dest_fd, buffer, nread) != nread)
        {
            perror("write");
            break;
        }
    }

    close(src_fd);
    close(dest_fd);
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        execute_script(argv[1]);
        return 0;
    }

    char input[100];
    char *args[10];
    int arg_count;

    init_history();
    setup_signal_handlers();

    char *home = getenv("HOME");
    if (home)
    {
        char config_path[1024];
        snprintf(config_path, sizeof(config_path), "%s/.my_shellrc", home);
        copy_default_config("src/default_my_shellrc", config_path);
    }

    read_config_file();

    while (1)
    {
        // Print the shell prompt
        printf("$ ");
        fflush(stdout);

        // Read user input character by character to handle special keys
        enable_raw_mode();
        int index = 0;
        char c;
        while (read(STDIN_FILENO, &c, 1) == 1 && c != '\n')
        {
            if (c == '\033') // Escape sequence
            {
                char seq[3];
                if (read(STDIN_FILENO, &seq[0], 1) == 0) break;
                if (read(STDIN_FILENO, &seq[1], 1) == 0) break;
                if (seq[0] == '[')
                {
                    if (seq[1] == 'A') // Up arrow
                    {
                        handle_up_arrow();
                    }
                    else if (seq[1] == 'B') // Down arrow
                    {
                        handle_down_arrow();
                    }
                }
            }
            else if (c == '\t') // Tab key
            {
                input[index] = '\0';
                handle_tab_completion(input, &index);
            }
            else
            {
                input[index++] = c;
                write(STDOUT_FILENO, &c, 1);
            }
        }
        input[index] = '\0';
        disable_raw_mode();

        // Remove the trailing newline character from the input
        input[strcspn(input, "\n")] = 0;

        // Handle empty input
        if (strlen(input) == 0)
        {
            continue;
        }

        // Expand environment variables
        expand_variables(input);

        // Add the command to history
        add_history(input);

        // Check for pipelines
        if (strchr(input, '|') != NULL)
        {
            execute_pipeline(input);
        }
        else
        {
            // Parse the input into arguments
            parse_input(input, args, &arg_count);

            // Expand aliases
            expand_aliases(args, &arg_count);

            // Handle job control commands
            if (strcmp(args[0], "jobs") == 0 || strcmp(args[0], "fg") == 0 || strcmp(args[0], "bg") == 0 || strcmp(args[0], "kill") == 0) {
                handle_job_control(args, NULL);
                continue;
            }

            // Handle export command
            if (strcmp(args[0], "export") == 0) {
                handle_export(args);
                continue;
            }

            // Handle alias command
            if (strcmp(args[0], "alias") == 0) {
                handle_alias(args);
                continue;
            }

            // Handle unalias command
            if (strcmp(args[0], "unalias") == 0) {
                handle_unalias(args);
                continue;
            }

            // Check for redirections
            int input_fd = -1, output_fd = -1, append_fd = -1;
            handle_redirection(args, &input_fd, &output_fd, &append_fd);

            if (input_fd != -1)
            {
                if (dup2(input_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(input_fd);
            }

            if (output_fd != -1)
            {
                if (dup2(output_fd, STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(output_fd);
            }

            if (append_fd != -1)
            {
                if (dup2(append_fd, STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(append_fd);
            }

            // Check for built-in commands
            if (strcmp(args[0], "exit") == 0)
            {
                return 0;
            }
            else if (strcmp(args[0], "cd") == 0)
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
            else if (strcmp(args[0], "history") == 0)
            {
                handle_history();
            }
            else
            {
                // Execute external command
                execute_command(args);
            }
        }
    }

    return 0;
}
