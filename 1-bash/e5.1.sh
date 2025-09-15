#!/bin/bash

if [[ ! -d /tmp/backup ]]; then
  mkdir /tmp/backup
fi
# mkdir -p /tmp/backup

cd /etc || exit 1
# cd /etc; exitcode=$?; if [[ $exitcode

for file in $(ls); do
  if [[ -f $file && -r $file ]]; then
    if [[ -f /tmp/backup/$file ]]; then
      echo "Overwriting $file !!!"
    fi      
    cp -p "$file" /tmp/backup
    echo "Copied $file"
  fi
done
