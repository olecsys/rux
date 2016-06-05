#!/bin/bash
mkdir /opt/raspberrypi
sudo apt-get install git
cd /opt/raspberrypi
sudo git clone git://github.com/raspberrypi/tools.git
sudo ln -sf /opt/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-objdump /usr/bin/raspbian-arm-objdump
read
