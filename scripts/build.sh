#!/bin/bash

function usage() {
	local script=$0
	__funcerror="Usage: $script [-s tests|rux_configure] [-q true|false] [-a x86|x64] 
-s(skip tests, rux.configure)	
-q(quiet true, false)
-a(architecture x86, x64)
"
	__funcfailed=1	
}
function main() {
	local old_dir="`pwd`/`basename "$0"`"
	old_dir=`dirname "$old_dir"`
	cd "`dirname "$0"`"
	local script_dir="`pwd`/`basename "$0"`"
	script_dir=`dirname "$script_dir"`
	cd "$old_dir"

	__funcfailed=0
	local red='\e[0;31m'
	local green='\e[0;32m'
	local nocolor='\e[0m'

	if [ $__funcfailed -eq 0 ]
	then
		local skip_tests=0
		local skip_rux_configure=0
		local skips=
		local quiet=false
		local platform=x64
		while getopts s:q:a: opt
		do
		case $opt in
		q) quiet=$OPTARG
		;;
		a) platform=$OPTARG
		;;
		s) IFS='|' read -ra skips <<< "$OPTARG"
		for skip in "${skips[@]}"
		do
			case $skip in
			rux_configure) skip_rux_configure=1
			;;
			tests) skip_tests=1
			;;
			esac
		done
		;;
		esac
		done

		local os=linux
		local msbuild2010=''
		local iswindows=1		
		if ! [ -z "$WINDIR" ]; then
			msbuild2010=$WINDIR/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe
			case $platform in
			x86) platform=Win32
			;;
			esac
		else
			iswindows=0
		fi

		if [ $__funcfailed -eq 0 ]; then
			if [ $iswindows -eq 1 ]; then
				"$msbuild2010" "$script_dir/../rux.framework/rux.framework.vcxproj" //verbosity:n //p:Configuration=Release //p:Platform=$platform //t:Clean
				"$msbuild2010" "$script_dir/../rux.framework/rux.framework.vcxproj" //verbosity:n //p:Configuration=Release //p:Platform=$platform //t:build || __funcfailed=1	
			else
				"$script_dir/../tools/rux.configure/$os/$platform/rux.configure" --build-project "$script_dir/../rux.framework/build.ruxprj" --configuration "$os $platform" || __funcfailed=1
			fi
		fi
		if [ $skip_tests -eq 0 ]; then
			if [ $__funcfailed -eq 0 ]; then
				if [ $iswindows -eq 1 ]; then
					"$msbuild2010" "$script_dir/../rux.framework/test/test.vcxproj" //verbosity:n //p:Configuration=Release //p:Platform=$platform //t:Clean
					"$msbuild2010" "$script_dir/../rux.framework/test/test.vcxproj" //verbosity:n //p:Configuration=Release //p:Platform=$platform //t:build || __funcfailed=1
				else
					"$script_dir/../tools/rux.configure/$os/$platform/rux.configure" --build-project "$script_dir/../rux.framework/test/test.ruxprj" --configuration "$os $platform" || __funcfailed=1
				fi
			fi
		fi
		if [ $__funcfailed -eq 0 ]; then
			if [ $iswindows -eq 1 ]; then
				"$msbuild2010" "$script_dir/../rux.engine/rux.engine.vcxproj" //verbosity:n //p:Configuration=Release //p:Platform=$platform //t:Clean
				"$msbuild2010" "$script_dir/../rux.engine/rux.engine.vcxproj" //verbosity:n //p:Configuration=Release //p:Platform=$platform //t:build || __funcfailed=1	
			else
				"$script_dir/../tools/rux.configure/$os/$platform/rux.configure" --build-project "$script_dir/../rux.engine/build.ruxprj" --configuration "$os $platform" || __funcfailed=1
			fi
		fi
		if [ $skip_tests -eq 0 ]; then
			if [ $__funcfailed -eq 0 ]; then
				if [ $iswindows -eq 1 ]; then
					"$script_dir/../build/$os/$platform/rux.framework.test.exe" || __funcfailed=1
				else
					"$script_dir/../build/$os/$platform/rux.basis.test" || __funcfailed=1
				fi
			fi
		fi
		if [ $skip_rux_configure -eq 0 ]; then
			if [ $__funcfailed -eq 0 ]; then
				if [ $iswindows -eq 0 ]; then
					"$script_dir/../tools/rux.configure/$os/$platform/rux.configure" --build-project "$script_dir/../rux.configure/build.ruxprj" --configuration "$os $platform" || __funcfailed=1
				fi
			fi
		fi
	fi
	if [ $__funcfailed -eq 0 ]
	then
		if [ "$quiet" == "true" ]; then
			echo -e "${green}rux project building succeeded${nocolor}"
		else
			echo -e "${green}rux project building succeeded.Press <ENTER> to continue${nocolor}"
			read
		fi
	else
		if [ "$quiet" == "true" ]; then
			echo -e "${red}rux project building failed${nocolor}"
		else
			echo -e "${red}rux project building failed.Press <ENTER> to continue${nocolor}"
		fi
		if ! [ -z "$__funcerror" ]; then
			echo -e "${red}Error: $__funcerror${nocolor}"
		fi
	fi
	cd "$old_dir"
}
main $@
if [ $__funcfailed -eq 0 ]; then
	exit 0
else
	exit 1
fi