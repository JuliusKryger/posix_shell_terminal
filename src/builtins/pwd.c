#include <stdio.h>
#include <unistd.h>
#include "builtins/pwd.h"

// Handle the 'pwd' command
void handle_pwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd");
    }
}
