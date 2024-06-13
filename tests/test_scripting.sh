#!/bin/bash

# Create a test script
cat << 'EOF' > test_script.sh
#!/bin/bash
export TEST_VAR=hello
echo $TEST_VAR
for i in 1 2 3
do
  echo "Number $i"
done
if [ "$TEST_VAR" == "hello" ]; then
  echo "Test passed"
else
  echo "Test failed"
fi
EOF

# Make the script executable
chmod +x test_script.sh

# Execute the script and capture the output
output=$(./../bin/shell test_script.sh)

# Check the output
if echo "$output" | grep -q "hello"; then
    echo "Test 1 Passed: Variable set and expanded correctly"
else
    echo "Test 1 Failed: Variable set and expanded correctly"
fi

if echo "$output" | grep -q "Number 1" && echo "$output" | grep -q "Number 2" && echo "$output" | grep -q "Number 3"; then
    echo "Test 2 Passed: Loop executed correctly"
else
    echo "Test 2 Failed: Loop executed correctly"
fi

if echo "$output" | grep -q "Test passed"; then
    echo "Test 3 Passed: Conditional executed correctly"
else
    echo "Test 3 Failed: Conditional executed correctly"
fi

# Clean up
rm test_script.sh
