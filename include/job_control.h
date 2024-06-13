#ifndef JOB_CONTROL_H
#define JOB_CONTROL_H

void add_job(pid_t pid, int bg, char *cmd);
void remove_job(pid_t pid);
void list_jobs();
void bring_job_to_foreground(int job_num);
void send_job_to_background(int job_num);
void kill_job(int job_num);
void handle_job_control(char **args, int *bg);
void setup_signal_handlers();
void handle_sigint(int sig);
void handle_sigtstp(int sig);

#endif // JOB_CONTROL_H
