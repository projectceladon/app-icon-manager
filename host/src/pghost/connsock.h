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

#ifndef __CONNSOCK_H__
#define __CONNSOCK_H__

#include "connection.h"

class ConnSock : public Connection
{
    public:
        int    readheader (char* buf, int length);
        int    read(char* buf, int length);
        int    readNonBlock(char* buf, int length);
        int    write(char* msg, int length);
        int    writeNonBlock(char* msg, int length);
        int    connReady();
        void   setCommSock (int sock);
        void   setSockNonBlocking();
        void   stop();
        ConnSock ();
        virtual ~ConnSock();
    private:
        int   m_sock               = 0;
        volatile int   conn_ready  = 0;

};

#endif

