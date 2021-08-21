#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_HOST_SETTINGS_LISTENER_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_HOST_SETTINGS_LISTENER_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <cstdint>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

namespace com {

namespace intel {

namespace serviceagent {

class IHostSettingsListener : public ::android::IInterface {
public:
DECLARE_META_INTERFACE(HostSettingsListener)
virtual ::android::binder::Status getHostSetting(const ::android::String16& key, ::android::String16* _aidl_return) = 0;
virtual ::android::binder::Status setHostSetting(const ::android::String16& key, const ::android::String16& value, int32_t* _aidl_return) = 0;
enum Call {
  GETHOSTSETTING = ::android::IBinder::FIRST_CALL_TRANSACTION + 0,
  SETHOSTSETTING = ::android::IBinder::FIRST_CALL_TRANSACTION + 1,
};
};  // class IHostSettingsListener

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_HOST_SETTINGS_LISTENER_H_
