#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_CLIPBOARD_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_CLIPBOARD_AGENT_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <com/intel/serviceagent/IClipboardAgent.h>

namespace com {

namespace intel {

namespace serviceagent {

class BpClipboardAgent : public ::android::BpInterface<IClipboardAgent> {
public:
explicit BpClipboardAgent(const ::android::sp<::android::IBinder>& _aidl_impl);
virtual ~BpClipboardAgent() = default;
::android::binder::Status hasText(bool* _aidl_return) override;
::android::binder::Status getText(::android::String16* _aidl_return) override;
::android::binder::Status setText(const ::android::String16& text) override;
::android::binder::Status addClipboardListener(const ::android::sp<::com::intel::serviceagent::IClipboardListener>& listener) override;
::android::binder::Status removeClipboardListener(const ::android::sp<::com::intel::serviceagent::IClipboardListener>& listener) override;
};  // class BpClipboardAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BP_CLIPBOARD_AGENT_H_
