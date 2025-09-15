#!/bin/bash

TASKS_FILE="tasks.txt"

if [[ "$1" = "add" ]]; then
    if [[ -z "$2" ]]; then
        echo "Please provide a task description."
        exit 1
    fi
    echo "$2" >> "$TASKS_FILE"
    echo "Task added."
    
elif [[ "$1" = "list" ]]; then
    if [ -f "$TASKS_FILE" ]; then
        cat -n "$TASKS_FILE"
    else
        echo "No tasks found."
    fi
    
elif [[ "$1" = "done" ]]; then
    if [ -f "$TASKS_FILE" ]; then
        if [[ -z "$2" ]]; then
            echo "Please provide a task number."
            exit 1
        fi
        sed -i "${2}s/^/[DONE] /" "$TASKS_FILE"
        echo "Task $2 marked as done."
    else
        echo "No tasks file found."
    fi
    
elif [[ "$1" = "remove" ]]; then
    if [ -f "$TASKS_FILE" ]; then
        sed -i "${2}d" "$TASKS_FILE"
        echo "Task $2 removed."
    else
        echo "No tasks file found."
    fi
    
elif [[ "$1" = "clear" ]]; then
    rm -f "$TASKS_FILE"
    echo "All tasks cleared."
    
else
    echo "Usage: $0 add|list|done|remove|clear"
fi
