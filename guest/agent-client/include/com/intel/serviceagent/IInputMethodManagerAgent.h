#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_INPUT_METHOD_MANAGER_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_INPUT_METHOD_MANAGER_AGENT_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <utils/String16.h>
#include <utils/StrongPointer.h>
#include <vector>

namespace com {

namespace intel {

namespace serviceagent {

class IInputMethodManagerAgent : public ::android::IInterface {
public:
DECLARE_META_INTERFACE(InputMethodManagerAgent)
virtual ::android::binder::Status getInputMethodList(::std::vector<::android::String16>* _aidl_return) = 0;
virtual ::android::binder::Status getEnabledInputMethodList(::std::vector<::android::String16>* _aidl_return) = 0;
enum Call {
  GETINPUTMETHODLIST = ::android::IBinder::FIRST_CALL_TRANSACTION + 0,
  GETENABLEDINPUTMETHODLIST = ::android::IBinder::FIRST_CALL_TRANSACTION + 1,
};
};  // class IInputMethodManagerAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_INPUT_METHOD_MANAGER_AGENT_H_
