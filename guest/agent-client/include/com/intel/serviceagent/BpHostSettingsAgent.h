#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_HOST_SETTINGS_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_HOST_SETTINGS_AGENT_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/intel/serviceagent/IHostSettingsAgent.h>

namespace com {

namespace intel {

namespace serviceagent {

class BpHostSettingsAgent : public ::android::BpInterface<IHostSettingsAgent> {
public:
explicit BpHostSettingsAgent(const ::android::sp<::android::IBinder>& _aidl_impl);
virtual ~BpHostSettingsAgent() = default;
::android::binder::Status getHostSetting(const ::android::String16& key, ::android::String16* _aidl_return) override;
::android::binder::Status setHostSetting(const ::android::String16& key, const ::android::String16& value, int32_t* _aidl_return) override;
};  // class BpHostSettingsAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_HOST_SETTINGS_AGENT_H_
