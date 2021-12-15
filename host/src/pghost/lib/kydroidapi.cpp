#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <sys/syslog.h>

#include "../ShortcutMgrLg.h"

using namespace std;

extern "C" {

/***********************************************************
   Function:       launch_app
   Description:    Lanch the app.
   Calls:
   Called By:
   Input:
   Output:
   Return:
   Others:
 ************************************************************/
bool launch_app(char* pkgname, bool fullscreen, int width, int height, int density)
{
    std::string m_pkgname = pkgname;
    ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
    scMgr->launchApp(pkgname, fullscreen, width, height, density);
    return true;
}

/***********************************************************
   Function:       close_app
   Description:    Close the app.
   Calls:
   Called By:
   Input:
   Output:
   Return:
   Others:
 ************************************************************/
bool close_app(char* appname, char* pkgname)
{
    std::string m_appname = appname;
    std::string m_pkgname = pkgname;
    ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
    scMgr->closeApp(appname, pkgname);

    return true;
}

/***********************************************************
   Function:       install_app
   Description:    Install the given app
   Calls:
   Called By:
   Input: filename:apk file name like com.tencent.mm_8.0.0.apk
   	  appname:Android apk name
	  pkgname:Android apk package name.
   Output:
   Return:
   Others:
 ************************************************************/
bool install_app(char *filename, char *appname, char *pkgname)
{
    std::string m_filename = filename;
    std::string m_appname = appname;
    std::string m_pkgname = pkgname;
    ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
    bool ret =  scMgr->installApp(filename);
    if (true == ret) {
        return true;
    }
    else {
        return false;
    }
}

/***********************************************************
   Function:       uninstall_app
   Description:    Uninstall the given app.
   Calls:
   Called By:
   Input:
   Output:
   Return:
   Others:
 ************************************************************/
bool uninstall_app(char* pkgname)
{
    std::string m_pkgname = pkgname;
    ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
    scMgr->uninstallApp(pkgname);

    return true;
}

/***********************************************************
   Function:       get_installed_applist
   Description:    Get the list of installed apps in Android guest.
   Calls:
   Called By:
   Input:
   Output:
   Return:
   Others:
 ************************************************************/
char* get_installed_applist()
{
    ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
    
    return scMgr->getInstalledApps();
}

/*
return the json string like below:
[{"appname":"the app name","package_name":"the app package name","size":100000000,"version":"app version"}, {...}, ...]
get_installed_applist_v1 is similar to get_installed_applist with some json fileds' renaming and added the size filed.
*/
char* get_installed_applist_v1()
{
    ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
    return scMgr->getInstalledAppsV1();
}

bool is_android_env_installed()
{
    // There 3 deb packages needs to be installed for Android guest:
    // android-virtual, vm-manager and cfc.
    char * INSTALLED_PKGS_CMD="/opt/cfc/mwc/bin/get_num_android_pkgs.sh";

    FILE* pf = popen(INSTALLED_PKGS_CMD, "r");
    char shell_cmd_buffer[1024];
    int num_pkgs = 0;
    if (pf) {
        if (!feof(pf) && fgets(shell_cmd_buffer, sizeof(shell_cmd_buffer), pf)) {
	    num_pkgs = atoi(shell_cmd_buffer);
	}
    }
    printf("Num installed android packages: %d\n", num_pkgs);
    if (num_pkgs >= 3) {
	return true;
    }
    else {
	return false;
    }
}

bool is_android_env_running()
{
    int ret = system("/opt/cfc/mwc/bin/check_android_running.sh");
    if (ret > 0) {
	return true;
    }
    else {
	return false;
    }
}

int get_android_version()
{
    ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
    return scMgr->getAndroidVersion();
}

}
