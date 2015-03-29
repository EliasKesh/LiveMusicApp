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

while true
do
pid=`pgrep LiveMusicApp`
if [ -z $pid ]
then
theDATE=$(date +"%d%H%M%S")
#	cd /home/Dropbox/LiveEffects
	./LiveMusicApp & 
	sleep 4
	cd /home/Dropbox/LiveEffects
	./GuitarEffects -c
fi
sleep 2
done