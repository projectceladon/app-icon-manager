#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_CLIPBOARD_LISTENER_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_CLIPBOARD_LISTENER_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <utils/StrongPointer.h>

namespace com {

namespace intel {

namespace serviceagent {

class IClipboardListener : public ::android::IInterface {
public:
DECLARE_META_INTERFACE(ClipboardListener)
virtual ::android::binder::Status onClipChanged() = 0;
enum Call {
  ONCLIPCHANGED = ::android::IBinder::FIRST_CALL_TRANSACTION + 0,
};
};  // class IClipboardListener

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_CLIPBOARD_LISTENER_H_
