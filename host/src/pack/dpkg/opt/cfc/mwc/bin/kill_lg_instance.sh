#!/bin/bash


NUM_PARAMS=$#

if [ "$NUM_PARAMS" -lt "1" ]
then
    echo "Usage: kill_lg_instance.sh <ld_instance_id>"
    exit 1
fi

if [ "$1" -eq "-1" ]
then
   echo "Single LG mode, killing the running LG_B1_Client"
   pid=`ps aux | grep "LG_B1_Client " | grep -v grep | awk '{print $2}'`
else
   inst="looking-glass${1}"
   pid=`ps aux | grep "LG_B1_Client " | grep -v grep | grep $inst | awk '{print $2}'`
fi

if [ ! -z "$pid" ] && [ "$pid" -gt 0 ]
then
    echo  "inside kill"	
    kill -KILL $pid
fi

exit 0

