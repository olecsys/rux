#!/bin/bash
project_name=$1
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`
cd "$script_dir/lib"
ndk-build clean
android.bat update project --name $project_name --path . --target "android-19" --subprojects
ndk-build
cpu=armeabi-v7a
cp "$script_dir/../../extern_lib/android/$cpu/libiconv.so" "$script_dir/lib/libs/$cpu/"
cp "$script_dir/../../build/android/$cpu/librux.engine.so" "$script_dir/lib/libs/$cpu/"
cpu=x86
cp "$script_dir/../../extern_lib/android/$cpu/libiconv.so" "$script_dir/lib/libs/$cpu/"
cp "$script_dir/../../build/android/$cpu/librux.engine.so" "$script_dir/lib/libs/$cpu/"
ant debug
ant clean
ant debug install
cd "$old_dir"