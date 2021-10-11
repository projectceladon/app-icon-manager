#!/bin/bash

RELEASE_PKG=
CIV_WORK_DIR=/opt/civ
CIV_DATA_DIR=$CIV_WORK_DIR/data

function check_parameter() {
	if [ $# -lt 1 ]; then
		echo "Usage: $0 <Release.deb>"
		exit -1
	fi
	if [ ! -f $1 ]; then
		echo "$1 does not exists!"
		exit -1
	fi
	RELEASE_PKG=$(realpath $1)
}

function extract_deb() {
	# Add new group for CiV
	sudo groupadd civ
	sudo usermod -a -G civ $USER

	# Create folder for CiV
	[ -d "/opt/civ" ] && sudo rm -rf $CIV_WORK_DIR
	sudo mkdir -p $CIV_WORK_DIR
	sudo chgrp -R civ $CIV_WORK_DIR
	sudo chmod 777 $CIV_WORK_DIR
	sudo chmod +s $CIV_WORK_DIR

	mkdir -p $CIV_DATA_DIR
	dpkg -x $RELEASE_PKG $CIV_DATA_DIR
}

function flash_image(){
	mkdir -p $CIV_DATA_DIR/Release_Deb/aaf
	mkdir -p $CIV_DATA_DIR/Release_Deb/hostshare

	cd $CIV_DATA_DIR/Release_Deb/
	cp /usr/share/qemu/OVMF.fd  .
	./scripts/start_flash_usb.sh caas_dev-flashfiles-*.zip --display-off
	mv -R $CIV_DATA_DIR/Release_Deb/userdata $HOME/.userdata

	cd scripts/
	sudo dpkg -r cfc
	sudo dpkg --no-debsig -i cfc-0.1.0-x64.deb
}

function install_vm-manager() {
	local REL_URL=$(wget -q -nv -O- https://api.github.com/repos/projectceladon/vm_manager/releases/latest 2>/dev/null | jq -r '.assets[] | select(.browser_download_url | contains("ubuntu-20.04.deb")) | .browser_download_url')
	if [ -z $REL_URL ]; then
		echo "Failed to acquire latest release for vm-manager, Please download/install from https://github.com/projectceladon/vm_manager/releases/"
		exit -1
	fi

	wget -q -nv -O /tmp/$(basename $REL_URL) $REL_URL
	sudo dpkg --no-debsig -i /tmp/$(basename $REL_URL)
	if [ $? -ne 0 ]; then
		echo "Failed install vm-manager, Please download/install from https://github.com/projectceladon/vm_manager/releases/"
		exit -1
	fi
}

function install_cfc() {
	sudo dpkg -r cfc
	sudo dpkg -i $CIV_DATA_DIR/Release_Deb/scripts/cfc-0.1.0-x64.deb
}

function change_script(){
	sudo dpkg --no-debsig -i $HOME/libsdl2-2.0-0_2.0.10+dfsg1-3_amd64.deb
	sudo sed -i "s/\$1/\"教育应用\"/g" /opt/cfc/mwc/bin/loadapp_single_lg.sh
	sudo cp /opt/lg/bin/LG_B1_Client /opt/lg/bin/LG_B1_Client_bak
	sudo -E cp $HOME/LG_B1_Client /opt/lg/bin/LG_B1_Client
	sudo chmod 755 /opt/lg/bin/LG_B1_Client
}

function configure_civ_ini() {
	echo "Update config file($HOME/.intel/.civ/penguin-peak.ini) ..."
	mkdir -p $HOME/.intel/.civ/ && cp /opt/cfc/mwc/bin/penguin-peak.ini $HOME/.intel/.civ/
	sed -i "s%\/home\/kylin\/civ%${CIV_DATA_DIR}/Release_Deb%g" $HOME/.intel/.civ/penguin-peak.ini
	sed -i "s%username%${USER}%g" ~/.intel/.civ/penguin-peak.ini
}

function configure_civ_service(){
	sudo sed -i "s%^CIV_PATH=.*$%CIV_PATH=${CIV_DATA_DIR}/Release_Deb%g" /etc/rc.civ
}

function setup_houdini() {
	cd /opt/houdini && sudo ./push_houdini.sh
}

function stop_civ_services() {
	# Trying to disable old CiV service which running as Root
	sudo systemctl stop civ
	sudo systemctl disable civ

	systemctl --user daemon-reload
	systemctl --user stop civ
	systemctl --user disable civ
}

function start_civ_services() {
	systemctl --user daemon-reload
	systemctl --user enable civ
	systemctl --user start civ
}

function wait_civ_adb_connect() {
	echo "Wait for Adb device online ..."
	local i="0"
	local MAX_CONN_CNT="10"
	while [ $i -lt $MAX_CONN_CNT ]; do
		adb kill-server
		adb reconnect
		sleep 1s
		timeout 10s adb wait-for-device && break
		i=$[$i+1]
	done

	if [ $i -ge $MAX_CONN_CNT ]; then
		echo "Failed: cannot establish adb connection!"
		exit -1
	fi
}

check_parameter "$@"

stop_civ_services
extract_deb
install_vm-manager
install_cfc
flash_image
configure_civ_ini
configure_civ_service
change_script
start_civ_services

wait_civ_adb_connect && setup_houdini
