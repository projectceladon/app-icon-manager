/*
Copyright (C) 2021 Intel Corporation
 
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License, as published
by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
 
SPDX-License-Identifier: GPL-3.0-or-later

Author: Wan Shuang (shuang.wan@intel.com)
Date: 2021.04.27
*/

#include <unistd.h>
#include <stdio.h>

// Please change the file path of kydroidapi.h to your file location.
#include "include/kydroidapi.h"

int main()
{
    bool ret;

    char* installed_apps = get_installed_applist();
    printf("installed_apps: %s\n", installed_apps);
    // for the apk to be installed, please change it to your file location
    ret = install_app("/home/kylin/VLC-Android-3.2.3-x86_64.apk", "vlc", "vlc");
    // for the app to be uninstalled, please change it to your app package name.
    ret = uninstall_app("org.videolan.vlc");

    ret = install_app("VLC-Android-3.2.3-x86_64.apk", "vlc", "vlc");
    // for the app to be uninstalled, please change it to your app package name.
    ret = uninstall_app("org.videolan.vlc");

    ret = is_android_env_installed();
    if (ret) {
	printf("Android has been installed.\n");
    }
    else {
	printf("Android is not installed.\n");
    }

    ret = is_android_env_running();
    if (ret) {
	printf ("Android guest is running.\n");
    }
    else {
	printf ("Android guest is not running yet.\n");
    }

    ret = launch_app("com.android.settings", true, 1024, 768,32);
    sleep(30);

    ret = close_app("com.android.settings", "com.android.settings"); 
    int androidver = get_android_version();
    printf ("Android version is :%d\n", androidver);

    return 0; 

}
