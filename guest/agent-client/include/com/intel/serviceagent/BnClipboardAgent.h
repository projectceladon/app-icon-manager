#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BN_CLIPBOARD_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BN_CLIPBOARD_AGENT_H_

#include <binder/IInterface.h>
#include <com/intel/serviceagent/IClipboardAgent.h>

namespace com {

namespace intel {

namespace serviceagent {

class BnClipboardAgent : public ::android::BnInterface<IClipboardAgent> {
public:
::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags = 0) override;
};  // class BnClipboardAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_BN_CLIPBOARD_AGENT_H_
