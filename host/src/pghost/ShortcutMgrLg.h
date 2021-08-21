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

#ifndef __SHORTCUT_MGR_LG_H__
#define __SHORTCUT_MGR_LG_H__

//#include <AgentService.h>
#include "AdbProxy.h"
using namespace std;

class ShortcutMgrLg {
 public:
  virtual ~ShortcutMgrLg();

  static ShortcutMgrLg* getShortcutMgrLg() {
    static ShortcutMgrLg* sInst = nullptr;
    if (sInst == nullptr) {
      sInst = new ShortcutMgrLg();
      if (sInst) {
        sInst->init();
      }
    }
    return sInst;
  }

  int createShortcut(const char* pkg);
  int createShortcutForInstalledApps();
  int createShortcutForInstalledApp(const char* app_pkg_name);
  int removeShortcut(const char* pkg);

  const char* getIconFile(const char* pkg);
  const char* getInstalledApps();
  bool closeApp(char* appname, char* pkgname);
  bool launchApp(char* pkgname, bool fullscreen, int width, int height, int density);
  bool launchAppBlk(char* pkgname);
  bool uninstallApp(const char* pkg);
  bool installApp(char* filename);
  int getAndroidVersion();
 private:
  ShortcutMgrLg();
  bool init();

 private:
  int createLaunchFile(const char* pkg);
  int createIconFile(const char* pkg);
  int updateInstalledApps();

 private:
  bool mInitialized = false;
  bool mEnableUserLauncherLinks = true;
  bool mEnableDesktopLinks = false;

  std::string mLaunchFileFolder;
  std::string mIconFileFolder;
  std::string mDesktopFolder;
  std::string mUserLauncherFolder;
  std::string mLauncherAppPath;
  std::string mIconFilePath;
  std::string mInstalledApps;
  // kmre2-uid-username
  std::string apkDownloadPath = "/var/lib/kydroid/kmre2-%d-%s/data/local/tmp/%s";
  std::string apkCachePath = "%s/.cache/uksc/wget_down/%s";
  AdbProxy* m_adbproxy_;
  std::map <string, string> m_app_intent_map;
};
#endif
