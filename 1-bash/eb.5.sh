#!/bin/bash

FILE_PATH="$1"

# Check if file exists
if [ ! -f "$FILE_PATH" ]; then
    echo "Error: File not found!"
    exit 1
fi

echo "Analyzing file: $FILE_PATH"

WORDS=$(cat "$FILE_PATH" | tr '[:upper:]' '[:lower:]' | tr -s ' ' '\n' | grep -v '^$')

# Count total and unique words
TOTAL_WORDS=$(echo "$WORDS" | wc -l)
UNIQUE_WORDS=$(echo "$WORDS" | sort | uniq | wc -l)

echo "Total words: $TOTAL_WORDS"
echo "Unique words: $UNIQUE_WORDS"

echo "Top 5 most common words:"
echo "$WORDS" | sort | uniq -c | sort -nr | head -5

# Count word categories without awk or sed or grep
SHORT=0
MEDIUM=0
LONG=0
i=0
for WORD in $WORDS; do
    if [[ $(( i % 1000 )) == 0 ]]; then
        echo "Processing word $i..."
    fi
    LENGTH=$(echo "$WORD" | wc -c)
    if [[ $LENGTH -le 3 ]]; then
        SHORT=$((SHORT + 1))
    elif [[ $LENGTH -ge 4 ]] && [[ $LENGTH -le 6 ]]; then
        MEDIUM=$((MEDIUM + 1))
    else
        LONG=$((LONG + 1))
    fi
    i=$((i + 1))
done

echo "Word categories:"
echo "  Short (≤3 chars): $SHORT"
echo "  Medium (4-6 chars): $MEDIUM"
echo "  Long (≥7 chars): $LONG"

echo "Word length histogram:"
printf "  Short: "
for i in $(seq 1 $SHORT); do
    printf "*"
done
printf "\n  Medium: "
for i in $(seq 1 $MEDIUM); do
    printf "*"
done
printf "\n  Long: "
for i in $(seq 1 $LONG); do
    printf "*"
done

echo "Analysis complete."