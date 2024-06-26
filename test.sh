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
test_failure "192.168.300.1"  # Invalid IP address

test_failure "-c error google.es"
test_failure "-w error google.es"

echo "Testing correct usage scenarios..."
test_success "-V"                           # Version information
test_success "-V argument another_arg"      # Version information
test_success "-c 1 google.com"              # Valid count option
test_success "-c 1 127.0.0.1"       # Valid timeout option
test_success "-c 1 localhost"       # Valid timeout option
test_success "-v -c 1 google.com"           # Verbose mode with count
test_success "--ttl 50 -c1 google.com"      # Valid TTL option
test_success "-c 1 --ttl 1 google.com"      # Valid TTL option
test_success "-c 2 localhost"         # Valid TTL option
test_success "-c 1 google.com google.es"    # Multiple hosts

echo "Testing special cases..."
test_success "-c 1 8.8.8.8"                    # Google DNS
test_success "-c 1 8.8.4.4"                    # Google DNS secondary
test_failure "-c 1 256.256.256.256"            # Invalid IP
test_success "--ttl 255 -c 1 8.8.8.8"          # Maximum TTL
test_failure "--ttl 256 -c 1 8.8.8.8"          # TTL too high
test_success "--ttl 1 -c 1 1.1.1.1"            # Minimum TTL

echo "Tests complete."
