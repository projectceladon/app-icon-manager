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

#ifndef __EVENTQUEUE_H__
#define __EVENTQUEUE_H__
#include <pthread.h>
//#include "queue.h"
#include <queue>
using namespace std;

#include <stdio.h>
#include <stdlib.h>

enum {
      EVENT_REQ_CHECK_IF_APP_LAUNCHED,
      EVENT_RES_CHECK_IF_APP_LAUNCHED,
      EVENT_REQ_GET_IDLE_LG_SLOT,
      EVENT_RES_IDLE_LG_SLOT_NOT_AVAILABLE,
      EVENT_RES_IDEL_LG_SLOT,
      EVENT_REQ_SET_LG_SLOT_APP_BUNDLE,
      EVENT_REQ_APP_CLOSE_BY_APPNAME,
      EVENT_NOTIFY_APP_DIED,
      EVENT_NOTIFY_APP_INSTALLED,
      EVENT_NOTIFY_APP_UNINSTALLED,
      EVENT_NOTIFY_LG_APP_CLOSED,
      EVENT_NOTIFY_APP_CLOSE_BY_APPNAME,
      EVENT_NOTIFY_APP_CLOSE_BY_APPNAME_NOTIFIED,
      EVENT_REQ_CLOSE_APP_LAST_OPENED,
      EVENT_RES_CLOSE_APP_LAST_OPENED,
      EVENT_RES_NO_APP_LAST_OPENED,
      EVENT_REQ_GET_APP_LAST_OPENED,
      EVENT_RES_GET_APP_LAST_OPENED,
      EVENT_CONNECTION_CLOSED,
      EVENT_REQ_APP_CRASH_BY_APPNAME,
      EVENT_RES_APP_CRASH_BY_APPNAME,
      EVENT_RES_UNKNOWN_EVENT
};

enum {EVENT_HANDLED, EVENT_NEED_CARE};

class Event {
    public:
	int event_type = 0;
	//void *event_data;
        char event_data[512];
	Event();
	~Event();
};

class EventQueue {
    public:
	int EnQueue (Event *event);
	Event* DeQueue ();

        Event* getEmptyEventObj();
        int freeEventObj (Event* event);
        int awaken();
	EventQueue();
	~EventQueue();
    private:
	//Queue <Event> m_msg_queue;
	queue<Event*> m_msg_queue;
	pthread_mutex_t m_queue_lock;
	pthread_cond_t m_queue_signal;

	queue <Event*> m_msg_queue_pool;
	pthread_mutex_t m_queue_pool_lock;

};

#endif
