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

#include <stdarg.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <mutex>

#include "Log.h"

#define gettid() syscall(__NR_gettid)

static FILE* gLogFile = stdout;
static std::mutex gLogMutex;
const char kLevelToChar[] = {
    'V', 'D', 'I', 'W', 'E',
};

int setLogFile(FILE* fp) {
  if (fp) {
    gLogFile = fp;
  }
  return 0;
}

int __log_print(int level, const char* tag, const char* fmt, ...) {
  va_list al;
  int ret;
  std::unique_lock<std::mutex> lk(gLogMutex);

  char name[16];
  prctl(PR_GET_NAME, name);

  time_t ts;
  struct tm lt;
  struct timeval tv;

  time(&ts);
  localtime_r(&ts, &lt);
  gettimeofday(&tv, nullptr);

  fprintf(gLogFile, "%.2d:%.2d:%.2d.%.4d  ", lt.tm_hour, lt.tm_min, lt.tm_sec,
          (int)(tv.tv_usec / 100));

  fprintf(gLogFile, "%s  %c  %s: ", name, kLevelToChar[level], tag);
  va_start(al, fmt);
  ret = vfprintf(gLogFile, fmt, al);
  va_end(al);
  fprintf(gLogFile, "\n");
  fflush(gLogFile);

  return ret;
}
