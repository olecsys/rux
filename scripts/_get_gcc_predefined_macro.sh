#!/bin/bash
if [ -h /usr/bin/arm-linux-gnueabi-g++-4.6 ];
then
	echo "" |/usr/bin/arm-linux-gnueabi-g++-4.6 -E -dM -x c++ - | sort
else 
	echo "arm-linux-gnueabi-g++-4.6 does not installed!!!"
fi
if [ -h /usr/bin/g++ ];
then
	echo "" |/usr/bin/g++ -E -dM -x c++ - | sort
else
	echo "g++ does not installed!!!"
fi
read
