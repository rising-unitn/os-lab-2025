#!/bin/bash

dir="$1"
count=0

list_files() {
    for item in "$1"/*; do
        if [ -d "$item" ]; then
            echo "$item"
            count=$((count + 1))
            list_files "$item"
        elif [ -f "$item" ]; then
            count=$((count + 1))
        fi
    done
}

list_files "$dir"
echo "Total files and directories found: $count"