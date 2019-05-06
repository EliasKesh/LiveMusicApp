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

./LiveMusicApp & 
sleep 3
aconnect "VMPK Output" "LiveMusic Input"
aconnect "LiveMusic Output":0 KMidimon
aconnect "LiveMusic Output":1 KMidimon
aconnect "LiveMusic Output":2 KMidimon
aconnect "LiveMusic Output":3 KMidimon
#aconnect "LiveMusic Output":Tempo KMidimon
#aconnect "LiveMusic Output":Click KMidimon
#aconnect "LiveMusic Output":Pedal KMidimon

