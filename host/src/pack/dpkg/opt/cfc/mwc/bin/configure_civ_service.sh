#!/bin/bash

echo "Input the CIV installation information ..."
read -p "CIV installation path:" civ_path

if [ $(id -u) = 0 ]; then
        USERNAME=$SUDO_USER
else
        USERNAME=$USER
fi

if [ -d $civ_path ]
then
    civ_path=$(realpath ${civ_path})
    echo "The path of CIV is: $civ_path"

    echo "Update config file($HOME/.intel/.civ/penguin-peak.ini) ..."
    mkdir -p $HOME/.intel/.civ/ && cp /opt/cfc/mwc/bin/penguin-peak.ini $HOME/.intel/.civ/
    sed -i "s%\/home\/kylin\/civ%${civ_path}%g" $HOME/.intel/.civ/penguin-peak.ini
    sed -i "s%/username/\.${USERNAME}%g" ~/.intel/.civ/penguin-peak.ini
    mkdir -p ${civ_path}/aaf
    mkdir -p ${civ_path}/hostshare

    echo "Update the /etc/rc.civ.post file ..."
    CIV_RC_FILE_POST=/etc/rc.civ.post
    if [ -f $CIV_RC_FILE_POST ]
    then
        sudo sed -i "s%^CIV_PATH=.*$%CIV_PATH=${civ_path}%g" $CIV_RC_FILE_POST
        echo "/etc/rc.civ.post updated."
    else
        echo "Couldn't find the CIV rc post file: $CIV_RC_FILE_POST!"
    fi

    echo "Update the /etc/rc.civ file ..."
    CIV_RC_FILE=/etc/rc.civ
    if [ -f $CIV_RC_FILE ]
    then 
        sudo sed -i "s%^CIV_PATH=.*$%CIV_PATH=${civ_path}%g" $CIV_RC_FILE
        echo "Enable $HOME/.config/rc.civ service ..."
        systemctl --user enable civ
        echo "To start the civ, run: systemctl --user start civ.service."
	exit 0
    else
	echo "Couldn't find the CIV rc file: $CIV_RC_FILE!"
	exit 1
    fi

else
    echo "The path of CIV:$civ_path doesn't exist!"
    exit 1
fi
