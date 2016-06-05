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
echo -e "\nEnter new root password:"
passwd
wget -O - http://packages.cubian.org/cubian.gpg.key | sudo apt-key add -
apt-get update
bash /home/cubie/scripts/_install_svnserve_and_create_repo.sh
adduser olecsys
groupadd -r sshusers
usermod -a -G sshusers olecsys
echo "    AllowGroups sshusers" >> /etc/ssh/sshd_config
mkdir /etc/openvpn
cp /home/cubie/addons/openvpn_client/* /etc/openvpn/
chmod -R 600 /etc/openvpn
mkdir /home/olecsys/scripts
cp /home/cubie/scripts/_make_svn_repo_dump.sh /home/olecsys/scripts/_make_svn_repo_dump.sh
echo -e "Enter in crontab 'SHELL=/bin/bash\n0 * * * * bash /home/olecsys/scripts/_make_svn_repo_dump.sh'"
crontab -e
userdel -r "cubie"

apt-get install openvpn

dpkg-reconfigure tzdata

apt-get install cubian-update
cubian-update
reboot
echo -e "\nPress <ENTER> to continue"
read OS
