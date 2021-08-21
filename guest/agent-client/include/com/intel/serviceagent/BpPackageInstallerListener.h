#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_PACKAGE_INSTALLER_LISTENER_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_PACKAGE_INSTALLER_LISTENER_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/intel/serviceagent/IPackageInstallerListener.h>

namespace com {

namespace intel {

namespace serviceagent {

class BpPackageInstallerListener : public ::android::BpInterface<IPackageInstallerListener> {
public:
explicit BpPackageInstallerListener(const ::android::sp<::android::IBinder>& _aidl_impl);
virtual ~BpPackageInstallerListener() = default;
::android::binder::Status onInstalled(const ::android::String16& pkg) override;
::android::binder::Status onUninstalled(const ::android::String16& pkg) override;
};  // class BpPackageInstallerListener

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_PACKAGE_INSTALLER_LISTENER_H_
