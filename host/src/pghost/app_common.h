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

#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#include <map>
#include <string>
#include <vector>

enum WindowingMode {
  WINDOWING_MODE_UNDEFINED,
  WINDOWING_MODE_FULLSCREEN,
  WINDOWING_MODE_PINNED,
  WINDOWING_MODE_SPLIT_SCREEN_PRIMARY,
  WINDOWING_MODE_SPLIT_SCREEN_SECONDARY,
  WINDOWING_MODE_FREEFORM,
};

enum ActivityType {
  ACTIVITY_TYPE_UNDEFINED,
  ACTIVITY_TYPE_STANDARD,
  ACTIVITY_TYPE_HOME,
  ACTIVITY_TYPE_RECENTS,
  ACTIVITY_TYPE_ASSISTANT,
};

struct TaskInfo {
  std::string name;
  std::string topActivityName;
  int taskId;
  int stackId;
  int userId;
  bool visible;
  int left;
  int top;
  int right;
  int bottom;
};

struct StackInfo {
  int stackId;
  int displayId;
  int userId;
  int left;
  int top;
  int right;
  int bottom;
  int windowingMode;
  int activityType;

  std::map<int, TaskInfo> tasks;
};

class AmInterface {
 public:
  virtual ~AmInterface(){};
  virtual int startActivity(const char* name) = 0;
  virtual int closeActivity(const char* name) = 0;
  virtual int getStackList(std::map<int, StackInfo>& stacks) = 0;
  virtual int setFocusedStack(int stackId) = 0;
  virtual int removeStack(int stackId) = 0;
  virtual int resizeTask(int taskId, int l, int t, int r, int b) = 0;
};
#endif  //__APP_COMMON_H__
