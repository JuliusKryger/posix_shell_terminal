#include <stdio.h>
#include <stdlib.h>
#include "builtins/history.h"
#include "utils.h"

// Handle the 'history' command
void handle_history()
{
    char *home = getenv("HOME");
    if (!home) return;

    char path[1024];
    snprintf(path, sizeof(path), "%s/.my_shell_history", home);

    FILE *file = fopen(path, "r");
    if (!file) {
        perror("fopen");
        return;
    }

    char line[1024];
    int index = 1;
    while (fgets(line, sizeof(line), file)) {
        printf("%4d  %s", index++, line);
    }

    fclose(file);
}
