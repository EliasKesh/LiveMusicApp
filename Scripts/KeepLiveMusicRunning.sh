#! /bin/sh
#---------------------------------------------------------------------
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
#---------------------------------------------------------------------#

sleep 4

while true
	cd /home/Dropbox/LiveEffects
do
pid=`pgrep LiveMusicApp`
if [ -z $pid ]
then
theDATE=$(date +"%d%H%M%S")
#	cd /home/Dropbox/LiveEffects
	./LiveMusicApp & 
	sleep 3
	./GuitarEffects -c
	./GuitarEffects -b
fi


pid=`pgrep guitarix`
if [ -z $pid ]
then
theDATE=$(date +"%d%H%M%S")
#	cd /home/Dropbox/LiveEffects
	guitarix & 
	sleep 3
	./GuitarEffects -c
	./GuitarEffects -b
fi
#    jack_connect Clementine:out_jackaudiosink-1_1 jack-volume:playback_1
#    jack_connect Clementine:out_jackaudiosink-1_1 jack-volume:playback_1

sleep 5
done