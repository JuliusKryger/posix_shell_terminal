# POSIX Shell Terminal

This is a simple POSIX shell terminal implemented in C. It supports basic shell commands such as `cd`, `pwd`, `echo`, and more.

## Directory Structure

```bash
/posix_shell_terminal
    /src                    # Source files
        /builtins           # Built-in commands implementation
            alias.c
            cd.c
            echo.c
            env.c
            pwd.c
            type.c
        builtins.c  
        command_execution.h
        history.h
        job_control.h
        main.c
        redirection.h
        tab_completion.h        
        utils.c
    /include                # Header files
        /builtins
            alias.h
            cd.h
            echo.h
            env.h
            pwd.h
            type.h
        builtins.h
        command_execution.h
        history.h
        job_control.h
        redirection.h
        tab_completion.h
        utils.h
    /bin                    # Compiled binaries
    /docs                   # Documentation
    /tests                  # Test cases
        #... All of the test cases resides in here
    .my_shellrc
    Makefile
    README.md
```


## Building the Project

To build the project, run:

```sh
make
```
## Running the Shell

To run the shell, execute:

```sh
make run
```

## Cleaning Up

To clean up the compiled binaries, run:

```sh
make clean
```#   p o s i x _ s h e l l _ t e r m i n a l  
 