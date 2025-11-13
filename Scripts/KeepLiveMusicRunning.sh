#!/bin/bash
#------------------------------------------------
#
#	File: 	KeepLiveAppRunning
#
#	Contains: 	This will keep LiveMusic app running if it quits.
#
#
#	Written By: 	Elias Keshishoglou on Thu Jun 25 20:49:03 PDT 2009
#
#	Copyright ©: 	2015 Elias Keshishoglou
#
#
#------------------------------------------------#

sleep 10

MainOutR="alsa_output.usb-Logitech_G935_Gaming_Headset-00.analog-stereo:playback_FR"
MainOutL="alsa_output.usb-Logitech_G935_Gaming_Headset-00.analog-stereo:playback_FL"

MainOutL="alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FL"
MainOutR="alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FR"

while true
do
pid=`pgrep -i LiveMusicApp`
if [ -z $pid ]
then
# theDATE=$(date +"%d%H%M%S")
echo "Launching LiveMusic"
    MusicApps.sh LiveMusicApp

	sleep 3
	GuitarEffects -c
	GuitarEffects -b
fi

pid=`pgrep -i Carla | head -n1 `
if [ -z $pid ]
then
# theDATE=$(date +"%d%H%M%S")
echo "Launching Carla"
#	cd /home/Dropbox/LiveEffects
    MusicApps.sh CarlaGuitar &
	sleep 3
	GuitarEffects -c
	# GuitarEffects -b
fi


# pid=`pgrep -i guitarix | head -n1 `
# if [ -z $pid ]
# then
# # theDATE=$(date +"%d%H%M%S")
# echo "Launching Guitarix"
# #	cd /home/Dropbox/LiveEffects
#     MusicApps.sh EffectsProcessorApp &
# 	sleep 3
# 	GuitarEffects -c
# 	# GuitarEffects -b
# fi

sleep 10
qdbus org.freedesktop.ScreenSaver /ScreenSaver SimulateUserActivity

pw-link -d "alsa_playback.java:output_FL" $MainOutL
pw-link -d "alsa_playback.java:output_FR" $MainOutR

pw-link "alsa_playback.java:output_FL" "Carla.0/LSP Mixer x4 Stereo:Audio input left 2"
pw-link "alsa_playback.java:output_FR" "Carla.0/LSP Mixer x4 Stereo:Audio input right 2"

pw-link -d "MPlayer:output_FL" "alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FL"
pw-link -d "MPlayer:output_FR" "alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FR"

pw-link -d "MPlayer:output_FL" "alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.pro-output-0:playback_AUX0"
pw-link -d "MPlayer:output_FR" "alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.pro-output-0:playback_AUX1"


# Mplayer
pw-link -d "MPlayer:output_FL"  $MainOutL
pw-link -d "MPlayer:output_FR"  $MainOutR
pw-link "MPlayer:output_FL"  "Carla.0/LSP Mixer x4 Stereo:Audio input left 3"
pw-link "MPlayer:output_FR"  "Carla.0/LSP Mixer x4 Stereo:Audio input right 3"

pw-link "alsa_playback.java:output_FL" "Carla.0/LSP Mixer x4 Stereo:Audio input left 2"
pw-link "alsa_playback.java:output_FL" "Carla.0/LSP Mixer x4 Stereo:Audio input left 2"

done
