#!/bin/bash

# Test that commands can be successfully piped
output=$(echo "echo Hello | grep H" | ./../bin/shell)
if echo "$output" | grep -q "Hello"; then
    echo "Test 1 Passed: Command piping"
else
    echo "Test 1 Failed: Command piping"
fi

# Test that the output of the first command is correctly passed as input to the second command
output=$(echo "echo Hello | grep H" | ./../bin/shell)
if echo "$output" | grep -q "Hello"; then
    echo "Test 2 Passed: Output of first command as input to second"
else
    echo "Test 2 Failed: Output of first command as input to second"
fi

# Test that chaining multiple commands with pipes works as expected
output=$(echo "echo Hello | grep H | tr 'H' 'J'" | ./../bin/shell)
if echo "$output" | grep -q "Jello"; then
    echo "Test 3 Passed: Chaining multiple commands"
else
    echo "Test 3 Failed: Chaining multiple commands"
fi
