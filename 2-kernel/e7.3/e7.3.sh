#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <PID>"
    exit 1
fi

TARGET_PID=$1

if ! ps -p "$TARGET_PID" >/dev/null; then
    echo "Error: Process with PID $TARGET_PID does not exist."
    exit 1
fi

echo "Sending signals to PID: $TARGET_PID"

echo "Sending SIGUSR1 five times..."
for i in {1..5}; do
    echo "Sending SIGUSR1 ($i/5)..."
    kill -SIGUSR1 "$TARGET_PID"
    if [[ $? -ne 0 ]]; then
        echo "Error sending SIGUSR1. Process might have terminated."
        exit 1
    fi
    sleep 1
done

echo "Sending SIGUSR2..."
kill -SIGUSR2 "$TARGET_PID"
if [[ $? -ne 0 ]]; then
    echo "Error sending SIGUSR2. Process might have terminated."
    exit 1
fi

echo "Waiting for 3 seconds..."
sleep 3

echo "Sending SIGTERM..."
kill -SIGTERM "$TARGET_PID"
if [[ $? -ne 0 ]]; then
    echo "Attempted to send SIGTERM. Process might have already terminated."
else
    echo "SIGTERM sent successfully."
fi
