#include <com/intel/serviceagent/IClipboardListener.h>
#include <com/intel/serviceagent/BpClipboardListener.h>

namespace com {

namespace intel {

namespace serviceagent {

IMPLEMENT_META_INTERFACE(ClipboardListener, "com.intel.serviceagent.IClipboardListener")

}  // namespace serviceagent

}  // namespace intel

}  // namespace com
#include <com/intel/serviceagent/BpClipboardListener.h>
#include <binder/Parcel.h>

namespace com {

namespace intel {

namespace serviceagent {

BpClipboardListener::BpClipboardListener(const ::android::sp<::android::IBinder>& _aidl_impl)
    : BpInterface<IClipboardListener>(_aidl_impl){
}

::android::binder::Status BpClipboardListener::onClipChanged() {
::android::Parcel _aidl_data;
::android::Parcel _aidl_reply;
::android::status_t _aidl_ret_status = ::android::OK;
::android::binder::Status _aidl_status;
_aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = remote()->transact(IClipboardListener::ONCLIPCHANGED, _aidl_data, &_aidl_reply);
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = _aidl_status.readFromParcel(_aidl_reply);
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
if (!_aidl_status.isOk()) {
return _aidl_status;
}
_aidl_error:
_aidl_status.setFromStatusT(_aidl_ret_status);
return _aidl_status;
}

}  // namespace serviceagent

}  // namespace intel

}  // namespace com
#include <com/intel/serviceagent/BnClipboardListener.h>
#include <binder/Parcel.h>

namespace com {

namespace intel {

namespace serviceagent {

::android::status_t BnClipboardListener::onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) {
::android::status_t _aidl_ret_status = ::android::OK;
switch (_aidl_code) {
case Call::ONCLIPCHANGED:
{
if (!(_aidl_data.checkInterface(this))) {
_aidl_ret_status = ::android::BAD_TYPE;
break;
}
::android::binder::Status _aidl_status(onClipChanged());
_aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
if (!_aidl_status.isOk()) {
break;
}
}
break;
default:
{
_aidl_ret_status = ::android::BBinder::onTransact(_aidl_code, _aidl_data, _aidl_reply, _aidl_flags);
}
break;
}
if (_aidl_ret_status == ::android::UNEXPECTED_NULL) {
_aidl_ret_status = ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_NULL_POINTER).writeToParcel(_aidl_reply);
}
return _aidl_ret_status;
}

}  // namespace serviceagent

}  // namespace intel

}  // namespace com
