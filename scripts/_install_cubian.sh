#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`

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
apt-get install p7zip-full

echo "Do you want to install Cubian?(Y/N)"
read answer
echo "Enter block device(example:sdb):"
read YOUR_DEVICE

CUBIAN_FILENAME=Cubian-nano+headless-x1-a20
PATH_TO_CUBIAN=/home/olecsys/rux/scripts/$CUBIAN_FILENAME
RUX_PATH=/home/olecsys/rux

umount "/dev/$YOUR_DEVICE"2

if [ "$answer" = "Y" ]
then
	rm -rf /tmp/$CUBIAN_FILENAME.img
	echo "$PATH_TO_CUBIAN".img.7z
	7z x "$PATH_TO_CUBIAN".img.7z -o/tmp/
	dd if=/tmp/"$CUBIAN_FILENAME".img of=/dev/$YOUR_DEVICE bs=4096; sync
	rm -rf /tmp/"$CUBIAN_FILENAME".img
fi
mkdir /media/my_cubie
mount "/dev/$YOUR_DEVICE"2 /media/my_cubie
if [ -d "/media/my_cubie/etc" ]
then
	echo "Do you want to copy advanced files?(Y/N)"
	read answer
	if [ "$answer" = "Y" ]
	then
		echo "# the loopback interface
auto lo
iface lo inet loopback
#
auto eth0
allow-hotplug eth0
iface eth0 inet static
address 192.168.86.90
netmask 255.255.255.0
gateway 192.168.86.1
dns-nameservers 192.168.86.1" > /media/my_cubie/etc/network/interfaces
		echo "nameserver 192.168.86.1" > /media/my_cubie/etc/resolv.conf
		mkdir /media/my_cubie/home/cubie/scripts
		mkdir /media/my_cubie/home/cubie/addons
		mkdir /media/my_cubie/home/cubie/addons/openvpn_client
		mkdir /media/my_cubie/home/cubie/addons/SVN
		cp -v $RUX_PATH/addons/openvpn_client/* /media/my_cubie/home/cubie/addons/openvpn_client/
		cp -v $RUX_PATH/addons/SVN/* /media/my_cubie/home/cubie/addons/SVN/
		cp -v $RUX_PATH/scripts/SVN/* /media/my_cubie/home/cubie/scripts/SVN/
		cp -v $RUX_PATH/scripts/_setup_cubian.sh /media/my_cubie/home/cubie/scripts/
	fi
else
	red='\e[0;31m'
	green='\e[0;32m'
	nocolor='\e[0m'
	echo -e "${red}rux Can't mount to device${nocolor}"
fi
umount "/dev/$YOUR_DEVICE"2
rm -rf /media/my_cubie
echo -e "\nPress <ENTER> to continue"
read OS
