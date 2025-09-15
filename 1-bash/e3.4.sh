#!/bin/bash
NEWCHAR=$3
OLDCHAR=$2
FILE=$1
cat $FILE | tr $OLDCHAR $NEWCHAR > $FILE.new
CHANGE="The character $OLDCHAR was replaced by the character $NEWCHAR in the file $FILE"
export CHANGE
echo $CHANGE  # Print the change message to the screen
