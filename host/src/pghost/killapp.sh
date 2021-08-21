#!/bin/bash

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "3" ]];
then
    echo "Usage: killapp.sh <appname> <activity> <lg_instance_id>"
    exit 1
fi

adb shell am force-stop $1

exit 0

