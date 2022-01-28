#!/bin/bash

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "3" ]];
then
    echo "Usage: loadapp.sh <appname> <activity> <lg_instance_id>"
    exit 1
fi

# Get the number of running Android guest instances.
num_civ_insts=`ps aux | grep qemu.*android | grep -v grep | wc -l`

# Start the civ service if Android guest is not started yet.
if [ "$num_civ_insts" -lt "1" ];
then
    systemctl --user start civ
fi

/opt/cfc/mwc/bin/resume_civ.sh

num_adb_devices=`adb devices | grep vsock.*device | grep -v grep | wc -l`
if [[ "$num_adb_devices" -lt "1" ]];
then
    adb connect "vsock:3:5555"
fi

function wait_bootcomplete() {
  for i in {1..15}; do
    boot_complete=`adb -s vsock:3:5555 shell getprop dev.bootcomplete`
    if [[ "$boot_complete" -ge "1" ]];
    then
      echo "adb is $boot_complete when $i"
      cuptime=`adb shell cat /proc/uptime | cut -d "." -f 1`
      if [[ "$cuptime" -lt "15" ]];
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

function configure_wm_density()
{
    adb -s vsock:3:5555 shell wm density 440 -d 1
    adb -s vsock:3:5555 shell wm density 440 -d 2
    adb -s vsock:3:5555 shell wm density 440 -d 3
    adb -s vsock:3:5555 shell wm size 1200x1920 -d 1
    adb -s vsock:3:5555 shell wm size 1200x1920 -d 2
    adb -s vsock:3:5555 shell wm size 1200x1920 -d 3
}

configure_wm_density

adb -s vsock:3:5555 shell am start -W -S -n $2 --display $3 --activity-no-animation

if [[ $"NUM_PARAMS" -ge "4" ]];
then
    /opt/lg/bin/LG_B1_Client -s false -M yes -R 16666 -f /dev/shm/looking-glass$3 -a true -t $4 -P $1
else
    /opt/lg/bin/LG_B1_Client -s false -M yes -R 16666 -f /dev/shm/looking-glass$3 -a true -t "安卓应用"
fi

exit 0

