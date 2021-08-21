# PenguinPeak App Icon Manager responsible for Android App launch icon creation/removal and support lunching Android App from Host OS side

Install App Icon Manager .deb package

1. Install cfc .deb file from folder host/bins
sudo dpkg -i scripts/cfc-0.1.0-x86.deb
Note: add no-debsig into /etc/dpkg/dpkg.cfg before run dpkg command in kylin OS environment.

2. Start mwc service
mwc.service will be stared by default after installation, if not, execute command below:
systemctl start mwc.service
Note: The mwc.service will be started automatically at boot time.

Create the host side app launch icons

1. Run terminal in normal user which current holds ubuntu desktop session
Please ensure CIV instance has been started already.
/opt/cfc/mwc/bin/mwc create-shortcut
If thing goes well, you should be able to see some .desktop files created under folder ~/applications/

2 create .desktop file for given app, example org.videolan.vlc
/opt/cfc/mwc/bin/mwc create-shortcut org.videolan.vlc
