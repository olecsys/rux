#!/bin/bash
RANLIB=ranlib
ARCHIVER=ar
ARCHIVER_FLAGS=rcus
OSPL="$(uname -s)"
MODULE_NAME="librux.basis.a"
OLD_PATH=`pwd`
BUILD_PATH="../build"
if [ $OSPL = "SunOS" ]
then
	cd "$BUILD_PATH"
	BUILD_PATH=`pwd`
	ARCHIVER=/usr/ccs/bin/ar
	ARCHIVER_FLAGS=-rv
	RANLIB=/usr/ccs/bin/ranlib
elif [ $OSPL = "Linux" ]
then
	BUILD_PATH="$1"
	cd "$BUILD_PATH"
	BUILD_PATH=`pwd`
	ARCHIVER=ar
fi
if [ -e "$BUILD_PATH/libz.a" ]
then
	rm -rf "$BUILD_PATH/temp"
	mkdir "$BUILD_PATH/temp"
	cd "$BUILD_PATH/temp"
	$ARCHIVER x "$BUILD_PATH/libz.a"
	cd "$OLD_PATH"
	find $BUILD_PATH/temp/*.o -exec $ARCHIVER $ARCHIVER_FLAGS "$BUILD_PATH/$MODULE_NAME" {} \;
	$RANLIB "$BUILD_PATH/$MODULE_NAME"
	rm -rf "$BUILD_PATH/temp"
fi
