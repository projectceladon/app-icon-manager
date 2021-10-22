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

#ifndef __TESTCLIENT_H__
#define __TESTCLIENT_H__

#include <pthread.h>

class Connmgr;
class EventQueue;
class Event;

enum {MODE_AUTOEXIT, MODE_TIMER};

class LGClient
{
    public:
        LGClient();
        virtual ~LGClient();
        int Init();
        virtual int loadApp();
        virtual int killApp();
        // mode needs to be got from test case.
        int setEndMode(int endmode);
	void setAppName(char* appname);
        void setActivity(char* activity);
	void setPkgName(char* pkgname);
	void setAppIconLabel(char* appiconlabel);
	// send the close app message to daemon and let the daemon
	// to notify the registered client.
	int closeApp();
	int closeAppLastOpened();
	char* getAppName();
	char* getActivityName();

        int Launch();
        void setCliCommParams(char* client, int port);
        int getResult();
        void Destroy();
    protected:
        int HandleEvent(Event* event);
        int mainLoop();
        static void* execLG(void* data);
        int connectToVatClient();
    private:
        pthread_t m_execlg          = 0;
	bool     m_thread_created   = false;
        int      m_end_mode         = 0;
        int      session_id         = 0;
        int      result             = 0;
        void*    result_extra_data  = NULL;
        volatile int running        = 0;
        Connmgr* m_connmgr          = NULL ;
        char*    m_cli_addr         = NULL;
        int      m_cli_port         = 0;
        int      m_sock             = 0;
        EventQueue* m_event_queue   = NULL;
	char     lg_instance_id[64];
        char     m_activity[512];
	char     m_appname[512];
	char     m_pkgname[512];
	std::string     m_app_icon_label;
        int      m_exec_mode        = 0;
	volatile bool release_res   = true;
	void     ReleaseRes();
};

#endif
