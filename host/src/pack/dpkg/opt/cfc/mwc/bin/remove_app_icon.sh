#!/bin/bash

sfilename=$0

if [ "x${sfilename}" == "x-bash" ]; then
	sfilename=$BASE_SOURCE
fi

exec_pwd=$(cd $(dirname $sfilename);pwd)

echo "Start to remove app .desktop file for Kylin OS ..."

dir_desktop_app=${HOME}/applications

cd $dir_desktop_app
NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -eq "1" ]];
then
  echo "Remove kylin launch icon for given app..."
  app_pkg=$1
  lunch_file="$1.desktop"
  if [[ -f $lunch_file ]];
  then
    echo "Processing app lunch file for kylin: $lunch_file"
    app_launch_file="/usr/share/applications/$lunch_file"
    if [[ -f $app_lunch_file ]];
    then
        sudo rm $app_lunch_file
    fi

    cd -
    echo "Start to update desktop app list for Kylin OS ..."
    desktop_applist_file=${HOME}/.config/ukui/desktop_applist
    desktop_menu_file=${HOME}/.config/ukui/ukui-menu.ini
    sed -i "\%$lunch_file%d" $desktop_applist_file
    sed -i "\%$lunch_file%d" $desktop_menu_file
  fi
else
  echo "Please specify the app package name to be removed!"
fi

echo "Android app lunch icon removal done."
