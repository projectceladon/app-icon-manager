LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -g -DDO_NOT_CHECK_MANUAL_BINDER_INTERFACES -DLOG_TAG=\"pm_agent_client\" -g -Wno-missing-field-initializers -Wno-unused-parameter
LOCAL_CPPFLAGS := -g -std=c++17 -Wall -Werror -Wno-unused-parameter
LOCAL_LDFLAGS :=  -g

LOCAL_MODULE := pm_agent_client
LOCAL_MODULE_CLASS := EXECUTABLES

LOCAL_SRC_FILES := \
	pm-agent-client.cpp \
	AgentService.cpp \
	IClipboardAgent.cpp \
	IClipboardListener.cpp \
	IHostSettingsAgent.cpp \
	IHostSettingsListener.cpp \
	IInputMethodManagerAgent.cpp \
	IPackageInstallerListener.cpp \
	IPackageManagerAgent.cpp \
	IServiceAgent.cpp

LOCAL_C_INCLUDES += \
        $(LOCAL_PATH) \
        $(LOCAL_PATH)/include

LOCAL_SHARED_LIBRARIES := \
        liblog \
        libcutils \
        libbinder \
	libutils

include $(BUILD_EXECUTABLE)
