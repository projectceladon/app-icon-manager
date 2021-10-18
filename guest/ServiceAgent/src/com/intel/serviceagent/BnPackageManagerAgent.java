/*
Copyright (C) 2021 Intel Corporation

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

Author: Mao Marc (marc.mao@intel.com)
Date: 2021.04.27

*/

package com.intel.serviceagent;

import android.app.Application;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.os.RemoteException;
import android.util.Log;

import android.util.DisplayMetrics;
import android.content.res.Resources;


import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class BnPackageManagerAgent extends IPackageManagerAgent.Stub {
    private static final String TAG = "ServiceAgent";
    private static final boolean DEBUG = false;

    private static final int PACKAGE_FLAG_ALL = 0;
    private static final int PACKAGE_FLAG_3RD_PARTY = 1;
    private static final int PACKAGE_FLAG_SYSTEM = 2;

    private static Application mApp = null;
    private static BnPackageManagerAgent mInst = null;
    private IPackageInstallerListener mInstallerListener = null;
    private static PackageManager mPm = null;

    private BnPackageManagerAgent(Application app) {
        mApp = app;
        mPm = mApp.getPackageManager();
        receiveBroadcast(mApp);
    }

    public static BnPackageManagerAgent getPackageManagerAgent(Application app) {
        if (mInst == null) {
            mInst = new BnPackageManagerAgent(app);
        }
        return mInst;
    }

    private Bitmap drawableToBitmap(Drawable drawable) {
        Bitmap bitmap = Bitmap.createBitmap(
                drawable.getIntrinsicWidth(),
                drawable.getIntrinsicHeight(),
                Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        drawable.setBounds(0, 0, drawable.getIntrinsicWidth(),
                drawable.getIntrinsicHeight());
        drawable.draw(canvas);
        return bitmap;
    }

    private BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent == null)
                return;
            final String a = intent.getAction();
            if ("android.intent.action.PACKAGE_ADDED".equals(a)) {
                String packageName = intent.getDataString();
                Log.d(TAG, "安装了 :" + packageName);
                if (mInstallerListener != null) {
                    try {
                        mInstallerListener.onInstalled(packageName);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
            }
            if ("android.intent.action.PACKAGE_REMOVED".equals(a)) {
                String packageName = intent.getDataString();
                Log.d(TAG, "卸载了 :" + packageName);
                if (mInstallerListener != null) {
                    try {
                        mInstallerListener.onUninstalled(packageName);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    };

    private void receiveBroadcast(Application app) {
        IntentFilter filter = new IntentFilter();
        filter.addAction("android.intent.action.PACKAGE_ADDED");
        filter.addAction("android.intent.action.PACKAGE_REMOVED");
        filter.addDataScheme("package");
        app.registerReceiver(broadcastReceiver, filter);
    }

    @Override
    public List<String> getPackageList(int flags) throws RemoteException {
        Log.d(TAG, "getPackageList");

        List<String> pkgNames = new ArrayList<>();
        List<PackageInfo> pkgInfos = mPm.getInstalledPackages(0);
        for (int i=0; i<pkgInfos.size(); i++) {
            PackageInfo pkgInfo = pkgInfos.get(i);
            final boolean isSystem =
                    (pkgInfo.applicationInfo.flags& ApplicationInfo.FLAG_SYSTEM) != 0;

            if ((flags == PACKAGE_FLAG_3RD_PARTY && !isSystem) ||
                (flags == PACKAGE_FLAG_SYSTEM && isSystem) ||
                (flags == PACKAGE_FLAG_ALL)) {
                pkgNames.add(pkgInfos.get(i).packageName);
                if (DEBUG) {
                    Log.d(TAG, pkgInfos.get(i).packageName);
                }
            }
        }
        return pkgNames;
    }

    @Override
    public String getAppName(String pkg) throws RemoteException {
	try {
             ApplicationInfo vappinfo = mPm.getApplicationInfo(pkg, 0);
             final String appname = (String) mPm.getApplicationLabel(vappinfo);
             return appname;
	}
	catch (Exception e) {
	    e.printStackTrace();
	}
        return null;
    }

    @Override
    public String getAppVersion(String pkg) throws RemoteException {
	try {
	    PackageInfo vpackageinfo = mPm.getPackageInfo(pkg, 0);
	    return vpackageinfo.versionName;
	}
	catch (Exception e) {
	    e.printStackTrace();
	}
        return null;
    }

    @Override
    public String getLaunchIntent(String pkg) throws RemoteException {
        Log.d(TAG, "getApplicationLaunchIntent");
        try {
            StringBuilder sb = new StringBuilder(pkg);
            Intent intent = mPm.getLaunchIntentForPackage(pkg);
            if (intent != null) {
                sb.append("/");
                sb.append(intent.getComponent().getClassName());
                Log.d(TAG, "intent for " + pkg +" " + intent.toString());
            }
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public byte[] getIcon(String pkg) throws RemoteException {
        Log.d(TAG, "getApplicationIcon");

        try {

            ApplicationInfo appInfo =
                mPm.getApplicationInfo(pkg, PackageManager.GET_META_DATA);
            Resources res = mPm.getResourcesForApplication(appInfo);
            Drawable applicationIcon;
            try {
                applicationIcon = res.getDrawableForDensity(appInfo.icon, DisplayMetrics.DENSITY_XHIGH, null);
            } catch (Resources.NotFoundException e) {
                Log.d(TAG, "getDrawableForDensity for: " + pkg + " raised Resource not found exception, try to call Packagemanger getApplicationIcon!");
                applicationIcon = mPm.getApplicationIcon(pkg);
            }

            Bitmap bm = drawableToBitmap(applicationIcon);

            // Use the fixed size 96*96 icon
            // TODO make it user configurable.
            Bitmap resized_bm = Bitmap.createScaledBitmap(bm, 96, 96, true);

            ByteArrayOutputStream os = new ByteArrayOutputStream();
            resized_bm.compress(Bitmap.CompressFormat.PNG, 100, os);
            byte[] data = os.toByteArray();

            if (DEBUG) {
                FileOutputStream fos = null;
                try {
                    File f = new File(mApp.getFilesDir() + "/2.png");
                    if (!f.exists()) {
                        f.createNewFile();
                    }
                    fos = new FileOutputStream(f);
                    fos.write(os.toByteArray());
                } catch (IOException e) {
                    e.printStackTrace();
                }
                try {
                    if (fos != null) {
                        fos.close();
                    }
                } catch (IOException e) {
                    // do nothing
                }
            }
            return data;
        }
        catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return new byte[0];
    }

    @Override
    public long getApkSize(String pkg) throws RemoteException {
	long apk_file_size = 0;
	try {

            ApplicationInfo appInfo =
                mPm.getApplicationInfo(pkg, PackageManager.GET_META_DATA);

            File apk_file = new File(appInfo.publicSourceDir);

            if (apk_file.exists()) {
                apk_file_size = apk_file.length();
            }

	    return apk_file_size;
	}
	catch (Exception e) {
	    e.printStackTrace();
	}
        return 0;
    }

    @Override
    public void addInstallerListener(IPackageInstallerListener listener) throws RemoteException {
        mInstallerListener = listener;
    }

    @Override
    public void removeInstallerListener(IPackageInstallerListener listener) throws RemoteException {
        mInstallerListener = null;
    }
}
