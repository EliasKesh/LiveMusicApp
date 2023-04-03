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
# ------------- AssignInterface 
# ------------- --------------- 
AssignInterface() {

echo "AssignInterface " $InLinkL

# ------------- input 
pw-link "${InLinkL}" "gx_head_amp:in_0"
pw-link "${InLinkR}" "gx_head_amp:in_0"

# ------------- Output 
pw-link "jack-volume:output_1"   "${OutLinkL}"
pw-link "jack-volume:output_2"   "${OutLinkL}"
pw-link "jack-volume:output_3"   "${OutLinkL}"
pw-link "jack-volume:output_4"   "${OutLinkL}"
pw-link "jack-volume:output_5"   "${OutLinkL}"
pw-link "jack-volume:output_1"   "${OutLinkR}"
pw-link "jack-volume:output_2"   "${OutLinkR}"
pw-link "jack-volume:output_3"   "${OutLinkR}"
pw-link "jack-volume:output_4"   "${OutLinkR}"
pw-link "jack-volume:output_5"   "${OutLinkR}"

}


ZoomVoice() {
echo "Zoom voice"

# ----------- zoom
OutputZoomL="alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:monitor_AUX0"
OutputZoomR="alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:monitor_AUX01"

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
pactl set-sink-volume $BlueTooth 90%


pw-link "Google Chrome:output_FL"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FL"

pw-link "Google Chrome:output_FR"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FR"

pw-link "Google Chrome:output_FL"   "alsa_output.pci-0000_00_1f.3.analog-stereo:playback_FL"

pw-link "Google Chrome:output_FR"   "alsa_output.pci-0000_00_1f.3.analog-stereo:playback_FR"

pw-link "Firefox:output_FL" "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:playback_AUX0"
pw-link F"irefox:output_FL" "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:playback_AUX1"
pw-link "Firefox:output_FR" "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:playback_AUX0"
pw-link "Firefox:output_FR" "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:playback_AUX1"

}


# ------------- --------------- 
# ------------- Main 
# ------------- --------------- 

set -x 
DeviceList="`pw-link -o`"

Device="Audio_AIR_192"
echo $DeviceList | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    InLinkL="alsa_input.usb-M-Audio_AIR_192_8-00.analog-stereo:capture_FL"
    InLinkR="alsa_input.usb-M-Audio_AIR_192_8-00.analog-stereo:capture_FR"
    
    OutLinkL="alsa_output.usb-M-Audio_AIR_192_8-00.analog-surround-40:playback_FL"
    OutLinkR="alsa_output.usb-M-Audio_AIR_192_8-00.analog-surround-40:playback_FR"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi

Device="C-Media_Electronics"
echo $DeviceList | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    InLinkL="alsa_input.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.mono-fallback:capture_MONO"
    InLinkR="alsa_input.usb-IK_Multimedia_iRig_HD_2-00.mono-fallback:capture_MONO"
    OutLinkL="alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FL"
    OutLinkR="alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FR"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi

Device="NUX_NUX_USB"
echo $DeviceList | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    InLinkL="alsa_input.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:capture_FL"
    InLinkR="alsa_input.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:capture_FR"
    OutLinkL="alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FL"
    OutLinkR="alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FR"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi


Device="PreSonus_AudioBox_Go"
echo $DeviceList | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    InLinkL="alsa_input.usb-PreSonus_AudioBox_Go_UGBA21410845-00.analog-stereo:capture_FR"
    InLinkR="alsa_input.usb-PreSonus_AudioBox_Go_UGBA21410845-00.analog-stereo:capture_FL"
    OutLinkL="alsa_output.usb-PreSonus_AudioBox_Go_UGBA21410845-00.analog-stereo:playback_FL"
    OutLinkR="alsa_output.usb-PreSonus_AudioBox_Go_UGBA21410845-00.analog-stereo:playback_FR"
     AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

  
fi

Device="ZOOM_Corporation_U-24"
echo $DeviceList | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    InLinkL="alsa_input.usb-ZOOM_Corporation_U-24-00.analog-stereo:capture_FL"
    InLinkR="alsa_input.usb-ZOOM_Corporation_U-24-00.analog-stereo:capture_FR"
    OutLinkL="alsa_output.usb-ZOOM_Corporation_U-24-00.analog-surround-40:playback_FL"
    OutLinkR="alsa_output.usb-ZOOM_Corporation_U-24-00.analog-surround-40:playback_FR"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi

Device="XSONIC_XTONE"
echo $DeviceList | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    InLinkL="alsa_input.usb-XSONIC_XTONE-00.analog-stereo:capture_FL"
    InLinkR="alsa_input.usb-XSONIC_XTONE-00.analog-stereo:capture_FR"
    OutLinkL="alsa_output.usb-XSONIC_XTONE-00.pro-output-0:playback_AUX0"
    OutLinkR="alsa_output.usb-XSONIC_XTONE-00.pro-output-0:playback_AUX1"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi

Device="Burr-Brown"
echo $DeviceList | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    InLinkL="alsa_input.usb-Burr-Brown_from_TI_USB_Audio_CODEC-00.analog-stereo-input:capture_FL"
    InLinkR="alsa_input.usb-Burr-Brown_from_TI_USB_Audio_CODEC-00.analog-stereo-input:capture_FR"
    OutLinkL="alsa_output.usb-Burr-Brown_from_TI_USB_Audio_CODEC-00.analog-stereo-output:playback_FL"
    OutLinkR="alsa_output.usb-Burr-Brown_from_TI_USB_Audio_CODEC-00.analog-stereo-output:playback_FR"
    AssignInterface
    # ------------- Microphone
    InputGuitarL="${InLinkR}"

    # ------------- Guitar Input
    InputGuitarR="${InLinkL}"

fi


# ------------- Output bluetooth
Device="bluez_output"
echo $DeviceList | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    InLinkL=""
    InLinkR=""
    OutLinkL="bluez_output.2C_FD_B3_A2_51_83.a2dp-sink:playback_FL"
    OutLinkR="bluez_output.2C_FD_B3_A2_51_83.a2dp-sink:playback_FR"
    AssignInterface
fi

# ------------- Output to headset
Device="G935"
echo $DeviceList | grep "${Device}"
if [ "$?" -eq 0 ]; then
    echo "We have " $Device
    InLinkL=""
    InLinkR=""
    OutLinkL="alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:monitor_AUX0"
    OutLinkR="alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:monitor_AUX1"
    AssignInterface
fi


ZoomVoice

BrowserSetup


# Loopback Link
pw-link "alsa_output.platform-snd_aloop.0.analog-stereo:monitor_FL" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FL"
pw-link "alsa_output.platform-snd_aloop.0.analog-stereo:monitor_FR" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FR"

pw-link "alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:monitor_FL" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FL"
pw-link "alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:monitor_FR" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FR"


pw-link "jack-volume:output_1"   "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:playback_AUX0"





# ------------- Output Mplayer to MP3 input
pw-link "MPlayer:output_FR" "jack-volume:input_2"
pw-link "MPlayer:output_FL" "jack-volume:input_2"

pw-link -d "MPlayer:output_FR" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FR"
pw-link -d "MPlayer:output_FL" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FL"

pw-link -d "alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FL" "gx_head_amp:in_0"
pw-link -d "alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FR" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FR"
pw-link -d "alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FL" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FL"
pw-link -d "alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FR" "alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FL"

# ------------- Guitarix to looper
pw-link "gx_head_fx:out_0" "sooperlooper:common_in_1"
pw-link "gx_head_fx:out_1" "sooperlooper:common_in_1"

# ------------- Guitarix Head to Guitarix FX
pw-link "gx_head_amp:out_0" "gx_head_fx:in_0"

# ------------- Guitarix out
pw-link "gx_head_fx:out_0" "jack-volume:input_1"
pw-link "gx_head_fx:out_1" "jack-volume:input_5"

# ------------- Qsynth
pw-link "qsynth:left"  "jack-volume:input_2"
pw-link "qsynth:right" "jack-volume:input_2"


pw-link "Midi-Bridge:LiveMusic Output:(capture_1) Guitarix" "gx_head_amp:midi_in_1"

# USB-C
# pw-link "Google Chrome:output_FL" "alsa_output.usb-0c76_USB_PnP_Audio_Device-00.analog-stereo:playback_FL"
# pw-link "Google Chrome:output_FR" "alsa_output.usb-0c76_USB_PnP_Audio_Device-00.analog-stereo:playback_FR"
# pw-link "Google Chrome:output_FL" "alsa_output.platform-snd_aloop.0.analog-stereo:playback_FL"
# pw-link "Google Chrome:output_FR" "alsa_output.platform-snd_aloop.0.analog-stereo:playback_FR"



echo "*** Manifold"
pw-link "${InputGuitarR}" "Manifold:in_1"
pw-link "${InputGuitarR}" "Manifold:in_2"
pw-link "${InputGuitarR}" "Manifold:in_3"
pw-link "${InputGuitarR}" "Manifold:in_4"

pw-link "Manifold:out_1" "jack-volume:input_1"
pw-link "Manifold:out_2" "jack-volume:input_5"
pw-link "Manifold:out_3" "jack-volume:input_1"
pw-link "Manifold:out_4" "jack-volume:input_5"

pw-link "${InputGuitarR}" "Manifold:input_1"
pw-link "${InputGuitarR}" "Manifold:input_2"
pw-link "Manifold:output_1" "jack-volume:input_1"
pw-link "Manifold:output_2" "jack-volume:input_5"

echo "*** ValhallaUberMod"
pw-link "${InputGuitarR}" "ValhallaUberMod:input_1"
pw-link "${InputGuitarR}" "ValhallaUberMod:input_2"
pw-link "ValhallaUberMod:output_1" "jack-volume:input_1"
pw-link "ValhallaUberMod:output_2" "jack-volume:input_5"

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

pw-link -d "Firefox:output_FL" "Plasma PA:input_FL"
pw-link -d "Firefox:output_FR" "Plasma PA:input_FR"
pw-link -d "alsa_input.usb-Logitech_G935_Gaming_Headset-00.pro-input-0:capture_AUX0" "plasma PA:input_FL"
pw-link -d "alsa_input.usb-Logitech_G935_Gaming_Headset-00.pro-input-0:capture_AUX0" "Plasma PA:input_FL"


pw-link -d "Plasma PA:input_FL" "alsa_input.usb-Logitech_G935_Gaming_Headset-00.pro-input-0:capture_AUX0
"
pw-link -d "Plasma PA:input_FL" "alsa_input.usb-Logitech_G935_Gaming_Headset-00.pro-input-0:capture_AUX0
"

pw-link -d "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:monitor_AUX0" "Plasma PA:input_FL"

pw-link -d "alsa_output.usb-Logitech_G935_Gaming_Headset-00.pro-output-0:monitor_AUX1" "Plasma PA:input_FR"

pw-link -d "alsa_output.usb-V_NO_NO_NONE_USB_MIC-E01-00.iec958-stereo:monitor_FL" "Plasma PA:input_FL"

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


