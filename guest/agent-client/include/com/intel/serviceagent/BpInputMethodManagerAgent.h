#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_INPUT_METHOD_MANAGER_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_INPUT_METHOD_MANAGER_AGENT_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/intel/serviceagent/IInputMethodManagerAgent.h>

namespace com {

namespace intel {

namespace serviceagent {

class BpInputMethodManagerAgent : public ::android::BpInterface<IInputMethodManagerAgent> {
public:
explicit BpInputMethodManagerAgent(const ::android::sp<::android::IBinder>& _aidl_impl);
virtual ~BpInputMethodManagerAgent() = default;
::android::binder::Status getInputMethodList(::std::vector<::android::String16>* _aidl_return) override;
::android::binder::Status getEnabledInputMethodList(::std::vector<::android::String16>* _aidl_return) override;
};  // class BpInputMethodManagerAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_INPUT_METHOD_MANAGER_AGENT_H_
