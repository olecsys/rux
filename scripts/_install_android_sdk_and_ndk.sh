#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`

if [ -z $current_os ]; then
	current_os="$(uname -s)"
fi

if [ $current_os = "SunOS" ]
then
	echo "Solaris: Run as root !"
elif [ $current_os = "Linux" ]
then
	current_os="linux"
	if [ `grep -i debian /etc/issue | wc -l` -gt "0" ]
	then			
		echo "Debian"
	elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
	then			
		echo "Ubuntu"
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

android_sdk_archive_name=android-sdk_r24.3.3-linux.tgz

install_success="true"
if [ ! -e "$script_dir/../tools/android/SDK/tools/android" ]
then
	if [ $install_success = "true" ]
	then
		install_success="false"
		cd /tmp && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		wget http://dl.google.com/android/$android_sdk_archive_name && install_success="true"
		if [ $install_success = "false" ]
		then
			rm -rf ./$android_sdk_archive_name
		fi
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		mkdir -p "$script_dir/../tools/android/SDK" && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		tar -zxvf /tmp/$android_sdk_archive_name -C "$script_dir/../tools/android/SDK" && install_success="true"
		if [ $install_success = "false" ]
		then
			rm -rf "$script_dir/../tools/android/SDK"
		fi
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		cd "$script_dir/../tools/android/SDK" && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		if [ ! -e "$script_dir/../tools/android/SDK/tools/android" ]
		then
			find . -mindepth 1 -maxdepth 1 -type d -print0 | while read -d $'\0' list_path
			do
				list_name=$(basename $list_path)
				if [ $install_success = "true" ]
				then
					install_success="false"
					cp -r "$list_name/"* ./ && install_success="true"
				fi	
				if [ $install_success = "true" ]
				then
					install_success="false"
					rm -rf "$list_name" && install_success="true"
				fi
			done
		fi
	fi
fi
if [ $install_success = "true" ]
then
	install_success="false"
	export PATH="$PATH":"$script_dir/../tools/android/SDK/tools":"$script_dir/../tools/android/SDK/platform-tools" && install_success="true"
fi
if [ $install_success = "true" ]
then
	install_success="false"
	cd "$script_dir/../tools/android/SDK/tools" && install_success="true"
fi
if [ $install_success = "true" ]
then
	install_success="false"
	( sleep 5 && while [ 1 ]; do sleep 1; echo y; done ) | ./android update sdk --no-ui --filter platform-tool,android-22,android-10,addon-google_apis-google-22,addon-google_apis-google-10,build-tools-23.0.0-preview,extra-android-support,sample-10,sample-22 && install_success="true"
fi

android_ndk_archive_name=android-ndk-r10e-linux-x86_64.bin

if [ ! -e "$script_dir/../tools/android/NDK/ndk-build" ]
then
	if [ $install_success = "true" ]
	then
		install_success="false"
		mkdir -p /tmp/ndk && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		cd /tmp/ndk && install_success="true"
	fi
	if [ ! -e "./$android_ndk_archive_name" ]
	then
		if [ $install_success = "true" ]
		then
			install_success="false"
			wget http://dl.google.com/android/ndk/$android_ndk_archive_name && install_success="true"
			if [ $install_success = "false" ]
			then
				rm -rf ./$android_ndk_archive_name
			fi
		fi
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		chmod a+x /tmp/ndk/$android_ndk_archive_name && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		./$android_ndk_archive_name && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		mkdir -p "$script_dir/../tools/android/NDK" && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		find . -mindepth 1 -maxdepth 1 -type d -print0 | while read -d $'\0' list_path
		do
			list_name=$(basename $list_path)
			if [ $install_success = "true" ]
			then
				install_success="false"
				cp -r "$list_name/"* "$script_dir/../tools/android/NDK/" && install_success="true"
			fi	
			if [ $install_success = "true" ]
			then
				install_success="false"
				rm -rf "$list_name" && install_success="true"
			fi
		done
		if [ $install_success = "false" ]
		then
			rm -rf "$script_dir/../tools/android/NDK"
		fi
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		export PATH="$PATH":"$script_dir/../tools/android/NDK" && install_success="true"
	fi
	if [ $install_success = "true" ]
	then
		install_success="false"
		rm -rf /tmp/ndk && install_success="true"
	fi
fi

script_dir=$old_script_dir
cd "$old_dir"
