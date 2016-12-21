#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`
cd "$old_dir"

os=linux
platform=elbrus
rebuild_success="true"

if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	"$script_dir/../tools/rux.configure/$os/x64/rux.configure" --generate-makefile "$script_dir/../rux.framework/build.ruxprj" && rebuild_success="true"
fi
if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	cd "$script_dir/../projects/$os/$platform/rux.framework" && rebuild_success="true"
fi
if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	make -j 4 -f Makefile && rebuild_success="true"
fi
if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	cd "$old_dir" && rebuild_success="true"
fi
if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	"$script_dir/../tools/rux.configure/$os/x64/rux.configure" --build-project "$script_dir/../rux.framework/build.ruxprj" --configuration "$os $platform" && rebuild_success="true"
fi

if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	"$script_dir/../tools/rux.configure/$os/x64/rux.configure" --generate-makefile "$script_dir/../rux.engine/build.ruxprj" && rebuild_success="true"
fi
if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	cd "$script_dir/../projects/$os/$platform/rux.engine" && rebuild_success="true"
fi
if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	make -j 4 -f Makefile && rebuild_success="true"
fi
if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	cd "$old_dir" && rebuild_success="true"
fi
if [ $rebuild_success = "true" ]
then	
	rebuild_success="false"
	"$script_dir/../tools/rux.configure/$os/x64/rux.configure" --build-project "$script_dir/../rux.engine/build.ruxprj" --configuration "$os $platform" && rebuild_success="true"
fi
red='\e[0;31m'
green='\e[0;32m'
nocolor='\e[0m'
if [ $rebuild_success = "true" ]
then
	echo -e "${green}rux project building succeeded.Press <ENTER> to continue${nocolor}"
else
	echo -e "${red}rux project building failed.Press <ENTER> to continue${nocolor}"
fi
cd "$old_dir"
read
