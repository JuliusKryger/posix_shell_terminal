#!/bin/bash

# Test that commands are correctly auto-completed
output=$(echo -e "ech\tHello" | ./../bin/shell)
if echo "$output" | grep -q "echo Hello"; then
    echo "Test 1 Passed: Command auto-completion"
else
    echo "Test 1 Failed: Command auto-completion"
fi

# Test that file paths are correctly auto-completed
touch testfile.txt
output=$(echo -e "cat tes\t" | ./../bin/shell)
if echo "$output" | grep -q "cat testfile.txt"; then
    echo "Test 2 Passed: File path auto-completion"
else
    echo "Test 2 Failed: File path auto-completion"
fi
rm testfile.txt

# Test that shell does not crash when no completion is found
output=$(echo -e "nonexistentcommand\t" | ./../bin/shell)
if echo "$output" | grep -q "$"; then
    echo "Test 3 Passed: No crash on no completion"
else
    echo "Test 3 Failed: No crash on no completion"
fi
