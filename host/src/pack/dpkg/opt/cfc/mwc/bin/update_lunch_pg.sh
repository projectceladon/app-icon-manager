#!/bin/bash

sfilename=$0

if [ "x${sfilename}" == "x-bash" ]; then
	sfilename=$BASE_SOURCE
fi

exec_pwd=$(cd $(dirname $sfilename);pwd)

echo "Start to update app .desktop file for Kylin OS ..."

#dir_desktop_app=${HOME}/.local/share/applications/
dir_desktop_app=${HOME}/applications

cd $dir_desktop_app
NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -eq "1" ]];
then
  echo "Update kylin launch icon for given app..."
  app_pkg=$1
  lunch_file="$1.desktop"
  if [[ -f $lunch_file ]];
  then
    echo "Processing app lunch file for kylin: $lunch_file"
    sed -i "s%^Exec=\/%Exec=sh -- \/%g" $lunch_file
    echo "Copy app .desktop file: $lunch_file to /usr/share/applications/ folder."
    #sudo cp "$lunch_file" /usr/share/applications/
    cd -
    echo "Start to update desktop app list for Kylin OS ..."
    desktop_applist_file=${HOME}/.config/ukui/desktop_applist
    desktop_menu_file=${HOME}/.config/ukui/ukui-menu.ini
    cat $desktop_applist_file | grep -q $lunch_file
    if [ "1" == $? ]
    then
      echo "Append the app lunch icon to kylin app luancher"
      echo $lunch_file >>$desktop_applist_file
    fi
    # Add the .desktop file into ukui-menu.ini
    $exec_pwd/update_pg_menu.py $lunch_file $desktop_menu_file
  fi
else
  echo "Update kylin luanch icons in batch mode..."
  lunch_files="`ls *.desktop`"
  for app_desktop_file in $lunch_files
  do
     echo "Processing app lunch file for kylin: $app_desktop_file"
     sed -i "s%^Exec=\/%Exec=sh -- \/%g" $app_desktop_file

  done
  echo "Copy app .desktop files to /usr/share/applications/ folder..."
  #sudo cp *.desktop /usr/share/applications/
  cd -

  echo "Start to update desktop app list for Kylin OS ..."
  desktop_applist_file=${HOME}/.config/ukui/desktop_applist
  desktop_menu_file=${HOME}/.config/ukui/ukui-menu.ini

  for app_desktop_file in $lunch_files
  do
     cat $desktop_applist_file | grep -q $app_desktop_file
     if [ "1" == $? ]
     then
	   echo "Append the app lunch icon to kylin app luancher"
	   echo $app_desktop_file >>$desktop_applist_file
     fi
     # Add the .desktop file into ukui-menu.ini
     $exec_pwd/update_pg_menu.py $app_desktop_file $desktop_menu_file
  done
fi

echo "Android app lunch icon updating done."
