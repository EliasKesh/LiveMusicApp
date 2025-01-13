#!/bin/bash
#---------------------------------------------------------------------
#
#   File:   PipeConnect
#
#   Contains:
#
#
#   Written By:     Elias Keshishoglou on Sat Apr 2 09:36:12 AM PDT 2022
#
#   Copyright :     2022 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
# pw-metadata -n settings 0 clock.force-rate 48000
# pw-metadata -n settings 0 clock.force-quantum 64

#systemctl --user stop pipewire pipewire-pulse wireplumber pipewire.socket pipewire-pulse.socket

    
# ------------- --------------- 
# ------------- GrabInterface 
# ------------- --------------- 
GrabInterface() {

echo "GrabInterface " $1

InLinkL=`echo "$DeviceList" | grep "${1}" | grep -i capture | tail -n 1`
InLinkR=`echo "$DeviceList" | grep "${1}" | grep -i capture | tail -n 2 | head -n 1`

OutLinkL=`echo "$DeviceListI" | grep "${1}" | grep -i playback | tail -n 1`
OutLinkR=`echo "$DeviceListI" | grep "${1}" | grep -i playback | tail -n 2 | head -n 1`

echo "OUTPUT "$InLinkL
echo "OUTPUT "$InLinkR
echo "OUTPUT "$OutLinkL
echo "OUTPUT "$OutLinkR
}


# ------------- --------------- 
# ------------- AssignInterface 
# ------------- --------------- 
AssignInterface() {

echo "AssignInterface " $InLinkL

# ------------- input 
pw-link "${InLinkL}" "gx_head_amp:in_0"
pw-link "${InLinkR}" "gx_head_amp:in_0"
pw-link "${InLinkL}" "Carla-Patchbay:input_1"
pw-link "${InLinkR}" "Carla-Patchbay:input_2"



# ------------- Output 
pw-link "jack-volume:output_1"   "${OutLinkL}"
pw-link "jack-volume:output_2"   "${OutLinkL}"
pw-link "jack-volume:output_3"   "${OutLinkL}"
pw-link "jack-volume:output_4"   "${OutLinkL}"
#pw-link "jack-volume:output_5"   "${OutLinkL}"
#pw-link "jack-volume:output_1"   "${OutLinkR}"
pw-link "jack-volume:output_2"   "${OutLinkR}"
pw-link "jack-volume:output_3"   "${OutLinkR}"
pw-link "jack-volume:output_4"   "${OutLinkR}"
pw-link "jack-volume:output_5"   "${OutLinkR}"

pw-link "Carla-Patchbay:output_1"   "${OutLinkL}"
pw-link "Carla-Patchbay:output_2"   "${OutLinkR}"

}

ZoomVoice() {
echo "Zoom voice"

# ----------- zoom
OutputZoomL="${G935MonL}"
OutputZoomR="${G935MonR}"

pw-link "ZOOM VoiceEngine:output_FL"   "${OutputZoomL}"
pw-link "ZOOM VoiceEngine:output_FR"   "${OutputZoomR}"

pw-link "ZOOM VoiceEngine:output_FL"   "alsa_input.platform-snd_aloop.0.analog-stereo:capture_FL"
pw-link "ZOOM VoiceEngine:output_FR"   "alsa_input.platform-snd_aloop.0.analog-stereo:capture_FR"

pw-link "ZOOM VoiceEngine:output_FL"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FL"
pw-link "ZOOM VoiceEngine:output_FR"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FR"


pw-link "alsa_input.usb-Logitech_G935_Gaming_Headset-00.mono-fallback:capture_MONO" "ZOOM VoiceEngine:input_MONO"

}


BrowserSetup() {
echo "BrowserSetup"
pw-link "Google Chrome:output_FL" "${OutputDeviceL}"

pw-link "Google Chrome:output_FR" "${OutputDeviceR}"


pw-link "Google Chrome:output_FL"   "alsa_input.platform-snd_aloop.0.analog-stereo:capture_FL"

pw-link "Google Chrome:output_FR"   "alsa_input.platform-snd_aloop.0.analog-stereo:capture_FR"

BlueTooth=`pactl list short sinks | grep bluez_output | awk '{print $2}'`

# bluez_output.2C_FD_B3_93_69_10.a2dp-sink:playback_FL"
pw-link "Google Chrome:output_FL"   "$BlueTooth:playback_FL"

pw-link "Google Chrome:output_FR"   "$BlueTooth:playback_FR"
pactl set-sink-volume $BlueTooth 100%


pw-link "Google Chrome:output_FL"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FL"

pw-link "Google Chrome:output_FR"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FR"

pw-link "Google Chrome:output_FL"   "alsa_output.pci-0000_00_1f.3.analog-stereo:playback_FL"

pw-link "Google Chrome:output_FR"   "alsa_output.pci-0000_00_1f.3.analog-stereo:playback_FR"

pw-link "Firefox:output_FL" "${G935PlayL}"
pw-link "Firefox:output_FL" "${G935PlayR}"
pw-link "Firefox:output_FR" "${G935PlayL}"
pw-link "Firefox:output_FR" "${G935PlayR}"

}


# ------------- --------------- 
# ------------- Main 
# ------------- --------------- 

#set -x 
export DeviceList="`pw-link -o`"
export DeviceListI="`pw-link -i`"

MainGuitarOut1=gx_head_fx:out_0
MainGuitarOut2=gx_head_fx:out_1
#MainGuitarOut1=ValhallaUberMod:output_1
#MainGuitarOut2=ValhallaUberMod:output_2

# ------------- Output to headset
Device="G935"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"
    AssignInterface
    G935ID=`pactl list short sinks | grep G935 | awk '{print $2}'`
    pactl set-sink-volume $G935ID 50%

    G935ID=`pactl list short sources | grep G935s | awk '{print $2}' | grep input`
    pactl set-source-volume $G935ID 70%


    G935MonL=`pw-link -o | grep G935 | grep monitor_FL`
    G935MonR=`pw-link -o | grep G935 | grep monitor_FR`

    G935PlayL=`pw-link -i | grep G935 | grep playback_FL`
    G935PlayR=`pw-link -i | grep G935 | grep playback_FR`

fi


# ------------- Output to FocusRight
Device="usb-Focusrite_Scarlett"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"
    Scarlett=`pactl list short sinks | grep Scarlett | awk '{print $2}'`
    pactl set-sink-volume $Scarlett 125%


    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"
    Scarlett=`pactl list short sources | grep Scarlett | awk '{print $2}' | grep input`
 #   pactl set-source-volume $Scarlett 125%
    pamixer --set-volume 125 --allow-boost --source $Scarlett
fi

Device="Audio_AIR_192"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

    PacID=`pactl list short sinks | grep $Device | awk '{print $2}'`
    pactl set-sink-volume $PacID 125%
    PacID=`pactl list short sources | grep $Device | awk '{print $2}' | grep input`
 #   pactl set-source-volume $Scarlett 125%
    pamixer --set-volume 125 --allow-boost --source $    PacID=`pactl list short sources | grep $Device | awk '{print $2}' | grep input`



fi

Device="C-Media_Electronics"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi


Device="AudioBox_Go"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"

     AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi

Device="ZOOM_Corporation_U-24"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi

Device="XTONE"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"
    # InLinkL="alsa_input.usb-XSONIC_XTONE-00.pro-input-0:capture_AUX0"
    # InLinkR="alsa_input.usb-XSONIC_XTONE-00.pro-input-0:capture_AUX1"
    # OutLinkL="alsa_output.usb-XSONIC_XTONE-00.pro-output-0:playback_AUX0"
    # OutLinkR="alsa_output.usb-XSONIC_XTONE-00.pro-output-0:playback_AUX1"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi

Device="Burr-Brown"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi

# ------------- Output bluetooth
Device="bluez_output"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"
    AssignInterface
fi

Device="NUX_USB"
echo "$DeviceList" | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    GrabInterface "$Device"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

    NuxID=`pactl list short sinks | grep NUX_USB | awk '{print $2}'`
    pactl set-sink-volume $NuxID 125%

    NuxID=`pactl list short sources | grep NUX_USB | awk '{print $2}' | grep input`
    pactl set-source-volume $NuxID 125%
fi

ZoomVoice

BrowserSetup


# Loopback Link
pw-link "alsa_output.platform-snd_aloop.0.analog-stereo:monitor_FL" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FL"
pw-link "alsa_output.platform-snd_aloop.0.analog-stereo:monitor_FR" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FR"

pw-link "${G935MonL}" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FL"
pw-link "${G935MonR}" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FR"

pw-link  "Carla:LSP Mixer x4 Stereo:Output L" "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:playback_AUX0"
pw-link  "Carla:LSP Mixer x4 Stereo:Output R" "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:playback_AUX1"


# pw-link "jack-volume:output_1"   "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:playback_AUX0"





# ------------- Output Mplayer to MP3 input
pw-link "MPlayer:output_FR" "jack-volume:input_3"
pw-link "MPlayer:output_FL" "jack-volume:input_3"

pw-link "MPlayer:output_FR" "Carla:LSP Mixer x4 Stereo:Audio input right 3"
pw-link "MPlayer:output_FL" "Carla:LSP Mixer x4 Stereo:Audio input left 3"

pw-link -d "MPlayer:output_FR" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FR"
pw-link -d "MPlayer:output_FL" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FL"

pw-link -d "alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FL" "gx_head_amp:in_0"
pw-link -d "alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FR" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FR"
pw-link -d "alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FL" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FL"
pw-link -d "alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FR" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FL"

# ------------- Guitarix to looper
pw-link "${MainGuitarOut1}" "sooperlooper:common_in_1"
pw-link "${MainGuitarOut2}" "sooperlooper:common_in_2"

# ------------- Guitarix Head to Guitarix FX
pw-link "gx_head_amp:out_0" "gx_head_fx:in_0"

# ------------- Guitarix out
#pw-link "gx_head_fx:out_0" "Carla:LSP Mixer x4 Stereo:Audio input right 1"
# pw-link "gx_head_fx:out_1" "Carla:LSP Mixer x4 Stereo:Audio input left 1"

pw-link "gx_head_fx:out_0" "Carla:rkr Shelf Boost:Audio In R"
pw-link "gx_head_fx:out_1" "Carla:rkr Shelf Boost:Audio In L"

# ------------- Qsynth
pw-link "qsynth:left"  "Carla:LSP Mixer x4 Stereo:Audio input left 2"
pw-link "qsynth:right" "Carla:LSP Mixer x4 Stereo:Audio input right 2"


pw-link "Midi-Bridge:LiveMusic Output:(capture_1) Guitarix" "gx_head_amp:midi_in_1"

pw-link "Midi-Bridge:Virtual MIDI Card 1:(capture_0) VirMIDI 6-0" "qsynth:midi_00"

# USB-C
# pw-link "Google Chrome:output_FL" "alsa_output.usb-0c76_USB_PnP_Audio_Device-00.analog-stereo:playback_FL"
# pw-link "Google Chrome:output_FR" "alsa_output.usb-0c76_USB_PnP_Audio_Device-00.analog-stereo:playback_FR"
# pw-link "Google Chrome:output_FL" "alsa_output.platform-snd_aloop.0.analog-stereo:playback_FL"
# pw-link "Google Chrome:output_FR" "alsa_output.platform-snd_aloop.0.analog-stereo:playback_FR"


# For Carla Single Rack
pw-link $InLinkR "${CarlPre}audio-in1"
pw-link $InLinkL "${CarlPre}audio-in2"
pw-link "${CarlPre}audio-out1" "jack-volume:input_1"
pw-link "${CarlPre}audio-out2" "jack-volume:input_5"

pw-link "${CarlPre}audio-out1" "Carla:LSP Mixer x4 Stereo:Audio input right 1"
pw-link "${CarlPre}audio-out2" "Carla:LSP Mixer x4 Stereo:Audio input left 1"


# Serial or parallel connection with Carla
# CarlaRackL="gx_head_fx:out_0"
# CarlaRackR="gx_head_fx:out_1"
# CarlaRackL=$InLinkL
# CarlaRackR=$InLinkR


echo "*** MHarmonizerMB"



pw-link "Midi-Bridge:LiveMusic Output:(capture_1) Guitarix" "Midi-Bridge:LvSwitch:(playback_0) SwitchIn"








pw-link -d "Firefox:output_FL" "Plasma PA:input_FL"
pw-link -d "Firefox:output_FR" "Plasma PA:input_FR"
pw-link -d "${G935PlayL}" "plasma PA:input_FL"
pw-link -d "${G935PlayR}" "Plasma PA:input_FR"


pw-link -d "Plasma PA:input_FL" "alsa_input.usb-Logitech_G935_Gaming_Headset-00.pro-input-0:capture_AUX0
"
pw-link -d "Plasma PA:input_FL" "alsa_input.usb-Logitech_G935_Gaming_Headset-00.pro-input-0:capture_AUX0
"

pw-link -d "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:monitor_AUX0" "Plasma PA:input_FL"

pw-link -d "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:monitor_AUX1" "Plasma PA:input_FR"
InputGuitarL
pw-link -d "alsa_output.usb-V_NO_NO_NONE_USB_MIC-E01-00.iec958-stereo:moniInputGuitarLtor_FL" "Plasma PA:input_FL"

pw-link -d "alsa_output.usb-V_NO_NO_NONE_USB_MIC-E01-00.iec958-stereo:monitor_FR" "Plasma PA:input_FR"

pw-link -d "alsa_output.pci-0000_00_1f.3.analog-stereo:monitor_FL" "Plasma PA:input_FL"
pw-link -d "alsa_output.pci-0000_00_1f.3.analog-stereo:monitor_FR" "Plasma PA:input_FR"
pw-link -d "alsa_output.platform-snd_aloop.0.analog-stereo:monitor_FL" "Plasma PA:input_FL"
pw-link -d "alsa_output.platform-snd_aloop.0.analog-stereo:monitor_FR" "Plasma PA:input_FR"

pw-link -d "alsa_input.platform-snd_aloop.0.analog-surround-41:capture_FL" "Plasma PA:input_FL"
pw-link -d "alsa_input.platform-snd_aloop.0.analog-surround-41:capture_FR" "Plasma PA:input_FR"
pw-link -d "alsa_input.platform-snd_aloop.0.analog-surround-41:capture_LFE" "Plasma PA:input_LFE"
pw-link -d "alsa_input.platform-snd_aloop.0.analog-surround-41:capture_RL" "Plasma PA:input_RL"
pw-link -d "alsa_input.platform-snd_aloop.0.analog-surround-41:capture_RR" "Plasma PA:input_RR"

pw-link -d "alsa_output.usb-XSONIC_XTONE-00.pro-output-0:monitor_AUX0" "Plasma PA:input_FL"
pw-link -d "alsa_output.usb-XSONIC_XTONE-00.pro-output-0:monitor_AUX1" "Plasma PA:input_FR"
pw-link -d "alsa_input.usb-XSONIC_XTONE-00.pro-input-0:capture_AUX0" "Plasma PA:input_FL"
pw-link -d "alsa_input.usb-XSONIC_XTONE-00.pro-input-0:capture_AUX1" "Plasma PA:input_FR"

pw-link -d "J3Sink:monitor_FL" "Plasma PA:input_FL"

echo "Delete 1 "${OutLinkL}" 2 "${G935PlayL}
pw-link -d "${OutLinkL}" "${G935PlayL}"
pw-link -d "${OutLinkL}" "${G935PlayR}"


set -x
CarlPre="LveMus.0/"
pw-link "${CarlaRackL}" "${CarlPre}abGate:Input"
pw-link "${CarlaRackR}" "${CarlPre}abGate:Input_2"

pw-link "${InputGuitarL}" "${CarlPre}abGate:Input"
pw-link "${InputGuitarR}" "${CarlPre}abGate:Input_2"


pw-link "${CarlPre}abGate:Output"  "${CarlPre}Calf Mono Compressor:In L"
pw-link "${CarlPre}abGate:Output"  "${CarlPre}Calf Mono Compressor:In L"
pw-link "${CarlPre}abGate:Output_2"  "${CarlPre}Calf Mono Compressor:In L"

pw-link "${CarlPre}Calf Mono Compressor:Out L" "${CarlPre}LSP Graphic Equalizer x16 Mono:Input"

pw-link "${CarlPre}LSP Graphic Equalizer x16 Mono:Output" "${CarlPre}GxMultiBandDistortion:In"
pw-link "${CarlPre}LSP Graphic Equalizer x16 Mono:Output" "${CarlPre}GxMultiBandDistortion:In_2"

pw-link "${CarlPre}GxMultiBandDistortion:Out" "${CarlPre}GxWahwah:In"
pw-link "${CarlPre}GxMultiBandDistortion:Out_2" "${CarlPre}GxWahwah:In_2"

pw-link "${CarlPre}GxWahwah:Out" "${CarlPre}TAP Chorus.Flanger:Input_L"
pw-link "${CarlPre}GxWahwah:Out_2" "${CarlPre}TAP Chorus.Flanger:Input_R"

pw-link "${CarlPre}TAP Chorus.Flanger:Output_L" "${CarlPre}GxZita_rev1-Stereo:In"
pw-link "${CarlPre}TAP Chorus.Flanger:Output_R" "${CarlPre}GxZita_rev1-Stereo:In1"

pw-link "${CarlPre}GxZita_rev1-Stereo:Out" "${CarlPre}StereoFX:input_1"
pw-link "${CarlPre}GxZita_rev1-Stereo:Out1" "${CarlPre}StereoFX:input_2"


pw-link "${CarlPre}StereoFX:output_1" "jack-volume:input_1"
pw-link "${CarlPre}StereoFX:output_2" "jack-volume:input_5"

pw-link "${CarlPre}StereoFX:output_1" "Carla:LSP Mixer x4 Stereo:Audio input left 1"
pw-link "${CarlPre}StereoFX:output_2" "Carla:LSP Mixer x4 Stereo:Audio input left 1"

exit
echo "************************************"

#pw-link "${InputGuitarR}" "Manifold:input_1"
#pw-link "${InputGuitarR}" "Manifold:input_2"
pw-link "${CarlaRackL}" "${CarlPre}MHarmonizerMB:input_1"
pw-link "${CarlaRackR}" "${CarlPre}MHarmonizerMB:input_2"
pw-link "${CarlaRackL}" "${CarlPre}MHarmonizerMB:input_3"
pw-link "${CarlaRackR}" "${CarlPre}MHarmonizerMB:input_4"

pw-link "${CarlPre}MHarmonizerMB:output_1" "jack-volume:input_1"
pw-link "${CarlPre}MHarmonizerMB:output_2" "jack-volume:input_5"
pw-link "${CarlPre}MHarmonizerMB:output_3" "jack-volume:input_1"
pw-link "${CarlPre}MHarmonizerMB:output_4" "jack-volume:input_5"

pw-link "${CarlPre}MHarmonizerMB:output_1" "Carla:LSP Mixer x4 Stereo:Audio input right 1"
pw-link "${CarlPre}MHarmonizerMB:output_2" "Carla:LSP Mixer x4 Stereo:Audio input left 1"
pw-link "${CarlPre}MHarmonizerMB:output_3" "Carla:LSP Mixer x4 Stereo:Audio input right 1"
pw-link "${CarlPre}MHarmonizerMB:output_4" "Carla:LSP Mixer x4 Stereo:Audio input left 1"



echo "*** MUltraMaximizer"
#pw-link "${InputGuitarR}" "Manifold:input_1"
#pw-link "${InputGuitarR}" "Manifold:input_2"
pw-link "${CarlaRackL}" "${CarlPre}MUltraMaximizer:input_1"
pw-link "${CarlaRackR}" "${CarlPre}MUltraMaximizer:input_2"
pw-link "${CarlaRackL}" "${CarlPre}MUltraMaximizer:input_3"
pw-link "${CarlaRackR}" "${CarlPre}MUltraMaximizer:input_4"
pw-link "${CarlaRackL}" "${CarlPre}MUltraMaximizer:input_5"
pw-link "${CarlaRackR}" "${CarlPre}MUltraMaximizer:input_6"
pw-link "${CarlaRackL}" "${CarlPre}MUltraMaximizer:input_7"
pw-link "${CarlaRackR}" "${CarlPre}MUltraMaximizer:input_8"

pw-link "${CarlPre}MUltraMaximizer:output_1" "jack-volume:input_1"
pw-link "${CarlPre}MUltraMaximizer:output_2" "jack-volume:input_5"
pw-link "${CarlPre}MUltraMaximizer:output_3" "jack-volume:input_1"
pw-link "${CarlPre}MUltraMaximizer:output_4" "jack-volume:input_5"
pw-link "${CarlPre}MUltraMaximizer:output_5" "jack-volume:input_1"
pw-link "${CarlPre}MUltraMaximizer:output_6" "jack-volume:input_5"
pw-link "${CarlPre}MUltraMaximizer:output_7" "jack-volume:input_1"
pw-link "${CarlPre}MUltraMaximizer:output_8" "jack-volume:input_5"

echo "*** kHs Chorus"
pw-link "${CarlaRackL}" "${CarlPre}kHs Chorus:input_1"
pw-link "${CarlaRackR}" "${CarlPre}kHs Chorus:input_2"

pw-link "${CarlPre}kHs Chorus:output_1" "jack-volume:input_1"
pw-link "${CarlPre}kHs Chorus:output_2" "jack-volume:input_5"

echo "*** Wider"
pw-link "${CarlaRackL}" "${CarlPre}Wider:input_1"
pw-link "${CarlaRackR}" "${CarlPre}Wider:input_2"

pw-link "${CarlPre}Wider:output_1" "jack-volume:input_1"
pw-link "${CarlPre}Wider:output_2" "jack-volume:input_5"

echo "*** Double"
pw-link "${CarlaRackL}" "${CarlPre}Doublelay:input_1"
pw-link "${CarlaRackR}" "${CarlPre}Doublelay:input_2"

pw-link "${CarlPre}Doublelay:output_1" "jack-volume:input_1"
pw-link "${CarlPre}Doublelay:output_2" "jack-volume:input_5"


echo "*** Manifold"
#pw-link "${InputGuitarR}" "Manifold:input_1"
#pw-link "${InputGuitarR}" "Manifold:input_2"
pw-link "${CarlaRackL}" "${CarlPre}Manifold:input_1"
pw-link "${CarlaRackR}" "${CarlPre}Manifold:input_2"

pw-link "${CarlPre}Manifold:output_1" "jack-volume:input_1"
pw-link "${CarlPre}Manifold:output_2" "jack-volume:input_5"

echo "*** ValhallaUberMod"
#pw-link "${InputGuitarR}" "ValhallaUberMod:input_1"
#pw-link "${InputGuitarR}" "ValhallaUberMod:input_2"
pw-link "${CarlaRackL}" "${CarlPre}ValhallaUberMod:input_1"
pw-link "${CarlaRackR}" "${CarlPre}ValhallaUberMod:input_2"
pw-link "${CarlPre}ValhallaUberMod:output_1" "jack-volume:input_1"
pw-link "${CarlPre}ValhallaUberMod:output_2" "jack-volume:input_5"

echo "*** Digital Multiply"
pw-link "${InputGuitarR}" "Acon Digital Multiply:input_1"
pw-link "${InputGuitarR}" "Acon Digital Multiply:input_2"
pw-link "${InputGuitarR}" "Acon Digital Multiply:input_3"
pw-link "${InputGuitarR}" "Acon Digital Multiply:input_4"
pw-link "${InputGuitarR}" "Acon Digital Multiply:input_5"
pw-link "${InputGuitarR}" "Acon Digital Multiply:input_6"
pw-link "${InputGuitarR}" "Acon Digital Multiply:input_7"
pw-link "${InputGuitarR}" "Acon Digital Multiply:input_8"

pw-link "Acon Digital Multiply:output_1" "jack-volume:input_1"
pw-link "Acon Digital Multiply:output_2" "jack-volume:input_5"
pw-link "Acon Digital Multiply:output_3" "jack-volume:input_1"
pw-link "Acon Digital Multiply:output_4" "jack-volume:input_5"
pw-link "Acon Digital Multiply:output_5" "jack-volume:input_1"
pw-link "Acon Digital Multiply:output_6" "jack-volume:input_5"
pw-link "Acon Digital Multiply:output_7" "jack-volume:input_1"
pw-link "Acon Digital Multiply:output_8" "jack-volume:input_5"

