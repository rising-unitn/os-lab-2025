#!/bin/bash
if [ $# -eq 0 ]; then
  echo "Remember to input the path"
  exit 1
fi

folder=$1

# optional
# if [[ ! -d $folder ]]; then
#   echo "$folder is not a directory"
#   exit 1
# fi

sh_current_folder=$(find "$folder" -maxdepth 1 -type f -name "*.sh" | wc -l)
sh_parent_folder=$(find "$folder/.." -maxdepth 1 -type f -name "*.sh" | wc -l)

if [[ $sh_current_folder -gt 0 || $sh_parent_folder -gt 0 ]]; then
  echo "Unsecure: shell scripts are here"
  exit 1
fi

cry_files=$(find "$folder" -maxdepth 1 -type f -exec grep "crypto" {} \; | wc -l)
no_ext_files=$(find "$folder" -maxdepth 1 -type f -exec basename {} \; | grep -v "\." | wc -l)

if [[ $cry_files -gt 0 || $no_ext_files -gt 0 ]]; then
  echo "Unsecure: suspicious files are here"
else
  echo "Secure!"
fi
