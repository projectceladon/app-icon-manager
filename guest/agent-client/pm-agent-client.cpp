/*
Copyright (C) <2021> Intel Corporation
 
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
http://www.apache.org/licenses/LICENSE-2.0
 
Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions
and limitations under the License.
 

SPDX-License-Identifier: Apache-2.0

Authors:
Mao Marc (marc.mao@intel.com)
Wan Shuang (shuang.wan@intel.com)
Date: 2021.04.27

*/

#include <sys/stat.h>
#include <sys/types.h>

#include <binder/IPCThreadState.h>
#include "AgentService.h"

using namespace android;
using namespace com::intel::serviceagent;

class PmListener : public BnPackageInstallerListener {
    ::android::binder::Status onInstalled(
	    const ::android::String16& pkg) override {
	binder::Status status;

	printf("package:%s installed\n", String8(pkg).string());

	return status;
    }

    ::android::binder::Status onUninstalled(
	    const ::android::String16& pkg) override {
	binder::Status status;

	printf("package:%s uninstalled\n", String8(pkg).string());

	return status;
    }
};

class ClipboardListener : public BnClipboardListener {
    ::android::binder::Status onClipChanged() override {
	binder::Status status;

	printf("clipboard updated\n");

	return status;
    }
};

class HostSettingsListener : public BnHostSettingsListener {
    ::android::binder::Status getHostSetting(
	    const ::android::String16& key,
	    ::android::String16* _aidl_return) override {
	binder::Status status;

	printf("get host setting:%s\n", String8(key).string());

	return status;
    }

    ::android::binder::Status setHostSetting(const ::android::String16& key,
	    const ::android::String16& value,
	    int32_t* _aidl_return) override {
	binder::Status status;

	printf("set host:%s -> %s\n", String8(key).string(),
		String8(value).string());

	return status;
    }
};

static int dump_app_desktop_files ()
{
    sp<IServiceAgent> s = AgentService::getAgentService();
    sp<IPackageManagerAgent> pma;
    sp<IPackageInstallerListener> listener = new PmListener();
    sp<IClipboardAgent> cba;
    sp<IHostSettingsListener> hostSettingsListener = new HostSettingsListener();

    if (s != nullptr) {
	binder::Status status;

	status = s->getPackageManagerAgent(&pma);
	if (!status.isOk()) {
	    printf("Failed to call getPackageManagerAgent res=%s\n",
		    status.toString8().string());
	    return -1;
	}

	std::vector<::android::String16> pkgs;
	status = pma->getPackageList(0, &pkgs);
	if (!status.isOk()) {
	    printf("Failed to call IPackageManagerAgent res=%s\n",
		    status.toString8().string());
	    return -1;
	}
	if (access("/sdcard/applications", F_OK) !=0) {
	    mkdir("/sdcard/applications", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	printf("%zd packages gotten from remote\n", pkgs.size());
	for (auto& pkg : pkgs) {
	    String16 intent;
	    status = pma->getLaunchIntent(pkg, &intent);
	    if (!status.isOk()) {
		printf("Failed to call getLaunchIntent res=%s\n",
			status.toString8().string());
		return -1;
	    }
	    const char* slash = "/";
	    if (!String8(intent).contains(slash)) {
		continue;
	    }

	    printf("package:%s --> %s\n", String8(pkg).string(),
		    String8(intent).string());

	    std::string fapp_desktop_launcher = "/sdcard/applications/";
	    fapp_desktop_launcher.append(String8(pkg).string());
	    fapp_desktop_launcher.append(".desktop");

	    String16 appname;
            status = pma->getAppName(pkg, &appname);

	    if (!status.isOk()) {
		printf("Failed to call getAppName, res=%s\n",
			status.toString8().string());
		return -1;
	    }

	    if (access(fapp_desktop_launcher.c_str(), F_OK) != 0) {

		FILE* fp = fopen(fapp_desktop_launcher .c_str(), "wb");
		if (fp == 0) {
		    fprintf(stderr, "Failed to create desktop file %s:%s", fapp_desktop_launcher.c_str(),
			    strerror(errno));
		    return -1;
		}

		fprintf(fp, "[Desktop Entry]\n");
		fprintf(fp, "Name[zh_CN]=%s\n", String8(appname).string());
		fprintf(fp, "Comment=Android App %s\n", String8(pkg).string());
		fprintf(fp, "Exec=/opt/cfc/mwc/bin/lg_launcher.sh %s %s %s\n",String8(pkg).string(), String8(intent).string(), String8(appname).string());
		fprintf(fp, "Icon=~/.local/share/applications/icon/%s.png\n",  String8(pkg).string());
		fprintf(fp, "Type=Application\n");
		fprintf(fp, "Terminal=false\n");
		fprintf(fp, "Categories=Application\n");
		fprintf(fp, "X-Desktop-File-Install-Version=0.24\n");

		fclose(fp);

	    }


	    std::vector<uint8_t> icon;
	    status = pma->getIcon(pkg, &icon);
	    if (!status.isOk()) {
		printf("Failed to call getLaunchIntent res=%s\n",
			status.toString8().string());
		return -1;
	    }
	    if (icon.size() > 0) {
		char path[256];
		if (access("/sdcard/applications/icon", F_OK) != 0) {
		    mkdir("/sdcard/applications/icon", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		snprintf(path, 256, "/sdcard/applications/icon/%s.png", String8(pkg).string());
		FILE* fp = fopen(path, "wb");
		if (fp == 0) {
		    printf("Failed to create png file\n");
		    return -1;
		}
		for (size_t i = 0; i < icon.size(); i++) {
		    uint8_t data = icon.at(i);
		    fwrite(&data, 1, 1, fp);
		}
		fclose(fp);
	    }
	}
    } else {
	printf("Failed to get servce\n");
    }

    printf(
	    "To test installer listener, try to install/uninstall package by adb\n");
    printf("test host settings listener by run host setting agent app");

    return 0;

}

int dump_app_json_strs()
{
    sp<IServiceAgent> s = AgentService::getAgentService();
    sp<IPackageManagerAgent> pma;
    sp<IPackageInstallerListener> listener = new PmListener();
    sp<IClipboardAgent> cba;
    sp<IHostSettingsListener> hostSettingsListener = new HostSettingsListener();

    if (s != nullptr) {
	binder::Status status;

	status = s->getPackageManagerAgent(&pma);
	if (!status.isOk()) {
	    printf("Failed to call getPackageManagerAgent res=%s\n",
		    status.toString8().string());
	    return -1;
	}

	std::vector<::android::String16> pkgs;
	status = pma->getPackageList(0, &pkgs);
	if (!status.isOk()) {
	    printf("Failed to call IPackageManagerAgent res=%s\n",
		    status.toString8().string());
	    return -1;
	}
	std::string finstalled_app_json = "/sdcard/installed_apps.json";
	FILE* fp = fopen(finstalled_app_json.c_str(), "wb");
	if (fp == 0) {
	    fprintf(stderr, "Failed to create json file %s:%s", finstalled_app_json.c_str(),
		    strerror(errno));
	    return -1;
	}

	bool first_line = true;
	fprintf (fp, "{\n  \"apps\":\n  [\n");
	for (auto& pkg : pkgs) {
	    String16 intent;
	    status = pma->getLaunchIntent(pkg, &intent);
	    if (!status.isOk()) {
		printf("Failed to call getLaunchIntent res=%s\n",
			status.toString8().string());
		return -1;
	    }
	    const char* slash = "/";
	    if (!String8(intent).contains(slash)) {
		continue;
	    }

	    if (!first_line) {
		fprintf(fp, ",\n");
	    }
	    if (first_line) {
		first_line = false;
	    }
            String16 appversion;
	    status = pma->getAppVersion(pkg, &appversion);
	    if (!status.isOk()) {
		printf("Failed to call getAppVersion, res=%s\n",
			status.toString8().string());
		return -1;
	    }
            String16 appname;
            status = pma->getAppName(pkg, &appname);
            if (!status.isOk()) {
		printf("Failed to call getAppName, res=%s\n",
			status.toString8().string());
		return -1;
	    }

	    fprintf (fp, "      {\n          \"Name\":\"%s\",\n          \"Intent\":\"%s\",\n          \"Version\":\"%s\",\n          \"AppLabel\":\"%s\"\n      }", String8(pkg).string(), String8(intent).string(), String8(appversion).string(), String8(appname).string());
	}
	fprintf(fp, "\n  ]\n}\n");
	fclose(fp);
    } else {
	printf("Failed to get servce\n");
    }

    return 0;
}

int dump_app_json_strs_v1()
{
    sp<IServiceAgent> s = AgentService::getAgentService();
    sp<IPackageManagerAgent> pma;
    sp<IPackageInstallerListener> listener = new PmListener();
    sp<IClipboardAgent> cba;
    sp<IHostSettingsListener> hostSettingsListener = new HostSettingsListener();

    if (s != nullptr) {
	binder::Status status;

	status = s->getPackageManagerAgent(&pma);
	if (!status.isOk()) {
	    printf("Failed to call getPackageManagerAgent res=%s\n",
		    status.toString8().string());
	    return -1;
	}

	std::vector<::android::String16> pkgs;
	status = pma->getPackageList(0, &pkgs);
	if (!status.isOk()) {
	    printf("Failed to call IPackageManagerAgent res=%s\n",
		    status.toString8().string());
	    return -1;
	}
	std::string finstalled_app_json = "/sdcard/installed_apps_v1.json";
	FILE* fp = fopen(finstalled_app_json.c_str(), "wb");
	if (fp == 0) {
	    fprintf(stderr, "Failed to create json file %s:%s", finstalled_app_json.c_str(),
		    strerror(errno));
	    return -1;
	}

	bool first_line = true;
	fprintf (fp, "{\n  \"apps\":\n  [\n");
	for (auto& pkg : pkgs) {
	    String16 intent;
	    status = pma->getLaunchIntent(pkg, &intent);
	    if (!status.isOk()) {
		printf("Failed to call getLaunchIntent res=%s\n",
			status.toString8().string());
		return -1;
	    }
	    const char* slash = "/";
	    if (!String8(intent).contains(slash)) {
		continue;
	    }

	    if (!first_line) {
		fprintf(fp, ",\n");
	    }
	    if (first_line) {
		first_line = false;
	    }
            String16 appversion;
	    status = pma->getAppVersion(pkg, &appversion);
	    if (!status.isOk()) {
		printf("Failed to call getAppVersion, res=%s\n",
			status.toString8().string());
		return -1;
	    }
            String16 appname;
            status = pma->getAppName(pkg, &appname);
            if (!status.isOk()) {
		printf("Failed to call getAppName, res=%s\n",
			status.toString8().string());
		return -1;
	    }
            int64_t apksize;
            status = pma->getApkSize(pkg, &apksize);
            if (!status.isOk()) {
                printf("Failed to call getApkSize, res=%s\n",
                        status.toString8().string());
                return -1;
            }

	    fprintf (fp, "      {\n          \"Name\":\"%s\",\n          \"Intent\":\"%s\",\n          \"Version\":\"%s\",\n          \"AppLabel\":\"%s\",\n          \"ApkSize\":%ld\n      }", String8(pkg).string(), String8(intent).string(), String8(appversion).string(), String8(appname).string(), apksize);
	}
	fprintf(fp, "\n  ]\n}\n");
	fclose(fp);
    } else {
	printf("Failed to get servce\n");
    }

    return 0;
}

int main(int argc, char* argv[]) {

    int ret = 0;
    if (argc == 2) {
	// dump the installed apps into json string
	const char* params = argv[1];
	if (params && !strcmp("dump-apps", params)) {
	    ret = dump_app_json_strs();
	}
        else if (params && !strcmp("dump-v1-apps", params)) {
            ret = dump_app_json_strs_v1();
        }
    }
    else {
	// dump the installed apps into .desktop files
	ret = dump_app_desktop_files();
    }

    return ret;
}
