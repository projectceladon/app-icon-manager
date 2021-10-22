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

#include <iostream>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#include "lgclient.h"
#include "connfactory.h"
#include "connmgr.h"
#include "eventqueue.h"
#include "common.h"

extern int compose_msg_body(char* dst, int max_len, int event, char* event_data);
extern int get_key_value (char* src, char* key, char* ret_buf, char* kv_separtor, char* key_separator);

using namespace std;

LGClient::LGClient()
{

}

LGClient::~LGClient()
{

}

int LGClient::connectToVatClient()
{
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char port[16];
    snprintf(port, sizeof(port), "%d", m_cli_port);

    if ((rv = getaddrinfo(m_cli_addr, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and connect to the first available
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((m_sock = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(m_sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(m_sock);
            perror("client: connect");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
	freeaddrinfo(servinfo);
        return -1;
    }

    freeaddrinfo(servinfo);

    return m_sock;
}

void LGClient::Destroy()
{
    if (!release_res) {
	ReleaseRes();
    }
    if (m_thread_created) {
	pthread_join(m_execlg, NULL);
    }
}

int LGClient::Init()
{
    memset (lg_instance_id, 0, sizeof(lg_instance_id));
    int ret = 0;
    ret = connectToVatClient();

    m_event_queue = new EventQueue();
    m_connmgr     = new Connmgr();
    m_connmgr->setEventQueue (m_event_queue);
    m_connmgr->setCommSock (m_sock);
    m_connmgr->connUp();

    return ret;
}

void LGClient::setCliCommParams(char* client, int port)
{
    m_cli_addr = client;
    m_cli_port = port;
}

int LGClient::loadApp()
{
    char cmd[512];
#ifdef LG_SINGLE_MODE
    if (m_app_icon_label.length() > 0) {
        snprintf (cmd, sizeof(cmd), "/opt/cfc/mwc/bin/loadapp_single_lg.sh \"%s\" \"%s\" \"%s\" \"%s\"", m_appname, m_activity, lg_instance_id, m_app_icon_label.c_str());
    }
    else {
        snprintf (cmd, sizeof(cmd), "/opt/cfc/mwc/bin/loadapp_single_lg.sh \"%s\" \"%s\" \"%s\" \"%s\"", m_appname, m_activity, lg_instance_id, "安卓应用");
    }
#else
    snprintf (cmd, sizeof(cmd), "/opt/cfc/mwc/bin/loadapp.sh %s %s %s", m_appname, m_activity, lg_instance_id);
#endif
    printf("cmd: %s\n", cmd);
    system (cmd);
    return 0;
}

int LGClient::killApp()
{
#ifndef LG_SINGLE_MODE
    char cmd[512];
    snprintf (cmd, sizeof(cmd), "/opt/cfc/mwc/bin/killapp.sh %s %s %s", m_appname, m_activity, lg_instance_id);
    system (cmd);
#endif

    if (running) {
        char lgmsg_body[256];
        snprintf (lgmsg_body, sizeof(lgmsg_body), "{lg_instance_id=%s;}", lg_instance_id);

        char msg_body[512];
        compose_msg_body(msg_body, sizeof(msg_body), EVENT_NOTIFY_LG_APP_CLOSED, lgmsg_body);
        m_connmgr->sendMsg (msg_body, (int) sizeof(msg_body));
    }
    return 0;
}

int LGClient::setEndMode(int endmode)
{
    m_end_mode = endmode;
    return m_end_mode;
}

void LGClient::setAppName (char* appname)
{
    snprintf(m_appname, sizeof(m_appname), "%s", appname);
}

void LGClient::setActivity (char* activity)
{
    snprintf(m_activity, sizeof(m_activity), "%s", activity);
}

void LGClient::setPkgName (char* pkgname)
{
    snprintf(m_pkgname, sizeof(m_pkgname), "%s", pkgname);
}

void LGClient::setAppIconLabel(char* appiconlabel)
{
    printf("LGClient::setAppIconLabel, appiconlable:%s\n", appiconlabel);
    if (appiconlabel) {
        m_app_icon_label = appiconlabel;
    }
}

void* LGClient::execLG(void* data)
{
    LGClient* instance = (LGClient*) data;
    instance->loadApp();
    instance->killApp();
    if (instance->running) {
        instance->running = 0;
        instance->m_event_queue->awaken();
    }
    return 0;
}

int LGClient::Launch()
{
    running = 1;

    if (MODE_TIMER == m_end_mode)
    {
        //TODO: get run time estimation from test case.
        //TODO: start a timer.
    }

    char lgapp_body[256];
    snprintf (lgapp_body, sizeof(lgapp_body), "{appname=%s,activity=%s,};", m_appname, m_activity);
    char msg_body[512];
    compose_msg_body(msg_body, sizeof(msg_body), EVENT_REQ_CHECK_IF_APP_LAUNCHED, lgapp_body);
    m_connmgr->sendMsg (msg_body, (int) sizeof(msg_body));

    mainLoop();
    return 0;
}

int LGClient::closeApp()
{
    running = 1;
    char lgapp_body[256];
    snprintf (lgapp_body, sizeof(lgapp_body), "{appname=%s,pkgname=%s,};", m_appname, m_pkgname);
    char msg_body[512];
    compose_msg_body(msg_body, sizeof(msg_body), EVENT_REQ_APP_CLOSE_BY_APPNAME, lgapp_body);
    m_connmgr->sendMsg (msg_body, (int) sizeof(msg_body));
    mainLoop();
    return 0;

}

int LGClient::closeAppLastOpened()
{
    running = 1;
    char lgmsg_body[256];
    snprintf (lgmsg_body, sizeof(lgmsg_body), "{lg_instance_id=%s;}", lg_instance_id);

    char msg_body[512];
    compose_msg_body(msg_body, sizeof(msg_body), EVENT_REQ_CLOSE_APP_LAST_OPENED, lgmsg_body);
    m_connmgr->sendMsg (msg_body, (int) sizeof(msg_body));
    mainLoop();

    return result;
}

int LGClient::HandleEvent(Event* event)
{
    // Handle event here.
    bool need_exec_lg = false;
    char tmp_lg_instance_id[64];
    int inst_id;
    switch (event->event_type)
    {
	case EVENT_RES_CHECK_IF_APP_LAUNCHED:
	    get_key_value (event->event_data + 1,
                           (char*) "lg_instance_id",
                           tmp_lg_instance_id,
                           (char*) "=",
                           (char*) ";");
            inst_id = atoi(tmp_lg_instance_id);
	    if (-1 == inst_id) {
	        // If -1 is retunred, that means the app is not launched yet and idle LG
		// instance is available. If app is not launched and no idle LG instance
		// the EVENT_RES_IDLE_LG_SLOT_NOT_AVAILABLE event should be returned.
                char lgapp_body[256];
                snprintf (lgapp_body, sizeof(lgapp_body), "{appname=%s,activity=%s,};", m_appname, m_activity);
                char msg_body[512];
                compose_msg_body(msg_body, sizeof(msg_body), EVENT_REQ_GET_IDLE_LG_SLOT, lgapp_body);
                m_connmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    else {
		snprintf(lg_instance_id, sizeof(lg_instance_id), "%d", inst_id);
                //TODO: set app window as top most
		running = 0;
		result = 0;
	    }
            break;
	case EVENT_RES_IDLE_LG_SLOT_NOT_AVAILABLE:
	    running = 0;
	    result = -1;
	case EVENT_CONNECTION_CLOSED:
	    running = 0;
	    result = -1;
	    break;
	case EVENT_RES_IDEL_LG_SLOT:
	    get_key_value (event->event_data + 1,
                           (char*) "lg_instance_id",
                           lg_instance_id,
                           (char*) "=",
                           (char*) ",");
	    if (atoi(lg_instance_id) > -1) {
		char lgapp_body[256];
                snprintf (lgapp_body, sizeof(lgapp_body), "{appname=%s,appactivity=%s,lg_instance_id:%s,};", m_appname, m_activity, lg_instance_id);
                char msg_body[512];
                compose_msg_body(msg_body, sizeof(msg_body), EVENT_REQ_SET_LG_SLOT_APP_BUNDLE, lgapp_body);
                m_connmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    need_exec_lg = true;
	    break;
        case EVENT_NOTIFY_APP_DIED:
	    printf("lgclient EVENT_NOTIFY_APP_DIED\n");
	    {
                 char lgmsg_body[256];
                 snprintf (lgmsg_body, sizeof(lgmsg_body), "{lg_instance_id=%s;}", lg_instance_id);

                 char msg_body[512];
                 compose_msg_body(msg_body, sizeof(msg_body), EVENT_NOTIFY_LG_APP_CLOSED, lgmsg_body);
                 m_connmgr->sendMsg (msg_body, (int) sizeof(msg_body));

                 //char lgapp_body[256];
                 //snprintf (lgapp_body, sizeof(lgapp_body), "/opt/cfc/mwc/bin/kill_app_process.sh %s", m_appname);
		 //system(lgapp_body);
	    }
 
	    running = 0;
	    result = 0;
	    break;
	case EVENT_NOTIFY_APP_CLOSE_BY_APPNAME_NOTIFIED:
	    printf ("lgclient EVENT_NOTIFY_APP_CLOSE_BY_APPNAME_NOTIFIED recieved\n");
	    running = 0;
	    result = 0;
	    break;
	case EVENT_RES_CLOSE_APP_LAST_OPENED:
            get_key_value (event->event_data + 1,
                           (char*) "appname",
                           m_appname,
                           (char*) "=",
                           (char*) ",");
	    // make the pkgname same as appname.
	    snprintf(m_pkgname, sizeof(m_pkgname), "%s", m_appname);
	    get_key_value (event->event_data + 1,
                           (char*) "appactivity",
                           m_activity,
                           (char*) "=",
                           (char*) ",");
	    //release_res = false;
	    running = 0;
	    result = 0;
	    break;
	case EVENT_RES_NO_APP_LAST_OPENED:
	    running = 0;
	    result = -1;
	default:
	    break;
    }

    if (need_exec_lg && running) {
        pthread_create(&m_execlg, NULL, &LGClient::execLG, this);
	m_thread_created = true;
    }

    return 0;
}

char* LGClient::getAppName()
{
    return m_appname;
}

char* LGClient::getActivityName()
{
    return m_activity;
}


void LGClient::ReleaseRes()
{
    m_connmgr->connDown();
    delete m_connmgr;
    m_connmgr = NULL;
    close (m_sock);
    m_sock = -1;

    delete m_event_queue;
    m_event_queue = NULL;

}

int LGClient::mainLoop()
{
    Event *pevent = 0;
    while (running)
    {
        pevent = m_event_queue->DeQueue();
        if (pevent) {
            cout << "Test Client -> Event type: " << pevent->event_type << endl;
        }
	else {
	    cout << "NULL event returned, probably time to leave. running: " << running << endl;
	}
        //TODO: Let pipemgr handle the event
	if (pevent) {
            HandleEvent (pevent);

            // Free the event after the event process done
            // TODO: use event pool to avoid allocate/free Event object frequently.
            //delete pevent;
            m_event_queue->freeEventObj (pevent);
	}

    }

    if (release_res) {
        ReleaseRes();
    }

    return 0;
}

int LGClient::getResult()
{
    return result;
}
