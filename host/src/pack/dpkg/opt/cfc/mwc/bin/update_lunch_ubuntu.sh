#!/bin/bash

sfilename=$0

if [ "x${sfilename}" == "x-bash" ]; then
	sfilename=$BASE_SOURCE
fi

exec_pwd=$(cd $(dirname $sfilename);pwd)

echo "Start to update app .desktop file for Ubuntu ..."

#dir_desktop_app=${HOME}/.local/share/applications/
dir_desktop_app=${HOME}/applications

cd $dir_desktop_app
NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -eq "1" ]];
then
  echo "Update ubuntu launch icon for given app..."
  app_pkg=$1
  lunch_file="$1.desktop"
  if [[ -f $lunch_file ]];
  then
    echo "Processing app lunch file for kylin: $lunch_file"
    sed -i "s%^Name\[zh_CN\]=%Name=%g" $lunch_file
    echo "Copy app .desktop file: $lunch_file to /usr/share/applications/ folder."
    cp "$lunch_file" ${HOME}/.local/applications/
    cd -
  fi
else
  echo "Update Ubuntu luanch icons in batch mode..."
  lunch_files="`ls *.desktop`"
  for app_desktop_file in $lunch_files
  do
     echo "Processing app lunch file for kylin: $app_desktop_file"
     sed -i "s%^Name\[zh_CN\]=%Name=%g" $app_desktop_file

  done
  echo "Copy app .desktop files to ~/.local/share/applications/ folder..."
  cp *.desktop ${HOME}/.local/share/applications/
  cd -
fi

echo "Android app lunch icon updating done."
