#!/bin/bash
sudo apt-get install qemu-kvm-extras-static qemu-system
if [ ! -d /opt/raspbian/ ]; then
	mkdir /opt/raspbian/
fi
if [ ! -d /media/raspbian/ ]; then
	sudo mkdir /media/raspbian/
fi
file /opt/raspbian/2013-02-09-wheezy-raspbian.img
sudo mount /opt/raspbian/2013-02-09-wheezy-raspbian.img -o offset=62914560 /media/raspbian
sudo nano /media/raspbian/etc/ld.so.preload
sudo umount /opt/raspbian/2013-02-09-wheezy-raspbian.img /media/raspbian
sudo rm -rf /media/raspbian
sudo setcap cap_net_admin_ep /usr/bin/qemu-system-arm
cd /opt/raspbian
qemu-system-arm -kernel kernel-qemu -cpu arm1176 -m 256 -M versatilepb -no-reboot -serial stdio -append "root=/dev/sda2 panic=1" -hda /opt/raspbian/2013-02-09-wheezy-raspbian.img -redir tcp:5022::22
#sudo fsck /dev/sda2
#shutdown -r now
read
