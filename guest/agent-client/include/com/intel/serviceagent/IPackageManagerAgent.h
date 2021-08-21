#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_PACKAGE_MANAGER_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_PACKAGE_MANAGER_AGENT_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <com/intel/serviceagent/IPackageInstallerListener.h>
#include <cstdint>
#include <utils/String16.h>
#include <utils/StrongPointer.h>
#include <vector>

namespace com {

namespace intel {

namespace serviceagent {

class IPackageManagerAgent : public ::android::IInterface {
public:
  DECLARE_META_INTERFACE(PackageManagerAgent)
  virtual ::android::binder::Status addInstallerListener(const ::android::sp<::com::intel::serviceagent::IPackageInstallerListener>& listener) = 0;
  virtual ::android::binder::Status removeInstallerListener(const ::android::sp<::com::intel::serviceagent::IPackageInstallerListener>& listener) = 0;
  virtual ::android::binder::Status getPackageList(int32_t flags, ::std::vector<::android::String16>* _aidl_return) = 0;
  virtual ::android::binder::Status getLaunchIntent(const ::android::String16& pkg, ::android::String16* _aidl_return) = 0;
  virtual ::android::binder::Status getAppName(const ::android::String16& pkg, ::android::String16* _aidl_return) = 0;
  virtual ::android::binder::Status getAppVersion(const ::android::String16& pkg, ::android::String16* _aidl_return) = 0;
  virtual ::android::binder::Status getIcon(const ::android::String16& pkg, ::std::vector<uint8_t>* _aidl_return) = 0;
};  // class IPackageManagerAgent

class IPackageManagerAgentDefault : public IPackageManagerAgent {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status addInstallerListener(const ::android::sp<::com::intel::serviceagent::IPackageInstallerListener>&) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status removeInstallerListener(const ::android::sp<::com::intel::serviceagent::IPackageInstallerListener>&) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getPackageList(int32_t, ::std::vector<::android::String16>*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getLaunchIntent(const ::android::String16&, ::android::String16*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getAppName(const ::android::String16&, ::android::String16*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getAppVersion(const ::android::String16&, ::android::String16*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status getIcon(const ::android::String16&, ::std::vector<uint8_t>*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IPackageManagerAgentDefault

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_PACKAGE_MANAGER_AGENT_H_
