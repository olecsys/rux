LOCAL_PATH := $(call my-dir)/../
NDK_ROOT := D:\\Android-NDK\\android-ndk-r9
include $(CLEAR_VARS)

LOCAL_MODULE := rux.basis
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../build/android/$(TARGET_ARCH_ABI)/librux.basis.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := rux.engine

GLOBAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_SRC_FILES = xan_gcmanager.cpp dllmain.cpp xan_gui_engine.cpp xan_gui_window.cpp xan_memorymanager.cpp xan_freetype.cpp xan_opengl_functions.cpp xan_typemanager.cpp xan_render_cache.cpp xan_render_font.cpp xan_render_context.cpp xan_modulemanager.cpp

LOCAL_C_INCLUDES := $(GLOBAL_C_INCLUDES) \
					$(LOCAL_PATH)/../../includes \
					$(LOCAL_PATH)/../../extern_includes \
					$(NDK_ROOT)/sources/android/support/include \
					$(NDK_ROOT)/sources/android/native_app_glue \

LOCAL_CPP_FEATURES := rtti exceptions

LOCAL_CPPFLAGS := -fPIC -Werror -O3 -isystem $(SYSROOT)/usr/include/

LOCAL_STATIC_LIBRARIES := rux.basis
					
include $(BUILD_SHARED_LIBRARY)

all: $(TARGET_ARCH_ABI)_post_build

define gen_post_build
$(TARGET_ARCH_ABI)_post_build:
	sh ./../_copy_to_build.sh $(1)
endef

$(eval $(call gen_post_build,$(TARGET_ARCH_ABI)))