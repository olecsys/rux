Windows:
1) Install MozillaBuild
2) C:\mozilla-build\start-shell-msvc2013.bat
3) Enter in opened terminal window: 
	cd js/src
	autoconf-2.13
	mkdir build_DBG.OBJ
	cd build_DBG.OBJ
	../configure --disable-shared-js 
#	(for x64 add '--target=x86_64-pc-mingw32 --host=x86_64-pc-mingw32')
	make