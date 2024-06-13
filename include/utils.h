#ifndef UTILS_H
#define UTILS_H

int is_executable(const char *path);
char *find_in_path(const char *command);
void parse_input(char *input, char **args, int *argc);

#endif // UTILS_H
