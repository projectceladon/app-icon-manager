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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <map>
#include <unordered_map>
#include <iostream>

#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "common.h"
#include "eventqueue.h"

#include "vatclient.h"
#define LISTENPORT 3001
#define LISTENQ 8
using namespace std;

static volatile int loop_running = 1;
std::map<int, VatClient*> clients;

typedef struct global_data_s {
   std::map<int, VatClient*>* clients;
   EventQueue*                eventqueue;
   volatile int running = 0;
} global_data_t;

static global_data_t g_data;
pthread_t    p_event_loop     = 0;

#ifdef LG_SINGLE_MODE
lgslot_t*    g_lg_slots[1];
#else
lgslot_t*    g_lg_slots[4];
#endif

extern int get_key_value (char* src, char* key, char* ret_buf, char* kv_separtor, char* key_separator);
#ifdef LG_SINGLE_MODE
static int HandleEvent(global_data_t* gdata, Event* event) {
    char appname[512];
    memset(appname, 0, sizeof(appname));
    cout <<"vatclidaemon::HandleEvent, event_data: " << event->event_data << endl;
    switch (event->event_type)
    {
        case EVENT_NOTIFY_APP_CLOSE_BY_APPNAME:
            {
                std::map<int, VatClient*>::iterator it = gdata->clients->begin();
                get_key_value (event->event_data + 1,
                           (char*) "appname",
                           appname,
                           (char*) "=",
                           (char*) ",");
	        while (it != gdata->clients->end())
                {
		    VatClient* vatclient = it->second;
		    if (strstr(vatclient->getAppname(),appname)) {
			if (strstr(g_lg_slots[0]->appname, appname) && g_lg_slots[0]->slot_status != LGSLOT_IDLE) {
			    g_lg_slots[0]->slot_status = LGSLOT_IDLE;
			    memset(g_lg_slots[0]->appname, 0, sizeof(g_lg_slots[0]->appname));
			    memset(g_lg_slots[0]->activity, 0, sizeof(g_lg_slots[0]->activity));
			}
                        vatclient->Close();
			break;
		    }
		    it++;
	        }
            }
            break;
	default:
            break;
    }

    return 0;
}
#else
static int HandleEvent(global_data_t* gdata, Event* event) {
    char appname[512];
    char lg_instance_id[64];
    memset(appname, 0, sizeof(appname));
    memset(lg_instance_id, 0, sizeof(lg_instance_id));
    int lg_slot = -1;
    cout <<"vatclidaemon::HandleEvent, event_data: " << event->event_data << endl;
    switch (event->event_type)
    {
        case EVENT_NOTIFY_APP_CLOSE_BY_APPNAME:
	    std::map<int, VatClient*>::iterator it = gdata->clients->begin();
            get_key_value (event->event_data + 1,
                           (char*) "lg_instance_id",
                           lg_instance_id,
                           (char*) "=",
                           (char*) ",");
	    lg_slot = atoi(lg_instance_id);
	    if (lg_slot > - 1) {
	        while (it != gdata->clients->end())
                {
		    VatClient* vatclient = it->second;
		    if (vatclient->getLGSlotID() == lg_slot) {
                        vatclient->Close();
			break;
		    }
		    it++;
	        }
		if (lg_slot >=0 && lg_slot < NUM_LG_SLOTS) {
		    if (strstr(g_lg_slots[lg_slot]->appname, appname) && g_lg_slots[lg_slot]->slot_status != LGSLOT_IDLE) {
			g_lg_slots[lg_slot]->slot_status = LGSLOT_IDLE;
			memset(g_lg_slots[lg_slot]->appname, 0, sizeof(g_lg_slots[lg_slot]->appname));
			memset(g_lg_slots[lg_slot]->activity,0, sizeof(g_lg_slots[lg_slot]->activity));
		    }
	        }
	    }
            break;
	default:
            break;
    }

    return 0;
}
#endif

static void* run_loop (void *data) {
    global_data_t* gdata = (global_data_t *) data;
    int ret;
    Event* pevent = 0;
    while (gdata->running) {
        pevent = gdata->eventqueue->DeQueue();
        if (pevent) {
            ret = HandleEvent (gdata, pevent);
            gdata->eventqueue->freeEventObj(pevent);
        }
    }
    return NULL;
}



#define EPOLL_MEVENTS 64

static int set_socket_nb(int fd_socket)
{
    int flags, s;

    flags = fcntl(fd_socket, F_GETFL, 0);
    if(flags == -1)
    {
        fprintf(stderr, "fcntl G_GETFL returns -1!\n");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl(fd_socket, F_SETFL, flags);
    if(s == -1)
    {
        fprintf(stderr, "fcntl F_SETFL returns -1!\n");
        return -1;
    }

    return 0;
}

int main (int argc, char **argv)
{

    if (argc < 1)
    {
        printf ("Usage: %s listen-port\n", argv[0]);
        exit(1);
    }

    memset(g_lg_slots, 0, sizeof(g_lg_slots));
    for (int i = 0; i<NUM_LG_SLOTS; i++) {
	lgslot_t* p_lgslot = new lgslot_t;
        if (p_lgslot) {
	    memset(p_lgslot, 0, sizeof(lgslot_t));
            p_lgslot->slot_status = LGSLOT_IDLE;
	    g_lg_slots[i] = p_lgslot;
	}
    }
    g_data.eventqueue = new EventQueue();
    g_data.clients = &clients;
    g_data.running = 1;
    pthread_create(&p_event_loop, NULL, &run_loop, &g_data);

    char* listenport = argv[1];

    int port = atoi(listenport);

    int listenfd;

    struct sockaddr_in servaddr;

    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Create a socket error!");
        exit(1);
    }

    const int on = 1;

    if (setsockopt (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
    {
        perror("setsocketopt failed");
        exit (1);
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons (port);

    bind (listenfd, (struct sockaddr *) & servaddr, sizeof(servaddr));
    int ret = set_socket_nb(listenfd);
    if(ret == -1) {
	printf("Error to set listening socket as non-blocking!");
        exit(-1);
    }

    listen (listenfd, LISTENQ);
    printf ("Client daemon is running, wait for connections...\n");

    struct epoll_event lg_epoll_event;
    struct epoll_event *epoll_events;
    int epoll_fd;
    epoll_fd = epoll_create1(0);
    if(epoll_fd == -1)
    {
        fprintf(stderr, "create epoll fd returns -1!\n");
        exit(-1);
    }

    lg_epoll_event.data.fd = listenfd;
    lg_epoll_event.events = EPOLLIN | EPOLLET;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &lg_epoll_event);
    if(ret == -1)
    {
        fprintf(stderr, "Add the epoll fd:%d return -1!\n", listenfd);
        exit(-1);
    }

    epoll_events = (struct epoll_event*) calloc(EPOLL_MEVENTS, sizeof(struct epoll_event));
    if (NULL == epoll_events) {
	fprintf(stderr, "epoll_events allocation fail, system may run out of memory, exit!\n");
        exit(-1);
    }

    do {
        fprintf(stdout, "Blocking and waiting for epoll event...\n");
        int num_events = epoll_wait(epoll_fd, epoll_events, EPOLL_MEVENTS, -1);
        fprintf(stdout, "Received epoll event\n");
        for(int i=0; i<num_events; i++)
        {
            if((epoll_events[i].events & EPOLLERR) ||
               (epoll_events[i].events & EPOLLHUP) ||
               (!(epoll_events[i].events & EPOLLIN)))
            {
                cout << "epoll error, i: " << i << " fd: " <<  epoll_events[i].data.fd << endl;
                int data_error_fd = epoll_events[i].data.fd;
                if (clients.find(data_error_fd) != clients.end()) {
                    VatClient* vatclient = clients.at(data_error_fd);
                    cout << "Remove the scoket fd: " << data_error_fd << " client: " << static_cast<void *> (vatclient) << endl;
                    clients.erase(data_error_fd);
                    vatclient->CleanUp();
                    delete vatclient;
                }
                cout << "Remove the socket fd: " << data_error_fd << " from the epoll list." << endl;
                int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, data_error_fd, NULL);
                if (ret >= 0) {
                    cout << "Removed the error socket fd from epoll: " << data_error_fd << endl;
                }
                else {
                    cout << "Error to remove error scoket fd from epoll fds: " << data_error_fd << " return: " << ret << endl;
                }
                close (data_error_fd);

                continue;
            }
            else if(listenfd == epoll_events[i].data.fd)
            {
		// Use loop to read all connection requests on listening socket fd.
                while(1)
                {
                    struct sockaddr socket_addr;
                    socklen_t socket_len;
                    char buf_host[NI_MAXHOST];
	            char buf_socket[NI_MAXSERV];

                    socket_len = sizeof(socket_addr);
                    int fd_req = accept(listenfd, &socket_addr, &socket_len);
                    if(fd_req == -1)
                    {
			    // All request read out, break the loop.
			    break;
                    }
                    ret = getnameinfo(&socket_addr, socket_len,
                                    buf_host, sizeof(buf_host),
                                    buf_socket, sizeof(buf_socket),
                                    NI_NUMERICHOST | NI_NUMERICSERV);
                    if(ret == 0)
                    {
			    cout << "New app launch requst accepted: " << fd_req << endl;
                    }

                    ret = set_socket_nb(fd_req);
                    if(ret == -1) {
                        cout << "Failed to set socket as non-blocking: " << fd_req << endl;
			close (fd_req);
			continue;
		    }

		    lg_epoll_event.data.fd = fd_req;
                    lg_epoll_event.events = EPOLLIN | EPOLLET;
                    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_req, &lg_epoll_event);
                    if(ret == -1)
                    {
			    cout << "Failed to add the app launcher fd: " << fd_req << " into epoll list!" << endl;
			    close (fd_req);
			    break;
                    }
		    // Create a new client and track the fd & client map
		    VatClient* vatclient = new VatClient();
		    vatclient->setLauncherCommSock(fd_req);
		    vatclient->setGlobalEvtQueue (g_data.eventqueue);
		    vatclient->set_lg_slots(g_lg_slots);
		    vatclient->Run();
		    clients.emplace (fd_req, vatclient);
                }
                continue;
            }
            else
            {
		// let the client to read data & handle te event accordingly.
		int data_ready_fd = epoll_events[i].data.fd;
		if (clients.find(data_ready_fd) != clients.end()) {
		    VatClient* vatclient = clients.at(data_ready_fd);
		    vatclient->readAndParseEvt();

		    if (vatclient->needRelease()) {
			cout << "fd: " << data_ready_fd << " vatclient->needRelease return true." << endl;
		        ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, data_ready_fd, NULL);
		        clients.erase(data_ready_fd);
			close(data_ready_fd);
			vatclient->CleanUp();
		        delete vatclient;
 		    }
		}
            }
        }
    } while (loop_running);
    g_data.running = 1;
    pthread_join (p_event_loop, NULL);
    exit (0);
}
