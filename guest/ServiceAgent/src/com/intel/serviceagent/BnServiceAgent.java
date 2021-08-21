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

public class BnServiceAgent extends IServiceAgent.Stub {
    private static final String TAG = "ServiceAgent";
    private static Application mApp = null;
    private static BnServiceAgent mInst = null;

    private BnServiceAgent(Application app){
        mApp = app;
    }

    public static BnServiceAgent getServiceAgent(Application app) {
        if (mInst == null) {
            mInst = new BnServiceAgent(app);
        }
        return mInst;
    }

    @Override
    public IPackageManagerAgent getPackageManagerAgent() throws RemoteException {
        return BnPackageManagerAgent.getPackageManagerAgent(mApp);
    }

    @Override
    public IClipboardAgent getClipboardAgent() throws RemoteException {
        return BnClipboardAgent.getClipboardAgent(mApp);
    }

    @Override
    public IHostSettingsAgent getHostSettingsAgent() throws RemoteException {
        return BnHostSettingsAgent.getHostSettingsAgent();
    }

    @Override
    public void addHostSettingsListener(IHostSettingsListener listener) throws RemoteException {
        BnHostSettingsAgent agent = BnHostSettingsAgent.getHostSettingsAgent();
        agent.addHostSettingsListener(listener);
    }

    @Override
    public void removeHostSettingsListener(IHostSettingsListener listener) throws RemoteException {
        BnHostSettingsAgent agent = BnHostSettingsAgent.getHostSettingsAgent();
        agent.removeHostSettigsListener(listener);
    }

    @Override
    public IInputMethodManagerAgent getInputMethodManagerAgent() throws RemoteException {
        return BnInputMethodManagerAgent.getInputMethodManagerAgent(mApp);
    }
}
