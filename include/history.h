#ifndef HISTORY_H
#define HISTORY_H

void init_history();
void add_history(const char *command);
void save_history();
void load_history();
void handle_up_arrow();
void handle_down_arrow();

#endif // HISTORY_H
