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

#include <signal.h>
#include <sys/prctl.h>
#include <unistd.h>

#include <memory>
#include <thread>
#include <mutex>

//#define CLOG_NDEBUG 0
#define CLOG_TAG "main"

//#include "ConfigMgr.h"
#include "ShortcutMgrLg.h"


#include "Log.h"

const char* shortOpts = "s:l:t:vm";

void safeExit() {

  exit(0);
}

void sigHandler(int num) {
  switch (num) {
    case SIGTERM:
    case SIGSEGV:
    case SIGABRT:
    case SIGINT:
    case SIGHUP:
    case SIGQUIT:
    case SIGALRM:
      CLOGD("%d signal received, exit", num);
      safeExit();
      break;
    default:
      CLOGD("Igored signal %d", num);
      break;
  }
}

void handleSignals() {
  signal(SIGPIPE, SIG_IGN);
  // exit signal
  signal(SIGALRM, sigHandler);
  signal(SIGTERM, sigHandler);
  signal(SIGINT, sigHandler);
  signal(SIGABRT, sigHandler);
  signal(SIGSEGV, sigHandler);
  signal(SIGQUIT, sigHandler);
  signal(SIGHUP, sigHandler);
}

int main(int argc, char* argv[]) {
  prctl(PR_SET_NAME, "main");

  CLOGV("%s", argv[0]);

  CLOGV("%s", __func__);

  const char* params = NULL;
  if (argc <2) {
      printf ("Usage: %s create-shortcut", argv[0]);
      exit(1);
  }

  params = argv[1];
  printf ("params: %s\n", params);
  if (params && !strcmp("create-shortcut", params)) {
      if (argc == 2) {
          printf("Try to create the app shortcuts\n");
          ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
          scMgr->createShortcutForInstalledApps();
      }
      else {
	  const char* app_pkg_name = argv[2];
	  ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
	  scMgr->createShortcutForInstalledApp(app_pkg_name);
      }
    return 0;
  }
  else {
	  printf("unknown params\n");
  }

  return 0;
}
