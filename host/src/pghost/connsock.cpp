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
#include <sys/mman.h>
#include <malloc.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "connsock.h"


Connection::~Connection()
{

}

int ConnSock::readheader (char* buf, int length)
{
    return read(buf, length);
}

int ConnSock::connReady()
{
    return conn_ready;
}

int ConnSock::read(char* buf, int length)
{
    int counts = 0;
    int count = 0;
    char* bufptr = buf;
    while (counts < length)
    {
         count = recv (m_sock, bufptr, length - counts, 0);
         if (count > 0) {
              counts += count;
              bufptr += count;
         }
         else if (count == 0) {
             //printf ("Socket: %d closed by peer!\n", m_sock);
             conn_ready = 0;
             return 0;
         }
         else {
             if (errno != EWOULDBLOCK) {
                 conn_ready = 0;
                 printf ("ConnSock::read => Error: recv returns: %d, errno: %d\n",
                         count, errno);
             }
             return -1;
         }
     }
     //printf ("ConnSock::read => length: %d, count: %d\n", length, count);
     return counts;
        
}

int ConnSock::readNonBlock(char* buf, int length)
{
    fd_set ready_sig;
    struct timeval tv;

    FD_ZERO(&ready_sig);
    FD_SET (m_sock, &ready_sig);

    tv.tv_sec  = 2;
    tv.tv_usec = 0;

    int retval = select (m_sock + 1, &ready_sig, 0, 0, &tv);

    if (-1 == retval)
    {
        printf ("select on socket: %d reset\n", m_sock);
        return -1;
    }
    else if (0 == retval)
    {
        printf ("select on socket: %d timeout\n", m_sock);
        return 0;
    }
    else {
        if (FD_ISSET(m_sock, &ready_sig))
        {
            int counts = 0;
            int count = 0;
            char* bufptr = buf;
            while (counts < length)
            {
                count = recv (m_sock, bufptr, length - counts, 0);
                if (count > 0) {
                    counts += count;
                    bufptr += count;
                }
                else if (count == 0) {
                    printf ("Socket: %d closed by peer!\n", m_sock);
                    conn_ready = 0;
                    return 0;
                }
                else {
                    if (errno != EWOULDBLOCK) {
                         conn_ready = 0;
                         printf ("ConnSock::read => Error: recv returns: %d, errno: %d\n",
                                 count, errno);
                    }
                    return -1;
                }
            }
            //printf ("ConnSock::read => length: %d, count: %d\n", length, count);
            return counts;
        }
        else {
            // Strange behavior, signal consumed by other threads?
            return 0;
        }
    }
        
}

int ConnSock::writeNonBlock(char* msg, int length)
{

    fd_set ready_sig;
    struct timeval tv;

    FD_ZERO(&ready_sig);
    FD_SET (m_sock, &ready_sig);

    tv.tv_sec  = 2;
    tv.tv_usec = 0;

    int counts = 0;

    int retval = select (m_sock + 1, 0, &ready_sig, 0, &tv);

    if (-1 == retval)
    {
        printf ("select on socket: %d reset\n", m_sock);
        return -1;
    }
    else if (0 == retval)
    {
        printf ("select on socket: %d timeout\n", m_sock);
        return 0;
    }
    else {
        if (FD_ISSET(m_sock, &ready_sig))
        {
            int count = 0;
            char* bufptr = msg;
            while (counts < length)
            {
                count = send (m_sock, bufptr, length - counts, 0);
                if (count >=0) {
                    counts += count;
                    bufptr += count;
                }
                else {
                    conn_ready = 0;
                    printf ("Error occurs in socket send call!\n");
                    return -1;
                }
            }
        }
    }
    //printf ("ConnSock::write => length: %d, ret: %d\n", length, ret);
    return counts;
}

int ConnSock::write(char* msg, int length)
{
    int counts = 0;
    int count = 0;
    char* bufptr = msg;
    while (counts < length)
    {
        count = send (m_sock, bufptr, length - counts, 0);
        if (count >=0) {
            counts += count;
            bufptr += count;
        }
        else {
            conn_ready = 0;
            printf ("Error occurs in socket send call!\n");
            return -1;
        }
    }
    //printf ("ConnSock::write => length: %d, ret: %d\n", length, ret);
    return counts;
}

void ConnSock::setCommSock (int sock)
{
    m_sock = sock;
    conn_ready = 1;
}

void ConnSock::setSockNonBlocking()
{
    int flags = fcntl(m_sock, F_GETFL, 0);
    fcntl(m_sock, F_SETFL, flags|O_NONBLOCK);   
    printf ("ConnSock::setSockNonBlocking called\n");
}

void ConnSock::stop()
{
    shutdown (m_sock, SHUT_RDWR);
}

ConnSock::ConnSock()
{

}


ConnSock::~ConnSock()
{

}
