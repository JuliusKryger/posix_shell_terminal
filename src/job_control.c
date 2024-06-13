#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "job_control.h"

#define MAX_JOBS 100

typedef struct {
    pid_t pid;
    int bg;
    char cmd[256];
} Job;

static Job jobs[MAX_JOBS];
static int job_count = 0;
static pid_t current_fg_pid = -1;

void add_job(pid_t pid, int bg, char *cmd) {
    if (job_count < MAX_JOBS) {
        jobs[job_count].pid = pid;
        jobs[job_count].bg = bg;
        strncpy(jobs[job_count].cmd, cmd, 256);
        job_count++;
    }
}

void remove_job(pid_t pid) {
    for (int i = 0; i < job_count; i++) {
        if (jobs[i].pid == pid) {
            for (int j = i; j < job_count - 1; j++) {
                jobs[j] = jobs[j + 1];
            }
            job_count--;
            break;
        }
    }
}

void list_jobs() {
    for (int i = 0; i < job_count; i++) {
        printf("[%d] %s %s\n", i + 1, jobs[i].bg ? "Running" : "Stopped", jobs[i].cmd);
    }
}

void bring_job_to_foreground(int job_num) {
    if (job_num > 0 && job_num <= job_count) {
        int status;
        pid_t pid = jobs[job_num - 1].pid;
        current_fg_pid = pid;
        tcsetpgrp(STDIN_FILENO, pid);
        kill(pid, SIGCONT);
        waitpid(pid, &status, WUNTRACED);
        tcsetpgrp(STDIN_FILENO, getpid());
        current_fg_pid = -1;
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            remove_job(pid);
        }
    } else {
        fprintf(stderr, "fg: %d: no such job\n", job_num);
    }
}

void send_job_to_background(int job_num) {
    if (job_num > 0 && job_num <= job_count) {
        pid_t pid = jobs[job_num - 1].pid;
        kill(pid, SIGCONT);
        jobs[job_num - 1].bg = 1;
    } else {
        fprintf(stderr, "bg: %d: no such job\n", job_num);
    }
}

void kill_job(int job_num) {
    if (job_num > 0 && job_num <= job_count) {
        pid_t pid = jobs[job_num - 1].pid;
        kill(pid, SIGKILL);
        remove_job(pid);
    } else {
        fprintf(stderr, "kill: %d: no such job\n", job_num);
    }
}

void handle_job_control(char **args, int *bg) {
    if (strcmp(args[0], "fg") == 0) {
        if (args[1]) {
            bring_job_to_foreground(atoi(args[1]));
        } else {
            fprintf(stderr, "fg: job number required\n");
        }
    } else if (strcmp(args[0], "bg") == 0) {
        if (args[1]) {
            send_job_to_background(atoi(args[1]));
        } else {
            fprintf(stderr, "bg: job number required\n");
        }
    } else if (strcmp(args[0], "kill") == 0) {
        if (args[1]) {
            kill_job(atoi(args[1]));
        } else {
            fprintf(stderr, "kill: job number required\n");
        }
    } else if (strcmp(args[0], "jobs") == 0) {
        list_jobs();
    } else if (strcmp(args[0], "&") == 0) {
        *bg = 1;
    }
}

void handle_sigint(int sig) {
    if (current_fg_pid != -1) {
        kill(current_fg_pid, SIGINT);
    }
}

void handle_sigtstp(int sig) {
    if (current_fg_pid != -1) {
        kill(current_fg_pid, SIGTSTP);
    }
}

void setup_signal_handlers() {
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = handle_sigtstp;
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}
