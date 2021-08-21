#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_PACKAGE_INSTALLER_LISTENER_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_PACKAGE_INSTALLER_LISTENER_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

namespace com {

namespace intel {

namespace serviceagent {

class IPackageInstallerListener : public ::android::IInterface {
public:
DECLARE_META_INTERFACE(PackageInstallerListener)
virtual ::android::binder::Status onInstalled(const ::android::String16& pkg) = 0;
virtual ::android::binder::Status onUninstalled(const ::android::String16& pkg) = 0;
enum Call {
  ONINSTALLED = ::android::IBinder::FIRST_CALL_TRANSACTION + 0,
  ONUNINSTALLED = ::android::IBinder::FIRST_CALL_TRANSACTION + 1,
};
};  // class IPackageInstallerListener

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_PACKAGE_INSTALLER_LISTENER_H_
