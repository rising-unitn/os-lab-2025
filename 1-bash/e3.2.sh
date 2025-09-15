#!/bin/bash
cd $1
find . -maxdepth 2 -size -4M -type f -mtime -10 -exec grep -il "error" {} \; | sort -r | uniq
