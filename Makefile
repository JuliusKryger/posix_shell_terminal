CC = gcc
CFLAGS = -Wall -Iinclude
SRC_DIR = src
BUILTINS_DIR = src/builtins
INCLUDE_DIR = include

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/utils.c \
       $(SRC_DIR)/builtins.c \
       $(BUILTINS_DIR)/alias.c \
       $(BUILTINS_DIR)/cd.c \
       $(BUILTINS_DIR)/echo.c \
       $(BUILTINS_DIR)/env.c \
       $(BUILTINS_DIR)/pwd.c \
       $(BUILTINS_DIR)/type.c

OBJS = $(SRCS:.c=.o)
BIN = bin/shell

TESTS = tests/test_cd.sh \
		tests/test_history.sh \
		tests/test_tab_completion.sh \
        tests/test_pipeline.sh \
		tests/test_redirection.sh \
		tests/test_job_control.sh \
        tests/test_signal_handling.sh \
		tests/test_environment_variables.sh \
        tests/test_scripting.sh \
		tests/test_alias.sh \
		tests/test_config_file.sh \

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BIN)

test: all
	@for test in $(TESTS); do \
		/bin/bash $$test; \
	done
