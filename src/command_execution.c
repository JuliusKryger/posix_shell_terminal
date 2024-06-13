#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "command_execution.h"
#include "redirection.h"
#include "utils.h"
#include "job_control.h"

void execute_command(char **args)
{
    char *command_path = find_in_path(args[0]);
    if (command_path)
    {
        int bg = 0;
        handle_job_control(args, &bg);

        // Fork and execute the command
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            execvp(command_path, args);
            // If execvp returns, there was an error
            perror("execvp");
            exit(1);
        }
        else if (pid > 0)
        {
            if (bg) {
                add_job(pid, 1, args[0]);
                printf("[%d] %d\n", job_count, pid);
            } else {
                // Parent process
                int status;
                waitpid(pid, &status, 0);
            }
        }
        else
        {
            // Fork failed
            perror("fork");
        }
    }
    else
    {
        // Command not found in PATH
        printf("%s: command not found\n", args[0]);
    }
}

void execute_pipeline(char *input)
{
    char *commands[10];
    int command_count = 0;

    char *command = strtok(input, "|");
    while (command != NULL && command_count < 10)
    {
        commands[command_count++] = command;
        command = strtok(NULL, "|");
    }

    int pipefds[2 * (command_count - 1)];
    for (int i = 0; i < command_count - 1; i++)
    {
        if (pipe(pipefds + i * 2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < command_count; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            if (i > 0)
            {
                if (dup2(pipefds[(i - 1) * 2], 0) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            if (i < command_count - 1)
            {
                if (dup2(pipefds[i * 2 + 1], 1) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            for (int j = 0; j < 2 * (command_count - 1); j++)
            {
                close(pipefds[j]);
            }

            char *args[10];
            int argc;
            parse_input(commands[i], args, &argc);
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

            execute_command(args);

            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 2 * (command_count - 1); i++)
    {
        close(pipefds[i]);
    }

    for (int i = 0; i < command_count; i++)
    {
        wait(NULL);
    }
}
