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

Author: Mao Marc (marc.mao@intel.com)
Date: 2021.04.27

*/

//#define CLOG_NDEBUG 0
#define CLOG_TAG "ShortcutMgrLg"

#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <json/json.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>

//#include "ConfigMgr.h"
#include "Log.h"
#include "ShortcutMgrLg.h"

//using namespace android;

ShortcutMgrLg::ShortcutMgrLg() { m_adbproxy_ = AdbProxy().getInstance(); }
ShortcutMgrLg::~ShortcutMgrLg() {}

bool ShortcutMgrLg::init() {
    const char* home = getenv("HOME");
    if (home) {
	mDesktopFolder.append(home);
	mDesktopFolder.append("/Desktop");
	mUserLauncherFolder.append(home);
	mUserLauncherFolder.append("/.local/share/applications");
    }

    mLaunchFileFolder = mUserLauncherFolder;
    mIconFileFolder = mLaunchFileFolder;
    mIconFileFolder.append("/icon");
    char c_file_path[512];
    snprintf(c_file_path, sizeof(c_file_path), "%s", mIconFileFolder.c_str());
    if (access(c_file_path, F_OK) != 0) {
	mkdir(c_file_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    mInitialized = true;
    return true;

}

int ShortcutMgrLg::createLaunchFile(const char* pkg) {
    return 0;
}

int ShortcutMgrLg::createIconFile(const char* pkg) {

    return 0;
}

const char* ShortcutMgrLg::getIconFile(const char* pkg) {
    if (!mInitialized) {
	CLOGE("%s:not initialized yet!", __func__);
	return nullptr;
    }
    mIconFilePath = mIconFileFolder;
    mIconFilePath.append("/");
    mIconFilePath.append(pkg);
    mIconFilePath.append(".png");

    char c_file_path[512];
    snprintf (c_file_path, sizeof(c_file_path), "%s", mIconFilePath.c_str());
    if (access(c_file_path, F_OK) != 0) {
	createIconFile(pkg);
    }
    if (access(c_file_path, F_OK) == 0) {
	return mIconFilePath.c_str();
    }
    return nullptr;
}

int ShortcutMgrLg::createShortcut(const char* pkg) {
    return 0;
}

#define PG_USE_HOME_DIR
int ShortcutMgrLg::createShortcutForInstalledApps() {
    m_adbproxy_->runShellCmd("/system/bin/pm_agent_client");
#ifdef PG_USE_HOME_DIR
    m_adbproxy_->runCmd("pull /sdcard/applications ~/");
    system("/opt/cfc/mwc/bin/process_lunch_file_pg.sh");
#else
    m_adbproxy_->runCmd("pull /sdcard/applications ~/.local/share/");
    system("/opt/cfc/mwc/bin/process_lunch_file.sh");
#endif
    return 0;
}

int ShortcutMgrLg::createShortcutForInstalledApp(const char* app_pkg_name)
{
    register uid_t v_uid;
    v_uid = geteuid ();
    register struct passwd *v_passwd;
    v_passwd = getpwuid (v_uid);
    const char *v_home_dir = v_passwd->pw_dir;
    char applications_folder[512];
    char icon_folder[512];
#ifdef PG_USE_HOME_DIR
    snprintf(applications_folder, sizeof(applications_folder), "%s/applications/", v_home_dir);
    if (access(applications_folder, F_OK) != 0) {
	mkdir(applications_folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    snprintf(icon_folder, sizeof(icon_folder), "%s/applications/icon/", v_home_dir);
    if (access(icon_folder, F_OK) != 0) {
	mkdir(icon_folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
#else
    snprintf(applications_folder, sizeof(applications_folder), "%s/.local/share/applications/", v_home_dir);
    if (access(applications_folder, F_OK) != 0) {
	mkdir(applications_folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    snprintf(icon_folder, sizeof(icon_folder), "%s/.local/share/applications/icon/", v_home_dir);
    if (access(icon_folder, F_OK) != 0) {
	mkdir(icon_folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
#endif

    m_adbproxy_->runShellCmd("/system/bin/pm_agent_client");

    char cmd_pull_desktop_file[1024];
    char cmd_pull_icon_file[1024];
    snprintf(cmd_pull_desktop_file, sizeof(cmd_pull_desktop_file), "pull /sdcard/applications/%s.desktop %s", app_pkg_name, applications_folder);
    m_adbproxy_->runCmd(cmd_pull_desktop_file);
    snprintf(cmd_pull_icon_file, sizeof(cmd_pull_icon_file), "pull /sdcard/applications/icon/%s.png %s", app_pkg_name, icon_folder);
    m_adbproxy_->runCmd(cmd_pull_icon_file);

#ifdef PG_USE_HOME_DIR
    system("/opt/cfc/mwc/bin/process_lunch_file_pg.sh");
#else
    system("/opt/cfc/mwc/bin/process_lunch_file.sh");
#endif

}

bool ShortcutMgrLg::closeApp(char* appname, char* pkgname)
{
    // force-close the app
#ifdef LG_SINGLE_MODE_CMD
    m_adbproxy_->closeActivity(pkgname);
#else
    m_adbproxy_->closeActivity(pkgname);
    char cmdbuf[512];
    snprintf(cmdbuf, sizeof(cmdbuf), "/opt/cfc/mwc/bin/closeapp.sh %s %s", appname, pkgname);
    system(cmdbuf);
#endif
    return true;
}

bool ShortcutMgrLg::launchAppBlk(char* pkgname) {
    cout << "Called in launchAppBlk" << endl;
    if (m_app_intent_map.size() == 0) {
	    updateInstalledApps();
    }
    std::string intent = m_app_intent_map[pkgname];
    cout << "intent: " << intent << endl;

    char cmdbuf[512];
    snprintf(cmdbuf, sizeof(cmdbuf), "/opt/cfc/mwc/bin/loadapp_single_lg.sh %s %s 0", pkgname, intent.c_str());
    system(cmdbuf);
    return true;
}

bool ShortcutMgrLg::launchApp(char* pkgname, bool fullscreen, int width, int height, int density)
{
    cout << "Called in launchApp" << endl;
    if (m_app_intent_map.size() == 0) {
	    updateInstalledApps();
    }
    std::string intent = m_app_intent_map[pkgname];
    cout << "intent: " << intent << endl;

    char cmdbuf[512];
#ifdef LG_SINGLE_MODE
    if (0==fork()) {
        snprintf(cmdbuf, sizeof(cmdbuf), "/opt/cfc/mwc/bin/loadapp_single_lg.sh %s %s 0", pkgname, intent.c_str());
        system(cmdbuf);
        exit(0);
    }
#else
    if (0 == fork()) {
        //child
	snprintf(cmdbuf, sizeof(cmdbuf), "/opt/cfc/mwc/bin/lg_launcher.sh %s %s", pkgname, intent.c_str());
	system(cmdbuf);
	exit(0);
    }
#endif
    return true;
}

int ShortcutMgrLg::updateInstalledApps()
{
    m_adbproxy_->runShellCmd("/system/bin/pm_agent_client dump-apps");

    char filepath[256];
    const char* home;

    mInstalledApps = "[";
    home = getenv("HOME");
    if (home) {
	snprintf(filepath, sizeof(filepath), "%s/.installed_apps.json", home);
	char cmdbuf[512];
	snprintf(cmdbuf, sizeof(cmdbuf), "adb pull /sdcard/installed_apps.json %s", filepath);
	system(cmdbuf);
	Json::Value root;
	std::ifstream ifs;
	ifs.open(filepath);
	Json::CharReaderBuilder builder;
	builder["collectComments"] = true;
	JSONCPP_STRING errs;
	if (!parseFromStream(builder, ifs, &root, &errs)) {
	    std::cout << errs << std::endl;
	}
	const Json::Value apps = root["apps"];
	std::string intent;
	std::string name;
	std::string version;
	//std::string label;
	for (unsigned int i = 0; i < apps.size(); i++) {
	    intent = apps[i]["Intent"].asString();
	    name = apps[i]["Name"].asString();
	    version = apps[i]["Version"].asString();
	    //label = apps[i]["AppLabel"].asString();
	    m_app_intent_map[name] = intent;
	    if ( 0 != i) {
		mInstalledApps += ", ";
	    }
	    char app_str[512];
	    snprintf(app_str, sizeof(app_str), "{\"app_name\":\"%s\",\"package_name\":\"%s\",\"version_name\":\"%s\"}", name.c_str(), name.c_str(), version.c_str());
            mInstalledApps += app_str;
	}
    }

    mInstalledApps += "]";
    return 0;
}

const char* ShortcutMgrLg::getInstalledApps()
{

    if (mInstalledApps.size() > 0) 
    {
	return mInstalledApps.c_str();
    }
    else {
	updateInstalledApps();
    }
    return mInstalledApps.c_str();

}

bool ShortcutMgrLg::uninstallApp(const char* pkg) {
    // uninstall the app via adb
    // refresh the installed app list
    char cmdbuf[256];
    snprintf (cmdbuf, sizeof(cmdbuf), "uninstall %s", pkg);
    m_adbproxy_->runCmd(cmdbuf);
    updateInstalledApps();
    return 0;
}

bool ShortcutMgrLg::installApp(char* filename)
{
    char cmdbuf[512];
    bool apk_file_checked = false;
    if (access(filename, F_OK) == 0) {
	apk_file_checked = true;
        snprintf(cmdbuf, sizeof(cmdbuf), "install %s", filename);
    }
    else {
	cout << "File: " << filename << " doesn't exist, try to find it in apk download folder ..." << endl;
	char downloaded_apk [512];

        register uid_t v_uid;
        v_uid = geteuid ();
	register struct passwd *v_passwd;
        v_passwd = getpwuid (v_uid);
        const char *v_home_dir = v_passwd->pw_dir;
	const char *v_user_name = v_passwd->pw_name;
        snprintf(downloaded_apk, sizeof(downloaded_apk), apkDownloadPath.c_str(), v_uid, v_user_name, filename);
	cout << "Check if file: " << downloaded_apk << " exists ..." << endl;
	if (access(downloaded_apk, F_OK) ==0 ) {
	    snprintf(cmdbuf, sizeof(cmdbuf), "install %s", downloaded_apk);
	    apk_file_checked = true;
	}
	else {
	    snprintf(downloaded_apk, sizeof(downloaded_apk), apkCachePath.c_str(), v_home_dir, filename);
	    cout << "Check if file: " << downloaded_apk << " exists ..." << endl;
	    if (access(downloaded_apk, F_OK) ==0 ) {
		snprintf(cmdbuf, sizeof(cmdbuf), "install %s", downloaded_apk);
                apk_file_checked = true;
	    }
	    else {
	        cout << "File: " << downloaded_apk << " doesn't exist as well!" << endl;
	    }
	}
    }
    if (apk_file_checked) {
        m_adbproxy_->runCmd(cmdbuf);
        updateInstalledApps();
    }
    else {
        cout << "Apk file: " << filename << " is not installed!" << endl;
    }
    return 0;
}

int ShortcutMgrLg::getAndroidVersion()
{
    return m_adbproxy_->getAndroidVersion();
}

int ShortcutMgrLg::removeShortcut(const char* pkg) {
    return 0;
}


