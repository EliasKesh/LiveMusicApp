#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	EffectsConnect
#
#	Contains:
#
#
#	Written By: 	Elias Keshishoglou on Wed Jun 26 07:38:22 EEST 2024
#
#	Copyright : 	2024 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
# set -x

# Set prefix for identifying Carla plugins and mixer devices
PreFix="Carla.0"
PreFixMix="Carla.0/"

#---------------------------------------------------------------------#
#*** ClearRack
#---------------------------------------------------------------------#
# ClearRack: Disconnect all audio connections with the specified prefix
function ClearRack {
    # Get and disconnect all connections containing PreFix
    IDList=$(pw-link --id -l | grep $PreFix | awk '{print $1}')
    // IDList=$(pw-link --id -o | grep $PreFix | awk '{print $1}')

    for theID in $IDList; do
        echo "Link "$theID
        pw-link -d $theID
        sleep 0.08
    done
}

#---------------------------------------------------------------------#
#*** ClearPlasma
#---------------------------------------------------------------------#
# ClearPlasma: Disconnect all Plasma PA audio connections 
function ClearPlasma {
    # Get and disconnect all Plasma PA connections
    IDList=$(pw-link --id -l | grep "Plasma PA" | awk '{print $1}')
# IDList=$(pw-link --id -o | grep "Plasma PA" | awk '{print $1}')

    for theID in $IDList; do
        echo "Link "$theID
        pw-link -d $theID
        sleep 0.08
    done
}

#---------------------------------------------------------------------#
#*** AlsaSet
#---------------------------------------------------------------------#
# AlsaSet: Configure ALSA mixer settings for all audio interfaces
function AlsaSet {

    # Detect available audio sinks and sources
    MySinks=$(aplay -l | grep "card 0:" | head -n 1)
    if [ "$?" -eq 0 ]; then
        echo "Sink Found "$MySinks
    fi

    MySources=$(arecord -l | grep "card 0:" | head -n 1)
    if [ "$?" -eq 0 ]; then
        echo "Source Found "$MySources
    fi

    # ----Card ------------------------------
    # Set volume levels for various audio cards
    CardNum=$(aplay -l | grep G935 | awk -F'[\t: ]' '{print $2}')
    # amixer -c $CardNum set  'PCM',0 50
    amixer -c $CardNum set 'Mic',0 65

    # ----Card ------------------------------
    CardNum=$(aplay -l | grep -i nux | awk -F'[\t: ]' '{print $2}')
    amixer -c $CardNum set "NUX USB Audio 2.0 ",0 2032
    amixer -c $CardNum set "NUX USB Audio 2.0 ",1 2032
    amixer -c $CardNum set 'Mic',0 2032
    amixer -c $CardNum set 'Mic',1 2032

    # ----Card ------------------------------
    CardNum=$(aplay -l | grep -i "Scarlett" | awk -F'[\t: ]' '{print $2}')
    # No Alsa Controls

    # ----Card ------------------------------
    CardNum=$(aplay -l | grep -i "USB" | head -n1 | tail -n1 | awk -F'[\t: ]' '{print $2}')
    amixer -c $CardNum set "Master",0 75
    amixer -c $CardNum set "Headphone",0 80
    amixer -c $CardNum set "PCM",0 220

    # ----Card ------------------------------
    CardNum=$(aplay -l | grep -i "USB" | head -n2 | tail -n1 | awk -F'[\t: ]' '{print $2}')
    amixer -c $CardNum set "Master",0 75
    amixer -c $CardNum set "Headphone",0 80
    amixer -c $CardNum set "PCM",0 220

    #amixer  set  'Master',0 35000 35000 on

    # amixer -c 0 scontrols
    amixer -c 0 set -M 'Master',0 65 on
    amixer -c 0 set 'Headphone',0 75 unmute
    # amixer -c 0 set  'Speaker',0 0 mute
    amixer -c 0 set 'PCM',0 100
    #amixer -c 0 set  'Mic Boost',0 85
    # amixer -c 0 set  'IEC958',0 0
    # amixer -c 0 set  'IEC958',1 0
    # amixer -c 0 set  'IEC958',2 0
    # amixer -c 0 set  'IEC958',3 0
    # amixer -c 0 set  'IEC958',4 0
    # amixer -c 0 set  'IEC958',5 0
    # amixer -c 0 set  'IEC958',6 0
    # amixer -c 0 set  'IEC958',7 0
    # amixer -c 0 set  'IEC958',8 0
    # amixer -c 0 set  'IEC958',9 0
    # amixer -c 0 set  'IEC958',10 0
    # amixer -c 0 set  'IEC958',11 0

    amixer -c 0 set 'Capture',0 100
    # amixer -c 0 set  'Auto-Mute Mode',0 0
    amixer -c 0 set 'Digital',0 0

    # Master Playback Volume
    # amixer -c 0 cset numid=9 100 unmute

    # Headphone
    amixer -c 0 cset numid=1 80 unmute

    # Mic Boost
    amixer -c 0 cset numid=8 80 unmute

    # Capture Volume
    amixer -c 0 cset numid=6 80 unmute

    # Speaker playback switch
    amixer -c 0 cset numid=4 0 unmute

    # Speaker playback volume
    amixer -c 0 cset numid=4 0 mute

    # PCM Playback
    amixer -c 0 cset -M numid=100 80 unmute
    #fi

    # amixer -c 1 scontrols
    amixer -c 1 set 'PCM',0 100

    # amixer -c 2 scontrols
    amixer -c 2 set 'PCM',0 100
    amixer -c 1 set 'Mic',0 1950
    amixer -c 1 set 'Mic',1 1950
    amixer -c 2 set 'Mic',0 1950
    amixer -c 2 set 'Mic',1 1950
    amixer -c 3 set 'Mic',0 1950
    amixer -c 3 set 'Mic',1 1950
    amixer -c 4 set 'Mic',0 1950
    amixer -c 4 set 'Mic',1 1950
    amixer -c 5 set 'Mic',0 1950
    amixer -c 5 set 'Mic',1 1950

    # amixer -c 2 scontrols
    amixer -c 2 set 'Speaker',0 100
    # amixer -c 2 set 'Mic',0 100
    amixer -c 2 set 'Auto Gain Control',0 on

    amixer -c 3 set 'PCM',0 45
    amixer -c 3 set -M 'Master',0 100 on

    amixer -c 4 set 'PCM',0 100
    amixer -c 5 set 'PCM',0 100
    amixer -c 6 set 'PCM',0 100
    amixer -c 7 set 'PCM',0 100
    amixer -c 8 set 'PCM',0 100
    amixer -c 9 set 'PCM',0 100

}

#---------------------------------------------------------------------#
#*** CheckInterface
# Device
# Input Volume
# Output Volume
#
#---------------------------------------------------------------------#
# CheckInterface: Verify audio interface exists and set its volumes
function CheckInterface {

    echo "CheckInterface " $1

    echo "$DeviceList" | grep "${Device}" >/dev/null 2>&1
    if [ "$?" -ne 0 ]; then
        InterfaceFound=0
        return 1
    fi

    echo "We Found " $1

    InLinkL=$(echo "$DeviceList" | grep "${1}" | grep -i capture | head -n 1) >/dev/null 2>&1
    InLinkR=$(echo "$DeviceList" | grep "${1}" | grep -i capture | head -n 2 | tail -n 1) >/dev/null 2>&1

    OutLinkL=$(echo "$DeviceListI" | grep "${1}" | grep -i playback | head -n 1) >/dev/null 2>&1
    OutLinkR=$(echo "$DeviceListI" | grep "${1}" | grep -i playback | head -n 2 | tail -n 1) >/dev/null 2>&1

    SetVolumes $1 $2 $3 $4
    InterfaceFound=1
    return 0
}

#---------------------------------------------------------------------#
#*** SetVolumes
#---------------------------------------------------------------------#
# SetVolumes: Set input/output volumes for an audio interface
function SetVolumes {
    echo "****SetVolumes "$1 $2 $3 $4
    # Set sink (output) volume
    InputVolume=$(pactl list short sinks | grep $1 | grep output | awk '{print $1}')
    pactl set-sink-volume $InputVolume $2"%"

    OutputVolume=$(pactl list short sources | grep $1 | grep input | awk '{print $1}')
    echo "SetVolumes Capture "$OutputVolume
    pamixer --set-volume $3 --allow-boost --source $OutputVolume
}

#---------------------------------------------------------------------#
#*** FindEffects
#---------------------------------------------------------------------#
# FindEffects: Locate effect plugin inputs/outputs in Carla
function FindEffects {

    echo "Find Effects ${1}"

    echo "$DeviceList" | grep "${1}" >/dev/null 2>&1

    if [ "$?" -ne 0 ]; then
        EffectFound=0
        return 1
    fi

    echo "Effect Found " "${1}"
    echo "$DeviceList" | grep "${1} | grep -i out" >/dev/null 2>&1
    echo "$DeviceListI" | grep "${1} | grep -i in" >/dev/null 2>&1

    ChainOutNextL="${EffectOutL}"
    ChainOutNextR="${EffectOutR}"
    ChainInNextL="${EffectInL}"
    ChainInNextR="${EffectInR}"

    EffectInL="$(echo "$DeviceListI" | grep "${1}" | grep -i in | head -n 1)"
    EffectInR="$(echo "$DeviceListI" | grep "${1}" | grep -i in | head -n 2 | tail -n 1)"

    EffectOutL="$(echo "$DeviceList" | grep "${1}" | grep -i out | head -n 1)"
    EffectOutR="$(echo "$DeviceList" | grep "${1}" | grep -i out | head -n 2 | tail -n 1)"

    # echo $EffectInL
    # echo $EffectInR
    # echo $EffectOutL
    # echo $EffectOutR
    EffectFound=1
    return 0

}

#---------------------------------------------------------------------#
#*** ChainNext
#---------------------------------------------------------------------#
# ChainNext: Connect stereo effect chain links
function ChainNext {
    echo "${1}" "${EffectInL}"
    echo "${2}" "${EffectInR}"

    # Connect left and right channels if effect was found
    if [ $EffectFound -ne 0 ]; then
        DoLink "${1}" "${EffectInL}"
        DoLink "${2}" "${EffectInR}"
    fi

}

function ChainNextLeft {
    if [ $EffectFound -ne 0 ]; then
        DoLink "${1}" "${EffectInL}"
    fi

}

function ChainNextRight {
    if [ $EffectFound -ne 0 ]; then
        DoLink "${1}" "${EffectInR}"
    fi

}

function DoLink {
    # Link two devices
    pw-link "${1}" "${2}"
}

function OldRack {

    # -------------------------------------------------------------------
    # Main Guitarix effect chain

    # Guitarix Head to Effects
    FindEffects gx_head_fx
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    # Guitarix Out to Carla Boost Preamp.
    FindEffects "${PreFix}:rkr Parametric EQ:Audio In"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "${PreFix}:the infamous lush life:Audio In"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    # Boost Preamp to Carla Mixer
    FindEffects "${PreFixMix}LSP Mixer x4 Stereo:Output"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    # Carla Mixer to output Device
    DoLink "${EffectOutL}" "${MainOutputL}"
    DoLink "${EffectOutR}" "${MainOutputR}"
    # Main Guitarix effect chain
    # -------------------------------------------------------------------

    # -------------------------------------------------------------------
    # Carla Internal Post Rack
    FindEffects "${PreFix}:rkr Shelf Boost"
    FindEffects "${PreFixMix}LSP Mixer x4 Stereo:Audio input"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"
    # Carla Internal Rack
    # -------------------------------------------------------------------

}

function SetInput {

    echo "*** SetInput"
    echo "${MainInputL}" "${MainInputR}"

    FindEffects "GxTuner"
    ChainNextLeft "${MainInputL}"
    ChainNextLeft "${MainInputR}"
    ChainNextRight "${MainInputL}"
    ChainNextRight "${MainInputR}"

    FindEffects "Gate Stereo"
    ChainNextLeft "${MainInputL}"
    ChainNextLeft "${MainInputR}"
    ChainNextRight "${MainInputL}"
    ChainNextRight "${MainInputR}"

}

function SetOutput {

    echo "*** SetOuput "
    # Carla Mixer to output device
    FindEffects "LSP Mixer x4 Stereo"
    DoLink "${EffectOutL}" "${MainOutputL}"
    DoLink "${EffectOutR}" "${MainOutputR}"

}

function NewRack {
    set -x
    # -------------------------------------------------------------------
    # Main Carla effect chain
    FindEffects "Gate Stereo"


 #   FindEffects "Multiband Compressor"
    FindEffects "LSP Multiband Compressor Stereo x8 (2)"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

#    FindEffects "LSP Multiband Compressor Stereo x8 (3)"
#    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "GxAmplifier-Stereo"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "rkr MuTroMojo"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    # Test
    # exit

    LastEffectOutR="${EffectOutR}"
    LastEffectOutL="${EffectOutL}"

    # FindEffects "GxDelay-Stereo"
    FindEffects "MDA DubDelay"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "rkr Flanger.Chorus"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    DoLink "${EffectOutL}" "${MixerInput1L}"
    DoLink "${EffectOutR}" "${MixerInput1R}"

    EffectOutR="${LastEffectOutR}"
    EffectOutL="${LastEffectOutL}"

    FindEffects "GxZita_rev1-Stereo"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "GxChorus-Stereo"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    #vFindEffects "rkr WahWah"
    # ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    #FindEffects "the infamous lush life"
    #ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "PurestSquish"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"
    set -x
    #FindEffects "LSP Profiler Stereo"
    #ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    # FindEffects "rkr Flanger.Chorus"
    # ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    # To Carla Mixer
    echo "Last Effect to Mixer 1 input"
    DoLink "${EffectOutL}" "${MixerInput1L}"
    DoLink "${EffectOutR}" "${MixerInput1R}"
    LastEffectOutR="${EffectOutR}"
    LastEffectOutL="${EffectOutL}"
    echo $LastEffectOutR
    # This script echoes the value of the variable LastEffectOutL.
    echo $LastEffectOutL

}

#---------------------------------------------------------------------#
#*** Main
#---------------------------------------------------------------------#
# Main script execution
# Clear all connections to Carla Rack
# ClearRack

# Clear the Plasma connections
ClearPlasma

# Get current audio device lists
export DeviceList="$(pw-link -o)"
export DeviceListI="$(pw-link -i)"

# Check and set up various devices
Device="Audio_AIR_192"
CheckInterface "$Device" 100 100

Device="C-Media_Electronics"
CheckInterface "$Device" 100 100

Device="AudioBox_Go"
CheckInterface "$Device" 100 100

Device="ZOOM_Corporation_U-24"
CheckInterface "$Device" 100 100

Device="Burr-Brown"
CheckInterface "$Device" 100 100

Device="bluez_output"
CheckInterface "$Device" 100 100

Device="NUX_USB"
CheckInterface "$Device" 100 100

Device="XSONIC_XTONE"
CheckInterface "$Device" 100 100
MainInputL=$InLinkL
MainInputR=$InLinkR
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetInput

Device="Positive_Grid_Spark_2"
CheckInterface "$Device" 100 100
MainInputL=$InLinkL
MainInputR=$InLinkR
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetInput

Device="Focusrite_Scarlett"
Device2="capture"
CheckInterface "$Device" 125 120 "$Device2"
MainInputL=$InLinkL
MainInputR=$InLinkR
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetInput


# ejk
# exit 

# Set up mixer inputs
MixerInput1R="${PreFixMix}LSP Mixer x4 Stereo:Audio input right 1"
MixerInput1L="${PreFixMix}LSP Mixer x4 Stereo:Audio input left 1"
MixerInput2R="${PreFixMix}LSP Mixer x4 Stereo:Audio input right 2"
MixerInput2L="${PreFixMix}LSP Mixer x4 Stereo:Audio input left 2"
MixerInput3L="${PreFixMix}LSP Mixer x4 Stereo:Audio input left 3"
MixerInput3R="${PreFixMix}LSP Mixer x4 Stereo:Audio input right 3"
MixerInput4L="${PreFixMix}LSP Mixer x4 Stereo:Audio input left 4"
MixerInput4R="${PreFixMix}LSP Mixer x4 Stereo:Audio input right 4"

Device="G935"
CheckInterface "$Device" 50 100

if [ $InterfaceFound -ne 0 ]; then
    MainOutputL=$OutLinkL
    MainOutputR=$OutLinkR
fi
SetOutput

Device="Positive_Grid_Spark_2"
CheckInterface "$Device" 50 100
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetOutput

Device="XSONIC_XTONE"
CheckInterface "$Device" 50 100
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetOutput

# Create new effects chain connections
NewRack

# -------------------------------------------------------------------
# Sound font to mixer
FindEffects "qsynth"
DoLink "qsynth:left" "${MixerInput2L}"
DoLink "qsynth:right" "${MixerInput2R}"
# Sound font to mixer
# -------------------------------------------------------------------

# -------------------------------------------------------------------
# Looper Connection
# FindEffects gx_head_fx
FindEffects "sooperlooper"
DoLink "${LastEffectOutL}" "${EffectInL}"
DoLink "${LastEffectOutR}" "${EffectInR}"

#ChainNextLeft "${MainInputL}"
#ChainNextLeft "${MainInputR}"
#ChainNextRight "${MainInputL}"
#ChainNextRight "${MainInputR}"

# Looper output
ChainNext "${ChainOutNextL}" "${ChainOutNextR}"
DoLink "${EffectOutL}" "${MixerInput4L}"
DoLink "${EffectOutR}" "${MixerInput4R}"

pw-link "Midi-Bridge:Midi Through:(capture_0) Midi Through Port-0" "qsynth:midi_00"

pw-link "Midi-Bridge:Virtual MIDI Card 1:(capture_0) VirMIDI 6-0" "qsynth:midi_00"

# Looper Connection
# -------------------------------------------------------------------

echo "****************************"
echo " Midi Connections "
# -------------------------------------------------------------------
# Configure MIDI connections

aconnect "EliasPedal3":"0" "LiveMusic":"0"
aconnect "EliasGuitar":"0" "LiveMusic":"0"
aconnect "EliasMidi":"0" "LiveMusic":"0"
aconnect "XTONE":"0" "LiveMusic":"0"

# Midi-Bridge:LiveMusic:(playback_0) LiveMusic

# Midi-Bridge:LiveDAW:(playback_1) LiveDAW
# DoLink "Midi-Bridge:nanoKONTROL2 3:(capture_0) nanoKONTROL2 nanoKONTROL2 _ CTR" "Midi-Bridge:LiveDAW:(playback_1) LiveDAW"
aconnect "nanoKONTROL2":"0" "LiveDAW":"1"
aconnect "LPD8":"0" "LiveDAW":"1"
aconnect "Reloop KeyFadr":"0" "LiveDAW":"1"
# Midi-Bridge:LiveMidi:(playback_2) LiveMidi
aconnect "TriplePlay Connect":"0" "LiveMidi":2
aconnect "Fishman TriplePlay":"0" "LiveMidi":2

# Midi-Bridge:LiveMusic Output:(capture_0) Fluid
aconnect "LiveMusic Output":"0" "FLUID Synth (qsynth)":"0"
aconnect "NMSVE":"0" "LiveMusic":"0"

# DoLink "Midi-Bridge:LiveMusic Output:(capture_0) Fluid"  "Midi-Bridge:KORG INC- nanoKONTROL2 at usb-0000:00:14-0-7-2-4-1-2-1- full speed:(playback_0) nanoKONTROL2 nanoKONTROL2 _ CTR"

# Midi-Bridge:LiveMusic Output:(capture_1) Guitarix
DoLink "Midi-Bridge:LiveMusic Output:(capture_1) Guitarix" "gx_head_amp:midi_in_1"
pw-link "Midi-Bridge:LiveMusic Output:(capture_1) Guitarix" "Midi-Bridge:LvSwitch:(playback_0) SwitchIn"

# DoLink "Midi-Bridge:LvSwitch:(playback_0) SwitchIn" "Midi-Bridge:LiveMusic Output:(capture_1) Guitarix"

# Midi-Bridge:LiveMusic Output:(capture_2) Looper
DoLink "Midi-Bridge:LiveMusic Output:(capture_2) Looper" "Midi-Bridge:sooperlooper:(playback_0) sooperlooper"
aconnect "LiveMusic Output":"2" "sooperlooper":"0"

# Midi-Bridge:LiveMusic Output:(capture_3) Transport

# Midi-Bridge:LiveMusic Output:(capture_4) Tempo

# Midi-Bridge:LiveMusic Output:(capture_5) Click
aconnect "LiveMusic Output":"5" "FLUID Synth (qsynth)":"0"

### FIX
# DoLink "Midi-Bridge:LiveMusic Output:(capture_5) Click" "FLUID Synth (qsynth)":"0"

# DoLink "Midi-Bridge:LiveMusic Output:(capture_5) Click" "Midi-Bridge:sooperlooper:(playback_0) sooperlooper"
aconnect "LiveMusic Output":"Click" "sooperlooper":"0"

# Midi-Bridge:LiveMusic Output:(capture_6) Pedal
aconnect "LiveMusic Output":"6" "NMSVE":"0"
aconnect "LiveMusic Output":"6" "EliasPedal3":"0"
# To Guitar Controller
aconnect "LiveMusic Output":"6" "EliasGuitar":"0"
aconnect "LiveMusic Output":"6" "EliasMidi":"0"
# To KorgNano
aconnect "LiveMusic Output":"6" "NMSVE":"0"

# Midi-Bridge:LiveMusic Output:(capture_7) DAWPort
# DoLink "Midi-Bridge:LiveMusic Output:(capture_7) DAWPort" "Midi-Bridge:nanoKONTROL2 3:(playback_0) nanoKONTROL2 nanoKONTROL2 _ CTR"

aconnect "LiveMusic Output":"7" "nanoKONTROL2":"0"

aconnect "LiveMusic Output":"7" "LPD8":"0"
aconnect "LiveMusic Output":"7" "Reloop KeyFadr":"0"

# Midi-Bridge:LiveMusic Output:(capture_8) DRLoop
# DoLink "Midi-Bridge:LiveMusic Output:(capture_8) DRLoop" "Midi-Bridge:sooperlooper:(playback_0) sooperlooper"
aconnect "LiveMusic Output":"8" "FLUID Synth (qsynth)":"0"
aconnect "LiveMusic Output":8 "sooperlooper":0

# Midi-Bridge:LiveMusic Output:(capture_9) User2

# Midi-Bridge:LiveMusic Output:(capture_10) User3

# Midi-Bridge:LiveMusic Output:(capture_11) Internal

aconnect "Midi Through":"0" "FLUID Synth (qsynth)":"0"
aconnect "Fishman TriplePlay":0 "MusE":0

# Disconnect unnecessary connections
aconnect -d "Fishman TriplePlay":"0" "FLUID Synth (qsynth)":"0"
aconnect -d "Fishman TriplePlay":"1" "FLUID Synth (qsynth)":"0"
aconnect -d "TriplePlay Connect":"0" "FLUID Synth (qsynth)":"0"
aconnect -d "TriplePlay Connect":"1" "FLUID Synth (qsynth)":"0"
aconnect -d "EliasGuitar":"0" "FLUID Synth (qsynth)":"0"
aconnect -d "System":"1" "FLUID Synth (qsynth)":"0"
aconnect -d "Midi Through":"1" "FLUID Synth (qsynth)":"0"
aconnect -d "Midi Through":"2" "FLUID Synth (qsynth)":"0"
aconnect -d "Midi Through":"3" "FLUID Synth (qsynth)":"0"
aconnect -d "nanoKONTROL2":"0" "FLUID Synth (qsynth)":"0"
aconnect -d "sooperlooper":"0" "FLUID Synth (qsynth)":"0"
# Midi Connections
# -------------------------------------------------------------------

# Set the Volumes at the Alsa level
AlsaSet

exit

# Disconnect Plasma PA connections
pw-link -d alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.pro-output-0:monitor_AUX0 "Plasma PA:input_FL"
pw-link -d alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.pro-output-0:monitor_AUX1 "Plasma PA:input_FR"

pw-link -d "alsa_input.usb-Focusrite_Scarlett_Solo_USB-00.pro-input-0:capture_AUX0" "Plasma PA:input_FL"
pw-link -d "alsa_input.usb-Focusrite_Scarlett_Solo_USB-00.pro-input-0:capture_AUX1" "Plasma PA:input_FR"

# -------------------------------------------------------------------
# Jack Mixer values

oscsend localhost 22752 /Carla/1/set_parameter_value if 0 1
oscsend localhost 22752 /Carla/1/set_parameter_value if 1 0
# Output Balance
oscsend localhost 22752 /Carla/1/set_parameter_value if 2 1.0
# Dry Balance
oscsend localhost 22752 /Carla/1/set_parameter_value if 3 0.25
# Wet Balance
oscsend localhost 22752 /Carla/1/set_parameter_value if 4 1
oscsend localhost 22752 /Carla/1/set_parameter_value if 5 3

# Solo
oscsend localhost 22752 /Carla/1/set_parameter_value if 10 0
# Mute
oscsend localhost 22752 /Carla/1/set_parameter_value if 11 0
# Phase Invert
oscsend localhost 22752 /Carla/1/set_parameter_value if 12 0
# Left Channel
oscsend localhost 22752 /Carla/1/set_parameter_value if 13 0
# Right Channel
oscsend localhost 22752 /Carla/1/set_parameter_value if 14 0
# Balance
oscsend localhost 22752 /Carla/1/set_parameter_value if 15 0
# Volume
oscsend localhost 22752 /Carla/1/set_parameter_value if 16 3

# Solo 2
oscsend localhost 22752 /Carla/1/set_parameter_value if 19 0
# Mute
oscsend localhost 22752 /Carla/1/set_parameter_value if 20 0
# Phase Invert
oscsend localhost 22752 /Carla/1/set_parameter_value if 21 0
# Left Channel
oscsend localhost 22752 /Carla/1/set_parameter_value if 22 0
# Right Channel
oscsend localhost 22752 /Carla/1/set_parameter_value if 23 0
# Balance
oscsend localhost 22752 /Carla/1/set_parameter_value if 24 0
# Volume
oscsend localhost 22752 /Carla/1/set_parameter_value if 25 3

# Solo 3
oscsend localhost 22752 /Carla/1/set_parameter_value if 28 0
# Mute
oscsend localhost 22752 /Carla/1/set_parameter_value if 29 0
# Phase Invert
oscsend localhost 22752 /Carla/1/set_parameter_value if 30 0
# Left Channel
oscsend localhost 22752 /Carla/1/set_parameter_value if 31 0
# Right Channel
oscsend localhost 22752 /Carla/1/set_parameter_value if 32 0
# Balance
oscsend localhost 22752 /Carla/1/set_parameter_value if 33 0
# Volume
oscsend localhost 22752 /Carla/1/set_parameter_value if 34 3

# Solo 3
oscsend localhost 22752 /Carla/1/set_parameter_value if 37 0
# Mute
oscsend localhost 22752 /Carla/1/set_parameter_value if 38 0
# Phase Invert
oscsend localhost 22752 /Carla/1/set_parameter_value if 39 0
# Left Channel
oscsend localhost 22752 /Carla/1/set_parameter_value if 40 0
# Right Channel
oscsend localhost 22752 /Carla/1/set_parameter_value if 41 0
# Balance
oscsend localhost 22752 /Carla/1/set_parameter_value if 42 0
# Volume
oscsend localhost 22752 /Carla/1/set_parameter_value if 43 3
# Jack Mixer values
# -------------------------------------------------------------------
