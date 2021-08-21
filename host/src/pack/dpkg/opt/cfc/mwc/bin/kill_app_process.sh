#!/bin/bash


NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "1" ]];
then
    echo "Usage: kill_app_process.sh <appname>"
    exit 1
fi

pid=`ps aux | grep "LG_B1_Client.*$2" | grep -v grep | awk '{print $2}'`

if [[ $pid > 0 ]];
then
    kill -KILL $pid
fi

exit 0

