#!/bin/bash
cd /opt/raspbian
qemu-system-arm -kernel kernel-qemu -cpu arm1176 -m 256 -M versatilepb -no-reboot -serial stdio -append "root=/dev/sda2 panic=1" -hda /opt/raspbian/2013-02-09-wheezy-raspbian.img -net nic,vlan=0 -net tap,vlan=0,ifname=tap0,script=/etc/qemu-ifup
read
