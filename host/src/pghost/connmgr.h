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

#ifndef __CONNMGR_H__
#define __CONNMGR_H__

#include "eventqueue.h"
#include "connection.h"
#include <pthread.h>

class Connmgr
{
    public:
        Connmgr();
        int              setEventQueue(EventQueue* eventqueue);
        void             setCommSock (int sock);
        int              sendMsg(char* msg, int length);
	int              readAndParseEvt();
        int              connUp(bool epoll_mode = false);
        int              connDown();
        ~Connmgr();
    private:
        void             EnQueueEvt (char* buf);
	void             NotifyConnectionClose();
        Connection*      m_conn          = NULL;
	int              m_socket          = 0;
        EventQueue*      m_eventqueue    = NULL;
	pthread_mutex_t  m_sock_wait_lock;
	pthread_cond_t   m_sock_signal;

	bool             m_epoll_mode   = false;
        volatile int     running        = 0;
        static void*     conn_loop (void *data);
	static void*     server_mode_conn_loop (void *data);
        pthread_t        m_p_conn       = 0;
        
};

#endif

