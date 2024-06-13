#ifndef COMMAND_EXECUTION_H
#define COMMAND_EXECUTION_H

void execute_command(char **args);
void execute_pipeline(char *input);
void handle_job_control(char **args, int *bg);

#endif // COMMAND_EXECUTION_H
