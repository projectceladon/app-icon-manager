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

/opt/lg/bin/LG_B1_Client guestClipboard:enable=true 
exit 0
