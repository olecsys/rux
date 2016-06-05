#!/bin/bash
arch=$1
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`
if [ ! -d "$script_dir/../build" ]; then
	mkdir "$script_dir/../build"
fi
if [ ! -d "$script_dir/../build/android" ]; then
	mkdir "$script_dir/../build/android"
fi
if [ ! -d "$script_dir/../build/android/$arch" ]; then
	mkdir "$script_dir/../build/android/$arch"
fi
OSPL="$(uname -o)"
if [ $OSPL = "Cygwin" ]
then
	find "$script_dir/lib/obj/local/$arch/" -iregex '.*\(.a\)$' -exec cp {} "$script_dir/../build/android/$arch" \;
fi
cd $old_dir