#!/bin/bash
local_repo_dumps_path=/opt/SVN/dumps
echo "username=guest
password=" > /tmp/smbclient.conf
smbclient //192.168.40.88/Public -A /tmp/smbclient.conf -c "cd Гамоскин; mkdir SVN; cd SVN; mkdir dumps; exit;"
find "$local_repo_dumps_path" -name "ruxsvn_*" -print0 | while read -d $'\0' dump_path
do
	filename=$(basename $dump_path)
	echo -e "$filename"
	directory=$(dirname $dump_path)
	smbclient //192.168.40.88/Public -A /tmp/smbclient.conf --directory=Гамоскин/SVN/dumps -c "lcd $directory; put $filename; exit;"
	res="$?"
	if [ $res = "0" ]
	then
		rm -rf "$dump_path"
	fi
done
rm /tmp/smbclient.conf
