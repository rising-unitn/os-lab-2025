#!/bin/bash

DIRECTORY="$1"
INTERVAL="$2"

if [[ ! -d "$DIRECTORY" ]]; then
    echo "Error: Not a directory!"
    exit 1
fi

if [[ -z "$INTERVAL" ]]; then
    echo "Error: No interval specified!"
    exit 1
fi

echo "Monitoring $DIRECTORY every $INTERVAL seconds. Press Ctrl+C to stop."

PREV_FILES=$(ls -la "$DIRECTORY")

while true; do
    sleep $INTERVAL
    
    echo "Checking at $(date)..."
    
    CURR_FILES=$(ls -la "$DIRECTORY")
    
    if [ "$PREV_FILES" != "$CURR_FILES" ]; then
        echo "Changes detected!"
        
        echo "New or modified files:"
        CURRENT_SET=$(ls "$DIRECTORY")
        for file in $CURRENT_SET; do
            if [ -f "$DIRECTORY/$file" ] && [ ! -f "/tmp/prev_$file" ]; then
                echo "+ $file ($(file -b "$DIRECTORY/$file"))"
            fi
        done
        
        echo "Possibly deleted files:"
        PREVIOUS_SET=$(ls /tmp/prev_*)
        for prev_file in $PREVIOUS_SET; do
            base_name=$(basename "$prev_file" | sed 's/^prev_//')
            if [ ! -f "$DIRECTORY/$base_name" ]; then
                echo "- $base_name"
            fi
        done
    else
        echo "No changes."
    fi
    
    rm -f /tmp/prev_*
    for file in $(ls "$DIRECTORY"); do
        if [ -f "$DIRECTORY/$file" ]; then
            touch "/tmp/prev_$file"
        fi
    done
    
    PREV_FILES="$CURR_FILES"
    echo "-----------------------------------"
done
