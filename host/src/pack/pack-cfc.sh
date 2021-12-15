cp ../pghost/mwc dpkg/opt/cfc/mwc/bin/mwc
cp ../pghost/mwc_hostdaemon dpkg/opt/cfc/mwc/bin/mwc_hostdaemon
cp ../pghost/mwc_launcher dpkg/opt/cfc/mwc/bin/mwc_launcher
cp ../pghost/loadapp.sh dpkg/opt/cfc/mwc/bin/loadapp.sh
cp ../pghost/killapp.sh dpkg/opt/cfc/mwc/bin/killapp.sh
cp ../pghost/closeapp.sh dpkg/opt/cfc/mwc/bin/closeapp.sh
cp ../pghost/api_test dpkg/opt/cfc/mwc/bin/api_test
cp ../pghost/libkydroid.so dpkg/opt/cfc/mwc/lib/libkydroid.so
cp ../pghost/msg_agent dpkg/opt/cfc/mwc/bin/msg_agent
cp ../pghost/kill_app_process.sh dpkg/opt/cfc/mwc/bin/kill_app_process.sh
cp ../pghost/check_android_running.sh dpkg/opt/cfc/mwc/bin/check_android_running.sh
dpkg -b dpkg cfc-0.1.0-x64.deb

