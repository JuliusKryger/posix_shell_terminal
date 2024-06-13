#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "history.h"

#define HISTORY_FILE ".my_shell_history"
#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 1024

static char *history[MAX_HISTORY];
static int history_count = 0;
static int current_history_index = 0;
static struct termios orig_termios;

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_cflag |= (CS8);
    raw.c_oflag &= ~(OPOST);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void init_history()
{
    load_history();
}

void add_history(const char *command)
{
    if (history_count < MAX_HISTORY)
    {
        history[history_count++] = strdup(command);
    }
    else
    {
        free(history[0]);
        memmove(history, history + 1, (MAX_HISTORY - 1) * sizeof(char *));
        history[MAX_HISTORY - 1] = strdup(command);
    }
    save_history();
}

void save_history()
{
    char *home = getenv("HOME");
    if (!home) return;

    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", home, HISTORY_FILE);

    FILE *file = fopen(path, "w");
    if (!file) return;

    for (int i = 0; i < history_count; i++)
    {
        fprintf(file, "%s\n", history[i]);
    }
    fclose(file);
}

void load_history()
{
    char *home = getenv("HOME");
    if (!home) return;

    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", home, HISTORY_FILE);

    FILE *file = fopen(path, "r");
    if (!file) return;

    char line[MAX_COMMAND_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;
        add_history(line);
    }
    fclose(file);
}

void handle_up_arrow()
{
    if (current_history_index > 0)
    {
        current_history_index--;
        printf("\r$ %s", history[current_history_index]);
        fflush(stdout);
    }
}

void handle_down_arrow()
{
    if (current_history_index < history_count - 1)
    {
        current_history_index++;
        printf("\r$ %s", history[current_history_index]);
        fflush(stdout);
    }
}
