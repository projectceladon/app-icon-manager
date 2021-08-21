#!/bin/bash

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "2" ]];
then
    echo "Usage: lg_launcher.sh <appname> <activity>"
    exit 1
fi

/opt/cfc/mwc/bin/mwc_launcher localhost 3000 $@

exit 0

