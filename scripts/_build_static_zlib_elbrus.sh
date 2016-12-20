#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`

host_platform="x64"
cd "$script_dir/../extern_sources/zlib"
make distclean
chmod +x ./configure
CROSS_PREFIX=/opt/mcst/lcc-1.21.09.e2k-v3.3.14/binutils/bin/e2k-linux- CC=/opt/mcst/lcc-1.21.09.e2k-v3.3.14/bin/lcc ./configure --static
make libz.a
cd "$old_dir"
read
