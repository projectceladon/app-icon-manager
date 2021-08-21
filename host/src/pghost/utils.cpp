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

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <string.h>


#include "eventqueue.h"

typedef struct s_event_map {
    const char * event_name;
    int  event_id;
} event_map_t;

event_map_t event_maps[] = {
    {"EVENT_REQ_CHECK_IF_APP_LAUNCHED",            EVENT_REQ_CHECK_IF_APP_LAUNCHED},
    {"EVENT_RES_CHECK_IF_APP_LAUNCHED",            EVENT_RES_CHECK_IF_APP_LAUNCHED},
    {"EVENT_REQ_GET_IDLE_LG_SLOT",                 EVENT_REQ_GET_IDLE_LG_SLOT},
    {"EVENT_RES_IDLE_LG_SLOT_NOT_AVAILABLE",       EVENT_RES_IDLE_LG_SLOT_NOT_AVAILABLE},
    {"EVENT_RES_IDEL_LG_SLOT",                     EVENT_RES_IDEL_LG_SLOT},
    {"EVENT_REQ_SET_LG_SLOT_APP_BUNDLE",           EVENT_REQ_SET_LG_SLOT_APP_BUNDLE},
    {"EVENT_REQ_APP_CLOSE_BY_APPNAME",             EVENT_REQ_APP_CLOSE_BY_APPNAME},
    {"EVENT_NOTIFY_APP_DIED",                      EVENT_NOTIFY_APP_DIED},
    {"EVENT_NOTIFY_APP_INSTALLED",                 EVENT_NOTIFY_APP_INSTALLED},
    {"EVENT_NOTIFY_APP_UNINSTALLED",               EVENT_NOTIFY_APP_UNINSTALLED},
    {"EVENT_NOTIFY_LG_APP_CLOSED",                 EVENT_NOTIFY_LG_APP_CLOSED},
    {"EVENT_NOTIFY_APP_CLOSE_BY_APPNAME",          EVENT_NOTIFY_APP_CLOSE_BY_APPNAME},
    {"EVENT_NOTIFY_APP_CLOSE_BY_APPNAME_NOTIFIED", EVENT_NOTIFY_APP_CLOSE_BY_APPNAME_NOTIFIED},
    {"EVENT_CONNECTION_CLOSED",                    EVENT_CONNECTION_CLOSED},
};

char* get_event_name_by_id(int event)
{
    char* event_name = 0;
    int i;
    int map_size = sizeof(event_maps)/sizeof(event_map_t);

    int matched = -1;
    for (i=0; i<map_size; i++)
    {
        if (event == event_maps[i].event_id)
        {
            matched = i;
            break;
        }
    }

    if (matched >= 0) {
        event_name = (char*) event_maps[matched].event_name;
    }

    return event_name;
}

int compose_msg_body(char* dst, int max_len, int event, char* event_data)
{
    int ret = 0;

    char* event_name = get_event_name_by_id (event);

    if (event_name)
    {
        char body_tmp[498];
        snprintf (body_tmp, sizeof(body_tmp), 
                  "EVENT_ID:%s;EVENT_DATA:%s",
                  event_name, event_data);
        int body_len = strlen(body_tmp);
        if (body_len > (int) sizeof(body_tmp))
        {
            body_len = sizeof(body_tmp);
            // raise error that message size exceeded the limite
        }
        snprintf (dst, max_len, "EVENT_LEN:%d;%s", body_len, body_tmp);
    }
    else {
        ret = -1;
    }

    return ret;
}

int get_event_id_by_name (char *evt_name)
{

   int ret = -1;

   int i;

   int map_size = sizeof(event_maps)/sizeof(event_map_t);

   for (i=0; i<map_size; i++)
   {
       if (0 == strcasecmp(event_maps[i].event_name, evt_name))
       {
           ret = event_maps[i].event_id;
           break;
       }
   }

   return ret;
}

int get_uniq_session_id()
{
    int ret = -1;
    srand(time(NULL));
    ret = rand();
    return ret;
}

int get_key_value (char* src, char* key, char* ret_buf, char* kv_separtor, char* key_separator)
{
    // be careful stack messing.
    //printf("src:%s, key:%s, ret_buf:%p, kv_separtor:%s, key_separator:%s\n", src, key, ret_buf, kv_separtor, key_separator);
    int len_key = strlen(key);
    char* pos_key = strstr(src, key);
    if (NULL == pos_key) {
        printf("Couldn't find the key: %s\n", key);
        return -1;
    }
    char* pos_value_start = pos_key + len_key + 1;
    char* pos_value_end = NULL;

    pos_value_end = strstr(pos_key, key_separator);

    if (NULL == pos_value_end) {
	printf ("Cound't find the separator: %s\n", key_separator);
	return -1;
    }
    int value_len = pos_value_end - pos_value_start;

    strncpy(ret_buf, pos_value_start, value_len);

    ret_buf[value_len] = '\0';

    //printf ("Key: %s, Value: %s\n", key, ret_buf);

    return 0;

}


static char* keyvalue_sep = (char *)":";
static char* key_sep = (char *)";";

