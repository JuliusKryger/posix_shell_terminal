#!/bin/bash

# Test that commands are saved to history
echo "echo Hello" | ./../bin/shell
echo "history" | ./../bin/shell | grep -q "echo Hello" && echo "Test 1 Passed" || echo "Test 1 Failed"

# Test that history navigation works with up arrow
echo -e "echo First\n\033[A" | ./../bin/shell | grep -q "First" && echo "Test 2 Passed" || echo "Test 2 Failed"

# Test that history command displays the list of commands
echo "echo Second" | ./../bin/shell
echo "history" | ./../bin/shell | grep -q "echo Second" && echo "Test 3 Passed" || echo "Test 3 Failed"
