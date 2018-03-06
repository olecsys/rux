#!/bin/bash

function main() {
	local old_dir="`pwd`/`basename "$0"`"
	old_dir=`dirname "$old_dir"`
	cd "`dirname "$0"`"
	local script_dir="`pwd`/`basename "$0"`"
	script_dir=`dirname "$script_dir"`
	cd "$old_dir"

	"$script_dir/build.sh" -a x64	
}
main $@
if [ $__funcfailed -eq 0 ]; then
	exit 0
else
	exit 1
fi