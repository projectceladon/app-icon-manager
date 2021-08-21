#!/bin/bash
cd ${HOME}/.local/share/applications/
lunch_files="*.desktop"
for f in $lunch_files
do
   echo "Processing app lunch file: $f"
   sed -i "s%^Icon=~%Icon=${HOME}%g" $f
done
