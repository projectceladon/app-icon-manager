#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_CLIPBOARD_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_CLIPBOARD_AGENT_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <com/intel/serviceagent/IClipboardListener.h>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

namespace com {

namespace intel {

namespace serviceagent {

class IClipboardAgent : public ::android::IInterface {
public:
DECLARE_META_INTERFACE(ClipboardAgent)
virtual ::android::binder::Status hasText(bool* _aidl_return) = 0;
virtual ::android::binder::Status getText(::android::String16* _aidl_return) = 0;
virtual ::android::binder::Status setText(const ::android::String16& text) = 0;
virtual ::android::binder::Status addClipboardListener(const ::android::sp<::com::intel::serviceagent::IClipboardListener>& listener) = 0;
virtual ::android::binder::Status removeClipboardListener(const ::android::sp<::com::intel::serviceagent::IClipboardListener>& listener) = 0;
enum Call {
  HASTEXT = ::android::IBinder::FIRST_CALL_TRANSACTION + 0,
  GETTEXT = ::android::IBinder::FIRST_CALL_TRANSACTION + 1,
  SETTEXT = ::android::IBinder::FIRST_CALL_TRANSACTION + 2,
  ADDCLIPBOARDLISTENER = ::android::IBinder::FIRST_CALL_TRANSACTION + 3,
  REMOVECLIPBOARDLISTENER = ::android::IBinder::FIRST_CALL_TRANSACTION + 4,
};
};  // class IClipboardAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_CLIPBOARD_AGENT_H_
