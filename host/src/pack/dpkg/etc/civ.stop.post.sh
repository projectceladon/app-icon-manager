#!/bin/sh -e

lg_pid=`ps aux | grep LG_B1_Client.*looking-glass | grep -v grep | awk '{print $2}'`

echo "Looking-glass client pid:$lg_pid"
if [ ! -z "$lg_pid" ];
then
    echo "Kill looking-glass client pid: $lg_pid"
    kill -KILL $lg_pid
fi

exit 0
