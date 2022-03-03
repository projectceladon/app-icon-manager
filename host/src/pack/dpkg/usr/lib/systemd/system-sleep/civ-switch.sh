#!/bin/bash

case $1 in
    pre)
        /opt/cfc/mwc/bin/force_pause_civ.sh
        ;;
    post)
       /opt/cfc/mwc/bin/condition_resume_civ.sh
       ;;
esac
