#!/bin/bash

logfile=$1

# Check if file exists
if [[ ! -f "$logfile" ]]; then
    echo "Error: Log file not found!"
    exit 1
fi

# Initialize counters
errors=0
warnings=0
infos=0

echo "Analyzing log file: $logfile"
echo "--------------------------"

while read -r line; do
    if [[ "$line" == *"ERROR"* ]]; then
        ((errors++))
    elif [[ "$line" == *"WARNING"* ]]; then
        ((warnings++))
    elif [[ "$line" == *"INFO"* ]]; then
        ((infos++))
    fi
done < "$logfile"

echo "Results:"
echo "  Errors: $errors"
echo "  Warnings: $warnings"
echo "  Infos: $infos"
echo "--------------------------"

# Determine log severity
if (( errors > 10 )); then
    echo "Critical: Too many errors detected!"
fi

if (( warnings > 20 )); then
    echo "Warning: High number of warnings!"
fi