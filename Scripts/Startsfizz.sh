#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	Startsfizz
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sat Apr 18 20:31:54 EEST 2026
#
#	Copyright : 	2026 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#


# source /home/elias/.profile
echo "Startsfizz "$1
# env | grep -E 'XDG|DBUS|PIPEWIRE'
# pw-jack -s 9600 sfizz_jack --jack_autoconnect=true --state "$1" & 
# script -q -c 'pw-jack -s 9600 sfizz_jack  /home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_one_shift_sc.sfz' &


pw-jack bash -c  "exec sfizz_jack '$1'  >> /tmp/sfizzExec w" &
echo "Done sfizz_jack"
