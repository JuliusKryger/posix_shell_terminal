#!/bin/bash

# Test output redirection
echo "echo Hello > output.txt" | ./../bin/shell
if [ "$(cat output.txt)" == "Hello" ]; then
    echo "Test 1 Passed: Output redirection"
else
    echo "Test 1 Failed: Output redirection"
fi
rm output.txt

# Test input redirection
echo "Hello" > input.txt
output=$(echo "cat < input.txt" | ./../bin/shell)
if [ "$output" == "Hello" ]; then
    echo "Test 2 Passed: Input redirection"
else
    echo "Test 2 Failed: Input redirection"
fi
rm input.txt

# Test appending output
echo "echo Hello > output.txt" | ./../bin/shell
echo "echo World >> output.txt" | ./../bin/shell
if [ "$(cat output.txt)" == "Hello
World" ]; then
    echo "Test 3 Passed: Appending output"
else
    echo "Test 3 Failed: Appending output"
fi
rm output.txt
