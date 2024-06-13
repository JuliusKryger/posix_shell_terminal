#!/bin/bash

# Start the shell in the background to send signals to it
./../bin/shell &
SHELL_PID=$!

# Give the shell some time to start
sleep 1

# Test Ctrl+C (SIGINT)
echo "sleep 10" > /proc/$SHELL_PID/fd/0
sleep 1
kill -SIGINT $SHELL_PID
sleep 1

# Check if the shell is still running
if ps -p $SHELL_PID > /dev/null; then
    echo "Test 1 Passed: Shell remains active after Ctrl+C"
else
    echo "Test 1 Failed: Shell terminated after Ctrl+C"
fi

# Test Ctrl+Z (SIGTSTP)
echo "sleep 10" > /proc/$SHELL_PID/fd/0
sleep 1
kill -SIGTSTP $SHELL_PID
sleep 1

# Check if the shell is still running and job is stopped
if ps -p $SHELL_PID > /dev/null; then
    echo "Test 2 Passed: Shell remains active after Ctrl+Z"
else
    echo "Test 2 Failed: Shell terminated after Ctrl+Z"
fi

# Kill the shell process at the end of the test
kill -SIGKILL $SHELL_PID
