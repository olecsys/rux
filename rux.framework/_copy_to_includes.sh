#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`
if [ ! -d "$script_dir/../includes" ]; then
	mkdir "$script_dir/../includes"
fi
if [ ! -d "$script_dir/../includes/openssl" ]; then
	mkdir "$script_dir/../includes/openssl"
fi
if [ ! -d "$script_dir/../build" ]; then
	mkdir "$script_dir/../build"
fi
cp "$script_dir/lib/"*.h "$script_dir/../includes/"
cp "$script_dir/../extern_includes/xan_platform.h" "$script_dir/../includes/"
cp "$script_dir/../extern_includes/xan_types.h" "$script_dir/../includes/"
cp "$script_dir/../extern_includes/xan_utils.h" "$script_dir/../includes/"
cp "$script_dir/../extern_includes/xan_utils_consts.h" "$script_dir/../includes/"
cp "$script_dir/../extern_includes/openssl/"*.h "$script_dir/../includes/openssl/"
cd $old_dir