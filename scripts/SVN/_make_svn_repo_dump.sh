#!/bin/bash
repo_path=/opt/SVN/rux
local_repo_dumps_path=/opt/SVN/dumps
mkdir -p "$local_repo_dumps_path"

if [ -e  "$local_repo_dumps_path/svnrev" ]
then
	old_rev=`cat $local_repo_dumps_path/svnrev`
else
	old_rev=0
fi
new_rev=`svnlook youngest $repo_path`
if [ $old_rev != $new_rev ]; then
	if [ $old_rev != "0" ]; then
		old_rev=$(($old_rev+1))
	fi
	date_string="$(date +%Y_%m_%d_%H_%M_%S)"
	dump_path=/opt/SVN/dumps/ruxsvn_$date_string\_rev_$old_rev\_$new_rev.dmp
	svnadmin dump -q -r $old_rev:$new_rev --incremental "$repo_path" > "$dump_path"
	res="$?"
	if [ $res = "0" ]
	then
		bzip2 "$dump_path"
		res="$?"
		if [ $res = "0" ]
		then
			chmod 777 "$dump_path".bz2
			echo -n $new_rev > "$local_repo_dumps_path/svnrev"
		fi
	fi
	rm -rf "$dump_path"
fi
