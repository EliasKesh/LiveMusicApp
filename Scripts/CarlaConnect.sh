#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	CarlaConnect
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Fri Oct 27 09:44:36 PM PDT 2023
#
#	Copyright : 	2023 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

EffectConnect() {
echo "1->"$1" 2->"$2
InEff=`pw-link -i | grep "$2" | head -n 1`
OutEff=`pw-link -o | grep "$1" | head -n 1`

echo "Main pw-link ["$InEff"] ["$OutEff"]"
pw-link  "$OutEff" "$InEff"

InEff=`pw-link -i | grep "$2" | grep -i _2 | head -n 1`
if [ -z "$InEff" ]; then
  InEff=`pw-link -i | grep "$2" | grep -i in1 | head -n 1`
  if [ -z "$InEff" ]; then
    InEff=`pw-link -i | grep "$2" | grep -i _1 | head -n 1`
    if [ -z "$InEff" ]; then
      InEff=`pw-link -i | grep "$2" | grep -i LR | head -n 1`
    fi
  fi
fi


OutEff=`pw-link -o | grep "$1" | grep -i _2 | head -n 1`
if [ -z "$OutEff" ]; then
  OutEff=`pw-link -o | grep "$1" | grep -i out1 | head -n 1`
  if [ -z "$OutEff" ]; then
    OutEff=`pw-link -o | grep "$1" | grep -i _1 | head -n 1`
    if [ -z "$OutEff" ]; then
      OutEff=`pw-link -o | grep "$1" | grep -i FR | head -n 1`
    fi
  fi
fi

echo "In1 pw-link ["$InEff"] ["$OutEff"]"
pw-link  "$OutEff" "$InEff"

MidiEff=`pw-link -i | grep "$2" | grep -i events-in | head -n 1`
pw-link "$MidiConrol" "$MidiEff"
echo "Midi pw-link ["$MidiConrol"] ["$MidiEff"]"

MidiEff=`pw-link -i | grep "$1" | grep -i events-in | head -n 1`
pw-link "$MidiConrol" "$MidiEff"
echo "Midi pw-link ["$MidiConrol"] ["$MidiEff"]"

# read
}


DisConnect() {

MyID=`pw-link -Il | grep "$1" | head -n 1 | tail -n 1 | awk '{print $1}'`
pw-link -d $MyID
MyID=`pw-link -Il | grep "$1" | head -n 1 | tail -n 2 | awk '{print $1}'`
pw-link -d $MyID
MyID=`pw-link -Il | grep "$1" | head -n 1 | tail -n 3 | awk '{print $1}'`
pw-link -d $MyID
MyID=`pw-link -Il | grep "$1" | head -n 1 | tail -n 4 | awk '{print $1}'`
pw-link -d $MyID

}

MidiControl() {
echo $1
PassThru=`pw-link -o | grep "a2j" | grep "Midi Through Port-0"`

MyID=`pw-link -Il | grep "a2j" | grep "Midi Through Port-0" | head -n 2 | tail -n 1 | awk '{print $1}'`
pw-link -d $MyID
echo "pw-link -d " $MyID

MidiEff="`pw-link -i | grep "$1" | grep -i events-in | head -n 1`"

pw-link "$PassThru" "$MidiEff"
echo "pw-link "$PassThru" "$MidiEff

sendmidi dev "Midi Through Port-0" cc $2 $3

}

Setup() {

# Delete previous links
for str in "${EffectsArr[@]}"; do
  echo "Effect "$str
  DisConnect "$str"
done

# Connect the input
pw-link "$JackInputL" "Carla:Gx Studio Preamp Mono:In"
pw-link "$JackInputR" "Carla:Gx Studio Preamp Mono:In_2"
pw-link "$JackInputR" "Carla:Gx Studio Preamp Mono:In"
pw-link "$JackInputL" "Carla:Gx Studio Preamp Mono:In_2"
echo "pw-link "$JackInputR" "$MidiEff

# inter plugin setup
for i in ${!EffectsArr[@]}; do
  echo "element $i is ${EffectsArr[$i]}"
  EffectConnect "${EffectsArr[$i]}" "${EffectsArr[$i+1]}"
done

# Connect the output
pw-link "StereoFX:output_1" "jack-volume:input_1"
pw-link "StereoFX:output_2" "jack-volume:input_5"

}

export DeviceList="`pw-link -o`"
MidiConrol="Midi-Bridge:LiveMusic Output:(capture_1) Guitarix"

JackInputL="alsa_input.usb-Focusrite_Scarlett_Solo_USB-00.iec958-stereo:capture_FL"
JackInputR="alsa_input.usb-Focusrite_Scarlett_Solo_USB-00.iec958-stereo:capture_FR"

#InputDevice="scarlett"
JackInputL=`pw-link -o | grep -i $InputDevice | head -n 1`
JackInputL=`pw-link -o | grep -i $InputDevice | tail -n 1`


EffectsArr=(
"abGate"
"Compressor"
"Equalizer"
"Wah"
"Distortion"
"Chorus"
"GxZita_rev1"
"StereoFX"
)

cPreAmp=0
cabGate=1
cCompressor=2
cEqualizer=3
cWah=4
cDistortion=5
cChorus=6
cReverb=7
cVolume=8

Setup

case "$1" in

  Jazz)
    echo "Jazz"
    set -x
    MidiControl "${EffectsArr[cChorus]}" 2 100
#    sleep 1
    MidiControl "${EffectsArr[cDistortion]}" 2 0

    ;;

  Str)
    echo "Str"
    MidiControl "${EffectsArr[cChorus]}" 2 20
    MidiControl "${EffectsArr[cDistortion]}" 2 0
 
    ;;

  Blues)
    echo "Blue"
    MidiControl "${EffectsArr[cDistortion]}" 2 100
    
    ;;

  Setup)
    echo "Setup"
    setup
    
    ;;

  *)
    
    ;;
esac



exit

pw-link "Carla:Gx Studio Preamp Mono:Out" "Carla:abGate:Input"

pw-link "Carla:abGate:Output" "Carla:Calf Mono Compressor:In L"

pw-link "Carla:Calf Mono Compressor:Out L" "Carla:LSP Graphic Equalizer x16 Mono:Input"

pw-link "Carla:LSP Graphic Equalizer x16 Mono:Output" "Carla:GxChorus-Stereo:In"
pw-link "Carla:LSP Graphic Equalizer x16 Mono:Output" "Carla:GxChorus-Stereo:In1"

pw-link  "Carla:GxZita_rev1-Stereo:Out" "Carla:GxChorus-Stereo:In"

pw-link "Carla:GxMultiBandDistortion:In" "Carla:GxZita_rev1-Stereo:Out" 



exit

Carla:Gx Studio Preamp Mono:Out
Carla:abGate:Output
Carla:Calf Mono Compressor:Out L
Carla:Calf Mono Compressor:events-out
Carla:LSP Graphic Equalizer x16 Mono:Output
Carla:LSP Graphic Equalizer x16 Mono:events-out
Carla:GxChorus-Stereo:Out
Carla:GxChorus-Stereo:Out1
Carla:GxZita_rev1-Stereo:Out
Carla:GxZita_rev1-Stereo:Out1
Carla:GxMultiBandDistortion:Out
Carla:GxMultiBandDistortion:events-out


Carla:Gx Studio Preamp Mono:In
Carla:Gx Studio Preamp Mono:events-in
Carla:abGate:Input
Carla:abGate:events-in
Carla:Calf Mono Compressor:In L
Carla:Calf Mono Compressor:events-in
Carla:LSP Graphic Equalizer x16 Mono:Input
Carla:LSP Graphic Equalizer x16 Mono:events-in
Carla:GxChorus-Stereo:In
Carla:GxChorus-Stereo:In1
Carla:GxChorus-Stereo:events-in
Carla:GxZita_rev1-Stereo:In
Carla:GxZita_rev1-Stereo:In1
Carla:GxZita_rev1-Stereo:events-in
Carla:GxMultiBandDistortion:In
Carla:GxMultiBandDistortion:events-in

# jack_lsp
# Dry/Wet is breath CC#2
# sendmidi dev "Midi Through Port-0" cc 2 0
# pw-link "a2j:Midi Through [14] (capture): Midi Through Port-0" "abGate:events-in"
# learn midi control via paramaters in Carla 

# QT_SCREEN_SCALE_FACTORS=1.6 nice -18 pw-jack carla-jack-multi /home/MySongs/CarlaConfig/Guitarix_Jazz_Rack.carxp &



