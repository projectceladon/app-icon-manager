#!/bin/bash

NUM_PARAMS=$#
FOLDER_1_UPDATE='adb -s vsock:3:5555 shell am broadcast -a android.intent.action.MEDIA_SCANNER_SCAN_FILE -d file:///storage/emulated/0/Download'
FOLDER_2_UPDATE='adb -s vsock:3:5555 shell am broadcast -a android.intent.action.MEDIA_SCANNER_SCAN_FILE -d file:///storage/emulated/0/Pictures'

if [ "$NUM_PARAMS" -lt "2" ];
then
    echo "Usage: lg_launcher.sh <appname> <activity> [app icon label]"
    exit 1
fi

num_start_app=`ps aux | grep startapp | grep -v grep | wc -l`

if [ "$num_start_app" -lt "1" ];
then
    if ! systemctl --user is-active civ ;
    then
        /opt/lg/bin/startapp
    fi
    # Resume CiV
    /opt/cfc/mwc/bin/resume_civ.sh
else
    exit 1
fi

# Balloon Guest memory to 4096MB
if [ ! -z ${ENABLE_CIV_BALLOON} ]; then
    /opt/cfc/mwc/bin/balloon_guest.sh 4096
fi

num_lg_insts=`ps aux | grep LG_B1_Client.*looking-glass | grep -v guestClipboard.*enable.*true | grep -v grep | wc -l`
previous_app=""

if [ ! "$num_lg_insts" -lt "1" ];
then
    previous_app=`/opt/cfc/mwc/bin/msg_agent localhost 3000 GET_APP_LASTOPENED | grep "appname" | grep -v grep | sed 's/^appname: *\(.*\),.*$/\1/g'`
fi

$FOLDER_1_UPDATE
$FOLDER_2_UPDATE
/opt/cfc/mwc/bin/mwc_launcher localhost 3000 $@

echo "appname:$1, previous_app:$previous_app"

new_app="$1"

if [ ! -z "$previous_app" ];
then
    if [ "$new_app" != "$previous_app" ];
    then
        echo "Close previous opened app: $previous_app"
        /opt/cfc/mwc/bin/resume_civ.sh
        adb -s vsock:3:5555 shell am force-stop $previous_app
    else
        echo "Open same app, do nothing."
    fi
fi

# Balloon Guest memory to 2048MB if no mwc_launcher live
if [ ! -z ${ENABLE_CIV_BALLOON} ]; then
    if [ -z $(pidof mwc_launcher) ]; then
        /opt/cfc/mwc/bin/balloon_guest.sh 2048
    fi
fi

# Pause CiV
if [ -z "$(pidof mwc_launcher)" ] && [ -z "$(pidof LG_B1_Client)" ] && [ -z "$(pgrep -ax adb | grep -v "fork-server")" ]; then
    /opt/cfc/mwc/bin/pause_civ.sh
fi

exit 0

