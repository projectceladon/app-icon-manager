#!/bin/bash

if [[ -f ../pghost/mwc ]];
then
    cp ../pghost/mwc dpkg/opt/cfc/mwc/bin/mwc
    echo "Copied mwc file for debian file packaging."
else
    echo "mwc not found, please execute the source compiling first!"
fi

if [[ -f ../pghost/mwc_hostdaemon ]];
then
    cp ../pghost/mwc_hostdaemon dpkg/opt/cfc/mwc/bin/mwc_hostdaemon
    echo "Copied mwc_hostdaemon file for debian file packaging."
else
    echo "mwc_hostdaemon not found, please execute the source compiling first!"
fi

if [[ -f ../pghost/mwc_launcher ]];
then
    cp ../pghost/mwc_launcher dpkg/opt/cfc/mwc/bin/mwc_launcher
    echo "Copied mwc_launcer file for debian file packagng."
else
    echo "mwc_launcher not foud, please execute the source compiling first!"
fi

if [[ -f ../pghost/api_test ]];
then
    cp ../pghost/api_test dpkg/opt/cfc/mwc/bin/api_test
    echo "Copied api_test file for debian file packagng."
else
    echo "api_test not foud, please execute the source compiling first!"
fi

if [[ -f ../pghost/libkydroid.so ]];
then
    cp ../pghost/libkydroid.so dpkg/opt/cfc/mwc/lib/libkydroid.so
    echo "Copied libkydroid.so file for debian file packagng."
else
    echo "libkydroid.so not foud, please execute the source compiling first!"
fi

if [[ -f ../pghost/msg_agent ]];
then
    cp ../pghost/msg_agent dpkg/opt/cfc/mwc/bin/msg_agent
    echo "Copied msg_agent file for debian file packagng."
else
    echo "msg_agent not foud, please execute the source compiling first!"
fi

if [[ -f ../pghost/comm_host_agent ]];
then
    cp ../pghost/comm_host_agent dpkg/opt/cfc/mwc/bin/comm_host_agent
    echo "Copied comm_host_agent file for debian file packagng."
else
    echo "comm_host_agent not foud, please execute the source compiling first!"
fi
dpkg -b dpkg cfc-0.1.0-x64.deb

