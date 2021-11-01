#!/bin/bash

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "3" ]];
then
    echo "Usage: loadapp.sh <appname> <activity> <lg_instance_id> [app icon label]"
    exit 1
fi

num_adb_devices=`adb devices | grep vsock.*device | grep -v grep | wc -l`
if [[ "$num_adb_devices" -lt "1" ]];
then
    adb connect "vsock:3:5555"
fi


adb -s vsock:3:5555 shell am start -n $2 --display 0

num_lg_insts=`ps aux | grep LG_B1_Client.*looking-glass | grep -v guestClipboard.*enable.*true | grep -v grep | wc -l`

echo "App name: $1, Activity: $2, LG Instance ID: $3 Label: $4"

if [[ "$num_lg_insts" -lt "1" ]];
then
    #/opt/lg/bin/LG_B1_Client -M yes -R 16666 -f /dev/shm/looking-glass0 -a true -t $1
    if [[ "$NUM_PARAMS" -ge "4" ]];
    then
        /opt/lg/bin/LG_B1_Client -M yes -R 16666 -f /dev/shm/looking-glass0 -a true -t $4
    else
        /opt/lg/bin/LG_B1_Client -M yes -R 16666 -f /dev/shm/looking-glass0 -a true -t "安卓应用"
    fi
    /opt/cfc/mwc/bin/msg_agent localhost 3000 CLOSEAPP_LASTOPENED
fi

exit 0

