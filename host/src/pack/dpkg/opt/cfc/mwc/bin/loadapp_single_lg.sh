#!/bin/bash

# Get the number of running Android guest instances.
num_civ_insts=`ps aux | grep qemu.*android | grep -v grep | wc -l`

# Start the civ service if Android guest is not started yet.
if [ "$num_civ_insts" -lt "1" ];
then
    systemctl --user start civ
fi

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "3" ]];
then
    echo "Usage: loadapp.sh <appname> <activity> <lg_instance_id> [app icon label]"
    exit 1
fi

/opt/cfc/mwc/bin/resume_civ.sh

num_adb_devices=`adb devices | grep vsock.*device | grep -v grep | wc -l`
if [[ "$num_adb_devices" -lt "1" ]];
then
    adb connect "vsock:3:5555"
fi


function wait_bootcomplete() {
  for i in {1..12}; do
    boot_complete=`adb -s vsock:3:5555 shell getprop dev.bootcomplete`
    if [[ "$boot_complete" -ge "1" ]];
    then
      echo "adb is $boot_complete when $i"
      cuptime=`adb shell cat /proc/uptime | cut -d "." -f 1`
      if [[ "$cuptime" -lt "12" ]];
      then
        echo "adb is $cuptime when $i"
        sleep 1
      else
        adb -s vsock:3:5555 shell cat /proc/uptime
        break
      fi
    else
      sleep 1
    fi
  done
}

wait_bootcomplete
adb -s vsock:3:5555 shell am start -n $2 --display 0

num_lg_insts=`ps aux | grep LG_B1_Client.*looking-glass | grep -v guestClipboard.*enable.*true | grep -v grep | wc -l`

echo "App name: $1, Activity: $2, LG Instance ID: $3 Label: $4"

if [[ "$num_lg_insts" -lt "1" ]];
then
    #/opt/lg/bin/LG_B1_Client -M yes -R 16666 -f /dev/shm/looking-glass0 -a true -t $1
    if [[ "$NUM_PARAMS" -ge "4" ]];
    then
        /opt/lg/bin/LG_B1_Client -M yes -R 16666 -f /dev/shm/looking-glass0 -a true -t $4 -P $1
    else
        /opt/lg/bin/LG_B1_Client -M yes -R 16666 -f /dev/shm/looking-glass0 -a true -t "安卓应用"
    fi
    /opt/cfc/mwc/bin/msg_agent localhost 3000 CLOSEAPP_LASTOPENED
else
    /opt/lg/bin/LG_B1_Client -Q $1 -t $4
fi

exit 0

