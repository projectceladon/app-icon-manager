#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_SERVICE_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_SERVICE_AGENT_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/intel/serviceagent/IServiceAgent.h>

namespace com {

namespace intel {

namespace serviceagent {

class BpServiceAgent : public ::android::BpInterface<IServiceAgent> {
public:
explicit BpServiceAgent(const ::android::sp<::android::IBinder>& _aidl_impl);
virtual ~BpServiceAgent() = default;
::android::binder::Status getPackageManagerAgent(::android::sp<::com::intel::serviceagent::IPackageManagerAgent>* _aidl_return) override;
::android::binder::Status getClipboardAgent(::android::sp<::com::intel::serviceagent::IClipboardAgent>* _aidl_return) override;
::android::binder::Status getHostSettingsAgent(::android::sp<::com::intel::serviceagent::IHostSettingsAgent>* _aidl_return) override;
::android::binder::Status addHostSettingsListener(const ::android::sp<::com::intel::serviceagent::IHostSettingsListener>& listener) override;
::android::binder::Status removeHostSettingsListener(const ::android::sp<::com::intel::serviceagent::IHostSettingsListener>& listener) override;
::android::binder::Status getInputMethodManagerAgent(::android::sp<::com::intel::serviceagent::IInputMethodManagerAgent>* _aidl_return) override;
};  // class BpServiceAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_SERVICE_AGENT_H_
