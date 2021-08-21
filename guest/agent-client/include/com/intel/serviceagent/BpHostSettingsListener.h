#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_HOST_SETTINGS_LISTENER_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_HOST_SETTINGS_LISTENER_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/intel/serviceagent/IHostSettingsListener.h>

namespace com {

namespace intel {

namespace serviceagent {

class BpHostSettingsListener : public ::android::BpInterface<IHostSettingsListener> {
public:
explicit BpHostSettingsListener(const ::android::sp<::android::IBinder>& _aidl_impl);
virtual ~BpHostSettingsListener() = default;
::android::binder::Status getHostSetting(const ::android::String16& key, ::android::String16* _aidl_return) override;
::android::binder::Status setHostSetting(const ::android::String16& key, const ::android::String16& value, int32_t* _aidl_return) override;
};  // class BpHostSettingsListener

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_HOST_SETTINGS_LISTENER_H_
