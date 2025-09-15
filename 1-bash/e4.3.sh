#!/bin/bash
file=$1
    
if [[ ! -f "$file" ]]; then
    echo "Error: File not found!"
    exit 1
fi

echo "Analyzing file: $file"
echo "======================="

# Get basic counts
total_lines=$(wc -l < "$file")
total_words=$(wc -w < "$file")
total_chars=$(wc -m < "$file")

echo "Basic statistics:"
echo "  Lines: $total_lines"
echo "  Words: $total_words"
echo "  Characters: $total_chars"
echo

echo "Top 10 most common words:"
cat "$file" | tr -s ' ' '\n' | sort | uniq -c | sort -rn | head -10
