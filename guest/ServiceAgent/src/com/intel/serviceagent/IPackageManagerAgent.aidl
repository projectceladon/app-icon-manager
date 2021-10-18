// IPackageManagerAgent.aidl
package com.intel.serviceagent;

// Declare any non-default types here with import statements
import com.intel.serviceagent.IPackageInstallerListener;

interface IPackageManagerAgent {
    void addInstallerListener(IPackageInstallerListener listener);
    void removeInstallerListener(IPackageInstallerListener listener);
    List<String> getPackageList(int flags);
    String getLaunchIntent(String pkg);
    String getAppName(String pkg);
    String getAppVersion(String pkg);
    byte[] getIcon(String pkg);
    long getApkSize(String pkg);
}
