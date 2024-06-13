#!/bin/bash

# Test running processes in the background
echo "sleep 5 &" | ./../bin/shell
jobs_output=$(echo "jobs" | ./../bin/shell)
if echo "$jobs_output" | grep -q "sleep 5"; then
    echo "Test 1 Passed: Background process"
else
    echo "Test 1 Failed: Background process"
fi

# Test bringing a job to the foreground
echo "sleep 10 &" | ./../bin/shell
job_num=$(echo "jobs" | ./../bin/shell | grep "sleep 10" | awk '{print $1}' | tr -d '[]')
fg_output=$(echo "fg $job_num" | ./../bin/shell)
if echo "$fg_output" | grep -q "sleep 10"; then
    echo "Test 2 Passed: Foreground job"
else
    echo "Test 2 Failed: Foreground job"
fi

# Test sending a job to the background
echo "sleep 15" | ./../bin/shell &
sleep 1
bg_output=$(echo "bg %1" | ./../bin/shell)
if echo "$bg_output" | grep -q "sleep 15"; then
    echo "Test 3 Passed: Background job"
else
    echo "Test 3 Failed: Background job"
fi

# Test killing a job
echo "sleep 20 &" | ./../bin/shell
job_num=$(echo "jobs" | ./../bin/shell | grep "sleep 20" | awk '{print $1}' | tr -d '[]')
kill_output=$(echo "kill $job_num" | ./../bin/shell)
jobs_output=$(echo "jobs" | ./../bin/shell)
if ! echo "$jobs_output" | grep -q "sleep 20"; then
    echo "Test 4 Passed: Kill job"
else
    echo "Test 4 Failed: Kill job"
fi