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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <sys/types.h>
#include <pthread.h>

#include "lgslot.h"
#include "common.h"

LGSlot::LGSlot ()
{
    if (pthread_mutex_init (&m_slots_lock, 0)) {
        printf("LGSlot, error to init slots lock!\n");
    }
}

LGSlot::~LGSlot()
{
    if (pthread_mutex_destroy(&m_slots_lock)) {
        printf("~LGSlot, error to destroy slots lock!\n");
    }
}
void LGSlot::SetLGSlots(lgslot_t** slots)
{
    m_lg_slots = slots;
}

int LGSlot::SetLGSlotIdle(int lgslot_id)
{
    if (NULL == m_lg_slots) {
        return -1;
    }

    if (lgslot_id >=0 && lgslot_id < NUM_LG_SLOTS) {
        pthread_mutex_lock(&m_slots_lock);
        m_lg_slots[lgslot_id]->slot_status = LGSLOT_IDLE;
        memset(m_lg_slots[lgslot_id]->appname, 0, sizeof(m_lg_slots[lgslot_id]->appname));
        memset(m_lg_slots[lgslot_id]->activity, 0, sizeof(m_lg_slots[lgslot_id]->activity));
        pthread_mutex_unlock(&m_slots_lock);
    }
    else {
        return -1;
    }

    return lgslot_id;
}

int LGSlot::BundleLGSlotApp(int lgslot_id, char* appname, char* activity)
{
    if (NULL == m_lg_slots) {
        return -1;
    }

    if (lgslot_id >= 0 && lgslot_id < NUM_LG_SLOTS) {
        pthread_mutex_lock(&m_slots_lock);
        m_lg_slots[lgslot_id]->slot_status = LGSLOT_USED;
        snprintf(m_lg_slots[lgslot_id]->appname, sizeof(m_lg_slots[lgslot_id]->appname), "%s", appname);
        snprintf(m_lg_slots[lgslot_id]->activity, sizeof(m_lg_slots[lgslot_id]->activity), "%s", activity);
        pthread_mutex_unlock(&m_slots_lock);
    }
    else {
        return -1;
    }

    return lgslot_id;
}

int LGSlot::ReserveOneLGSlot()
{
    if (NULL == m_lg_slots) {
        return -1;
    }

    int idle_slot = -1;
    pthread_mutex_lock(&m_slots_lock);
    for (int i=0; i<NUM_LG_SLOTS; i++) {
        if (LGSLOT_IDLE == m_lg_slots[i]->slot_status) {
            printf("found one idle slot:%d and set the slot as reserved\n", i);
            idle_slot = i;
            m_lg_slots[i]->slot_status = LGSLOT_RESERVED;
            break;
        }
    }
    pthread_mutex_unlock(&m_slots_lock);
    return idle_slot;
}
