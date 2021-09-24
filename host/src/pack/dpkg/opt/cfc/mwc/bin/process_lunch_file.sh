#!/bin/bash
cd ${HOME}/.local/share/applications/
lunch_files="*.desktop"
echo "Update icon file path of .desktop files..."
for f in $lunch_files
do
   #echo "Processing app lunch file: $f"
   sed -i "s%^Icon=~%Icon=${HOME}%g" $f
done
