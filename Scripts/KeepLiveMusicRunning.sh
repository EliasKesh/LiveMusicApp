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
set -x
sleep 10

MainOutR="alsa_output.usb-Logitech_G935_Gaming_Headset-00.analog-stereo:playback_FR"
MainOutL="alsa_output.usb-Logitech_G935_Gaming_Headset-00.analog-stereo:playback_FL"

MainOutL="alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FL"
MainOutR="alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FR"

MainOutL="alsa_output.usb-Focusrite_Scarlett_Solo_4th_Gen_S1C3HQ25739430-00.pro-output-0:playback_AUX0"
MainOutR="alsa_output.usb-Focusrite_Scarlett_Solo_4th_Gen_S1C3HQ25739430-00.pro-output-0:playback_AUX1"

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

# pid=`pgrep -fi PCtoSFIZZ.py | head -n1 `
# if [ -z $pid ]
# then
# # theDATE=$(date +"%d%H%M%S")
# echo "Launching PCtoSFIZZ.py"
# 
# # 	python3 /home/elias/workspace/LiveMusicApp/Scripts/PCtoSFIZZ.py & 
# # script -q -c 'python3 /home/elias/workspace/LiveMusicApp/Scripts/PCtoSFIZZ.py' /dev/null &
# pw-jack bash -c  exec "python3 /home/elias/workspace/LiveMusicApp/Scripts/PCtoSFIZZ.py "
# fi

sleep 10
qdbus org.freedesktop.ScreenSaver /ScreenSaver SimulateUserActivity

pw-link -d "alsa_playback.java:output_FL" $MainOutL  >/dev/null 2>&1
pw-link -d "alsa_playback.java:output_FR" $MainOutR  >/dev/null 2>&1

pw-link "alsa_playback.java:output_FL" "Carla.0/LSP Mixer x4 Stereo:Audio input left 2"  >/dev/null 2>&1
pw-link "alsa_playback.java:output_FR" "Carla.0/LSP Mixer x4 Stereo:Audio input right 2"  >/dev/null 2>&1

pw-link -d "MPlayer:output_FL" "alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FL"  >/dev/null 2>&1
pw-link -d "MPlayer:output_FR" "alsa_output.usb-SteelSeries_Arctis_Nova_7-00.analog-stereo:playback_FR"  >/dev/null 2>&1

pw-link -d "MPlayer:output_FL" "alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.pro-output-0:playback_AUX0"  >/dev/null 2>&1
pw-link -d "MPlayer:output_FR" "alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.pro-output-0:playback_AUX1"  >/dev/null 2>&1


# Mplayer
pw-link -d "MPlayer:output_FL"  $MainOutL  >/dev/null 2>&1
pw-link -d "MPlayer:output_FR"  $MainOutR  >/dev/null 2>&1
pw-link "MPlayer:output_FL"  "Carla.0/LSP Mixer x4 Stereo:Audio input left 3"  >/dev/null 2>&1
pw-link "MPlayer:output_FR"  "Carla.0/LSP Mixer x4 Stereo:Audio input right 3"  >/dev/null 2>&1

pw-link "alsa_playback.java:output_FR" "Carla.0/LSP Mixer x4 Stereo:Audio input left 1"  >/dev/null 2>&1
pw-link "alsa_playback.java:output_FL" "Carla.0/LSP Mixer x4 Stereo:Audio input left 2"  >/dev/null 2>&1

# Muse Score
pw-link "mscore:output_1" $MainOutL  >/dev/null 2>&1
pw-link "mscore:output_2" $MainOutR  >/dev/null 2>&1

timeout 1 pw-link sfizz:output_1 "Carla.0/LSP Mixer x4 Stereo:Audio input left 2" >/dev/null 2>&1; true  
timeout 1 pw-link sfizz:output_2 -L "Carla.0/LSP Mixer x4 Stereo:Audio input right 2" >/dev/null 2>&1 ; true
timeout 1 pw-link "Carla.0/MIDI Guitar 2:event-out" "sfizz:input" >/dev/null 2>&1; true

pw-link "Midi-Bridge:LiveMusic OutputFluid (capture)" "Midi-Bridge:RtMidiIn ClientRtMidi input (playback)"  >/dev/null 2>&1



done
   
   
