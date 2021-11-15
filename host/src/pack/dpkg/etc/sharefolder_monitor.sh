#!/bin/bash

MONITOR_FOLDER_1=$HOME"/Android/Download"
MONITOR_FOLDER_2=$HOME"/Android/Pictures"

FOLDER_1_UPDATE='adb -s vsock:3:5555 shell am broadcast -a android.intent.action.MEDIA_SCANNER_SCAN_FILE -d file:///storage/emulated/0/Download'
FOLDER_2_UPDATE='adb -s vsock:3:5555 shell am broadcast -a android.intent.action.MEDIA_SCANNER_SCAN_FILE -d file:///storage/emulated/0/Pictures'


function folder_notify() {
    inotifywait -rm -e create -e delete -e move $MONITOR_FOLDER_1 $MONITOR_FOLDER_2
}

function check_all_when_first_run() {
    chmod -R o+rx $MONITOR_FOLDER_1
    chmod -R o+rx $MONITOR_FOLDER_2
    $FOLDER_1_UPDATE
    $FOLDER_2_UPDATE
}

check_all_when_first_run

folder_notify | while read -r line
do
    echo $line
    if [[ "$line" =~ "CREATE" ]] || [[ "$line" =~ "MOVED_TO" ]];then
        chmod -R o+rx $MONITOR_FOLDER_1
        chmod -R o+rx $MONITOR_FOLDER_2
    fi

    $FOLDER_1_UPDATE < /dev/null
    $FOLDER_2_UPDATE < /dev/null

done

