#!/bin/bash

DISTRIB_URI="//192.168.40.88/IntegraVideo7"
DISTRIB_DIR="/mnt/IntegraVideo7"
DEBUG_REPO_URI="//192.168.40.37/video7_lin"
DEBUG_REPO_DIR="/mnt/video7_linux_debug_symbols"

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
			su --preserve-environment -c "apt-get install cifs-utils subversion binutils && mkdir -p $DISTRIB_DIR && mount -t cifs $DISTRIB_URI $DISTRIB_DIR -o guest,file_mode=0777,dir_mode=0777 && mkdir -p $DEBUG_REPO_DIR && mount -t cifs $DEBUG_REPO_URI $DEBUG_REPO_DIR -o guest,file_mode=0555,dir_mode=0555"
		elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
		then			
			sudo apt-get install cifs-utils subversion binutils && mkdir -p $DISTRIB_DIR && mount -t cifs $DISTRIB_URI $DISTRIB_DIR -o guest,file_mode=0777,dir_mode=0777 && mkdir -p $DEBUG_REPO_DIR && mount -t cifs $DEBUG_REPO_URI $DEBUG_REPO_DIR -o guest,file_mode=0555,dir_mode=0555
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
	apt-get install cifs-utils subversion binutils && mkdir -p $DISTRIB_DIR && mount -t cifs $DISTRIB_URI $DISTRIB_DIR -o guest,file_mode=0777,dir_mode=0777 && mkdir -p $DEBUG_REPO_DIR && mount -t cifs $DEBUG_REPO_URI $DEBUG_REPO_DIR -o guest,file_mode=0555,dir_mode=0555
fi
svn up


OSPL="$(uname -s)"
version=0.0.0
version=$(< `pwd`/client_version.txt)
version=`echo "$version" | sed 's/[^0-9\.]//g'`
PATH_TO_COPY=`pwd`/build_setup/client/$1
PATH_TO_COPY_DEBUG=`pwd`/build_setup/client_debug




distrib_filename=video-client-$version-x64.tar.gz
distrib_debug_filename=video-client-$version-x64.debug.tar.gz
distrib_dir=/mnt/IntegraVideo7/linux/client/x64/
if [  -e $distrib_dir$distrib_filename ]
then
	major=`echo $version | awk -F"." '{print $1}'`
	minor=`echo $version | awk -F"." '{print $2}'`
	revision=`echo $version | awk -F"." '{print $3}'`
	first=${revision:0:1}
	second=${revision:1:1}
	third=${revision:2:1}
	if [ $first = "0" ]
	then
		if [ $second = "0" ]
		then
			revision=$third
		else
			revision=$second$third
		fi
	fi
	revision=$[revision+1]
	len=${#revision}
	if [ $len = "1" ]
	then
		revision=00$revision
	elif [ $len = "2" ]
	then
		revision=0$revision
	fi
	version=$major.$minor.$revision
	echo $version > "`pwd`/client_version.txt"
	distrib_filename=video-client-$version-x64.tar.gz
	distrib_debug_filename=video-client-$version-x64.debug.tar.gz
fi




bash copy_video_client_lin64.sh $PATH_TO_COPY
bash "./load_debug_files.sh"   "$DEBUG_REPO_DIR" "$PATH_TO_COPY" "$PATH_TO_COPY_DEBUG"
rm -rf ./$distrib_filename
rm -rf "./$distrib_debug_filename"
TAR_FLAGS=-czf
if [ $OSPL = "SunOS" ]
then
	TAR_FLAGS=-cf
fi
cd `pwd`/build_setup
tar $TAR_FLAGS $distrib_filename client
tar -C ./client_debug $TAR_FLAGS "$distrib_debug_filename" .
cd ..


if [ ! -e $distrib_dir$distrib_filename ]
then
	mv `pwd`/build_setup/$distrib_filename $distrib_dir$distrib_filename
	mv `pwd`/build_setup/$distrib_debug_filename $distrib_dir$distrib_debug_filename
	svn ci "`pwd`/client_version.txt" -m "Video Client Version $version"
	echo "Distributive '$distrib_filename' was created successfully"
else
	echo "Distributive '$distrib_filename' already exists"		
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
			su --preserve-environment -c "umount $DISTRIB_DIR"
			su --preserve-environment -c "umount $DEBUG_REPO_DIR"
		elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
		then
			current_os="Ubuntu"
			sudo umount $DISTRIB_DIR
			sudo umount $DEBUG_REPO_DIR
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
	umount $DISTRIB_DIR
	umount $DEBUG_REPO_DIR
fi





if [ -d `pwd`/build_setup ]; then
	rm -rf `pwd`/build_setup
fi
