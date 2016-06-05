#!/bin/bash
old_dir="`pwd`/`basename "$0"`"
old_dir=`dirname "$old_dir"`
cd "`dirname "$0"`"
script_dir="`pwd`/`basename "$0"`"
script_dir=`dirname "$script_dir"`

mp4_dir=/home/olecsys/to_convert
if [ ! -d "$mp4_dir" ]
then
	mp4_dir=/home/elk/to_convert
fi
find "$mp4_dir" -name "*.mp4" -print0 | while read -d $'\0' mp4_path
do
	echo "$mp4_path"
	divx_path=$(echo "$mp4_path" | sed -e 's/\.mp4//g')
	if [ ! -e "$divx_path.avi" ]
	then
		ffmpeg -i "$mp4_path" -vf "scale='min(iw,720)':-1" -vtag DIVX -vcodec mpeg4 -qscale:v 4 -acodec libmp3lame -qscale:a 4 "$divx_path.avi" < /dev/null
		res="$?"
		if [ $res != "0" ]
		then
			#rm "$mp4_path"
			rm "$divx_path.avi"
		fi
	fi	
done
