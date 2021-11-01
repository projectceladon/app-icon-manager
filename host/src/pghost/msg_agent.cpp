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
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <assert.h>

#include "lgclient.h"
#include "common.h"

#include "AdbProxy.h"

typedef struct s_cmd_map {
    const char * cmd_name;
    int  cmd_id;
} cmd_map_t;

enum {
    CMD_CLOSEAPP,
    CMD_CLOSEAPP_LASTOPENED,
    CMD_GET_APP_LASTOPENED
};

cmd_map_t cmd_maps[] = {
    {"CLOSEAPP",                  CMD_CLOSEAPP},
    {"CLOSEAPP_LASTOPENED",       CMD_CLOSEAPP_LASTOPENED},
    {"GET_APP_LASTOPENED",        CMD_GET_APP_LASTOPENED},
};

int get_cmd_id_by_name (char *cmd_name)
{

   int ret = -1;

   int i;

   int map_size = sizeof(cmd_maps)/sizeof(cmd_map_t);

   for (i=0; i<map_size; i++)
   {
       if (0 == strcasecmp(cmd_maps[i].cmd_name, cmd_name))
       {
           ret = cmd_maps[i].cmd_id;
           break;
       }
   }

   return ret;
}


int main(int argc, char **argv)
{

    if (argc < 4 )
    {
        printf ("Usage: %s lg-daemon-address lg-daemon-port msg args\n",
                argv[0]);
        exit(1);
    }
    
    char* lg_daemon_server = argv[1];
    char* lg_daemon_port   = argv[2];
    int ret = 0;
    int cmd_id = get_cmd_id_by_name(argv[3]);
    if (cmd_id < 0) {
	ret = -1;
        printf ("Unknown arguments, argv[3]:%s, argc:%d\n", argv[3], argc);
    }
    else {
        switch (cmd_id)
	{
            case CMD_CLOSEAPP:
                {
                    char* appname            = argv[4];
                    char* pkgname         = argv[5];
                    int port = atoi (lg_daemon_port);
                    LGClient* client = new LGClient();
                    client->setEndMode (MODE_AUTOEXIT);
                    client->setAppName (appname);
                    client->setPkgName (pkgname);
                    client->setCliCommParams (lg_daemon_server, port);
                    client->Init();
                    client->closeApp();
                    ret = client->getResult();
                    client->Destroy();
                    delete client;
                }
                break;
	    case CMD_CLOSEAPP_LASTOPENED:
                {
                    int port = atoi (lg_daemon_port);
                    LGClient* client = new LGClient();
                    client->setEndMode (MODE_AUTOEXIT);
                    client->setCliCommParams (lg_daemon_server, port);
                    ret = client->Init();
                    if (ret >=0) {
                        // for daemon service to update launched app record.
                        ret = client->closeAppLastOpened();
                        if (ret >=0 ) {
                            // appname is same with pkgname right now.
                            char* appname = client->getAppName();
                            char* activity = client->getActivityName();
                            printf("appname: %s, activity: %s\n", appname, activity);
                            // close the app
                            AdbProxy* adb_proxy = AdbProxy().getInstance();
                            adb_proxy->closeActivity(appname);
                        }
                        ret = client->getResult();
                    }
                    client->Destroy();
                    delete client;
		}
                break;
	    case CMD_GET_APP_LASTOPENED:
                {
                    int port = atoi (lg_daemon_port);
                    LGClient* client = new LGClient();
                    client->setEndMode (MODE_AUTOEXIT);
                    client->setCliCommParams (lg_daemon_server, port);
                    ret = client->Init();
                    if (ret >=0) {
                        // for daemon service to update launched app record.
                        ret = client->getAppLastOpened();
                        if (ret >=0 ) {
                            // appname is same with pkgname right now.
                            char* appname = client->getAppName();
                            char* activity = client->getActivityName();
			    printf("appname: %s, activity: %s\n", appname, activity);
                        }
			else {
			    printf("no app opened previously!\n");
			}
                    }
                    client->Destroy();
                    delete client;
                }
                break;
            default:
                break;
	}
    }

    exit(ret);
}
