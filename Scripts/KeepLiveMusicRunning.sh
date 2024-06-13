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
# systemctl --user reenable dbus-org.jackaudio.service
pid=`pgrep -i LiveMusicApp`
if [ -z $pid ]
then
theDATE=$(date +"%d%H%M%S")
echo "Launching LiveMusic"
#	cd /home/Dropbox/LiveEffects
	if [[ $3 == "Yes" ]]; then
 		nice -15 `GDK_DPI_SCALE=1.4 GTK_THEME=LiveMusicTheme $1 -w -e -v 1 -l 1 -f 100` >> $LogFile 2>&1 &
 	else
 		nice -15 `GDK_DPI_SCALE=1.4 GTK_THEME=LiveMusicTheme $1 -e -v 1 -l 1 -f 140` >> $LogFile 2>&1 &
 	fi

	sleep 3
	GuitarEffects -c
	GuitarEffects -b
fi

pid=`pgrep -i guitarix | head -n1 `
if [ -z $pid ]
then
theDATE=$(date +"%d%H%M%S")
echo "Launching Guitarix"
#	cd /home/Dropbox/LiveEffects
    MusicApps.sh EffectsProcessorApp &
	# sleep 6
	# GuitarEffects -c
	# GuitarEffects -b
fi
#    jack_connect Clementine:out_jackaudiosink-1_1 jack-volume:playback_1
#    jack_connect Clementine:out_jackaudiosink-1_1 jack-volume:playback_1

sleep 6
qdbus org.freedesktop.ScreenSaver /ScreenSaver SimulateUserActivity

# GuitarEffects -c

#jack_disconnect "a2j:LiveMusic Output [129] (capture): [1] GuitarEffects" "gx_head_amp:midi_in_1"
#jack_connect "a2j:LiveMusic Output [129] (capture): [1] GuitarEffects" "gx_head_amp:midi_in_1"
done
