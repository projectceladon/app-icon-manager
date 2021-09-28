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
sleep 10

/opt/lg/bin/LG_B1_Client guestClipboard:enable=true
exit 0
