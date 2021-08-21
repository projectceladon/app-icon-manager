#!/bin/bash

NUM_PARAMS=$#

if [[ "$NUM_PARAMS" -lt "2" ]];
then
    echo "Usage: closeapp.sh <appname> <packagename>"
    exit 1
fi

/opt/cfc/mwc/bin/msg_agent localhost 3000 CLOSEAPP $@

exit 0

