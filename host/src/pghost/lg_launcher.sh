#!/bin/bash

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "2" ]];
then
    echo "Usage: lg_launcher.sh <appname> <activity> [app icon label]"
    exit 1
fi

# Balloon Guest memory to 4096MB
/opt/cfc/mwc/bin/balloon_guest.sh 4096

/opt/cfc/mwc/bin/mwc_launcher localhost 3000 $@

# Balloon Guest memory to 2048MB if no mwc_launcher live
if [ -z $(pidof mwc_launcher) ]; then
    /opt/cfc/mwc/bin/balloon_guest.sh 2048
fi

exit 0

