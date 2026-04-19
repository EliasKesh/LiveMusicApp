#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	MplayerConnect
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sun Nov 30 10:18:50 EET 2025
#
#	Copyright : 	2025 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

pw-link -i | grep "Carla.0/LSP Mixer x4 Stereo:Audio input right 3"


if [ "$?" -ne 0 ]
then
    echo "OK!"
        exit

else
    echo "OK!"
fi

sleep .25

pw-link "MPlayer:output_FL"  "Carla.0/LSP Mixer x4 Stereo:Audio input left 3"
pw-link "MPlayer:output_FR"  "Carla.0/LSP Mixer x4 Stereo:Audio input right 3"
 
pw-link -d "MPlayer:output_FL" "alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FL"

pw-link -d "MPlayer:output_FR" "alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FR"





