#!/bin/bash
set -e

# Find all relevant files
files=$(find common javaparser javarunner stdlib stdlib_native -type f \( -name "*.c" -o -name "*.h" \))

# Apply clang-format
echo "$files" | xargs clang-format -i -style=file
