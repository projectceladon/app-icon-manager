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
#include "ShortcutMgrLg.h"

int main(int argc, char **argv)
{

    if (argc !=5 )
    {
        printf ("Usage: %s lg-daemon-address lg-daemon-port appname activity\n",
                argv[0]);
        exit(1);
    }

    char* lg_daemon_server = argv[1];
    char* lg_daemon_port   = argv[2];

    char* appname            = argv[3];
    char* activity         = argv[4];
    int ret = 0;
    int port = atoi (lg_daemon_port); 

//#ifdef LG_SINGLE_MODE_CMD
//    ShortcutMgrLg* scMgr = ShortcutMgrLg::getShortcutMgrLg();
//    scMgr->launchAppBlk(appname);
//#else
    LGClient* client = new LGClient();

    client->setEndMode (MODE_AUTOEXIT);
    client->setAppName (appname);
    client->setActivity (activity);

    client->setCliCommParams (lg_daemon_server, port);
    client->Init();
    client->Launch();

    ret = client->getResult();

    printf ("The LG Launch result is %d\n", ret);

    client->Destroy();

    delete client;
//#endif
    exit(ret);
}
