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

pid=`pgrep -i guitarix | head -n1 `
if [ -z $pid ]
then
# theDATE=$(date +"%d%H%M%S")
echo "Launching Guitarix"
#	cd /home/Dropbox/LiveEffects
    MusicApps.sh EffectsProcessorApp &
	sleep 3
	GuitarEffects -c
	# GuitarEffects -b
fi


sleep 10
qdbus org.freedesktop.ScreenSaver /ScreenSaver SimulateUserActivity

done
