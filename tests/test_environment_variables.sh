#!/bin/bash

# Test setting an environment variable
echo "export TEST_VAR=hello" | ./../bin/shell
output=$(echo "echo \$TEST_VAR" | ./../bin/shell)
if [ "$output" == "hello" ]; then
    echo "Test 1 Passed: Setting environment variable"
else
    echo "Test 1 Failed: Setting environment variable"
fi

# Test expanding an environment variable in a command
output=$(echo "echo \$TEST_VAR world" | ./../bin/shell)
if [ "$output" == "hello world" ]; then
    echo "Test 2 Passed: Expanding environment variable in command"
else
    echo "Test 2 Failed: Expanding environment variable in command"
fi

# Test displaying the value of an environment variable
output=$(echo "echo \$TEST_VAR" | ./../bin/shell)
if [ "$output" == "hello" ]; then
    echo "Test 3 Passed: Displaying environment variable"
else
    echo "Test 3 Failed: Displaying environment variable"
fi
