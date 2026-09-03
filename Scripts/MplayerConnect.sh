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
 
 
bluez_output.7C:E9:13:2C:34:24:playback_FL
 
 
pw-link -d "MPlayer:output_FL" "bluez_output.7C:E9:13:2C:34:24:playback_FL"

pw-link -d "MPlayer:output_FR" "bluez_output.7C:E9:13:2C:34:24:playback_FR"



pw-link -d "MPlayer:output_FL" "alsa_output.usb-Focusrite_Scarlett_Solo_4th_Gen_S1C3HQ25739430-00.pro-output-0:playback_AUX0"

pw-link -d "MPlayer:output_FR" "alsa_output.usb-Focusrite_Scarlett_Solo_4th_Gen_S1C3HQ25739430-00.pro-output-0:playback_AUX1
"

pw-link -d "MPlayer:output_FL" "alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FL"

pw-link -d "MPlayer:output_FR" "alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FR"





