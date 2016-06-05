#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`

if [ -z $current_os ]; then
	current_os="$(uname -s)"
fi
ID_UTILITY="id"
if [ $current_os = "SunOS" ]
then
	ID_UTILITY="/usr/xpg4/bin/id"
fi
if [ ! `$ID_UTILITY -u` = "0" ]
then
	if [ $current_os = "SunOS" ]
	then
		echo "Solaris: Run as root !"
	elif [ $current_os = "MINGW32_NT-6.1" ]
	then
		current_os="windows"
	elif [ $current_os = "Linux" ]
	then
		current_os="linux"
		if [ `grep -i debian /etc/issue | wc -l` -gt "0" ]
		then			
			su --preserve-environment -c "apt-get install git zip unzip"
		elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
		then			
			sudo apt-get install git zip unzip
		else
			echo "Run as root !"
		fi
	elif [ $current_os = "Darwin" ]
	then
		echo "Mac X: Run as root !"
	elif [ $current_os = "HP-UX" ]
	then
		echo "HP-UX: Run as root !"
	else
		echo "UnknownOS: Run as root !"
	fi
else
	if [ `grep -i debian /etc/issue | wc -l` -gt "0" ]
	then			
		su --preserve-environment -c "apt-get install git zip unzip"
	elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
	then			
		sudo apt-get install git zip unzip
	fi
fi

host_platform="$1"
if [ -z $host_platform ]; then
	if [ $current_os = "windows" ]
	then
		host_platform="x86"
	else
		host_platform="$(uname -m)"
		if [ $host_platform = "armv6l" ]
		then	
			host_platform="armv6"
		elif [ $host_platform = "i686" ]
		then	
			host_platform="x86"
		elif [ $host_platform = "x86_64" ]
		then	
			host_platform="x64"
		fi
	fi
fi
base_path="$script_dir/../.."
install_success="true"
if [ ! -e "$base_path/tools/depot_tools/gclient" ]
then
	if [ $install_success = "true" ]
	then
		install_success="false"
		git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git "$base_path/tools/depot_tools" && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		export PATH="$base_path/tools/depot_tools":"$PATH" && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		gclient && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		gclient sync && install_success="true"
	fi
	if [ $install_success = "false" ]
	then
		rm -rf "$base_path/tools/depot_tools"
	fi	
else
	if [ $install_success = "true" ]
	then
		install_success="false"
		export PATH="$base_path/tools/depot_tools":"$PATH" && install_success="true"
	fi
fi
get_v8_and_compile() {
	if [ $install_success = "true" ]
	then
		if [ ! -d "$base_path/downloaded_sources/v8" ]
		then
			if [ $install_success = "true" ]
			then
				install_success="false"
				mkdir -p "$base_path/downloaded_sources" && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				cd "$base_path/downloaded_sources" && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				fetch v8 && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				cd "$base_path/downloaded_sources/v8" && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				patch "$base_path/downloaded_sources/v8/tools/gyp/v8.gyp" -i "$base_path/addons/v8/v8.gyp.patch"  && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				patch "$base_path/downloaded_sources/v8/third_party/icu/icu.gyp" -i "$base_path/addons/v8/icu.gyp.patch"  && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				make native && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				mkdir -p "$base_path/extern_includes/v8" && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				cp  -rf "$base_path/downloaded_sources/v8/include/"* "$base_path/extern_includes/v8/" && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				mkdir -p "$base_path/extern_lib/$current_os/$host_platform/v8" && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				cp  -rf "$base_path/downloaded_sources/v8/out/native/obj.target/third_party/icu/"* "$base_path/extern_lib/$current_os/$host_platform/v8/" && install_success="true"
			fi
			if [ $install_success = "true" ]
			then
				install_success="false"
				cp  -rf "$base_path/downloaded_sources/v8/out/native/obj.target/tools/gyp/"* "$base_path/extern_lib/$current_os/$host_platform/v8/" && install_success="true"
			fi
		fi
		if [ $install_success = "false" ]
		then
			rm -rf "$script_dir/.gclient"*
			rm -rf "$script_dir/_gclient"*
			rm -rf "$base_path/downloaded_sources/v8"
		fi
	fi
}
if [ $install_success = "true" ]
then
	get_v8_and_compile
fi
red='\e[0;31m'
green='\e[0;32m'
nocolor='\e[0m'
if [ $install_success = "true" ]
then
	if [ ! -e "$script_dir/call_gclient_step_success" ]
	then
		echo -e "${green}\nPlease call file '$script_dir/v8/call_gclient.bat from cmd.exe' and then call this script again${nocolor}"
	else
		echo -e "${green}\nV8 compilation succeeded.Press <ENTER> to continue${nocolor}"
	fi
else
	echo -e "${red}V8 compilation failed.Press <ENTER> to continue${nocolor}"
fi

script_dir=$old_script_dir
cd "$old_dir"
read $symbol
