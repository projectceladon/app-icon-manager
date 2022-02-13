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
*/

#ifndef __LGSLOT_H__
#define __LGSLOT_H__
#include <pthread.h>
using namespace std;

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

class LGSlot {
    public:
        LGSlot ();
        void SetLGSlots(lgslot_t** slots);
        int SetLGSlotIdle(int lgslot_id);
        int BundleLGSlotApp(int lgslot_id, char* appname, char* activity);
        int ReserveOneLGSlot();
	~LGSlot();
    private:
        //Slot array lock protection;
        pthread_mutex_t m_slots_lock;
        lgslot_t**   m_lg_slots        = NULL;

};

#endif
