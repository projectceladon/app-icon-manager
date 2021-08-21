#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BN_PACKAGE_MANAGER_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BN_PACKAGE_MANAGER_AGENT_H_

#include <binder/IInterface.h>
#include <com/intel/serviceagent/IPackageManagerAgent.h>

namespace com {

namespace intel {

namespace serviceagent {

class BnPackageManagerAgent : public ::android::BnInterface<IPackageManagerAgent> {
public:
  explicit BnPackageManagerAgent();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnPackageManagerAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BN_PACKAGE_MANAGER_AGENT_H_
