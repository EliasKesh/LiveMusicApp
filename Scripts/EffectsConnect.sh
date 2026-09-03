#!/bin/bash
#---------------------------------------------------------------------
#
#	File:		EffectsConnect.sh
#
#	Contains:	Rebuilds PipeWire/Carla audio routing and MIDI
#			connections for the live rig. Run with "clear" as
#			the first argument to tear down existing Carla
#			links before reconnecting.
#
#	Written By:	Elias Keshishoglou on Wed Jun 26 07:38:22 EEST 2024
#
#	Copyright:	2024 Elias Keshishoglou all rights reserved.
#
#---------------------------------------------------------------------#
set -x

#=======================================================================
# CONFIGURATION - all connection points live here
# Edit these instead of hunting for literal device/plugin names inside
# the functions or the main flow below.
#=======================================================================

# --- Carla plugin / mixer prefixes, as they show up in pw-link -----
PreFix="Carla.0"
PreFixMix="Carla.0/"

# --- OSC target for Carla mixer parameter changes -------------------
# (used only by the disabled reference block at the end of this file)
OscHost="localhost"
OscPort="22752"

# --- Hardware interfaces (match strings against `pw-link -i/-o`) ---
DevAudioAir="Audio_AIR_192"
DevCMedia="C-Media_Electronics"
DevAudioBoxGo="AudioBox_Go"
DevZoomU24="ZOOM_Corporation_U-24"
DevBurrBrown="Burr-Brown"
DevBluezOutput="bluez_output"
DevNuxUsb="NUX_USB"
DevXTone="XSONIC_XTONE"
DevSpark2="Positive_Grid_Spark_2"
DevScarlett="Focusrite_Scarlett"
DevSteelSeries="SteelSeries"

# --- ALSA card match strings (used by AlsaSet) ----------------------
AlsaDevG935="G935"
AlsaDevArctis="Arctis"
AlsaDevNux="nux"
AlsaDevScarlett="Scarlett"
AlsaDevUsb="USB"
AlsaMicCards="1 2 3 4 5"
AlsaPcmCards="4 5 6 7 8 9"

# --- Carla effect/plugin names (used by FindEffects) ----------------
FxTuner="x42 Instrument Tuner"
FxMidiGuitar="MIDI Guitar 2"
FxGate="LSP Gate"
FxGateMono="LSP Gate Mono"
FxCompMono="LSP Compressor Mono"
FxGottComp="GOTT Compressor Mono"
FxGeq16="LSP Graphic Equalizer x16 Mono"
FxTubeScreamer="GxTubeScreamer"
FxAmplifier="GxAmplifier-X"
FxMuTroMojo="rkr MuTroMojo"
FxDelay="${PreFixMix}windowswaylodelayUD"
# MOD 2Voices pitch/harmonizer. It has one mono input and two outputs.
Fx2Voices="2Voices"
FxReverb="GxZita_rev1-Stereo"
FxLoudnessComp="${PreFixMix}LSP Loudness Compensator Stereo"
FxMixer="LSP Mixer x4 Stereo"
FxLooper="sooperlooper"

# --- MIDI device / port names ---------------------------------------
MidiLinuxSampler="LinuxSampler:midi_in_0"
MidiSoundGenerator="LinuxSampler:midi_in_0"

# Keywords for FindPort (see below) - loose substrings, not exact
# literal port names, so a punctuation change doesn't break the link.
KwMidiThrough=("Midi Through" "Port-0")
KwMidiVirtual0=("Virtual Raw MIDI 2-0")
KwMidiVirtual1=("Virtual Raw MIDI 2-1")
KwNanoKontrolCtrl=("nanoKONTROL2" "CTRL")
KwLiveDaw=("LiveDAW")
KwLvSwitchIn=("LvSwitch" "SwitchIn")
KwOutFluid=("LiveMusic" "Fluid")
KwOutClick=("LiveMusic" "Click")
KwOutGuitarix=("LiveMusic" "Guitarix")
KwOutDawPort=("LiveMusic" "DAWPort")
KwOutPedal=("LiveMusic" "Pedal")
KwEliasMidiBt=("EliasMidi" "Bluetooth")

# sendmidi (in the disabled reference block) talks to an ALSA/rtmidi
# device name, not a pw-link port, so this one stays a plain literal.
MidiThroughDevName="Midi Through Port-0"

MidiPedal="EliasPedal3"
MidiGuitarCtrl="EliasGuitar"
MidiMidiCtrl="EliasMidi"
MidiXToneCtrl="XTONE"
MidiNanoKontrol="nanoKONTROL2"
MidiLpd8="LPD8"
MidiKeyFadr="Reloop KeyFadr"
MidiTriplePlayConnect="TriplePlay Connect"
MidiFishman="Fishman TriplePlay"
MidiNmsve="NMSVE"

#=======================================================================
# FUNCTIONS
#=======================================================================

#---------------------------------------------------------------------#
#*** ClearRack
#---------------------------------------------------------------------#
# ClearRack: Disconnect all audio connections with the Carla prefix
function ClearRack {
    IDList=$(pw-link --id -l | grep "$PreFix" | awk '{print $1}')

    for theID in $IDList; do
        echo "Link $theID"
        pw-link -d "$theID"
        sleep 0.08
    done
}

#---------------------------------------------------------------------#
#*** ClearPlasma
#---------------------------------------------------------------------#
# ClearPlasma: Disconnect all Plasma PA audio connections
function ClearPlasma {
    IDList=$(pw-link --id -l | grep "Plasma PA" | awk '{print $1}')

    for theID in $IDList; do
        echo "Link $theID"
        pw-link -d "$theID"
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
        echo "Sink Found $MySinks"
    fi

    MySources=$(arecord -l | grep "card 0:" | head -n 1)
    if [ "$?" -eq 0 ]; then
        echo "Source Found $MySources"
    fi

    # ---- G935 headset ------------------------------
    CardNum=$(aplay -l | grep "$AlsaDevG935" | awk -F'[\t: ]' '{print $2}')
    amixer -c "$CardNum" set 'PCM',0 50
    amixer -c "$CardNum" set 'Mic',0 65

    # ---- Arctis headset -----------------------------
    CardNum=$(aplay -l | grep "$AlsaDevArctis" | awk -F'[\t: ]' '{print $2}')
    amixer -c "$CardNum" set 'PCM',0 100
    amixer -c "$CardNum" set 'Mic',0 65

    # ---- NUX interface -------------------------------
    CardNum=$(aplay -l | grep -i "$AlsaDevNux" | awk -F'[\t: ]' '{print $2}')
    amixer -c "$CardNum" set "NUX USB Audio 2.0 ",0 2032
    amixer -c "$CardNum" set "NUX USB Audio 2.0 ",1 2032
    amixer -c "$CardNum" set 'Mic',0 2032
    amixer -c "$CardNum" set 'Mic',1 2032

    # ---- Scarlett (no ALSA controls) -----------------
    CardNum=$(aplay -l | grep -i "$AlsaDevScarlett" | awk -F'[\t: ]' '{print $2}')

    # ---- Generic USB device #1 -----------------------
    CardNum=$(aplay -l | grep -i "$AlsaDevUsb" | head -n1 | tail -n1 | awk -F'[\t: ]' '{print $2}')
    amixer -c "$CardNum" set "Master",0 75
    amixer -c "$CardNum" set "Headphone",0 80
    amixer -c "$CardNum" set "PCM",0 220

    # ---- Generic USB device #2 -----------------------
    CardNum=$(aplay -l | grep -i "$AlsaDevUsb" | head -n2 | tail -n1 | awk -F'[\t: ]' '{print $2}')
    amixer -c "$CardNum" set "Master",0 75
    amixer -c "$CardNum" set "Headphone",0 80
    amixer -c "$CardNum" set "PCM",0 220

    # ---- Card 0 (onboard) -----------------------------
    amixer -c 0 set -M 'Master',0 65 on
    amixer -c 0 set 'Headphone',0 75 unmute
    amixer -c 0 set 'PCM',0 100
    amixer -c 0 set 'Capture',0 100
    amixer -c 0 set 'Digital',0 0

    amixer -c 0 cset numid=1 80 unmute   # Headphone
    amixer -c 0 cset numid=8 80 unmute   # Mic Boost
    amixer -c 0 cset numid=6 80 unmute   # Capture Volume
    amixer -c 0 cset numid=4 0 unmute    # Speaker playback switch
    amixer -c 0 cset numid=4 0 mute      # Speaker playback volume
    amixer -c 0 cset -M numid=100 80 unmute  # PCM Playback

    # ---- Card 1 / 2 PCM levels -------------------------
    amixer -c 1 set 'PCM',0 100
    amixer -c 2 set 'PCM',0 100

    # ---- Mic levels across cards 1-5 -------------------
    for card in $AlsaMicCards; do
        amixer -c "$card" set 'Mic',0 1950
        amixer -c "$card" set 'Mic',1 1950
    done

    amixer -c 2 set 'Speaker',0 100
    amixer -c 2 set 'Auto Gain Control',0 on

    amixer -c 3 set 'PCM',0 45
    amixer -c 3 set -M 'Master',0 100 on

    # ---- PCM levels across cards 4-9 -------------------
    for card in $AlsaPcmCards; do
        amixer -c "$card" set 'PCM',0 100
    done
}

#---------------------------------------------------------------------#
#*** CheckInterface
# Device
# Input Volume
# Output Volume
#---------------------------------------------------------------------#
# CheckInterface: Verify audio interface exists and set its volumes
function CheckInterface {

    echo "CheckInterface $1"

    echo "$DeviceList" | grep "${1}" >/dev/null 2>&1
    if [ "$?" -ne 0 ]; then
        InterfaceFound=0
        return 1
    fi

    echo "We Found $1"

    InLinkL=$(echo "$DeviceList" | grep "${1}" | grep -i capture | head -n 1)
    InLinkR=$(echo "$DeviceList" | grep "${1}" | grep -i capture | head -n 2 | tail -n 1)

    OutLinkL=$(echo "$DeviceListI" | grep "${1}" | grep -i playback | head -n 1)
    OutLinkR=$(echo "$DeviceListI" | grep "${1}" | grep -i playback | head -n 2 | tail -n 1)

    SetVolumes "$1" "$2" "$3" "$4"
    InterfaceFound=1
    return 0
}

#---------------------------------------------------------------------#
#*** SetVolumes
#---------------------------------------------------------------------#
# SetVolumes: Set input/output volumes for an audio interface
function SetVolumes {
    echo "****SetVolumes $1 $2 $3 $4"

    # Set sink (output) volume
    InputVolume=$(pactl list short sinks | grep "$1" | grep output | awk '{print $1}')
    pactl set-sink-volume "$InputVolume" "$2%"

    # Set source (input) volume
    OutputVolume=$(pactl list short sources | grep "$1" | grep input | awk '{print $1}')
    echo "SetVolumes Capture $OutputVolume"
    pamixer --set-volume "$3" --allow-boost --source "$OutputVolume"
}

#---------------------------------------------------------------------#
#*** FindPort
#---------------------------------------------------------------------#
# FindPort: Resolve a live port name from loose keywords instead of a
# hardcoded literal, since Midi-Bridge naming punctuation (colons,
# spacing) has changed between PipeWire versions/restarts before.
#   $1        direction: "out" = capture/source ports (pw-link -o,
#             cached in $DeviceList) | "in" = playback/sink ports
#             (pw-link -i, cached in $DeviceListI)
#   $2, $3... substrings that must ALL appear in the port name
#             (case-insensitive, ANDed together); first match wins.
# Prints the matched port name (or nothing, with a warning on stderr).
function FindPort {
    local direction="$1"; shift
    local list
    if [ "$direction" == "out" ]; then
        list="$DeviceList"
    else
        list="$DeviceListI"
    fi

    for kw in "$@"; do
        list=$(echo "$list" | grep -i -- "$kw")
    done

    local match
    match=$(echo "$list" | head -n 1)
    if [ -z "$match" ]; then
        echo "FindPort: no match for [$*]" >&2
    fi
    echo "$match"
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

    echo "Effect Found ${1}"

    ChainOutNextL="${EffectOutL}"
    ChainOutNextR="${EffectOutR}"
    ChainInNextL="${EffectInL}"
    ChainInNextR="${EffectInR}"

    EffectInL="$(echo "$DeviceListI" | grep "${1}" | grep -i in | head -n 1)"
    EffectInR="$(echo "$DeviceListI" | grep "${1}" | grep -i in | head -n 2 | tail -n 1)"

    EffectOutL="$(echo "$DeviceList" | grep "${1}" | grep -i out | head -n 1)"
    EffectOutR="$(echo "$DeviceList" | grep "${1}" | grep -i out | head -n 2 | tail -n 1)"

    EffectFound=1
    return 0
}

#---------------------------------------------------------------------#
#*** ChainNext / ChainNextLeft / ChainNextRight / DoLink
#---------------------------------------------------------------------#
# ChainNext: Connect stereo effect chain links
function ChainNext {
    echo "${1}" "${EffectInL}"
    echo "${2}" "${EffectInR}"

    if [ "$EffectFound" -ne 0 ]; then
        DoLink "${1}" "${EffectInL}"
        DoLink "${2}" "${EffectInR}"
    fi
}

function ChainNextLeft {
    if [ "$EffectFound" -ne 0 ]; then
        DoLink "${1}" "${EffectInL}"
    fi
}

function ChainNextRight {
    if [ "$EffectFound" -ne 0 ]; then
        DoLink "${1}" "${EffectInR}"
    fi
}

function DoLink {
    pw-link "${1}" "${2}"
}

#---------------------------------------------------------------------#
#*** SetInput / SetOutput
#---------------------------------------------------------------------#
function SetInput {
    echo "*** SetInput"
    echo "${MainInputL}" "${MainInputR}"

    FindEffects "$FxTuner"
    ChainNextLeft "${MainInputL}"
    ChainNextLeft "${MainInputR}"
    ChainNextRight "${MainInputL}"
    ChainNextRight "${MainInputR}"

    FindEffects "$FxMidiGuitar"
    ChainNextLeft "${MainInputL}"
    ChainNextLeft "${MainInputR}"
    ChainNextRight "${MainInputL}"
    ChainNextRight "${MainInputR}"

    FindEffects "$FxGate"
    ChainNextLeft "${MainInputL}"
    ChainNextLeft "${MainInputR}"
    ChainNextRight "${MainInputL}"
    ChainNextRight "${MainInputR}"
}

function SetOutput {
    echo "*** SetOutput"
    FindEffects "$FxMixer"
    DoLink "${EffectOutL}" "${MainOutputL}"
    DoLink "${EffectOutR}" "${MainOutputR}"
}

#---------------------------------------------------------------------#
#*** NewRack - current Carla effect chain
#---------------------------------------------------------------------#
function NewRack {

    pw-metadata -n settings 0 clock.force-rate 96000
    set -x

    FindEffects "$FxLooper"
    LoopIn1="${EffectInL}"
    LoopIn2="${EffectInR}"
    LoopOut1="${EffectOutL}"
    LoopOut2="${EffectOutR}"

    # -------------------------------------------------------------------
    # Main Carla effect chain
    FindEffects "$FxGateMono"

    FindEffects "$FxCompMono"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "$FxGottComp"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "$FxGeq16"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "$FxTubeScreamer"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "$FxAmplifier"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "$FxMuTroMojo"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    FindEffects "$FxReverb"
    # ChainNext "${ChainOutNextL}" "${ChainOutNextR}"
    # 
    # FindEffects "$FxMuTroMojo"

    # -------------------------------------------------------------------
    # Waylo + MOD 2Voices are PARALLEL branches.
    #
    # Waylo remains the normal stereo doubler/delay branch. MOD 2Voices
    # receives the guitar signal from the left channel (the guitar source
    # is effectively mono) and produces two independent pitch-shifted
    # voices. The Waylo stereo output and the two 2Voices outputs are then
    # mixed together before entering the reverb.
    #
    # 2Voices is not handled by FindEffects because it has ONE input and
    # TWO outputs, whereas FindEffects assumes a conventional stereo
    # input/output plugin.
    # -------------------------------------------------------------------

   ParallelInL="${ChainOutNextL}"
   ParallelInR="${ChainOutNextR}"

    # --- Waylo branch ---------------------------------------------------
    FindEffects "$FxDelay"
    WayloInL="${EffectInL}"
    WayloInR="${EffectInR}"
    WayloOutL="${EffectOutL}"
    WayloOutR="${EffectOutR}"

    DoLink "${ParallelInL}" "${WayloInL}"
    DoLink "${ParallelInR}" "${WayloInR}"

    # --- MOD 2Voices branch --------------------------------------------
    # Carla exposes the LV2 ports using their LV2 names: In, Out 1, Out 2.
    TwoVoicesIn="${PreFixMix}${Fx2Voices}:In"
    TwoVoicesOut1="${PreFixMix}${Fx2Voices}:Out 1"
    TwoVoicesOut2="${PreFixMix}${Fx2Voices}:Out 2"

    # Feed the mono guitar signal to the harmonizer.
    DoLink "${ParallelInL}" "${TwoVoicesIn}"

    # --- Rejoin the parallel branches at the reverb -------------------
    FindEffects "$FxReverb"
    ReverbInL="${EffectInL}"
    ReverbInR="${EffectInR}"

    DoLink "${WayloOutL}" "${ReverbInL}"
    DoLink "${TwoVoicesOut1}" "${ReverbInL}"
    DoLink "${WayloOutR}" "${ReverbInR}"
    DoLink "${TwoVoicesOut2}" "${ReverbInR}"

    # Continue the normal chain from the reverb output.
    ChainNext "${EffectOutL}" "${EffectOutR}"

    FindEffects "$FxLoudnessComp"
    ChainNext "${ChainOutNextL}" "${ChainOutNextR}"

    # To Carla Mixer
    echo "Last Effect to Mixer 1 input"
    DoLink "${EffectOutL}" "${MixerInput1L}"
    DoLink "${EffectOutR}" "${MixerInput1R}"
    LastEffectOutR="${EffectOutR}"
    LastEffectOutL="${EffectOutL}"
    echo "$LastEffectOutR"
    echo "$LastEffectOutL"

    # Sooper Looper
    DoLink "${LastEffectOutL}" "${LoopIn1}"
    DoLink "${LastEffectOutR}" "${LoopIn2}"
}

#=======================================================================
# MAIN
#=======================================================================

# Clear all connections to the Carla rack if requested
if [ "$1" == "clear" ]; then
    echo "ClearRack"
    ClearRack
    ClearPlasma
fi

# Set the Volumes at the ALSA level
#/home/ebin/AlsaSet.sh

# Get current audio device lists
export DeviceList="$(pw-link -o)"
export DeviceListI="$(pw-link -i)"

# --- Check and set up input/output devices --------------------------
Device="$DevAudioAir";      CheckInterface "$Device" 100 100
Device="$DevCMedia";        CheckInterface "$Device" 100 100
Device="$DevAudioBoxGo";    CheckInterface "$Device" 100 100
Device="$DevZoomU24";       CheckInterface "$Device" 100 100
Device="$DevBurrBrown";     CheckInterface "$Device" 100 100
Device="$DevBluezOutput";   CheckInterface "$Device" 60 60
Device="$DevNuxUsb";        CheckInterface "$Device" 100 100

Device="$DevXTone"
CheckInterface "$Device" 100 100
MainInputL=$InLinkL
MainInputR=$InLinkR
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetInput

Device="$DevSpark2"
CheckInterface "$Device" 100 100
MainInputL=$InLinkL
MainInputR=$InLinkR
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetInput

Device="$DevScarlett"
Device2="capture"
CheckInterface "$Device" 125 120 "$Device2"
MainInputL=$InLinkL
MainInputR=$InLinkR
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetInput

Device="$DevSteelSeries"
CheckInterface "$Device" 125 120
MainInputL=$InLinkL
MainInputR=$InLinkR
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetInput

# --- Carla mixer input ports -----------------------------------------
MixerInput1R="${PreFixMix}${FxMixer}:Audio input right 1"
MixerInput1L="${PreFixMix}${FxMixer}:Audio input left 1"
MixerInput2R="${PreFixMix}${FxMixer}:Audio input right 2"
MixerInput2L="${PreFixMix}${FxMixer}:Audio input left 2"
MixerInput3L="${PreFixMix}${FxMixer}:Audio input left 3"
MixerInput3R="${PreFixMix}${FxMixer}:Audio input right 3"
MixerInput4L="${PreFixMix}${FxMixer}:Audio input left 4"
MixerInput4R="${PreFixMix}${FxMixer}:Audio input right 4"

# --- Route mixer output back out to each interface --------------------
Device="$DevScarlett"
CheckInterface "$Device" 100 100
if [ "$InterfaceFound" -ne 0 ]; then
    MainOutputL=$OutLinkL
    MainOutputR=$OutLinkR
fi
SetOutput

Device="$DevSteelSeries"
CheckInterface "$Device" 115 30
if [ "$InterfaceFound" -ne 0 ]; then
    MainOutputL=$OutLinkL
    MainOutputR=$OutLinkR
fi
SetOutput

Device="$DevSpark2"
CheckInterface "$Device" 50 100
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetOutput

Device="$DevXTone"
CheckInterface "$Device" 50 100
MainOutputL=$OutLinkL
MainOutputR=$OutLinkR
SetOutput

# Build the current effects chain
NewRack

# -------------------------------------------------------------------
# Soundfont synth to mixer
echo "qsynth:left  $MixerInput2L"
DoLink "qsynth:left" "${MixerInput2L}"
DoLink "qsynth:right" "${MixerInput2R}"

echo "****************************"
echo " Midi Connections "
# -------------------------------------------------------------------

# Resolve the Midi-Bridge ports we need up front, by keyword rather
# than exact literal - see FindPort() above.
PortVirtual0Cap=$(FindPort out "${KwMidiVirtual0[@]}")
PortVirtual1Cap=$(FindPort out "${KwMidiVirtual1[@]}")
PortThroughCap=$(FindPort out "${KwMidiThrough[@]}")
PortFluidCap=$(FindPort out "${KwOutFluid[@]}")
PortClickCap=$(FindPort out "${KwOutClick[@]}")
PortGuitarixCap=$(FindPort out "${KwOutGuitarix[@]}")
PortDawPortCap=$(FindPort out "${KwOutDawPort[@]}")
PortPedalCap=$(FindPort out "${KwOutPedal[@]}")
PortEliasMidiBtCap=$(FindPort out "${KwEliasMidiBt[@]}")

PortNanoCtrlPlay=$(FindPort in "${KwNanoKontrolCtrl[@]}")
PortNanoCtrlCap=$(FindPort out "${KwNanoKontrolCtrl[@]}")
PortLiveDawPlay=$(FindPort in "${KwLiveDaw[@]}")
PortLvSwitchInPlay=$(FindPort in "${KwLvSwitchIn[@]}")
PortEliasMidiBtPlay=$(FindPort in "${KwEliasMidiBt[@]}")
# "LiveMusic" (plain) vs "LiveMusic Output:*" is a substring trap -
# every Output port also contains "LiveMusic", so filter those out.
PortLiveMusicPlay=$(echo "$DeviceListI" | grep -i "LiveMusic" | grep -vi "Output" | head -n 1)

# LinuxSampler
pw-link "$PortVirtual0Cap" "$MidiLinuxSampler"
pw-link "$PortVirtual1Cap" "$MidiLinuxSampler"
pw-link "${PreFixMix}${FxMidiGuitar}:event-out" "$MidiLinuxSampler"
pw-link "$PortFluidCap" "$MidiLinuxSampler"

pw-link "LinuxSampler:0" "${MixerInput2L}"
pw-link "LinuxSampler:1" "${MixerInput2R}"

# -- Midi Fluid
# Note: the old rig had separate VirMIDI 5-x/6-x/7-x pairs feeding this;
# the current PipeWire graph only exposes one virtual pair, so those
# have collapsed into the two links below.
pw-link "$PortThroughCap" "${MidiSoundGenerator}"
pw-link "$PortVirtual0Cap" "${MidiSoundGenerator}"
pw-link "$PortVirtual1Cap" "${MidiSoundGenerator}"
pw-link "$PortClickCap" "${MidiSoundGenerator}"
pw-link "$PortFluidCap" "${MidiSoundGenerator}"
pw-link "${PreFixMix}${FxMidiGuitar}:event-out" "${MidiSoundGenerator}"

pw-link "FluidSynth:output_1" "${MixerInput2L}"
pw-link "FluidSynth:output_2" "${MixerInput2R}"

DoLink "qsynth:left" "${ChainOutNextL}"
DoLink "qsynth:right" "${ChainOutNextR}"

pw-link "$PortGuitarixCap" "$PortLvSwitchInPlay"
pw-link "$PortDawPortCap" "$PortNanoCtrlPlay"
pw-link "$PortNanoCtrlCap" "$PortLiveDawPlay"

# From Guitar to software
pw-link "$PortEliasMidiBtCap" "$PortLiveMusicPlay"
pw-link "$PortPedalCap" "$PortEliasMidiBtPlay"

aconnect "$MidiNanoKontrol":"0" "LiveDAW":"1"

# Reference - Midi-Bridge ports seen on this rig (not connected here):
# Inputs:  Midi Through: Port-0, nanoKONTROL2: _ CTRL, Virtual Raw MIDI 2-0/2-1,
#          LiveMusic:, LiveDAW:, LiveMidi:, LvSwitch:SwitchIn,
#          FLUID Synth (qsynth) input, sooperlooper, bluez_midi.server:in
# Outputs: Virtual Raw MIDI 2-0/2-1, LiveMusic Output:{Fluid,Guitarix,Looper,
#          Transport,Tempo,Click,Pedal,DAWPort,DRLoop,User2,User3,Internal},
#          LvSwitch:SwitchOut, sooperlooper, bluez_midi.server:out

exit

#=======================================================================
# DISABLED / REFERENCE ONLY
# Everything below this point was already unreachable in the original
# script (it lived after an unconditional `exit`), kept here in case
# any of it needs to be revived. Nothing here currently runs.
#=======================================================================

# --- Disconnect Plasma PA connections ---------------------------------
# pw-link -d alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.pro-output-0:monitor_AUX0 "Plasma PA:input_FL"
# pw-link -d alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.pro-output-0:monitor_AUX1 "Plasma PA:input_FR"
# pw-link -d "alsa_input.usb-Focusrite_Scarlett_Solo_USB-00.pro-input-0:capture_AUX0" "Plasma PA:input_FL"
# pw-link -d "alsa_input.usb-Focusrite_Scarlett_Solo_USB-00.pro-input-0:capture_AUX1" "Plasma PA:input_FR"

# --- Older ALSA-sequencer based MIDI routing (aconnect) ----------------
# aconnect "$MidiPedal":"0" "LiveMusic":"0"
# aconnect "$MidiGuitarCtrl":"0" "LiveMusic":"0"
# aconnect "$MidiMidiCtrl":"0" "LiveMusic":"0"
# aconnect "$MidiXToneCtrl":"0" "LiveMusic":"0"
#
# aconnect "$MidiNanoKontrol":"0" "LiveDAW":"1"
# aconnect "$MidiLpd8":"0" "LiveDAW":"1"
# aconnect "$MidiKeyFadr":"0" "LiveDAW":"1"
#
# aconnect "$MidiTriplePlayConnect":"0" "LiveMidi":2
# aconnect "$MidiFishman":"0" "LiveMidi":2
#
# aconnect "$MidiNmsve":"0" "LiveMusic":"0"
# aconnect "LiveMusic Output":"2" "$FxLooper":"0"
# aconnect "LiveMusic Output":"Click" "$FxLooper":"0"
#
# aconnect "LiveMusic Output":"6" "$MidiNmsve":"0"
# aconnect "LiveMusic Output":"6" "$MidiPedal":"0"
# aconnect "LiveMusic Output":"6" "$MidiGuitarCtrl":"0"    # To Guitar Controller
# aconnect "LiveMusic Output":"6" "$MidiMidiCtrl":"0"
#
# aconnect "LiveMusic Output":"7" "$MidiNanoKontrol":"0"   # To KorgNano
# aconnect "LiveMusic Output":"7" "$MidiLpd8":"0"
# aconnect "LiveMusic Output":"7" "$MidiKeyFadr":"0"

# --- Carla mixer channel strip defaults via OSC ------------------------
# Channel strip parameter indices: 0/1 in/out?, 2 output balance,
# 3 dry balance, 4/5 wet balance, 10/19/28/37 solo, 11/20/29/38 mute,
# 12/21/30/39 phase invert, 13/22/31/40 left ch, 14/23/32/41 right ch,
# 15/24/33/42 balance, 16/25/34/43 volume.
# for strip in 1 2 3 4; do
#     base=$(( (strip - 1) * 9 + 1 ))
#     ...
# done
#
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 0 1
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 1 0
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 2 1.0   # Output Balance
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 3 0.25  # Dry Balance
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 4 1     # Wet Balance
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 5 3
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 10 0    # Solo
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 11 0    # Mute
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 12 0    # Phase Invert
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 13 0    # Left Channel
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 14 0    # Right Channel
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 15 0    # Balance
# oscsend $OscHost $OscPort /Carla/1/set_parameter_value if 16 3    # Volume
# (same pattern repeats for strips 2-4 at offsets 19, 28, 37)

# --- Bank/program change on start-up ------------------------------------
# sendmidi dev "$MidiThroughDevName" ch 1 cc 0 0 cc 32 1 pc 1
