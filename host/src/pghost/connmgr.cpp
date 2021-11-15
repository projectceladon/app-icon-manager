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

#include <unistd.h>
#include <pthread.h>
#include <sstream>
#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "connmgr.h"
#include "connfactory.h"
#include "common.h"

using namespace std;


Connmgr::Connmgr()
{
    m_conn = ConnFactory::CreateConnInst("Socket");
}

int Connmgr::setEventQueue(EventQueue* eventqueue)
{
    m_eventqueue = eventqueue;
    return 0;
}

int Connmgr::connUp(bool epoll_mode)
{
    running = 1;
    m_epoll_mode = epoll_mode;
    // create the event thread here.
    if (!m_epoll_mode) {
	pthread_create (&m_p_conn, NULL, &Connmgr::conn_loop, this);
    }
    return 0;
}

int Connmgr::connDown()
{
    // join the thread here.
    if (m_conn) {
        m_conn->stop();
    }

    if (!m_epoll_mode) {
	running = 0;
	pthread_join (m_p_conn, NULL);
    }

    if (m_conn) {
        delete m_conn;
	m_conn = NULL;
    }

    return 0;
}

extern int get_event_id_by_name (char* evt_name);
extern int get_key_value (char* src, char* key, char* ret_buf, char* kv_separtor, char* key_separator);

extern int compose_msg_body(char* dst, int max_len, int event, char* event_data);

void Connmgr::EnQueueEvt (char* buf)
{

    // Event message format:
    // EVENT_LEN:<lenght of event message>;
    // EVENT_ID:<Event ID>;
    // EVENT_DATA:<Event Data>
    // Event data will be parsed by event handler and
    //   the sender needs to comply with the message 
    //   encapsulation definitions.

    char event_id[64];
    get_key_value (buf,
	    (char*) "EVENT_ID",
	    event_id,
	    (char*) ":",
	    (char*) ";");

    int event_type = get_event_id_by_name(event_id);

    char event_body[512];
    get_key_value (buf,
	    (char*) "EVENT_DATA",
	    event_body,
	    (char*) ":",
	    (char*) ";");
    char evtbodynew[512];

    snprintf (evtbodynew, sizeof(evtbodynew), "%s;}", event_body);

    Event *eventobj = 0;
    eventobj = m_eventqueue->getEmptyEventObj();
    if (eventobj) {
        eventobj->event_type = event_type;

        compose_msg_body(eventobj->event_data,
	        (int) sizeof(eventobj->event_data),
	        event_type,
	        evtbodynew);

        //cout << "The event data is : " << eventobj->event_data << endl;

        m_eventqueue->EnQueue (eventobj);
    }
}

void Connmgr::setCommSock (int sock)
{
    m_socket = sock;
    m_conn->setCommSock(sock);
}

int Connmgr::sendMsg(char* msg, int length)
{
    int ret = 0;

    ret = m_conn->write (msg, length);

    if (ret < 0) {
	cout << "Connmgr::sendMsg => send message error!\n";
    }
    else {
	cout << "Connmgr::sendMsg => MSG: " << msg << " sent." << endl;
    }

    return ret;
}

static const char* frame_header_pattern = "filetype:%d;colordepth:%d;width:%d;height:%d;frame_seq:%d;timestamp:%d;data_len:%d;";

int Connmgr::readAndParseEvt()
{
    int header_size = sizeof(databody_t);
    char header_buf[512];
    char event_buf[1024];
    int event_len;
    memset(header_buf, 0, header_size);
    volatile int read_counts = 0;
    event_len = 512;

    read_counts = m_conn->read(event_buf, event_len);
    printf("readAndParseEvt read_counts: %d, event_buf: %s\n", read_counts, event_buf);
    if (read_counts > 0) {
        EnQueueEvt(event_buf);
    }
    else {
        printf("Error, read count of bytes <= 0 !");
    }
    return read_counts;
}

void Connmgr::NotifyConnectionClose()
{
    int event_type = EVENT_CONNECTION_CLOSED;

    char event_body[512];
    snprintf(event_body, sizeof(event_body), "EVENT_DATA:{socket_fd:%d,};", m_socket);

    Event *eventobj = 0;
    eventobj = m_eventqueue->getEmptyEventObj();
    if (eventobj) {
        eventobj->event_type = event_type;

        compose_msg_body(eventobj->event_data,
	        (int) sizeof(eventobj->event_data),
	        event_type,
	        event_body);

        //cout << "The event data is : " << eventobj->event_data << endl;

        m_eventqueue->EnQueue (eventobj);
    }
}

void* Connmgr::conn_loop (void* data)
{

    Connmgr* connmgr = (Connmgr*) data;
    Connection* pconn = connmgr->m_conn;

    // Allocates event obj dynamically, needs to be optimized.
    char event_buf[1024];
    int event_len;
    volatile int read_counts = 0;
    while (connmgr->running)
    {
	event_len = 512;
	read_counts = pconn->read(event_buf, event_len);
	if (read_counts > 0) {
	    connmgr->EnQueueEvt(event_buf);
	}
	else {
	    // TODO: broadcast the socket closed message.
	    cout << "Read from socket returns: " << read_counts << ", close the connection." << endl;
	    connmgr->running = 0;
            connmgr->NotifyConnectionClose();
	}
    }
    return 0;
}

Connmgr::~Connmgr()
{

}

