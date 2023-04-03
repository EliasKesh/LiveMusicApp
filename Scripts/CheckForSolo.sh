#!/bin/bash
#---------------------------------------------------------------------
#
#   File:   CheckForSolo
#
#   Contains:
#
#
#   Written By:     Elias Keshishoglou on Sun Mar 21 12:07:41 PM PDT 2021
#
#   Copyright :     2021 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

cd /home/MySongs/Solo
for dir in $(find ../Charts -name BK\*.mp3 )
do
    theName=`dirname $dir`
    x=$(echo "$theName" | sed 's/^..\/Charts\///')
    
    if [[ ! -L $x ]]
    then
        echo "Creating "$theName
        ln -s $theName ./
#    else
#        echo "$x exists."       
    fi
    #  echo `sed -i s/^.// $dir`
done

