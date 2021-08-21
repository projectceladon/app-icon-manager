#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_CLIPBOARD_LISTENER_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_CLIPBOARD_LISTENER_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/intel/serviceagent/IClipboardListener.h>

namespace com {

namespace intel {

namespace serviceagent {

class BpClipboardListener : public ::android::BpInterface<IClipboardListener> {
public:
explicit BpClipboardListener(const ::android::sp<::android::IBinder>& _aidl_impl);
virtual ~BpClipboardListener() = default;
::android::binder::Status onClipChanged() override;
};  // class BpClipboardListener

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_CLIPBOARD_LISTENER_H_
