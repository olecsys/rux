#!/bin/bash
local_repo_dumps_path=/home/olecsys/rux/scripts
echo "username=guest
password=" > /tmp/smbclient.conf
smbclient //192.168.40.88/Public -A /tmp/smbclient.conf -c "cd Гамоскин; mkdir SVN; cd SVN; mkdir dumps1; exit;"
find "$local_repo_dumps_path" -name "*.sh" -print0 | while read -d $'\0' dump_path
do
	filename=$(basename $dump_path)
	echo -e "$filename"
	directory=$(dirname $dump_path)
	smbclient //192.168.40.88/Public -A /tmp/smbclient.conf --directory=Гамоскин/SVN/dumps1 -c "lcd $directory; put $filename; exit;"
	res="$?"
	if [ $res = "0" ]
	then
		echo "SUCCESS"
	else
		echo "FAILED"
	fi
done
rm /tmp/smbclient.conf