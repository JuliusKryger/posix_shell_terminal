#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "redirection.h"

void handle_redirection(char **args, int *input_fd, int *output_fd, int *append_fd)
{
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            *output_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*output_fd < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            args[i] = NULL;
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            *append_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (*append_fd < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            args[i] = NULL;
        }
        else if (strcmp(args[i], "<") == 0)
        {
            *input_fd = open(args[i + 1], O_RDONLY);
            if (*input_fd < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            args[i] = NULL;
        }
    }
}
