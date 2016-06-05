LOCAL_PATH := $(call my-dir)/../
NDK_ROOT := D:\\Android-NDK\\android-ndk-r9
include $(CLEAR_VARS)

LOCAL_MODULE := rux.basis
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../build/android/$(TARGET_ARCH_ABI)/librux.basis.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := rux.basis.test

GLOBAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_SRC_FILES = main.cpp

LOCAL_C_INCLUDES := $(GLOBAL_C_INCLUDES) \
					$(LOCAL_PATH)/../../../includes \
					$(LOCAL_PATH)/../../../extern_includes \
					$(NDK_ROOT)/sources/android/support/include \

LOCAL_LDLIBS    := -llog -landroid

LOCAL_CPP_FEATURES := rtti exceptions

LOCAL_CPPFLAGS := -fPIC -Werror -O3 -isystem $(SYSROOT)/usr/include/

LOCAL_STATIC_LIBRARIES := rux.basis android_native_app_glue
					
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)