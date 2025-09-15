#!/bin/bash
HOUR=$(date +%H)
echo "The current hour is $HOUR"

if [[ "$HOUR" == "0"* ]]; then
  echo "Hour starts with 0..."
  HOUR=$(echo $HOUR | cut -f 2 -d 0)
fi

if (( HOUR < 0 || HOUR > 23 )); then
  echo "Invalid hour"
  exit 1
fi

if (( HOUR >= 5 && HOUR < 12 )); then
  echo "Good morning!"
elif (( HOUR >= 12 && HOUR < 18 )); then
  echo "Good afternoon!"
elif (( HOUR >= 18 || HOUR < 5 )); then
  echo "Good night!"
else
  echo "Something went wrong..."
fi
