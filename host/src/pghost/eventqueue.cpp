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

#include "eventqueue.h"

Event* EventQueue::getEmptyEventObj()
{
    Event* eventobj = 0;

    if (pthread_mutex_lock (&m_queue_pool_lock)) {
        printf("Error to lock the event queue!\n");
	return NULL;
    }
    if (!m_msg_queue_pool.empty()) {
        eventobj = m_msg_queue_pool.front();
	m_msg_queue_pool.pop();
    }

    if (pthread_mutex_unlock (&m_queue_pool_lock)) {
        printf("Error to unlock the event queue!\n");
	return NULL;
    }

    if (0 == eventobj)
    {
        eventobj = new Event();
    }

    return eventobj;
}

int EventQueue::freeEventObj (Event* event)
{
    if (pthread_mutex_lock (&m_queue_pool_lock)) {
	printf("freeEventObj, error to lock the queue lock!\n");
    }
    m_msg_queue_pool.push (event);
    if (pthread_mutex_unlock (&m_queue_pool_lock)) {
	printf("freeEventObj, error to unlock the queue lock!\n");
    }
    return 0;
}

int EventQueue::EnQueue (Event *event)
{
    if (pthread_mutex_lock (&m_queue_lock)) {
	printf("EnQueue, error to lock queue lock!\n");
    }
    m_msg_queue.push (event);
    if (pthread_cond_signal (&m_queue_signal)) {
	printf("EnQueue, error to raise queue condition signal!\n");
    }
    if (pthread_mutex_unlock (&m_queue_lock)) {
	printf("EnQueue, error to unlock queue lock!\n");
    }
    return 0;
}

int EventQueue::awaken()
{
    if (pthread_cond_signal(&m_queue_signal)) {
	printf("swaken, error to raise queue condition signal!\n");
    }
    return 0;
}

Event* EventQueue::DeQueue()
{
    Event* curEvent = 0;
    if (pthread_mutex_lock (&m_queue_lock)) {
	printf("DeQueue, error to lock the queue lock!\n");
    }
    if (m_msg_queue.empty()) {
	if (pthread_cond_wait (&m_queue_signal, &m_queue_lock)) {
	    printf("DeQueue, error to wait queue condition signal!\n");
	}
    }

    if (!m_msg_queue.empty()) {
        curEvent = m_msg_queue.front();
        m_msg_queue.pop();
    }

    if (pthread_mutex_unlock (&m_queue_lock)) {
	printf("Dequeue: Error to unlock the queue lock!\n");
    }

    return curEvent;
}

EventQueue::EventQueue()
{
    if (pthread_mutex_init (&m_queue_lock, 0)) {
	printf("EventQueue, error to init queue lock!\n");
    }
    if (pthread_cond_init (&m_queue_signal, 0)) {
	printf("EventQueue, error to initialize event queue condition signal!\n");
    }
    if (pthread_mutex_init (&m_queue_pool_lock, 0)) {
	printf("EventQueue, error to init queue pool lock!\n");
    }
}

EventQueue::~EventQueue()
{
    if (pthread_mutex_destroy(&m_queue_lock)) {
	printf("~EventQueue, error to destroy queue lock!\n");
    }
    if (pthread_cond_destroy(&m_queue_signal)) {
	printf("~EventQueue, error to destory queue condition signal!\n");
    }
    if (pthread_mutex_destroy(&m_queue_pool_lock)) {
	printf("~EventQueue, error to destory queue pool lock!\n");
    }
}

Event::Event()
{

}

Event::~Event()
{
    //free(event_data);
}
