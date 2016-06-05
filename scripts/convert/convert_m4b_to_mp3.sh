#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`

echo "Enter m4b directory:"
read m4b_dir

find "$m4b_dir" -name "*.m4b" -print0 | while read -d $'\0' m4b_path
do
	echo "$m4b_path"
	mp3_path=$(echo "$m4b_path" | sed -e 's/\.m4b//g')
	faad --stdio "$m4b_path" | lame --preset standard - "$mp3_path.mp3"
done
