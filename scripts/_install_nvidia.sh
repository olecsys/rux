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
current_vga="$(lspci|grep -i -o ' nvidia ')"
if [[ ! -z $current_vga ]]; then
	current_vga="NVIDIA"
else
	current_vga="unknown"
fi

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
if [ $current_os = "Ubuntu" ]
then
	if [ $current_vga = "NVIDIA" ]
	then
		add-apt-repository ppa:ubuntu-x-swat/x-updates
		apt-get update
		apt-get purge nvidia-current nvidia-settings
		apt-get install linux-headers-`(uname -r)`
		apt-get install nvidia-current nvidia-settings
	fi
elif [ $current_os = "Debian" ]
then	
	if [ $current_vga = "NVIDIA" ]
	then
		aptitude update
		current_distr="$(lsb_release -sc)"
		if [ $current_distr = "squeeze" ]
		then
			aptitude -r install linux-headers-2.6-$(uname -r|sed 's,[^-]*-[^-]*-,,') nvidia-kernel-dkms		
		elif [ $current_distr = "wheezy" ]
		then
			aptitude -r install linux-headers-$(uname -r|sed 's,[^-]*-[^-]*-,,') nvidia-kernel-dkms
		fi
		dkms_installed="$(aptitude search nvidia-kernel-dkms|grep -o 'i   nvidia-kernel-dkms ')"
		if [[ ! -z $dkms_installed ]]; then
			mkdir /etc/X11/xorg.conf.d
			echo -e 'Section "Device"\n\tIdentifier "My GPU"\n\tDriver "nvidia"\nEndSection' > /etc/X11/xorg.conf.d/20-nvidia.conf
			if [ $current_distr = "squeeze" ]
			then
				update-initramfs -u
			fi
		else
			echo -e "\nnvidia-kernel-dkms is not installed"
		fi		
	fi
fi
echo -e "\nPress <ENTER> to continue"
read OS
