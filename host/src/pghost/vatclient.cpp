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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "vatclient.h"
#include "eventqueue.h"
//#include "pipemgr.h"
#include "common.h"

#include "connmgr.h"


using namespace std;

VatClient::VatClient()
{
}

VatClient::~VatClient()
{

}

void VatClient::setLauncherCommSock(int sock)
{
    m_socklauncher = sock;
}

void VatClient::setGlobalEvtQueue(EventQueue* queue)
{
    g_eventqueue = queue;
}

int VatClient::getLGSlotID()
{
    return slot_id;
}
char* VatClient::getAppname()
{
    return m_appname.c_str();
}

char* VatClient::getActivity()
{
    return m_activity.c_str();
}


extern int compose_msg_body(char* dst, int max_len, int event, char* event_data);
void VatClient::Close()
{
    char lg_instance_id[64];
    char msg_body[512];
    memset(lg_instance_id, 0, sizeof(lg_instance_id));
    memset(msg_body, 0, sizeof(msg_body));

    snprintf (lg_instance_id, sizeof(lg_instance_id), "{lg_instance_id=%d,};", slot_id);
    compose_msg_body(msg_body, sizeof(msg_body), EVENT_NOTIFY_APP_DIED, lg_instance_id);

    m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
}

bool VatClient::needRelease() {
    cout << "Called in needRelease(), running: " << running << endl;
    return (0 == running);
}

void VatClient::set_lg_slots(lgslot_t** slots)
{
    m_lg_slots = slots;
}

extern int get_event_id_by_name (char* evt_name);
extern int get_key_value (char* src, char* key, char* ret_buf, char* kv_separtor, char* key_separator);
void VatClient::EnQueueGlobalEvt (char* buf)
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

        cout << "The event data is : " << eventobj->event_data << endl;

        g_eventqueue->EnQueue (eventobj);
    }
}

#ifdef LG_SINGLE_MODE
int VatClient::HandleEventSingleLG(Event* event)
{
    int ret = EVENT_NEED_CARE;

    char activity[512];
    char appname[512];
    char lg_instance_id[64];
    char msg_body[512];
    memset(activity, 0, sizeof(activity));
    memset(appname, 0, sizeof(appname));
    memset(lg_instance_id, 0, sizeof(lg_instance_id));
    memset(msg_body, 0, sizeof(msg_body));

    int idle_slot = -1;
    int kill_lg_process = -1;

    char lgslot_body[64];
    char app_close_body[256];
    char app_last_opened[256];

    switch (event->event_type)
    {
	case EVENT_REQ_CHECK_IF_APP_LAUNCHED:
	    //send the EVENT_RES_CHECK_IF_APP_LAUNCHED event back
	    //client shoudl send the request message then: EVENT_REQ_GET_IDLE_LG_SLOT
	    snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d;}", -1);
	    char msg_body[512];
	    compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_CHECK_IF_APP_LAUNCHED, lgslot_body);
	    m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    break;
	case EVENT_REQ_GET_IDLE_LG_SLOT:
	    idle_slot = 0;
	    snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d,};", idle_slot);
	    compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_IDEL_LG_SLOT, lgslot_body);
	    m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    break;
	case EVENT_REQ_SET_LG_SLOT_APP_BUNDLE:
	    get_key_value (event->event_data + 1,
		    (char*) "appactivity",
		    activity,
		    (char*) "=",
		    (char*) ",");
	    get_key_value (event->event_data + 1,
		    (char*) "appname",
		    appname,
		    (char*) "=",
		    (char*) ",");
	    m_activity = activity;
	    m_appname = appname;

	    m_lg_slots[0]->slot_status = LGSLOT_USED;
	    snprintf(m_lg_slots[0]->appname, sizeof(m_lg_slots[0]->appname), "%s", appname);
	    snprintf(m_lg_slots[0]->activity, sizeof(m_lg_slots[0]->activity), "%s", activity);
	    break;
	case EVENT_REQ_APP_CRASH_BY_APPNAME:
	    {
		get_key_value (event->event_data + 1,
			(char*) "appname",
			appname,
			(char*) "=",
			(char*) ",");
                if (strstr(m_lg_slots[0]->appname, appname) && m_lg_slots[0]->slot_status != LGSLOT_IDLE) {
                            m_lg_slots[0]->slot_status = LGSLOT_IDLE;
                            memset(m_lg_slots[0]->appname, 0, sizeof(m_lg_slots[0]->appname));
                            memset(m_lg_slots[0]->activity, 0, sizeof(m_lg_slots[0]->activity));
                            kill_lg_process = 1;
                }
		snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d, kill_lg_process=%d,};", -1, kill_lg_process);
		char msg_body[512];
		compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_APP_CRASH_BY_APPNAME, lgslot_body);
		m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    running = 0;
	    break;
	case EVENT_REQ_APP_CLOSE_BY_APPNAME:
	    {
		get_key_value (event->event_data + 1,
			(char*) "appname",
			appname,
			(char*) "=",
			(char*) ",");
		// Send the slot of the launched app to client
		snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d;}", -1);
		char msg_body[512];
		compose_msg_body(msg_body, sizeof(msg_body), EVENT_NOTIFY_APP_CLOSE_BY_APPNAME_NOTIFIED, lgslot_body);
		m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
            // Compose the global message.
	    snprintf(app_close_body, sizeof(app_close_body), "{appname=%s,lg_instance_id=%d,};", appname, 0);
	    compose_msg_body(msg_body, sizeof(msg_body), EVENT_NOTIFY_APP_CLOSE_BY_APPNAME, app_close_body);
            EnQueueGlobalEvt (msg_body);
	    running = 0;
	    break;

	case EVENT_REQ_CLOSE_APP_LAST_OPENED:
	    if (m_lg_slots[0]->slot_status != LGSLOT_USED) {
                snprintf(app_last_opened, sizeof(app_last_opened), "{lg_instance_id=%d;};", -1);
                compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_NO_APP_LAST_OPENED, app_last_opened);
                m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    else {
                snprintf(app_last_opened, sizeof(app_last_opened), "{appname=%s,appactivity=%s,};", m_lg_slots[0]->appname, m_lg_slots[0]->activity);
                compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_CLOSE_APP_LAST_OPENED, app_last_opened);
                m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
                // Compose the global message.
                snprintf(app_close_body, sizeof(app_close_body), "{appname=%s,lg_instance_id=%d,};", m_lg_slots[0]->appname, 0);
                compose_msg_body(msg_body, sizeof(msg_body), EVENT_NOTIFY_APP_CLOSE_BY_APPNAME, app_close_body);
                EnQueueGlobalEvt (msg_body);
	    }
            running = 0;
            break;

	case EVENT_REQ_GET_APP_LAST_OPENED:
	    if (m_lg_slots[0]->slot_status != LGSLOT_USED) {
                snprintf(app_last_opened, sizeof(app_last_opened), "{lg_instance_id=%d;};", -1);
                compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_NO_APP_LAST_OPENED, app_last_opened);
                m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    else {
                snprintf(app_last_opened, sizeof(app_last_opened), "{appname=%s,appactivity=%s,};", m_lg_slots[0]->appname, m_lg_slots[0]->activity);
                compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_GET_APP_LAST_OPENED, app_last_opened);
                m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
            running = 0;
            break;

	case EVENT_NOTIFY_LG_APP_CLOSED:
	    running = 0;
	    break;
        case EVENT_CONNECTION_CLOSED:
           running = 0;
           break;
	default:
	    break;
    }

    return ret;
}
#endif




void* VatClient::run_loop (void* data)
{
    VatClient* vatclient = (VatClient*) data;
    int ret;
    Event* pevent = 0;
    while (vatclient->running) {
	pevent = vatclient->m_eventqueue->DeQueue();
	if (pevent) {
#ifdef LG_SINGLE_MODE
	    ret = vatclient->HandleEventSingleLG (pevent);
#else
	    ret = vatclient->HandleEvent (pevent);
#endif
	    vatclient->m_eventqueue->freeEventObj(pevent);
	}
    }
    return NULL;
}


int VatClient::HandleEvent(Event* event)
{
    int ret = EVENT_NEED_CARE;

    char activity[512];
    char appname[512];
    char lg_instance_id[64];
    char msg_body[512];
    memset(activity, 0, sizeof(activity));
    memset(appname, 0, sizeof(appname));
    memset(lg_instance_id, 0, sizeof(lg_instance_id));
    memset(msg_body, 0, sizeof(msg_body));

    int slot_found = -1;
    int idle_slot = -1;
    int kill_lg_process = -1;

    char lgslot_body[64];
    int lg_slot = -1;
    char app_close_body[256];
    char app_last_opened[256];

    switch (event->event_type)
    {
	case EVENT_REQ_CHECK_IF_APP_LAUNCHED:
	    get_key_value (event->event_data + 1,
		    (char*) "appactivity",
		    activity,
		    (char*) "=",
		    (char*) ",");
	    // Check if the app to be launched has been running already
	    // And also check if there are idle slots.
	    for (int i=0; i<NUM_LG_SLOTS; i++) {
		if (LGSLOT_USED == m_lg_slots[i]->slot_status) {
		    char* launched_activity = m_lg_slots[i]->activity;
		    if (strstr(launched_activity, activity)) {
			slot_found = i;
			break;
		    }
		}
		if (LGSLOT_IDLE == m_lg_slots[i]->slot_status) {
		    idle_slot = i;
		}
	    }
	    printf("slot_found: %d, idle_slot: %d\n", slot_found, idle_slot);
	    if (slot_found > -1) {
		slot_id = slot_found;
		// Send the slot of the launched app to client
		snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d;}", slot_found);
		char msg_body[512];
		compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_CHECK_IF_APP_LAUNCHED, lgslot_body);
		m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    else {
		if (idle_slot > -1) {
		    //send the EVENT_RES_CHECK_IF_APP_LAUNCHED event back
		    //client shoudl send the request message then: EVENT_REQ_GET_IDLE_LG_SLOT
		    char lgslot_body[64];
		    snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d;}", -1);
		    char msg_body[512];
		    compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_CHECK_IF_APP_LAUNCHED, lgslot_body);
		    m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
		}
		else {
		    // send the EVENT_RES_IDLE_LG_SLOT_NOT_AVAILABLE event back
		    running = 0;
		    char lgslot_body[64];
		    snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d;}", -1);
		    char msg_body[512];
		    compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_IDLE_LG_SLOT_NOT_AVAILABLE, lgslot_body);
		    m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
		}
	    }
	    break;
	case EVENT_REQ_GET_IDLE_LG_SLOT:
	    idle_slot = -1;
	    for (int i=0; i<NUM_LG_SLOTS; i++) {
		if (LGSLOT_IDLE == m_lg_slots[i]->slot_status) {
		    printf("found one idle slot:%d and set the slot as reserved\n", i);
		    idle_slot = i;
		    slot_id = idle_slot;
		    m_lg_slots[i]->slot_status = LGSLOT_RESERVED;
		    break;
		}
	    }
	    printf("idle_slot:%d\n", idle_slot);
	    if (idle_slot > -1) {
		snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d,};", idle_slot);
		char msg_body[512];
		compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_IDEL_LG_SLOT, lgslot_body);
		m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    else {
		snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d,};", -1);
		char msg_body[512];
		compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_IDLE_LG_SLOT_NOT_AVAILABLE, lgslot_body);
		m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    break;
	case EVENT_REQ_SET_LG_SLOT_APP_BUNDLE:
	    get_key_value (event->event_data + 1,
		    (char*) "appactivity",
		    activity,
		    (char*) "=",
		    (char*) ",");
	    get_key_value (event->event_data + 1,
		    (char*) "appname",
		    appname,
		    (char*) "=",
		    (char*) ",");
	    get_key_value (event->event_data + 1,
		    (char*) "lg_instance_id",
		    lg_instance_id,
		    (char*) "=",
		    (char*) ",");
	    lg_slot = atoi(lg_instance_id);
	    if (lg_slot < NUM_LG_SLOTS) {
		m_lg_slots[lg_slot]->slot_status = LGSLOT_USED;
		slot_id = lg_slot;
		snprintf(m_lg_slots[lg_slot]->appname, sizeof(m_lg_slots[lg_slot]->appname), "%s", appname);
		snprintf(m_lg_slots[lg_slot]->activity, sizeof(m_lg_slots[lg_slot]->activity), "%s", activity);
	    }
	    for (int i=0; i<NUM_LG_SLOTS; i++) {
		printf("slot:%d, status:%d, appname:%s, activity:%s\n", i, m_lg_slots[i]->slot_status, m_lg_slots[i]->appname, m_lg_slots[i]->activity);
	    }
	    break;
	case EVENT_REQ_APP_CRASH_BY_APPNAME:
	    get_key_value (event->event_data + 1,
		    (char*) "appname",
		    appname,
		    (char*) "=",
		    (char*) ",");
	    // Check if the app to be killed has been running already
	    for (int i=0; i<NUM_LG_SLOTS; i++) {
		if (LGSLOT_USED == m_lg_slots[i]->slot_status) {
		    char* launched_appname = m_lg_slots[i]->appname;
		    if (strstr(launched_appname, appname)) {
                        m_lg_slots[i]->slot_status = LGSLOT_IDLE;
                        memset(m_lg_slots[i]->appname, 0, sizeof(m_lg_slots[i]->appname));
                        memset(m_lg_slots[i]->activity, 0, sizeof(m_lg_slots[i]->activity));
                        kill_lg_process = 1;
			slot_found = i;
			break;
		    }
		}
	    }
	    //if (slot_found > -1) {
		// Send the slot of the launched app to client
		snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d, kill_lg_process=%d,};", slot_found, kill_lg_process);
		char msg_body[512];
		compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_APP_CRASH_BY_APPNAME, lgslot_body);
		m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    //}

	    running = 0;
	    break;
	case EVENT_REQ_APP_CLOSE_BY_APPNAME:
	    get_key_value (event->event_data + 1,
		    (char*) "appname",
		    appname,
		    (char*) "=",
		    (char*) ",");
	    // Check if the app to be launched has been running already
	    for (int i=0; i<NUM_LG_SLOTS; i++) {
		if (LGSLOT_USED == m_lg_slots[i]->slot_status) {
		    char* launched_appname = m_lg_slots[i]->appname;
		    if (strstr(launched_appname, appname)) {
			slot_found = i;
			break;
		    }
		}
	    }
	    if (slot_found > -1) {
		// Send the slot of the launched app to client
		snprintf (lgslot_body, sizeof(lgslot_body), "{lg_instance_id=%d;}", -1);
		char msg_body[512];
		compose_msg_body(msg_body, sizeof(msg_body), EVENT_NOTIFY_APP_CLOSE_BY_APPNAME_NOTIFIED, lgslot_body);
		m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    
	    // Compose the global message.
	    snprintf(app_close_body, sizeof(app_close_body), "{appname=%s,lg_instance_id=%d,};", appname, slot_found);
	    compose_msg_body(msg_body, sizeof(msg_body), EVENT_NOTIFY_APP_CLOSE_BY_APPNAME, app_close_body);
	    EnQueueGlobalEvt (msg_body);
            

	    running = 0;
	    break;
	case EVENT_REQ_CLOSE_APP_LAST_OPENED:
	   get_key_value (event->event_data + 1,
		    (char*) "lg_instance_id",
		    lg_instance_id,
		    (char*) "=",
		    (char*) ",");
	    lg_slot = atoi(lg_instance_id);
	    if (lg_slot < NUM_LG_SLOTS) {
	        if (m_lg_slots[lg_slot]->slot_status != LGSLOT_USED) {
                    snprintf(app_last_opened, sizeof(app_last_opened), "{lg_instance_id=%d;};", -1);
                    compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_NO_APP_LAST_OPENED, app_last_opened);
                    m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	        }
	        else {
                    snprintf(app_last_opened, sizeof(app_last_opened), "{appname=%s,appactivity=%s,};", m_lg_slots[lg_slot]->appname, m_lg_slots[lg_slot]->activity);
                    compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_CLOSE_APP_LAST_OPENED, app_last_opened);
                    m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
                    // Compose the global message.
                    snprintf(app_close_body, sizeof(app_close_body), "{appname=%s,lg_instance_id=%d,};", m_lg_slots[lg_slot]->appname, lg_slot);
                    compose_msg_body(msg_body, sizeof(msg_body), EVENT_NOTIFY_APP_CLOSE_BY_APPNAME, app_close_body);
                    EnQueueGlobalEvt (msg_body);
	        }
	    }
            running = 0;
            break;

	case EVENT_NOTIFY_LG_APP_CLOSED:
	    get_key_value (event->event_data + 1,
		    (char*) "lg_instance_id",
		    lg_instance_id,
		    (char*) "=",
		    (char*) ";");
	    lg_slot = atoi(lg_instance_id);
	    if (lg_slot >=0 && lg_slot < NUM_LG_SLOTS) {
		m_lg_slots[lg_slot]->slot_status = LGSLOT_IDLE;
		memset(m_lg_slots[lg_slot]->appname, 0, sizeof(m_lg_slots[lg_slot]->appname));
                memset(m_lg_slots[lg_slot]->activity, 0, sizeof(m_lg_slots[lg_slot]->activity));
	    }
	    running = 0;
	    break;
	default:
            /*
	     * If unknown event sent to server, server sendback the error message to client.
	     * Client needs to exit by default after got the event EVENT_RES_UNKNOWN_EVENT.
	     * The closed client will trigger the socket read 0 size event in server. Server
	     * then could do the graceful shutdown accordingly.
	     */
	    {
		snprintf (lgslot_body, sizeof(lgslot_body), "{Error, unknown event sent to daemon!};");
		char msg_body[512];
		compose_msg_body(msg_body, sizeof(msg_body), EVENT_RES_UNKNOWN_EVENT, lgslot_body);
		m_launcherconnmgr->sendMsg (msg_body, (int) sizeof(msg_body));
	    }
	    break;
    }

    return ret;
}


void VatClient::readAndParseEvt()
{
    int ret = m_launcherconnmgr->readAndParseEvt();
    printf("readAndParseEvt, ret = %d\n", ret);
    if (ret <= 0) {
       printf("socket read error, ret: %d, vatclient to exit\n", ret);
        running = 0;
    }
}



void VatClient::Run()
{
    running = 1;

    m_eventqueue = new EventQueue();

    m_launcherconnmgr = new Connmgr();
    m_launcherconnmgr->setEventQueue (m_eventqueue);
    m_launcherconnmgr->setCommSock(m_socklauncher);

    m_launcherconnmgr->connUp(true);

    pthread_create(&m_client_loop, NULL, &VatClient::run_loop, this);
}

void VatClient::CleanUp()
{
    // Stop the instance and free the resources.
    m_launcherconnmgr->connDown();
    running = 0;
    m_eventqueue->awaken();
    pthread_join (m_client_loop, NULL);
    delete m_eventqueue;
    delete m_launcherconnmgr;

    if (slot_id >=0 && slot_id < NUM_LG_SLOTS) {
        if (LGSLOT_USED == m_lg_slots[slot_id]->slot_status) {
            m_lg_slots[slot_id]->slot_status = LGSLOT_IDLE;
            memset(m_lg_slots[slot_id]->appname, 0, sizeof(m_lg_slots[slot_id]->appname));
            memset(m_lg_slots[slot_id]->activity, 0, sizeof(m_lg_slots[slot_id]->activity));
	}
    }
}

