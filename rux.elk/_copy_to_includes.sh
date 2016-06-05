#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`
if [ ! -d "$script_dir/../includes" ]; then
	mkdir "$script_dir/../includes"
fi
cp "$script_dir/lib/"*.h "$script_dir/../includes/"
cd $old_dir