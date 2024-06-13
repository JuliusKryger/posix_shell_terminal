# Detailed Explanation of the POSIX Shell Terminal Implementation Project Structure

The project is structured as follows:

**Outdated**
```bash
/posix_shell_terminal
    /src                    # Source files
        main.c
        utils.c
        builtins.c          # Built-in commands implementation
    /include                # Header files
        utils.h
        builtins.h
    /bin                    # Compiled binaries
    /docs                   # Documentation
    /tests                  # Test cases
        test_cd.sh
    /examples               # Example scripts
    README.md
    Makefile
```
# Header Files
## include/utils.h

This header file declares utility functions used throughout the shell.

* is_executable(const char *path): Checks if a file exists and is executable.
* find_in_path(const char *command): Searches for a command in the system's PATH.
* parse_input(char *input, char **args, int *argc): Parses user input into arguments.
* execute_command(char **args): Executes a command by forking a child process.

## include/builtins.h

This header file declares functions for built-in shell commands.

* handle_cd(char **args): Handles the cd command.
* handle_pwd(): Handles the pwd command.
* handle_echo(char **args): Handles the echo command.
* handle_type(char **args): Handles the type command.


# Source Files
## src/utils.c

This file implements the utility functions declared in utils.h.

* is_executable(): Uses access() to check if the file at path is executable.
* find_in_path(): Searches each directory in the PATH environment variable for the given command.
* parse_input(): Splits the input string into arguments using strtok().
* execute_command(): Forks a child process to execute the command using execvp().

## src/builtins.c

This file implements the built-in shell commands declared in builtins.h.

* handle_cd(): Changes the current working directory using chdir(). Supports ~ for the home directory.
* handle_pwd(): Prints the current working directory using getcwd().
* handle_echo(): Prints the arguments to the terminal.
* handle_type(): Identifies if a command is built-in or an external command found in the PATH.

## src/main.c

This file contains the main loop of the shell, handling user input and executing commands.

* Prints the shell prompt ($ ).
* Reads user input using fgets().
* Removes the trailing newline character.
* Parses the input into arguments.
* Checks if the command is a built-in command and handles it accordingly.
* If the command is not built-in, it attempts to execute it as an external command.

## Makefile
The Makefile automates the build process for the shell.
* CC: Specifies the compiler (gcc).
* CFLAGS: Compiler flags for warnings and include paths.
* SRC: Source files.
* BIN: Output binary file.
* all: Default target to build the shell.
* clean: Removes the compiled binary.
* run: Builds and runs the shell.