#! /bin/sh
#---------------------------------------------
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
#---------------------------------------------#


# ./LiveMusicApp -v 6 > ./LiveText.txt &

pid=`pgrep -i LiveMusicApp`
if [ -z $pid ]
then
# ./LiveMusicApp -v 0x140 &
./LiveMusicApp  &
sleep 3
fi 

aconnect "VMPK Output" "LiveMusic Input":0
aconnect "LiveMusic Output":0 "qmidiroute:in"
aconnect "LiveMusic Output":1 "qmidiroute:in"
aconnect "LiveMusic Output":2 "qmidiroute:in"
aconnect "LiveMusic Output":3 "qmidiroute:in"
aconnect "LiveMusic Output":7 "qmidiroute:in"

aconnect "EliasPedal3":0 "LiveMusic Input":0
aconnect "Fishman TriplePlay":0 "LiveMusic Input":0
aconnect "LPD8":0 "LiveMusic Input":0
aconnect "USB Uno MIDI Interface":0 "LiveMusic Input":0
aconnect "Midi Through":0 "LiveMusic Input":0

aconnect  "LiveMusic Output":6 "EliasPedal3":0
aconnect  "LiveMusic Output":6 "LPD8":0
aconnect  "LiveMusic Output":6 "USB Uno MIDI Interface":0
aconnect "LiveMusic Output":"5" "FLUID Synth (qsynth)":"0"
aconnect "LiveMusic Output":"8" "FLUID Synth (qsynth)":"0"

aconnect  "Reloop KeyFadr":"0" "LiveDAW Input":"0"
aconnect  "LiveMusic Output":7 "Reloop KeyFadr":"0"

# aconnect  "LiveMusic Output":6 "Midi Through":0

#aconnect "LiveMusic Output":Tempo KMidimon
#aconnect "LiveMusic Output":Click KMidimon
#aconnect "LiveMusic Output":Pedal KMidimon

exit

aconnect -d "Fishman TriplePlay":"0" "LiveMusic Input":"0"
aconnect -d "USB Uno MIDI Interface":"0" "LiveMusic Input":"0"
aconnect -d "EliasPedal3":"0" "LiveMusic Input":"0"
aconnect -d "LPD8":"0" "LiveMusic Input":"0"
aconnect -d "Reloop KeyFadr MIDI 1":"0" "LiveDAW Input":"0"

aconnect -d "LiveMusic Output":"0" "FLUID Synth (qsynth)":"0"
aconnect -d "LiveMusic Output":"5" "FLUID Synth (qsynth)":"0"
aconnect -d "LiveMusic Output":"6" "EliasPedal3":"0"
aconnect -d "LiveMusic Output":"7" "LPD8":"0"
aconnect -d "Midi Through":"0" "FLUID Synth (qsynth)":"0"





