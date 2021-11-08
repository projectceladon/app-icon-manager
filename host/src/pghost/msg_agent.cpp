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

    if (strstr("CLOSEAPP", argv[3]) !=0 && argc >= 5) {
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
    } else {
	if (strstr("CLOSEAPP_LASTOPENED", argv[3]) != 0) {
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
                    // close the app
	            AdbProxy* adb_proxy = AdbProxy().getInstance();
	            adb_proxy->closeActivity(appname);
	        }
	        ret = client->getResult();
            }
	    client->Destroy();
	    delete client;
	}
	else {
	    printf ("Unknown arguments, argv[3]:%s, argc:%d\n", argv[3], argc);
	}
    }

    exit(ret);
}
