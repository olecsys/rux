#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`
NDK_ROOT=/opt/android/android-ndk-r9c
ICONV=libiconv-1.14
ICONV_PATH=$script_dir/$ICONV
if [ -e "$script_dir/iconv.patch" ]; then
	rm "$script_dir/iconv.patch"
fi
echo "diff --ignore-file-name-case -wuprN $ICONV.orig/libcharset/lib/localcharset.c $ICONV/libcharset/lib/localcharset.c
--- $ICONV.orig/libcharset/lib/localcharset.c  2009-06-21 07:17:33.000000000 -0400
+++ $ICONV/libcharset/lib/localcharset.c       2012-12-18 10:20:27.000000000 -0500
@@ -45,8 +45,8 @@
 # endif
 #endif

-#if !defined WIN32_NATIVE
+#if !defined(WIN32_NATIVE) && !defined(__ANDROID__)
 # include <unistd.h>
 # if HAVE_LANGINFO_CODESET
 #  include <langinfo.h>
 # else
@@ -361,7 +361,7 @@ locale_charset (void)
   const char *codeset;
   const char *aliases;

-#if !(defined WIN32_NATIVE || defined OS2)
+#if !(defined WIN32_NATIVE || defined OS2 || defined __ANDROID__)

 # if HAVE_LANGINFO_CODESET " > "$script_dir/iconv.patch"
patch -b -p0 < "$script_dir/iconv.patch"
if [ -e "$script_dir/iconv.patch" ]; then
	rm "$script_dir/iconv.patch"
fi
cd $ICONV_PATH
./configure
if [ -d "$ICONV_PATH/jni" ]; then
	rm -rf "$ICONV_PATH/jni"
fi
if [ ! -d "$ICONV_PATH/jni" ]; then
	mkdir "$ICONV_PATH/jni"
fi
if [ ! -e "$ICONV_PATH/jni/Android.mk" ]; then
	echo "LOCAL_PATH := \$(call my-dir)
include \$(CLEAR_VARS)
LOCAL_MODULE    := iconv
LOCAL_CFLAGS    := \\
    -Wno-multichar \\
    -D_ANDROID \\
    -DLIBDIR=\"\\\"c\\\"\" \\
    -DBUILDING_LIBICONV \\
    -DIN_LIBRARY
LOCAL_C_INCLUDES := \\
    $ICONV_PATH \\
    $ICONV_PATH/include \\
    $ICONV_PATH/lib \\
    $ICONV_PATH/libcharset/include
LOCAL_SRC_FILES := \\
    $ICONV_PATH/lib/iconv.c \\
    $ICONV_PATH/lib/relocatable.c \\
    $ICONV_PATH/libcharset/lib/localcharset.c
include \$(BUILD_SHARED_LIBRARY)
" > "$ICONV_PATH/jni/Android.mk"
fi
if [ ! -e "$ICONV_PATH/jni/Application.mk" ]; then
	echo "APP_OPTIM := release
APP_PLATFORM := android-9
APP_ABI := armeabi-v7a x86" > "$ICONV_PATH/jni/Application.mk"
fi
$NDK_ROOT/ndk-build
if [ -d "$ICONV_PATH/jni" ]; then
	rm -rf "$ICONV_PATH/jni"
fi
if [ -d "$ICONV_PATH/libs" ]; then
	if [ -e "$ICONV_PATH/libs/armeabi-v7a/libiconv.so" ]; then
		cp "$ICONV_PATH/libs/armeabi-v7a/libiconv.so" "$script_dir/../extern_lib/android/armeabi-v7a/"
	fi
	if [ -e "$ICONV_PATH/libs/x86/libiconv.so" ]; then
		cp "$ICONV_PATH/libs/x86/libiconv.so" "$script_dir/../extern_lib/android/x86/"
	fi
	rm -rf "$ICONV_PATH/libs"
fi
if [ -e "$ICONV_PATH/libcharset/lib/localcharset.c.orig" ]; then
	rm "$ICONV_PATH/libcharset/lib/localcharset.c"
	mv "$ICONV_PATH/libcharset/lib/localcharset.c.orig" "$ICONV_PATH/libcharset/lib/localcharset.c"
fi
cd $old_dir