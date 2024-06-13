#!/bin/bash

# Test defining an alias
output=$(echo "alias ll='ls -l'" | ./../bin/shell)
output=$(echo "ll" | ./../bin/shell)
if echo "$output" | grep -q "total"; then
    echo "Test 1 Passed: Alias defined and used"
else
    echo "Test 1 Failed: Alias defined and used"
fi

# Test removing an alias
echo "unalias ll" | ./../bin/shell
output=$(echo "ll" | ./../bin/shell 2>&1)
if echo "$output" | grep -q "command not found"; then
    echo "Test 2 Passed: Alias removed"
else
    echo "Test 2 Failed: Alias removed"
fi

# Test alias expansion
echo "alias test_alias='echo This is a test'" | ./../bin/shell
output=$(echo "test_alias" | ./../bin/shell)
if echo "$output" | grep -q "This is a test"; then
    echo "Test 3 Passed: Alias expanded correctly"
else
    echo "Test 3 Failed: Alias expanded correctly"
fi
