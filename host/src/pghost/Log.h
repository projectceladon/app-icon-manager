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

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

enum CLOG_PRI {
  VERBOSE,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
};

#ifndef CLOG_NDEBUG
#define CLOG_NDBEUG 1
#endif

#ifndef CLOG_TAG
#define CLOG_TAG "Log"
#endif

int setLogFile(FILE* fp);
int __log_print(int level, const char* tag, const char* fmt, ...);

#if CLOG_NDBEUG
#define CLOGV(...)
#else
#define CLOGV(...) __log_print(VERBOSE, CLOG_TAG, __VA_ARGS__)
#endif
#define CLOGD(...) __log_print(DEBUG, CLOG_TAG, __VA_ARGS__)
#define CLOGI(...) __log_print(INFO, CLOG_TAG, __VA_ARGS__)
#define CLOGW(...) __log_print(WARNING, CLOG_TAG, __VA_ARGS__)
#define CLOGE(...) __log_print(ERROR, CLOG_TAG, __VA_ARGS__)

#endif  //__LOG_H__
