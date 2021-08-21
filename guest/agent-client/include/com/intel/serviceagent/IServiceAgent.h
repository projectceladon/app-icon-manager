#ifndef AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_SERVICE_AGENT_H_
#define AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_SERVICE_AGENT_H_

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <com/intel/serviceagent/IClipboardAgent.h>
#include <com/intel/serviceagent/IHostSettingsAgent.h>
#include <com/intel/serviceagent/IHostSettingsListener.h>
#include <com/intel/serviceagent/IInputMethodManagerAgent.h>
#include <com/intel/serviceagent/IPackageManagerAgent.h>
#include <utils/StrongPointer.h>

namespace com {

namespace intel {

namespace serviceagent {

class IServiceAgent : public ::android::IInterface {
public:
DECLARE_META_INTERFACE(ServiceAgent)
virtual ::android::binder::Status getPackageManagerAgent(::android::sp<::com::intel::serviceagent::IPackageManagerAgent>* _aidl_return) = 0;
virtual ::android::binder::Status getClipboardAgent(::android::sp<::com::intel::serviceagent::IClipboardAgent>* _aidl_return) = 0;
virtual ::android::binder::Status getHostSettingsAgent(::android::sp<::com::intel::serviceagent::IHostSettingsAgent>* _aidl_return) = 0;
virtual ::android::binder::Status addHostSettingsListener(const ::android::sp<::com::intel::serviceagent::IHostSettingsListener>& listener) = 0;
virtual ::android::binder::Status removeHostSettingsListener(const ::android::sp<::com::intel::serviceagent::IHostSettingsListener>& listener) = 0;
virtual ::android::binder::Status getInputMethodManagerAgent(::android::sp<::com::intel::serviceagent::IInputMethodManagerAgent>* _aidl_return) = 0;
enum Call {
  GETPACKAGEMANAGERAGENT = ::android::IBinder::FIRST_CALL_TRANSACTION + 0,
  GETCLIPBOARDAGENT = ::android::IBinder::FIRST_CALL_TRANSACTION + 1,
  GETHOSTSETTINGSAGENT = ::android::IBinder::FIRST_CALL_TRANSACTION + 2,
  ADDHOSTSETTINGSLISTENER = ::android::IBinder::FIRST_CALL_TRANSACTION + 3,
  REMOVEHOSTSETTINGSLISTENER = ::android::IBinder::FIRST_CALL_TRANSACTION + 4,
  GETINPUTMETHODMANAGERAGENT = ::android::IBinder::FIRST_CALL_TRANSACTION + 5,
};
};  // class IServiceAgent

}  // namespace serviceagent

}  // namespace intel

}  // namespace com

#endif  // AIDL_GENERATED_COM_INTEL_SERVICEAGENT_I_SERVICE_AGENT_H_
