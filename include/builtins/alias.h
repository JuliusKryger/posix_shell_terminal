#ifndef BUILTINS_ALIAS_H
#define BUILTINS_ALIAS_H

void handle_alias(char **args);
void handle_unalias(char **args);
void expand_aliases(char **args, int *argc);

#endif // BUILTINS_ALIAS_H
