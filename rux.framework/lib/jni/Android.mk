LOCAL_PATH := $(call my-dir)/../
NDK_ROOT := D:\\Android-NDK\\android-ndk-r9
include $(CLEAR_VARS)

LOCAL_MODULE := rux.basis

LOCAL_SRC_FILES =   xan_crtsect.cpp xan_asm.cpp xan_base64.cpp xan_boolean.cpp xan_char.cpp xan_console.cpp xan_debug.cpp xan_double.cpp xan_engine.cpp xan_error.cpp xan_event.cpp xan_float.cpp xan_gcref.cpp xan_gui_defines.cpp xan_int32.cpp xan_int64.cpp xan_interlocked.cpp xan_malloc_event.cpp xan_malloc_thread.cpp xan_math.cpp xan_memorymanager_defines.cpp xan_methodinfo.cpp xan_methodparameter.cpp xan_module.cpp xan_null.cpp xan_object.cpp xan_pipe.cpp xan_randomnumbergenerator.cpp xan_rawdata.cpp xan_regexp.cpp xan_registry.cpp xan_security.cpp xan_stdredirect.cpp xan_string.cpp xan_thread.cpp xan_time.cpp xan_type.cpp xan_typemanager_defines.cpp xan_uint32.cpp xan_uint64.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_PATH)/../../extern_includes \
					$(NDK_ROOT)/sources/android/support/include \
					$(NDK_ROOT)/sources/android/native_app_glue \

LOCAL_CPP_FEATURES := rtti exceptions

LOCAL_CPPFLAGS := -fPIC -Werror -O3 -isystem $(SYSROOT)/usr/include/
	
include $(BUILD_STATIC_LIBRARY)

all: $(TARGET_ARCH_ABI)_post_build

define gen_post_build
$(TARGET_ARCH_ABI)_post_build:
	sh ./../_copy_to_includes.sh
	sh ./../_copy_to_build.sh $(1)
endef

$(eval $(call gen_post_build,$(TARGET_ARCH_ABI)))