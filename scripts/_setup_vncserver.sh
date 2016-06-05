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

apt-get install tightvncserver
apt-get install gnome-core

echo -e "\nPlease enter desired VNC server user:"
read username

DISPLAY="1"
DEPTH="16"
GEOMETRY="1024x768"
NAME="my-vnc-server"
OPTIONS="-name $NAME -depth $DEPTH -geometry $GEOMETRY :$DISPLAY"

su $username -c "/usr/bin/vncserver $OPTIONS"
su $username -c "/usr/bin/vncserver -kill :$DISPLAY"

echo "#! /bin/sh
### BEGIN INIT INFO
# Provides: VNC server
# Required-Start: \$local_fs \$remote_fs
# Required-Stop: \$local_fs \$remote_fs
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: VNC server initscript
# Description: This file starts the VNC server at boot and should be
# placed in /etc/init.d.
### END INIT INFO
PATH=\"\$PATH:/usr/X11R6/bin/\"
# The Username:Group that will run VNC
export USER=\""$username"\"
#\${RUNAS}
# The display that VNC will use
DISPLAY=\""$DISPLAY"\"
# Color depth (between 8 and 32)
DEPTH=\""$DEPTH"\"
# The Desktop geometry to use.
#GEOMETRY=\"<WIDTH>x<HEIGHT>\"
GEOMETRY=\""$GEOMETRY"\"
# The name that the VNC Desktop will have.
NAME=\""$NAME"\"
OPTIONS=\""$OPTIONS"\"
#. /lib/lsb/init-functions
case \"\$1\" in
start)
log_action_begin_msg \"Starting vncserver for user '\${USER}' on localhost:\${DISPLAY}\"
su \${USER} -c \"/usr/bin/vncserver \${OPTIONS}\"
;;
stop)
log_action_begin_msg \"Stoping vncserver for user '\${USER}' on localhost:\${DISPLAY}\"
su \${USER} -c \"/usr/bin/vncserver -kill :\${DISPLAY}\"
;;
restart)
\$0 stop
\$0 start
;;
esac
exit 0" > "/etc/init.d/vncserver"
chmod +x "/etc/init.d/vncserver"
insserv -d "/etc/init.d/vncserver"
update-rc.d vncserver defaults 99
service vncserver restart