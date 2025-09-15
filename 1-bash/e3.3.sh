#!/bin/bash
find /var/log/ -type f -exec grep -il "error" {} \; > ~/error_files
cat ~/error_files | tr '\n' ':'
