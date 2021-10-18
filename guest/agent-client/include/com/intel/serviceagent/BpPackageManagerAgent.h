#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_PACKAGE_MANAGER_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_PACKAGE_MANAGER_AGENT_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/intel/serviceagent/IPackageManagerAgent.h>

namespace com {

namespace intel {

namespace serviceagent {

class BpPackageManagerAgent : public ::android::BpInterface<IPackageManagerAgent> {
public:
  explicit BpPackageManagerAgent(const ::android::sp<::android::IBinder>& _aidl_impl);
  virtual ~BpPackageManagerAgent() = default;
  ::android::binder::Status addInstallerListener(const ::android::sp<::com::intel::serviceagent::IPackageInstallerListener>& listener) override;
  ::android::binder::Status removeInstallerListener(const ::android::sp<::com::intel::serviceagent::IPackageInstallerListener>& listener) override;
  ::android::binder::Status getPackageList(int32_t flags, ::std::vector<::android::String16>* _aidl_return) override;
  ::android::binder::Status getLaunchIntent(const ::android::String16& pkg, ::android::String16* _aidl_return) override;
  ::android::binder::Status getAppName(const ::android::String16& pkg, ::android::String16* _aidl_return) override;
  ::android::binder::Status getAppVersion(const ::android::String16& pkg, ::android::String16* _aidl_return) override;
  ::android::binder::Status getIcon(const ::android::String16& pkg, ::std::vector<uint8_t>* _aidl_return) override;
  ::android::binder::Status getApkSize(const ::android::String16& pkg, int64_t* _aidl_return) override;
};  // class BpPackageManagerAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_PACKAGE_MANAGER_AGENT_H_
