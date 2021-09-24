#!/bin/bash

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "3" ]];
then
    echo "Usage: loadapp.sh <appname> <activity> <lg_instance_id>"
    exit 1
fi

adb shell am start -n $2 --display 0

num_lg_insts=`ps aux | grep LG_B1_Client | grep -v guestClipboard.*enable.*true | grep -v grep | wc -l`

if [[ "$num_lg_insts" -lt "1" ]];
then
    #/opt/lg/bin/LG_B1_Client -M yes -R 16666 -f /dev/shm/looking-glass0 -a true -t $1
    /opt/lg/bin/LG_B1_Client -M yes -R 16666 -f /dev/shm/looking-glass0 -a true -t "安卓应用"
    /opt/cfc/mwc/bin/msg_agent localhost 3000 CLOSEAPP_LASTOPENED
fi

exit 0

