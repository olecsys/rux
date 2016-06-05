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
			su --preserve-environment -c "apt-get install git zip mercurial libasound2-dev libcurl4-openssl-dev libnotify-dev libxt-dev libiw-dev libidl-dev mesa-common-dev autoconf2.13 yasm libgtk2.0-dev libdbus-1-dev libdbus-glib-1-dev python-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libpulse-dev unzip gcc g++ make patch python-pip"
			if [ `lsb_release -ds|grep squeeze|wc -l` -gt "0" ]
			then
				if [ `grep -i squeeze-backports /etc/apt/sources.list | wc -l` -eq "0" ]
				then
					su --preserve-environment -c "echo \"deb http://backports.debian.org/debian-backports squeeze-backports main\" >> /etc/apt/sources.list && aptitude update"
				fi
				su --preserve-environment -c "aptitude -t squeeze-backports install yasm mercurial && pip install pythonbrew && pythonbrew_install"
				source "$HOME/.pythonbrew/etc/bashrc"
				pythonbrew install 2.7.3
				pythonbrew use 2.7.3
			fi
		elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
		then			
			sudo apt-get install git zip unzip mercurial g++ make autoconf2.13 yasm libgtk2.0-dev libglib2.0-dev libdbus-1-dev libdbus-glib-1-dev libasound2-dev libcurl4-openssl-dev libiw-dev libxt-dev mesa-common-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libpulse-dev m4 flex ccache
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
		su --preserve-environment -c "apt-get install git zip mercurial libasound2-dev libcurl4-openssl-dev libnotify-dev libxt-dev libiw-dev libidl-dev mesa-common-dev autoconf2.13 yasm libgtk2.0-dev libdbus-1-dev libdbus-glib-1-dev python-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libpulse-dev unzip gcc g++ make patch python-pip"
		if [ `lsb_release -ds|grep squeeze|wc -l` -gt "0" ]
		then
			if [ `grep -i squeeze-backports /etc/apt/sources.list | wc -l` -eq "0" ]
			then
				su --preserve-environment -c "echo \"deb http://backports.debian.org/debian-backports squeeze-backports main\" >> /etc/apt/sources.list && aptitude update"
			fi
			su --preserve-environment -c "aptitude -t squeeze-backports install yasm mercurial && pip install pythonbrew && pythonbrew_install && source \"$HOME/.pythonbrew/etc/bashrc\" && pythonbrew install 2.7.3"
			pythonbrew use 2.7.3
		fi
	elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
	then			
		sudo apt-get install git zip unzip mercurial g++ make autoconf2.13 yasm libgtk2.0-dev libglib2.0-dev libdbus-1-dev libdbus-glib-1-dev libasound2-dev libcurl4-openssl-dev libiw-dev libxt-dev mesa-common-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libpulse-dev m4 flex ccache
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

if [ $current_os = "windows" ]
then
	autoconf_name=autoconf-2.13
	make_name=mozmake.exe
	static_lib_suffix=lib
else
	autoconf_name=autoconf2.13
	make_name=make
	static_lib_prefix=lib
	static_lib_suffix=a
fi

spidermonkey_name=mozjs-31.2.0.rc0

install_success="true"
if [ ! -d "$script_dir/../SpiderMonkey/js/src" ]
then
	rm -rf "$script_dir/../SpiderMonkey"
	if [ $install_success = "true" ]
	then
		install_success="false"
		mkdir "$script_dir/../SpiderMonkey" && install_success="true"
	fi
	#git clone --depth 1 https://github.com/mozilla/gecko-dev.git "$script_dir/../SpiderMonkey"
	if [ $install_success = "true" ]
	then
		install_success="false"
		cd "$script_dir/../SpiderMonkey" && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		wget https://people.mozilla.org/~sstangl/$spidermonkey_name.tar.bz2 && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		tar xjfv $spidermonkey_name.tar.bz2 -C ./ && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		rm -rf $spidermonkey_name.tar.bz2 && install_success="true"
	fi
	find . -mindepth 1 -maxdepth 1 -type d -print0 | while read -d $'\0' spidermonkey_path
	do
		spidermonkey_name=$(basename $spidermonkey_path)
		if [ $install_success = "true" ]
		then
			install_success="false"
			cp -r "$spidermonkey_name/"* ./ && install_success="true"
		fi	
		if [ $install_success = "true" ]
		then
			install_success="false"
			rm -rf "$spidermonkey_name" && install_success="true"
		fi
	done
fi

compile_spidermonkey() {

	echo "step10"

	platform=$1
	if [ $current_os = "windows" ]
	then
		if [ $platform = "x64" ]
		then
			cxx_flags="--target=x86_64-pc-mingw32 --host=x86_64-pc-mingw32"
		fi
	fi
	if [ $install_success = "true" ]
	then
		echo "step2"
		install_success="false"
		cd "$script_dir/../SpiderMonkey/js/src" && install_success="true"
	fi
	
	
	if [ $install_success = "true" ]
	then
		echo "step3"
		install_success="false"
		$autoconf_name && install_success="true"
	fi
	rm -rf build_OPT.OBJ
	if [ $install_success = "true" ]
	then
		echo "step4"
		install_success="false"	
		mkdir build_OPT.OBJ && install_success="true"
	fi
		
	if [ $install_success = "true" ]
	then
		echo "step5"
		install_success="false"	
		cd build_OPT.OBJ && install_success="true"
	fi
		
	if [ $install_success = "true" ]
	then
		install_success="false"	
		../configure --enable-nspr-build --disable-shared-js $cxx_flags && install_success="true"
	fi
	
	if [ $install_success = "true" ]
	then
		install_success="false"	
		$make_name && install_success="true"
	fi
		
	if [ $install_success = "true" ]
	then
		install_success="false"
		#cp ./$static_lib_prefix"js_static."$static_lib_suffix "$script_dir/../extern_lib/$current_os/$platform/" && install_success="true"
		
		install_success="true"#temp
	fi

	if [ $install_success = "true" ]
	then
		install_success="false"
		#$make_name clean && install_success="true"
		
		install_success="true"#temp
	fi
		
	if [ $install_success = "true" ]
	then
		install_success="false"
		#cd ../ && install_success="true"
		
		install_success="true"#temp
	fi
		
	if [ $install_success = "true" ]
	then
		install_success="false"	
		#rm -rf build_OPT.OBJ && install_success="true"
		
		install_success="true"#temp
	fi	
}
compile_spidermonkey $host_platform
red='\e[0;31m'
green='\e[0;32m'
nocolor='\e[0m'
if [ $install_success = "true" ]
then
	echo -e "${green}SpiderMonkey compilation succeeded.Press <ENTER> to continue${nocolor}"
else
	echo -e "${red}SpiderMonkey compilation failed.Press <ENTER> to continue${nocolor}"
fi

script_dir=$old_script_dir
cd "$old_dir"
read $symbol
