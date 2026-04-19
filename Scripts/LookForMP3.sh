#!/bin/bash
#---------------------------------------------------------------
#
#	File: 	LookForMP3
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sun Aug 18 17:50:57 EEST 2024
#
#	Copyright : 	2024 Elias Keshishoglou all rights reserved.
#
#
#--------------------------------------------------------------#

for file in $(find /home/Music/Learn -iname \*mp4)
do
     # check for .mscbackup
    NewFile=$file".mp3"
    if [ ! -f $NewFile ]; then
        echo "File "$file"  New "$NewFile
#        echo "File not found! "$NewFile
        ffmpeg -i $file $NewFile
    fi
#        LiveMusicCharts.py . -vfiz
done
