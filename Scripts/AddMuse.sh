#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	AddMuse
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sun May 19 23:03:42 EEST 2024
#
#	Copyright : 	2024 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

for file in $(find /home/Music/Learn -iname \*gp?)
do
     # check for .mscbackup
    NewFile=$file".mscz"
#    echo "File "$file"  New "$NewFile
    if [ ! -f $NewFile ]; then
        echo "File not found! "$NewFile
        mscore -f $file -S EliasTab -o $NewFile
    fi
done



