#!/bin/bash

NUM_PARAMS=$#

if [ "$NUM_PARAMS" -lt "2" ];
then
    echo "Usage: lg_launcher.sh <appname> <activity> [app icon label]"
    exit 1
fi

num_start_app=`ps aux | grep startapp | grep -v grep | wc -l`

if [ "$num_start_app" -lt "1" ];
then
    if [ "$(adb get-state | grep "device" | awk '{print $1}')" != "device" ];
    then
        /opt/lg/bin/startapp
    fi
else
    exit 1
fi

# Balloon Guest memory to 4096MB
# /opt/cfc/mwc/bin/balloon_guest.sh 4096

num_lg_insts=`ps aux | grep LG_B1_Client.*looking-glass | grep -v guestClipboard.*enable.*true | grep -v grep | wc -l`
previous_app=""

if [ ! "$num_lg_insts" -lt "1" ];
then
    previous_app=`/opt/cfc/mwc/bin/msg_agent localhost 3000 GET_APP_LASTOPENED | grep "appname" | grep -v grep | sed 's/^appname: *\(.*\),.*$/\1/g'`
fi

/opt/cfc/mwc/bin/mwc_launcher localhost 3000 $@

echo "appname:$1, previous_app:$previous_app"

new_app="$1"

if [ ! -z "$previous_app" ];
then
    if [ "$new_app" != "$previous_app" ];
    then
        echo "Close previous opened app: $previous_app"
        adb -s vsock:3:5555 shell am force-stop $previous_app
    else
        echo "Open same app, do nothing."
    fi
fi

# Balloon Guest memory to 2048MB if no mwc_launcher live
#if [ -z $(pidof mwc_launcher) ]; then
#    /opt/cfc/mwc/bin/balloon_guest.sh 2048
#fi

exit 0

