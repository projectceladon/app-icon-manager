#!/bin/sh -e

while true
do
    if pgrep -u `whoami` -x "ukui-session" > /dev/null
    then
        break
    else
        sleep 1
    fi
done

/bin/sh -e /etc/rc.civ start
/bin/bash /etc/sharefolder_monitor.sh &
sleep 10

/opt/lg/bin/LG_B1_Client_clipboard guestClipboard:enable=true

#Balloon guest memory to 2048M on boot
#/opt/cfc/mwc/bin/balloon_guest.sh 2048
exit 0
