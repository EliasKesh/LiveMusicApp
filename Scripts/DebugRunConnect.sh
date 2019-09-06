#! /bin/sh
#---------------------------------------------------------------------
#
#	File: 	DebugRunConnect
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Thu Apr 11 08:41:59 PDT 2019
#
#	Copyright ©: 	2019 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

LiveMusicApp -v 6 > ./LiveText.txt &
# LiveMusicApp  &
sleep 3
aconnect "VMPK Output" "LiveMusic Input"
aconnect "LiveMusic Output":0 KMidimon
aconnect "LiveMusic Output":1 KMidimon
aconnect "LiveMusic Output":2 KMidimon
aconnect "LiveMusic Output":3 KMidimon

aconnect "EliasPedal3":0 "LiveMusic Input":0
aconnect "Fishman TriplePlay":0 "LiveMusic Input":0

aconnect  "LiveMusic Output":6 "EliasPedal3":0

#aconnect "LiveMusic Output":Tempo KMidimon
#aconnect "LiveMusic Output":Click KMidimon
#aconnect "LiveMusic Output":Pedal KMidimon

