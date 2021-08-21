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
*/

#ifndef KYDROIDAPI_H
#define KYDROIDAPI_H
#ifdef __cplusplus
extern "C" {
#endif

bool launch_app(char* pkgname, bool fullscreen, int width, int height, int density);

bool close_app(char* appname, char* pkgname);

bool install_app(char *filename, char *appname, char *pkgname);

bool uninstall_app(char* pkgname);

char* get_installed_applist();

bool is_android_env_installed();

bool is_android_env_running();

int get_android_version();

#ifdef __cplusplus
}
#endif

#endif
