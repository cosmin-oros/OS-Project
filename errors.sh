#!/bin/bash

# Check if the user passed in a file name
if [ $# -eq 0 ]; then
  echo "Usage: $0 <filename>"
  exit 1
fi

# Check if the file exists
if [ ! -f "$1" ]; then
  echo "File not found: $1"
  exit 1
fi

# Compile the file and count the errors and warnings
output=$(gcc -o /dev/null -Wall -Wextra -pedantic-errors "$1" 2>&1)
errors=$(echo "$output" | grep -c "error:")
warnings=$(echo "$output" | grep -c "warning:")

# Print the results
echo "$errors $warnings"

# Exit successfully
exit 0