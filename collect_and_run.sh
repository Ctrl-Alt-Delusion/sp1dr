#!/bin/bash

# Change to the target directory
cd /Users/mustafasiddiqui/CAD/sp1dr

# Output file to store all content
OUTPUT_FILE="all_code.txt"

# Find and concatenate all .cpp and .hpp files recursively
find . -type f \( -name "*.cpp" -o -name "*.hpp" \) -exec cat {} + > "$OUTPUT_FILE"

echo "All contents have been stored in $OUTPUT_FILE"

# Compile and run the stored content if needed
#g++ "$OUTPUT_FILE" -o output_executable
#./output_executable

