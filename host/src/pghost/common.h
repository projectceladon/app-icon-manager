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

#ifndef __COMMON_H__
#define __COMMON_H__

enum {MODE_TBD, MODE_PRECAPTURE, MODE_EXECUTION};

enum {LAUNCHER, VATSERVER};

#define MAX_FRAME_SIZE 1024000

enum {LGSLOT_IDLE, LGSLOT_RESERVED, LGSLOT_USED};
#ifdef LG_SINGLE_MODE
#define NUM_LG_SLOTS 1
#else
#define NUM_LG_SLOTS 2
#endif

typedef struct lgslot_s {
    int slot_status;
    char appname[512];
    char activity[512];
} lgslot_t;

enum {
    DATA_EVENT,
    DATA_DATA
};

typedef struct databody_s {
    int  datatype;
    int  length;
} databody_t;

#endif
