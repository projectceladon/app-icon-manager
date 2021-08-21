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
import android.os.RemoteException;
import android.util.Log;
import android.view.inputmethod.InputMethodInfo;
import android.view.inputmethod.InputMethodManager;

import java.util.ArrayList;
import java.util.List;

import static android.content.Context.INPUT_METHOD_SERVICE;

public class BnInputMethodManagerAgent extends IInputMethodManagerAgent.Stub {
    private static final String TAG = "ServiceAgent";
    private static final boolean DEBUG = false;

    private static Application mApp = null;
    private InputMethodManager mInputMethodManager = null;
    private static BnInputMethodManagerAgent mInst = null;

    public BnInputMethodManagerAgent(Application app) {
        mApp = app;
        mInputMethodManager = (InputMethodManager) mApp.getSystemService(INPUT_METHOD_SERVICE);
    }

    public static BnInputMethodManagerAgent getInputMethodManagerAgent(Application app) {
        if (mInst == null) {
            mInst = new BnInputMethodManagerAgent(app);
        }
        return mInst;
    }

    @Override
    public List<String> getInputMethodList() throws RemoteException {
        if (mInputMethodManager != null) {
            List<String> imeNames = new ArrayList<>();
            List<InputMethodInfo> imeInfos = mInputMethodManager.getInputMethodList();
            for (int i = 0; i < imeInfos.size(); i++) {
                InputMethodInfo imeInfo = imeInfos.get(i);
                String pkg = imeInfo.getPackageName();
                String svc = imeInfo.getServiceName();
                imeNames.add(pkg + "/" + svc.substring(svc.lastIndexOf('.')));
                if (DEBUG) {
                    Log.d(TAG, "installed IMEs:" + pkg + "/" + svc);
                }
            }
            return imeNames;
        }
        return null;
    }

    @Override
    public List<String> getEnabledInputMethodList() throws RemoteException {
        if (mInputMethodManager != null) {
            List<String> imeNames = new ArrayList<>();
            List<InputMethodInfo> imeInfos = mInputMethodManager.getEnabledInputMethodList();
            for (int i = 0; i < imeInfos.size(); i++) {
                InputMethodInfo imeInfo = imeInfos.get(i);
                String pkg = imeInfo.getPackageName();
                String svc = imeInfo.getServiceName();
                imeNames.add(pkg + "/" + svc.substring(svc.lastIndexOf('.')));
                if (DEBUG) {
                    Log.d(TAG, "installed IMEs:" + pkg + "/" + svc);
                }
            }
            return imeNames;
        }
        return null;
    }
}
