#!/bin/bash

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "3" ]];
then
    echo "Usage: loadapp.sh <appname> <activity> <lg_instance_id>"
    exit 1
fi

adb -s vsock:3:5555 shell am start -n $2 --display $3

if [[ $"NUM_PARAMS" -ge "4" ]];
then
    /opt/lg/bin/LG_B1_Client -s false -M yes -R 16666 -f /dev/shm/looking-glass$3 -a true -t $4 -P $1
else
    /opt/lg/bin/LG_B1_Client -s false -M yes -R 16666 -f /dev/shm/looking-glass$3 -a true -t "安卓应用"
fi

exit 0

