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

#ifndef __VATCLIENT_H__
#define __VATCLIENT_H__

class EventQueue;
class PipeMgr;
class Event;
class Connmgr;

#include "common.h"

class VatClient
{
    public:
        VatClient();
        void setLauncherCommSock(int sock);
	void setGlobalEvtQueue(EventQueue* queue);
	void Run();
        void CleanUp();
	void readAndParseEvt();
	bool needRelease();
	void set_lg_slots(lgslot_t** slots);
	int getLGSlotID();
	void Close();
	char* getAppname();
	char* getActivity();
        ~VatClient();
    private:

	void         EnQueueGlobalEvt (char* buf);
        int          m_socklauncher    = 0;

        Connmgr*     m_launcherconnmgr = NULL;

        EventQueue*  m_eventqueue      = NULL;
	EventQueue*  g_eventqueue      = NULL;

        int          HandleEvent(Event* event);
	int          HandleEventSingleLG (Event* event);

        volatile int running = 1;
	static void* run_loop (void *data);
	pthread_t    m_client_loop     = 0;
	volatile int slot_id           = -1;
	lgslot_t**   m_lg_slots        = NULL;
	std::string  m_appname;
	std::string  m_activity;
};

#endif
