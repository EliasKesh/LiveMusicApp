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

sleep 10

cd /home/Dropbox/LiveEffects
while true
do
systemctl --user reenable dbus-org.jackaudio.service
pid=`pgrep LiveMusicApp`
if [ -z $pid ]
then
theDATE=$(date +"%d%H%M%S")
echo "Launching LiveMusic"
#	cd /home/Dropbox/LiveEffects
 nice -15 `GTK_THEME=LiveMusicTheme LiveMusicApp -e -v 1 -l 1` >> $LogFile 2>&1 &
	sleep 3
	./GuitarEffects -c
	./GuitarEffects -b
fi


pid=`pgrep guitarix`
if [ -z $pid ]
then
theDATE=$(date +"%d%H%M%S")
echo "Launching Guitarix"
#	cd /home/Dropbox/LiveEffects
	guitarix  -p 7000 & 
	sleep 3
	./GuitarEffects -c
	./GuitarEffects -b
fi
#    jack_connect Clementine:out_jackaudiosink-1_1 jack-volume:playback_1
#    jack_connect Clementine:out_jackaudiosink-1_1 jack-volume:playback_1

sleep 5
done