#!/bin/bash
current_os=$1
current_user=$2
current_user_group=$3
current_machine="$(uname -m)"	
current_pkg_utility=$4
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
			current_pkg_utility="apt-get"
			current_os="Debian"
			su --preserve-environment -c "bash $0 $current_os $current_user $current_user_group $current_pkg_utility"
		elif [ `grep -i ubuntu /etc/issue | wc -l` -gt "0" ]
		then
			current_pkg_utility="apt-get"
			current_os="Ubuntu"
			sudo bash $0 $current_os $current_user $current_user_group $current_pkg_utility
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
if [ $current_pkg_utility = "apt-get" ]
then
	apt-get install openssl	
fi
openssl_installed="false"
if [ -h /usr/bin/openssl ]
then
	openssl_installed="true"	
elif [ -e /usr/bin/openssl ]	
then
	openssl_installed="true"
else
	echo "openssl does not installed!!!"	
fi
if [ $openssl_installed = "true" ]
then
	if [ ! -d `pwd`/../build ]; then		
		mkdir `pwd`/../build		
		chown -R -- $current_user:$current_user_group `pwd`/../build
	fi
	if [ ! -e `pwd`/../build/private.pem ]	
	then
		openssl genrsa -out `pwd`/../build/private.pem 2048
		openssl rsa -in `pwd`/../build/private.pem -out `pwd`/../build/public.pem -outform PEM -pubout
	fi
	private_key=`grep -v -- ----- ../build/private.pem | tr -d '\n'`	
	
	echo $private_key > ../build/private
	echo $private_key | base64 -d > ../build/base64
	
	echo $private_key | base64 -d | openssl asn1parse -inform DER -i
	public_key=`grep -v -- ----- ../build/public.pem | tr -d '\n'`	
	echo $public_key | base64 -d | openssl asn1parse -inform DER -i
	echo $public_key | base64 -d | openssl asn1parse -inform DER -i -strparse 19	
	#openssl rsautl -encrypt -pkcs -in ../build/text_to_encode -out ../build/encoded_text -inkey ../build/public.pem -pubin
	openssl rsautl -encrypt -raw -in ../addons/text_to_encode -out ../build/encoded_text -inkey ../build/public.pem -pubin
	openssl rsautl -decrypt -raw -in ../build/encoded_text -out ../build/decoded_text -inkey ../build/private.pem
	chown -R -- $current_user:$current_user_group `pwd`/../build/private.pem
	chown -R -- $current_user:$current_user_group `pwd`/../build/public.pem
fi
echo "Press <ENTER> to continue"
read OS
