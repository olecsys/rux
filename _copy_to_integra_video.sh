#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`

ARCHITECTURE=$1
if [ -z $ARCHITECTURE ]; then
	ARCHITECTURE="$(uname -m)"
	if [ $ARCHITECTURE = "x86_64" ]
	then
		ARCHITECTURE="x64"
	elif [ $ARCHITECTURE = "i686" ]
	then
		ARCHITECTURE="x86"
	elif [ $ARCHITECTURE = "armv6l" ]
	then
		ARCHITECTURE="armv6"
	fi
fi
OSPL="$(uname -s)"
RUX_DIRECTORY="`pwd`"
cd "$RUX_DIRECTORY/.."
CORE_DIRECTORY="`pwd`/core"
cd "$RUX_DIRECTORY"
if [ ! -d "$CORE_DIRECTORY/crox_headers" ]; then
	mkdir "$CORE_DIRECTORY/crox_headers"
fi
if [ ! -d "$CORE_DIRECTORY/crox_headers/openssl" ]; then
	mkdir "$CORE_DIRECTORY/crox_headers/openssl"
fi
cp "$script_dir/includes/"*.h "$CORE_DIRECTORY/crox_headers"
cp "$script_dir/includes/openssl/"*.h "$CORE_DIRECTORY/crox_headers/openssl"
if [ $OSPL = "SunOS" ]
then
	ARCHITECTURE="x64"
	if [ ! -d "$CORE_DIRECTORY/crox_lib" ]; then
		mkdir "$CORE_DIRECTORY/crox_lib"
	fi
	if [ ! -d "$CORE_DIRECTORY/crox_lib/solaris" ]; then
		mkdir "$CORE_DIRECTORY/crox_lib/solaris"
	fi
	if [ ! -d "$CORE_DIRECTORY/crox_lib/solaris/$ARCHITECTURE" ]; then
		mkdir "$CORE_DIRECTORY/crox_lib/solaris/$ARCHITECTURE"
	fi
	if [ ! -d "$CORE_DIRECTORY/crox_lib/solaris/$ARCHITECTURE/debug" ]; then
		mkdir "$CORE_DIRECTORY/crox_lib/solaris/$ARCHITECTURE/debug"
	fi
	cp "$script_dir/build/solaris/$ARCHITECTURE/"*.a "$CORE_DIRECTORY/crox_lib/solaris/$ARCHITECTURE/debug"
	cp "$script_dir/build/solaris/$ARCHITECTURE/librux.engine.so" "$CORE_DIRECTORY/bin/solaris/$ARCHITECTURE/librux.engine.so"
elif [ $OSPL = "Linux" ]
then
	if [ ! -d "$CORE_DIRECTORY/crox_lib" ]; then
		mkdir "$CORE_DIRECTORY/crox_lib"
	fi
	if [ ! -d "$CORE_DIRECTORY/crox_lib/linux" ]; then
		mkdir "$CORE_DIRECTORY/crox_lib/linux"
	fi
	if [ ! -d "$CORE_DIRECTORY/crox_lib/linux/$ARCHITECTURE" ]; then
		mkdir "$CORE_DIRECTORY/crox_lib/linux/$ARCHITECTURE"
	fi
	if [ ! -d "$CORE_DIRECTORY/crox_lib/linux/$ARCHITECTURE/debug" ]; then
		mkdir "$CORE_DIRECTORY/crox_lib/linux/$ARCHITECTURE/debug"
	fi
	cp "$script_dir/build/linux/$ARCHITECTURE/"*.a "$CORE_DIRECTORY/crox_lib/linux/$ARCHITECTURE/debug"
	cp "$script_dir/build/linux/$ARCHITECTURE/librux.engine.so" "$CORE_DIRECTORY/bin/linux/$ARCHITECTURE/librux.engine.so"
fi
echo "Done.Please enter any key."
read

