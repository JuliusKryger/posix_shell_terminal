#!/bin/bash

# Create a default configuration file
cat << 'EOF' > ~/.my_shellrc
export TEST_VAR=config_test
alias ll='ls -l'
EOF

# Start a new shell session and capture the output of commands
output=$(echo -e "echo \$TEST_VAR\nll" | ./../bin/shell)

# Check if the environment variable is set
if echo "$output" | grep -q "config_test"; then
    echo "Test 1 Passed: Environment variable from config file"
else
    echo "Test 1 Failed: Environment variable from config file"
fi

# Check if the alias is set
if echo "$output" | grep -q "total"; then
    echo "Test 2 Passed: Alias from config file"
else
    echo "Test 2 Failed: Alias from config file"
fi

# Clean up
rm ~/.my_shellrc
