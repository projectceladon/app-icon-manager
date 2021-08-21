#!/bin/bash

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "3" ]];
then
    echo "Usage: loadapp.sh <appname> <activity> <lg_instance_id>"
    exit 1
fi

adb shell am start -n $2 --display $3
/opt/lg/bin/LG_B1_Client -M yes -R 16666 -f /dev/shm/looking-glass$3 -a true -t $1


exit 0

