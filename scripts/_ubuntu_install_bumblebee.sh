#!/bin/bash
if [ -e `pwd`/_ubuntu_install_bumblebee_step2 ]
then
	sudo rm /tmp/_ubuntu_install_bumblebee_step2
	current_release="$(lsb_release -sc)"
	if [ $current_release = "12.04.1" ]
	then
		sudo add-apt-repository ppa:bumblebee/stable
		sudo add-apt-repository ppa:xorg-edgers/ppa
		sudo apt-get update
		sudo apt-get install nvidia-325 nvidia-settings-325
		sudo add-apt-repository -r ppa:xorg-edgers/ppa
		sudo apt-get update
		sudo apt-get install bumblebee bumblebee-nvidia
		sudo apt-get dist-upgrade
	elif [ $current_release = "12.04.2" ]
	then
		sudo add-apt-repository ppa:bumblebee/stable
		sudo add-apt-repository ppa:xorg-edgers/ppa
		sudo apt-get update
		sudo apt-get install nvidia-325 nvidia-settings-325
		sudo add-apt-repository -r ppa:xorg-edgers/ppa
		sudo apt-get install bumblebee bumblebee-nvidia linux-headers-generic-lts-quantal
		sudo apt-get dist-upgrade
	elif [ $current_release = "12.10" ]
	then
		sudo add-apt-repository ppa:bumblebee/stable
		sudo add-apt-repository ppa:xorg-edgers/ppa
		sudo apt-get update
		sudo apt-get install nvidia-325 nvidia-settings-325
		sudo add-apt-repository -r ppa:xorg-edgers/ppa
		sudo apt-get update 
		sudo apt-get install bumblebee bumblebee-nvidia linux-headers-generic
		sudo apt-get dist-upgrade
	elif [ $current_release = "13.04" ]
	then	
		sudo add-apt-repository ppa:bumblebee/stable
		sudo add-apt-repository ppa:xorg-edgers/ppa
		sudo apt-get update
		sudo apt-get install nvidia-325 nvidia-settings-325
		sudo add-apt-repository -r ppa:xorg-edgers/ppa
		sudo apt-get update
		sudo apt-get install bumblebee bumblebee-nvidia
		sudo apt-get dist-upgrade
	fi	
else	
	sudo apt-get update && sudo apt-get dist-upgrade
	sudo touch /tmp/_ubuntu_install_bumblebee_step2	
fi
sudo reboot
read
