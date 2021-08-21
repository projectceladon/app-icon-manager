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

#ifndef __CONNECTION_H__
#define __CONNECTION_H__

class Connection
{
    public:
        virtual int   readheader(char* buf, int length) = 0;
        virtual int   read(char* buf, int length) = 0;
        virtual int   write(char* msg, int length) = 0;
        virtual void  setCommSock (int sock) = 0;
        virtual void  setSockNonBlocking() = 0;
        virtual int   connReady() = 0;
        virtual void  stop() = 0;
        virtual ~Connection();
};


#endif

