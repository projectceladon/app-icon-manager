# App Icon Manager responsible for Android App launch icon creation/remova in host OS desktop and support lunching Android App running in Android VM guest from Host OS.

Install App Icon Manager .deb package

1. Install cfc .deb file from folder host/bins
sudo dpkg -i scripts/cfc-0.1.0-x86.deb
Note: add no-debsig into /etc/dpkg/dpkg.cfg before run dpkg command in kylin OS environment.

2. Start mwc service
mwc.service will be stared by default after installation, if not, execute command below:
systemctl start mwc.service
Note: The mwc.service will be started automatically at boot time.

Create the host side app launch icons

1. Run terminal in normal user which currently holds ubuntu desktop session
Please ensure CIV instance has been started already.
/opt/cfc/mwc/bin/mwc create-shortcut
If thing goes well, you should be able to see some .desktop files created under folder ~/applications/. For ubuntu, just copy this folder ~/.local/share. The new created app lunch icons will be present in Ubuntu app launcher automatically then.

2 create .desktop file for given app, example org.videolan.vlc
/opt/cfc/mwc/bin/mwc create-shortcut org.videolan.vlc

3. Post processing for Kylin OS adaptation
/opt/cfc/mwc/bin/update_lunch_pg.sh
3.1 Update .desktop file of given app only (ex org.videolan.vlc)
/opt/cfc/mwc/bin/update_lunch_pg.sh org.videolan.vlc

Package building

The build steps are based on Ubuntu 18.04 desktop
1. Install the build dependencies
apt-get install git make cmake build-essential clang libjsoncpp-dev

2. clone the source
git clone https://github.com/projectceladon/app-icon-manager.git

3. build out the binaries
cd app-icon-manager/host/src/pghost
# Build out the binary files
make
cd ../pack
./pack.sh
# Move the deb file to the bins folder
mv cfc-0.1.0-x64.deb ../../bins
cd ../pghost
# clean out the intermediate build outs
make clean
