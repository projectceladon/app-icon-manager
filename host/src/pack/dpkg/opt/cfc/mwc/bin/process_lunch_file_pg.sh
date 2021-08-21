#!/bin/bash
cd ${HOME}/applications/
lunch_files="*.desktop"
for f in $lunch_files
do
   echo "Processing app lunch file: $f"
   sed -i "s%^Icon=~/.local/share/%Icon=${HOME}/%g" $f
done
