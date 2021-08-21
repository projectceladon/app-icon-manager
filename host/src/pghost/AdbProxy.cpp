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

#define CLOG_NDEBUG 0
#define CLOG_TAG "AdbProxy"

#include "AdbProxy.h"
#include "Log.h"

int AdbProxy::startActivity(const char* name) {
  CLOGV("%s", __func__);

  char cmd[256];

  snprintf(cmd, 256, "am start %s", name);
  return runShell(cmd);
}
int AdbProxy::closeActivity(const char* name) {
  CLOGV("%s", __func__);

  char cmd[256];

  snprintf(cmd, 256, "am force-stop %s", name);
  return runShell(cmd);
}

int AdbProxy::getStackList(std::map<int, StackInfo>& stacks) {
  return __getStackList(stacks);
}

int AdbProxy::setFocusedStack(int stackId) {
  return 0;
}
int AdbProxy::removeStack(int stackId) {
  return 0;
}
int AdbProxy::resizeTask(int taskId, int l, int t, int r, int b) {
  return 0;
}

bool AdbProxy::setProp(const char* prop, int value) {
  CLOGV("%s", __func__);

  char cmd[128];

  snprintf(cmd, 32, "setprop %s %d", prop, value);
  return runShell(cmd);
}

bool AdbProxy::inputTap(int x, int y) {
  CLOGV("%s", __func__);

  char cmd[32];

  snprintf(cmd, 32, "input tap %d %d", x, y);
  return runShell(cmd);
}
bool AdbProxy::inputSwipe(int x1, int y1, int x2, int y2) {
  CLOGV("%s", __func__);

  char cmd[32];

  snprintf(cmd, 32, "input swipe %d %d %d %d", x1, y1, x2, y2);
  return runShell(cmd);
}
bool AdbProxy::inputKey(int keycode) {
  CLOGV("%s", __func__);

  char cmd[32];

  snprintf(cmd, 32, "input keyevent %d", keycode);
  return runShell(cmd);
}
bool AdbProxy::inputBack() {
  CLOGV("%s", __func__);

  return inputKey(KEYCODE_BACK);
}

/* am stack positiontask: place <TASK_ID> in <STACK_ID> at <POSITION>
 */
bool AdbProxy::setTopActivity(int stackId, int taskId) {
  CLOGV("%s", __func__);

  char cmd[64];

  snprintf(cmd, 64, "am stack positiontask %d %d %d", taskId, stackId, 1);
  runShell(cmd);  // need to run twice in AndroidP
  return runShell(cmd);
}

/* am task resize: makes sure <TASK_ID> is in a stack with the specified bounds.
Forces the task to be resizeable and creates a stack if no existing stack
has the specified bounds.
*/
bool AdbProxy::setActivityBounds(int taskId, int l, int t, int r, int b) {
  CLOGV("%s", __func__);

  char cmd[64];

  snprintf(cmd, 64, "am task resize %d %d %d %d %d", taskId, l, t, r, b);
  return runShell(cmd);
}

int AdbProxy::getAndroidVersion() {
  CLOGV("%s", __func__);

  int android_version = 10;

  FILE* pf = popen(msgAndroidVerison, "r");

  if (pf) {
    // find the head in first
    while (!feof(pf) && fgets(mMsgBuf, kMsgBufSize, pf)) {
      char* msg_header = strstr(mMsgBuf, "ro.system.build.version.release]:");
      if (msg_header) {
	  char* left_pos = strstr(msg_header, "[");
	  char* right_pos = strstr(left_pos, "]");

	  if (left_pos && right_pos && right_pos > left_pos) {
	      size_t len = right_pos - left_pos;
	      char version[4];
	      memset (version, 0 , sizeof(version));
	      if (len > sizeof(version))
		  len = sizeof(version);
	      snprintf(version, len, "%s", left_pos+1);
	      android_version = atoi(version);
	  }
	  break;
      }
    }

    pclose(pf);
  }
  return android_version;
}

bool AdbProxy::getDeviceName() {
  CLOGV("%s", __func__);

  mConnected = false;

  CLOGD("%s", msgDeviceList);

  FILE* pf = popen(msgDeviceList, "r");

  if (pf) {
    // find the head in first
    while (!feof(pf) && fgets(mMsgBuf, kMsgBufSize, pf)) {
      if (strstr(mMsgBuf, msgDeviceListHead))
        break;
    }
    // find a online device
    while (!feof(pf) && fgets(mMsgBuf, kMsgBufSize, pf)) {
      if (strstr(mMsgBuf, "\tdevice")) {
        char name[32];
        sscanf(mMsgBuf, "%32s", name);
        mDeviceName = name;
        mConnected = true;
        CLOGD("Use adb device %s", name);
        break;
      }
    }
    pclose(pf);
  }
  return mConnected;
}
bool AdbProxy::connect() {
  CLOGV("%s", __func__);

  if (getDeviceName())
    return true;

  CLOGD("%s", msgConnect);

  FILE* pf = popen(msgConnect, "r");
  if (pf) {
    while (!feof(pf) && fgets(mMsgBuf, kMsgBufSize, pf)) {
      if (strstr(mMsgBuf, msgConnectOk)) {
        getDeviceName();
        break;
      }
    }
    pclose(pf);
  }
  if (!mConnected) {
    CLOGE("Failed to connect adb:error=%d(%s)\n", errno, strerror(errno));
  }
  return mConnected;
}

bool AdbProxy::runShellCmd(const char* cmd) {
  CLOGV("%s", __func__);

  if (!mConnected) {
    connect();
  }

  snprintf(mMsgBuf, kMsgBufSize, "adb -s %s shell %s", mDeviceName.c_str(),
           cmd);
  CLOGD("%s", mMsgBuf);

  FILE* pf = popen(mMsgBuf, "r");
  if (pf) {
    while (!feof(pf) && fgets(mMsgBuf, kMsgBufSize, pf)) {
      if (strstr(mMsgBuf, msgErrorBroken) || strstr(mMsgBuf, msgErrorOffline)) {
        mConnected = false;
        runShell(cmd);
        break;
      }
    }
    pclose(pf);
  } else {
    CLOGE("Failed to run cmd %s, error=%d(%s)\n", cmd, errno, strerror(errno));
    return false;
  }
  return true;
}

bool AdbProxy::runCmd(const char* cmd) {
  CLOGV("%s", __func__);

  if (!mConnected) {
    connect();
  }

  snprintf(mMsgBuf, kMsgBufSize, "adb -s %s %s", mDeviceName.c_str(),
           cmd);
  CLOGD("%s", mMsgBuf);

  FILE* pf = popen(mMsgBuf, "r");
  if (pf) {
    while (!feof(pf) && fgets(mMsgBuf, kMsgBufSize, pf)) {
      if (strstr(mMsgBuf, msgErrorBroken) || strstr(mMsgBuf, msgErrorOffline)) {
        mConnected = false;
        runShell(cmd);
        break;
      }
    }
    pclose(pf);
  } else {
    CLOGE("Failed to run cmd %s, error=%d(%s)\n", cmd, errno, strerror(errno));
    return false;
  }
  return true;
}


bool AdbProxy::runShell(const char* cmd) {
  CLOGV("%s", __func__);

  if (!mConnected) {
    connect();
  }

  snprintf(mMsgBuf, kMsgBufSize, "adb -s %s shell %s", mDeviceName.c_str(),
           cmd);
  CLOGD("%s", mMsgBuf);

  FILE* pf = popen(mMsgBuf, "r");
  if (pf) {
    while (!feof(pf) && fgets(mMsgBuf, kMsgBufSize, pf)) {
      if (strstr(mMsgBuf, msgErrorBroken) || strstr(mMsgBuf, msgErrorOffline)) {
        mConnected = false;
        runShell(cmd);
        break;
      }
    }
    pclose(pf);
  } else {
    CLOGE("Failed to run cmd %s, error=%d(%s)\n", cmd, errno, strerror(errno));
    return false;
  }
  return true;
}

/* am stack list: list all of the activity stacks and their sizes.
--------- Android P -----------------
Stack id=6 bounds=[0,0][1280,720] displayId=0 userId=0
 configuration={1.0 ?mcc?mnc [en_US] ldltr sw720dp w1280dp h648dp 160dpi lrg
long land finger -keyb/v/h -nav/h winConfig={ mBounds=Rect(0, 0 - 1280, 672)
mAppBounds=Rect(0, 0 - 1280, 672) mWindowingMode=freeform
mActivityType=standard} s.5}
taskId=13:
com.android.calendar/com.android.calendar.AllInOneActivity
bounds=[320,168][960,504] userId=0 visible=true
topActivity=ComponentInfo{com.android.calendar/com.android.calendar.AllInOneActivity}
*/
int AdbProxy::__getStackList(std::map<int, StackInfo>& stacks) {
  CLOGV("%s", __func__);

  if (!mConnected) {
    connect();
  }

  snprintf(mMsgBuf, kMsgBufSize, "adb -s %s shell am stack list",
           mDeviceName.c_str());

  FILE* pf = popen(mMsgBuf, "r");
  if (pf) {
    StackInfo stack;
    stack.stackId = -1;
    TaskInfo task;

    while (!feof(pf) && fgets(mMsgBuf, kMsgBufSize, pf)) {
      if (strstr(mMsgBuf, msgErrorBroken) || strstr(mMsgBuf, msgErrorOffline)) {
        mConnected = false;
        getStackList(stacks);
        break;
      }

      if (strncmp(mMsgBuf, "Stack id=", 9) == 0) {
        CLOGV("stack:%s", mMsgBuf);

        if (sscanf(mMsgBuf,
                   "Stack id=%d bounds=[%d,%d][%d,%d] displayId=%d userId=%d",
                   &stack.stackId, &stack.left, &stack.top, &stack.right,
                   &stack.bottom, &stack.displayId, &stack.userId) > 0) {
          stacks.emplace(stack.stackId, stack);
          continue;
        }
      } else if (strncmp(mMsgBuf, " configuration=", 15) == 0) {
        CLOGV("configration:%s", mMsgBuf);

        char* p = strstr(mMsgBuf, "mWindowingMode=");
        char str1[32], str2[32];
        if (sscanf(p, "mWindowingMode=%32s mActivityType=%32s", str1, str2) > 0) {
          stacks.at(stack.stackId).windowingMode = toWindowingMode(str1);
          stacks.at(stack.stackId).activityType = toActivityType(str2);
        }
      } else if (strncmp(mMsgBuf, "  taskId=", 9) == 0) {
        CLOGV("task:%s", mMsgBuf);

        char name[256];
        char topActivity[256];
        char visible[8];
        if (sscanf(mMsgBuf,
                   "  taskId=%d: %256s bounds=[%d,%d][%d,%d] userId=%d visible=%8s "
                   "topActivity=ComponentInfo{%256s}",
                   &task.taskId, name, &task.left, &task.top, &task.right,
                   &task.bottom, &task.userId, visible, topActivity) > 0) {
          topActivity[strlen(topActivity) - 1] = '\0';
          task.stackId = stack.stackId;
          task.name = name;
          task.topActivityName = topActivity;
          task.visible = (strncmp(visible, "true", 8) == 0);

          stacks.at(task.stackId).tasks.emplace(task.taskId, task);
        }
      }
    }
    pclose(pf);
  } else {
    CLOGE("Failed to run cmd %s, error=%d(%s)\n", mMsgBuf, errno,
          strerror(errno));
    return -1;
  }
  return 0;
}

//#define UNIT_TEST 1
#if UNIT_TEST

int main(int argc, char* argv[]) {
  AdbProxy* proxy = AdbProxy::getInstance();
  std::map<int, StackInfo> stacks;

  proxy->getStackList(stacks);
  for (auto& stack : stacks) {
    CLOGD(
        "Stack Id %d display %d user %d bound <%d %d %d %d> windowingMode=%d "
        "activityType=%d",
        stack.second.stackId, stack.second.displayId, stack.second.userId,
        stack.second.left, stack.second.top, stack.second.right,
        stack.second.bottom, stack.second.windowingMode,
        stack.second.activityType);

    for (auto& task : stack.second.tasks) {
      CLOGD("  taskId %d name=%s bound <%d %d %d %d> topActivity=%s",
            task.second.taskId, task.second.name.c_str(), task.second.left,
            task.second.top, task.second.right, task.second.bottom,
            task.second.topActivityName.c_str());
    }
  }
  return 0;
}
#endif
