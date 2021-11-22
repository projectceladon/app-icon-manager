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

#ifndef __ADB_PROXY_H__
#define __ADB_PROXY_H__

#include <stdio.h>
#include <string.h>

#include <list>
#include <map>
#include <string>
#include <vector>

#include "app_common.h"

class AdbProxy : public AmInterface {
 public:
  static AdbProxy* getInstance() {
    static AdbProxy sInstance;
    return &sInstance;
  }

 public:
  AdbProxy(){};
  ~AdbProxy(){};

  // AmInterface
  int startActivity(const char* name) override;
  int closeActivity(const char* name) override;
  int getStackList(std::map<int, StackInfo>& stacks) override;
  int setFocusedStack(int stackId) override;
  int removeStack(int stackId) override;
  int resizeTask(int taskId, int l, int t, int r, int b) override;

  bool setProp(const char* prop, int value);
  bool inputTap(int x, int y);
  bool inputSwipe(int x1, int y1, int x2, int y2);
  bool inputKey(int keycode);
  bool inputBack();

  //bool closeActivity(const char* name);
  int __getStackList(std::map<int, StackInfo>& stacks);
  bool setTopActivity(int stackId, int taskId);
  bool setActivityBounds(int taskId, int l, int t, int r, int b);

  bool runShellCmd(const char* cmd);
  bool runCmd(const char* cmd);
  bool installApp(const char* cmd);
  int  getAndroidVersion();
 protected:
  bool getDeviceName();
  bool connect();
  bool runShell(const char* cmd);

 private:
  int toWindowingMode(const char* s) {
    if (strstr(s, "undefined"))
      return WINDOWING_MODE_UNDEFINED;
    else if (strstr(s, "fullscreen"))
      return WINDOWING_MODE_FULLSCREEN;
    else if (strstr(s, "pinned"))
      return WINDOWING_MODE_PINNED;
    else if (strstr(s, "split-screen-primary"))
      return WINDOWING_MODE_SPLIT_SCREEN_PRIMARY;
    else if (strstr(s, "split-screen-secondary"))
      return WINDOWING_MODE_SPLIT_SCREEN_SECONDARY;
    else if (strstr(s, "freeform"))
      return WINDOWING_MODE_FREEFORM;
    else
      return WINDOWING_MODE_FULLSCREEN;
  }

  int toActivityType(const char* s) {
    if (strstr(s, "undefined"))
      return ACTIVITY_TYPE_UNDEFINED;
    else if (strstr(s, "standard"))
      return ACTIVITY_TYPE_STANDARD;
    else if (strstr(s, "home"))
      return ACTIVITY_TYPE_HOME;
    else if (strstr(s, "recents"))
      return ACTIVITY_TYPE_RECENTS;
    else if (strstr(s, "assistant"))
      return ACTIVITY_TYPE_ASSISTANT;
    else
      return ACTIVITY_TYPE_STANDARD;
  }

 private:
  static const int kMsgBufSize = 1024;
  const char* msgDeviceList = "adb devices";
  const char* msgDeviceListHead = "List of devices attached";
#ifdef VSOCK_ADB
  const char* msgConnect = "adb connect vsock:3:5555";
  const char* msgConnectOk = "connected to vsock:3:5555";
#else
  const char* msgConnect = "adb connect localhost";
  const char* msgConnectOk = "connected to localhost:5555";
#endif
  const char* msgErrorBroken = "error: device not found";
  const char* msgErrorOffline = "error: device offline";
#ifdef VSOCK_ADB
  const char* msgAndroidVerison = "adb -s vsock:3:5555 shell getprop | grep -i ro.system.build.version.release";
#else
  const char* msgAndroidVerison = "adb shell getprop | grep -i ro.system.build.version.release";
#endif
  const int KEYCODE_HOME = 3;
  const int KEYCODE_BACK = 4;
  const int KEYCODE_MENU = 82;

  bool mConnected = false;
  std::string mDeviceName;
  char mMsgBuf[kMsgBufSize];

  std::map<int, TaskInfo*> mTasks;
  std::map<int, StackInfo*> mStacks;
};

#endif  //__ADB_PROXY_H__
