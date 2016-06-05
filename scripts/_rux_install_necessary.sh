#!/bin/bash
current_os=$1
current_user=$2
current_user_group=$3
current_machine="$(uname -m)"	
if [ -z $current_user ]; then
	current_user="$(whoami)"
fi
if [ -z $current_user_group ]; then
	current_user_group="$(id -g --name)"
fi
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
	elif [ $current_os = "Linux" ]
	then
		if [ `grep -i debian /etc/issue | wc -l` -gt "0" ]
		then
			current_os="Debian"
			su --preserve-environment -c "bash $0 $current_os $current_user $current_user_group"
		elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
		then
			current_os="Ubuntu"
			sudo bash $0 $current_os $current_user $current_user_group
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
	exit
fi
apt-get install yasm
apt-get install build-essential
apt-get install libx11-dev
apt-get install libxrandr-dev
apt-get install mesa-common-dev
apt-get install libgl1-mesa-dev
if [ $current_os = "Ubuntu" ]
then
	if [ $current_machine = "x86_64" ]
	then	
		apt-get install g++-multilib
	fi
	if [ ! -d "/opt/raspberrypi" ]; then
		if [ $current_machine = "x86_64" ]
		then	
			apt-get install ia32-libs
		fi
		mkdir /opt/raspberrypi
		apt-get install git
		old_dir=`pwd`
		cd /opt/raspberrypi
		git clone git://github.com/raspberrypi/tools.git
		ln -sf /opt/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-objdump /usr/bin/raspbian-arm-objdump
		chown -R -- $current_user:$current_user_group /opt/raspberrypi
		cd $old_dir
	fi
elif [ $current_os = "Debian" ]
then
	if [ $current_machine = "x86_64" ]
	then	
		apt-get install g++-multilib
	fi
fi
echo "\nPress <ENTER> to continue"
read OS
