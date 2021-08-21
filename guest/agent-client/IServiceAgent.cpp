#include <com/intel/serviceagent/IServiceAgent.h>
#include <com/intel/serviceagent/BpServiceAgent.h>

namespace com {

namespace intel {

namespace serviceagent {

IMPLEMENT_META_INTERFACE(ServiceAgent, "com.intel.serviceagent.IServiceAgent")

}  // namespace serviceagent

}  // namespace intel

}  // namespace com
#include <com/intel/serviceagent/BpServiceAgent.h>
#include <binder/Parcel.h>

namespace com {

namespace intel {

namespace serviceagent {

BpServiceAgent::BpServiceAgent(const ::android::sp<::android::IBinder>& _aidl_impl)
    : BpInterface<IServiceAgent>(_aidl_impl){
}

::android::binder::Status BpServiceAgent::getPackageManagerAgent(::android::sp<::com::intel::serviceagent::IPackageManagerAgent>* _aidl_return) {
::android::Parcel _aidl_data;
::android::Parcel _aidl_reply;
::android::status_t _aidl_ret_status = ::android::OK;
::android::binder::Status _aidl_status;
_aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = remote()->transact(IServiceAgent::GETPACKAGEMANAGERAGENT, _aidl_data, &_aidl_reply);
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
_aidl_ret_status = _aidl_reply.readStrongBinder(_aidl_return);
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_error:
_aidl_status.setFromStatusT(_aidl_ret_status);
return _aidl_status;
}

::android::binder::Status BpServiceAgent::getClipboardAgent(::android::sp<::com::intel::serviceagent::IClipboardAgent>* _aidl_return) {
::android::Parcel _aidl_data;
::android::Parcel _aidl_reply;
::android::status_t _aidl_ret_status = ::android::OK;
::android::binder::Status _aidl_status;
_aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = remote()->transact(IServiceAgent::GETCLIPBOARDAGENT, _aidl_data, &_aidl_reply);
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
_aidl_ret_status = _aidl_reply.readStrongBinder(_aidl_return);
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_error:
_aidl_status.setFromStatusT(_aidl_ret_status);
return _aidl_status;
}

::android::binder::Status BpServiceAgent::getHostSettingsAgent(::android::sp<::com::intel::serviceagent::IHostSettingsAgent>* _aidl_return) {
::android::Parcel _aidl_data;
::android::Parcel _aidl_reply;
::android::status_t _aidl_ret_status = ::android::OK;
::android::binder::Status _aidl_status;
_aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = remote()->transact(IServiceAgent::GETHOSTSETTINGSAGENT, _aidl_data, &_aidl_reply);
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
_aidl_ret_status = _aidl_reply.readStrongBinder(_aidl_return);
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_error:
_aidl_status.setFromStatusT(_aidl_ret_status);
return _aidl_status;
}

::android::binder::Status BpServiceAgent::addHostSettingsListener(const ::android::sp<::com::intel::serviceagent::IHostSettingsListener>& listener) {
::android::Parcel _aidl_data;
::android::Parcel _aidl_reply;
::android::status_t _aidl_ret_status = ::android::OK;
::android::binder::Status _aidl_status;
_aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = _aidl_data.writeStrongBinder(::com::intel::serviceagent::IHostSettingsListener::asBinder(listener));
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = remote()->transact(IServiceAgent::ADDHOSTSETTINGSLISTENER, _aidl_data, &_aidl_reply);
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

::android::binder::Status BpServiceAgent::removeHostSettingsListener(const ::android::sp<::com::intel::serviceagent::IHostSettingsListener>& listener) {
::android::Parcel _aidl_data;
::android::Parcel _aidl_reply;
::android::status_t _aidl_ret_status = ::android::OK;
::android::binder::Status _aidl_status;
_aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = _aidl_data.writeStrongBinder(::com::intel::serviceagent::IHostSettingsListener::asBinder(listener));
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = remote()->transact(IServiceAgent::REMOVEHOSTSETTINGSLISTENER, _aidl_data, &_aidl_reply);
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

::android::binder::Status BpServiceAgent::getInputMethodManagerAgent(::android::sp<::com::intel::serviceagent::IInputMethodManagerAgent>* _aidl_return) {
::android::Parcel _aidl_data;
::android::Parcel _aidl_reply;
::android::status_t _aidl_ret_status = ::android::OK;
::android::binder::Status _aidl_status;
_aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_ret_status = remote()->transact(IServiceAgent::GETINPUTMETHODMANAGERAGENT, _aidl_data, &_aidl_reply);
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
_aidl_ret_status = _aidl_reply.readStrongBinder(_aidl_return);
if (((_aidl_ret_status) != (::android::OK))) {
goto _aidl_error;
}
_aidl_error:
_aidl_status.setFromStatusT(_aidl_ret_status);
return _aidl_status;
}

}  // namespace serviceagent

}  // namespace intel

}  // namespace com
#include <com/intel/serviceagent/BnServiceAgent.h>
#include <binder/Parcel.h>

namespace com {

namespace intel {

namespace serviceagent {

::android::status_t BnServiceAgent::onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) {
::android::status_t _aidl_ret_status = ::android::OK;
switch (_aidl_code) {
case Call::GETPACKAGEMANAGERAGENT:
{
::android::sp<::com::intel::serviceagent::IPackageManagerAgent> _aidl_return;
if (!(_aidl_data.checkInterface(this))) {
_aidl_ret_status = ::android::BAD_TYPE;
break;
}
::android::binder::Status _aidl_status(getPackageManagerAgent(&_aidl_return));
_aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
if (!_aidl_status.isOk()) {
break;
}
_aidl_ret_status = _aidl_reply->writeStrongBinder(::com::intel::serviceagent::IPackageManagerAgent::asBinder(_aidl_return));
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
}
break;
case Call::GETCLIPBOARDAGENT:
{
::android::sp<::com::intel::serviceagent::IClipboardAgent> _aidl_return;
if (!(_aidl_data.checkInterface(this))) {
_aidl_ret_status = ::android::BAD_TYPE;
break;
}
::android::binder::Status _aidl_status(getClipboardAgent(&_aidl_return));
_aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
if (!_aidl_status.isOk()) {
break;
}
_aidl_ret_status = _aidl_reply->writeStrongBinder(::com::intel::serviceagent::IClipboardAgent::asBinder(_aidl_return));
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
}
break;
case Call::GETHOSTSETTINGSAGENT:
{
::android::sp<::com::intel::serviceagent::IHostSettingsAgent> _aidl_return;
if (!(_aidl_data.checkInterface(this))) {
_aidl_ret_status = ::android::BAD_TYPE;
break;
}
::android::binder::Status _aidl_status(getHostSettingsAgent(&_aidl_return));
_aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
if (!_aidl_status.isOk()) {
break;
}
_aidl_ret_status = _aidl_reply->writeStrongBinder(::com::intel::serviceagent::IHostSettingsAgent::asBinder(_aidl_return));
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
}
break;
case Call::ADDHOSTSETTINGSLISTENER:
{
::android::sp<::com::intel::serviceagent::IHostSettingsListener> in_listener;
if (!(_aidl_data.checkInterface(this))) {
_aidl_ret_status = ::android::BAD_TYPE;
break;
}
_aidl_ret_status = _aidl_data.readStrongBinder(&in_listener);
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
::android::binder::Status _aidl_status(addHostSettingsListener(in_listener));
_aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
if (!_aidl_status.isOk()) {
break;
}
}
break;
case Call::REMOVEHOSTSETTINGSLISTENER:
{
::android::sp<::com::intel::serviceagent::IHostSettingsListener> in_listener;
if (!(_aidl_data.checkInterface(this))) {
_aidl_ret_status = ::android::BAD_TYPE;
break;
}
_aidl_ret_status = _aidl_data.readStrongBinder(&in_listener);
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
::android::binder::Status _aidl_status(removeHostSettingsListener(in_listener));
_aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
if (!_aidl_status.isOk()) {
break;
}
}
break;
case Call::GETINPUTMETHODMANAGERAGENT:
{
::android::sp<::com::intel::serviceagent::IInputMethodManagerAgent> _aidl_return;
if (!(_aidl_data.checkInterface(this))) {
_aidl_ret_status = ::android::BAD_TYPE;
break;
}
::android::binder::Status _aidl_status(getInputMethodManagerAgent(&_aidl_return));
_aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
if (((_aidl_ret_status) != (::android::OK))) {
break;
}
if (!_aidl_status.isOk()) {
break;
}
_aidl_ret_status = _aidl_reply->writeStrongBinder(::com::intel::serviceagent::IInputMethodManagerAgent::asBinder(_aidl_return));
if (((_aidl_ret_status) != (::android::OK))) {
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
