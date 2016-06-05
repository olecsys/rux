#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`

host_platform="$(uname -m)"
if [ $host_platform = "armv6l" ]
then	
	host_platform="armv6"
fi
if [ $host_platform = "i686" ]
then	
	host_platform="x86"
fi
if [ $host_platform = "x86_64" ]
then	
	host_platform="x64"
fi
cd "$script_dir/../extern_sources/zlib"
make distclean
chmod +x ./configure
if [ $host_platform = "x64" ]
then	
	./configure --64 --static
else
	./configure --static
fi
make libz.a
cd "$old_dir"
read
