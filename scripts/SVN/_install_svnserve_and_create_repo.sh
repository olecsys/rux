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
apt-get install subversion bzip2 cifs-utils
if [ -e /etc/init.d/svnserve ]
then
  update-rc.d -f svnserve remove
  rm -rf /etc/init.d/svnserve
fi
if [ ! -d "/opt/SVN" ]; then
		mkdir /opt/SVN
		svnadmin create /opt/SVN/rux
		cp $script_dir/../../addons/SVN/svnserve.conf /opt/SVN/rux/conf/svnserve.conf
		cp $script_dir/../../addons/SVN/passwd /opt/SVN/rux/conf/passwd
		cp $script_dir/_make_svn_repo_dump.sh /opt/SVN/rux/hooks/post-commit
		chmod +x /opt/SVN/rux/hooks/post-commit
		#svnadmin load /opt/SVN/rux < $script_dir/../addons/SVN/dumps
fi
local_repo_dumps_path=/opt/SVN/dumps
mkdir -p "$local_repo_dumps_path"
chmod -R 777 "$local_repo_dumps_path"
cp $script_dir/../addons/svnserve /etc/init.d/svnserve
chmod +x /etc/init.d/svnserve
update-rc.d svnserve defaults 99
/etc/init.d/svnserve restart
echo -e "\nPress <ENTER> to continue"
read OS
