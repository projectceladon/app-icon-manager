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
import android.content.ClipData;
import android.content.ClipboardManager;
import android.os.RemoteException;
import android.util.Log;

import static android.content.Context.CLIPBOARD_SERVICE;

public class BnClipboardAgent extends IClipboardAgent.Stub implements ClipboardManager.OnPrimaryClipChangedListener {
    private static final String TAG = "ServiceAgent";
    private static Application mApp = null;
    private static BnClipboardAgent mInst = null;
    private static ClipboardManager mClipboardManager = null;
    private IClipboardListener mClipboardListener = null;

    private BnClipboardAgent(Application app) {
        mApp = app;
        mClipboardManager  = (ClipboardManager)mApp.getSystemService(CLIPBOARD_SERVICE);
        mClipboardManager.addPrimaryClipChangedListener(this);
    }

    public static BnClipboardAgent getClipboardAgent(Application app) {
        if (mInst == null) {
            mInst = new BnClipboardAgent(app);
        }
        return mInst;
    }

    @Override
    public boolean hasText() throws RemoteException {
        if (mClipboardManager.hasPrimaryClip()) {
            ClipData clipData = mClipboardManager.getPrimaryClip();
            if (clipData != null) {
                return true;
            }
        }
        return false;
    }

    @Override
    public String getText() throws RemoteException {
        Log.d(TAG, "get text from clipboard");
        String txt = new String("");
        if (mClipboardManager.hasPrimaryClip()) {
            ClipData clipData = mClipboardManager.getPrimaryClip();
            ClipData.Item item = clipData.getItemAt(0);
            txt = item.getText().toString();
        }
        return txt;
    }

    @Override
    public void setText(String text) throws RemoteException {
        Log.d(TAG, "set text " + text + " to clipboard");
        ClipData clipData;
        clipData = ClipData.newPlainText("txt", text);
        mClipboardManager.setPrimaryClip(clipData);
    }

    @Override
    public void addClipboardListener(IClipboardListener listener) throws RemoteException {
        mClipboardListener = listener;
    }

    @Override
    public void removeClipboardListener(IClipboardListener listener) throws RemoteException {
        mClipboardListener = null;
    }

    @Override
    public void onPrimaryClipChanged() {
        if (mClipboardListener != null){
            try {
                mClipboardListener.onClipChanged();
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    }
}
