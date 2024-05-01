#!/bin/bash

make > /dev/null 2>&1

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}PASS${NC}: $2"
    else
        echo -e "${RED}FAIL${NC}: $2"
    fi
}

test_failure() {
    ./ft_ping $1 > /dev/null 2>&1
    result=$?
    if [ $result -ne 1 ]; then
        print_result 1 "Expected failure for ft_ping $1, got $result"
    else
        print_result 0 "Expected failure for ft_ping $1"
    fi
}

test_success() {
    ./ft_ping $1 > /dev/null 2>&1
    result=$?
    # Expect exit status 0 for success
    if [ $result -ne 0 ]; then
        print_result 1 "Expected success for ft_ping $1, got $result"
    else
        print_result 0 "Expected success for ft_ping $1"
    fi
}

echo "Starting tests for ft_ping..."

echo "Testing incorrect usage scenarios..."
test_failure "-z"
test_failure "--bad-option"
test_failure ""

# Test for correct usage with different options
echo "Testing correct usage scenarios..."
test_success "-c 5 google.com"     # Valid count option
test_success "-w 500 google.com"   # Valid timeout option
test_success "-V"                  # Version information
test_success "-v -c 3 google.com"  # Verbose mode with count
test_success "--ttl 50 google.com" # Valid TTL option

echo "Tests complete."
