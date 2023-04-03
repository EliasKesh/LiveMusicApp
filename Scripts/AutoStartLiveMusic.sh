#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	AutoStartLiveMusic
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Fri Feb 11 06:12:48 PM PST 2022
#
#	Copyright : 	2022 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
DeviceList=`arecord -l`
# Device="FastTrack"
# Device="USB Audio CODEC"
Device="USB"
echo $DeviceList | grep $Device
if [ "$?" -eq 0 ]; then
    echo "In Music"
    konsole --workdir  /home/elias/livemusic/LiveMusicApp &
    sleep 2
    GuitarEffects -e
fi
