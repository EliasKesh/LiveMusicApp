#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	PipeConnect
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sat Apr 2 09:36:12 AM PDT 2022
#
#	Copyright : 	2022 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

OutputDeviceL="alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:playback_FL"
OutputDeviceR="alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:playback_FR"
OutputZoomL="alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:playback_FL"
OutputZoomR="alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:playback_FR"

OutputGuitarRawL="alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.iec958-stereo:playback_FL"
OutputGuitarRawR="alsa_output.usb-Focusrite_Scarlett_Solo_USB-00.iec958-stereo:playback_FR"
OutputGuitarL="${OutputDeviceL}"
OutputGuitarR="${OutputDeviceR}"

# Microphone
#InputGuitarL="alsa_input.usb-Focusrite_Scarlett_Solo_USB-00.iec958-stereo:capture_FL"

# Guitar Input
InputGuitarR="alsa_input.usb-Focusrite_Scarlett_Solo_USB-00.iec958-stereo:capture_FR"


pw-link "Google Chrome:output_FL" "${OutputDeviceL}"

pw-link "Google Chrome:output_FR" "${OutputDeviceR}"


pw-link "Google Chrome:output_FL"   "alsa_input.platform-snd_aloop.0.analog-stereo:capture_FL"

pw-link "Google Chrome:output_FR"   "alsa_input.platform-snd_aloop.0.analog-stereo:capture_FR"


pw-link "Google Chrome:output_FL"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FL"

pw-link "Google Chrome:output_FR"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FR"


# ----------- zoom 

pw-link "ZOOM VoiceEngine:output_FL"   "${OutputZoomL}"
pw-link "ZOOM VoiceEngine:output_FR"   "${OutputZoomR}"


pw-link "ZOOM VoiceEngine:output_FL"   "alsa_input.platform-snd_aloop.0.analog-stereo:capture_FL"
pw-link "ZOOM VoiceEngine:output_FR"   "alsa_input.platform-snd_aloop.0.analog-stereo:capture_FR"


pw-link "ZOOM VoiceEngine:output_FL"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FL"
pw-link "ZOOM VoiceEngine:output_FR"   "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:monitor_FR"


pw-link "alsa_input.usb-Logitech_G935_Gaming_Headset-00.mono-fallback:capture_MONO" "ZOOM VoiceEngine:input_MONO"


# Loopback Link
pw-link "alsa_output.platform-snd_aloop.0.analog-stereo:monitor_FL" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FL"
pw-link "alsa_output.platform-snd_aloop.0.analog-stereo:monitor_FR" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FR"

pw-link "alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:monitor_FL" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FL"
pw-link "alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:monitor_FR" "alsa_output.pci-0000_00_1f.3-platform-skl_hda_dsp_generic.HiFi__hw_sofhdadsp__sink:playback_FR"



set -x
# -----------  Guitar Effects
pw-link "jack-volume:output_1"   "${OutputGuitarL}"
pw-link "jack-volume:output_2"   "${OutputGuitarL}"
pw-link "jack-volume:output_3"   "${OutputGuitarL}"
pw-link "jack-volume:output_4"   "${OutputGuitarL}"
pw-link "jack-volume:output_1"   "${OutputGuitarR}"
pw-link "jack-volume:output_2"   "${OutputGuitarR}"
pw-link "jack-volume:output_3"   "${OutputGuitarR}"
pw-link "jack-volume:output_4"   "${OutputGuitarR}"

pw-link "jack-volume:output_1"   "${OutputGuitarRawL}"
pw-link "jack-volume:output_2"   "${OutputGuitarRawL}"
pw-link "jack-volume:output_3"   "${OutputGuitarRawL}"
pw-link "jack-volume:output_4"   "${OutputGuitarRawL}"
pw-link "jack-volume:output_1"   "${OutputGuitarRawR}"
pw-link "jack-volume:output_2"   "${OutputGuitarRawR}"
pw-link "jack-volume:output_3"   "${OutputGuitarRawR}"
pw-link "jack-volume:output_4"   "${OutputGuitarRawR}"


# iRig
pw-link "jack-volume:output_1" "alsa_output.usb-IK_Multimedia_iRig_HD_2-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_2" "alsa_output.usb-IK_Multimedia_iRig_HD_2-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_3" "alsa_output.usb-IK_Multimedia_iRig_HD_2-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_4" "alsa_output.usb-IK_Multimedia_iRig_HD_2-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_1" "alsa_output.usb-IK_Multimedia_iRig_HD_2-00.analog-stereo:playback_FR"
pw-link "jack-volume:output_2" "alsa_output.usb-IK_Multimedia_iRig_HD_2-00.analog-stereo:playback_FR"
pw-link "jack-volume:output_3" "alsa_output.usb-IK_Multimedia_iRig_HD_2-00.analog-stereo:playback_FR"
pw-link "jack-volume:output_4" "alsa_output.usb-IK_Multimedia_iRig_HD_2-00.analog-stereo:playback_FR"



pw-link "jack-volume:output_1" "alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_2" "alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_3" "alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_4" "alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_1" "alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FR"
pw-link "jack-volume:output_2" "alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FR"
pw-link "jack-volume:output_3" "alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FR"
pw-link "jack-volume:output_4" "alsa_output.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.analog-stereo:playback_FR"


pw-link "jack-volume:output_1" "alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_2" "alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_3" "alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_4" "alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FL"
pw-link "jack-volume:output_1" "alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FR"
pw-link "jack-volume:output_2" "alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FR"
pw-link "jack-volume:output_3" "alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FR"
pw-link "jack-volume:output_4" "alsa_output.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:playback_FR"


pw-link -d alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FL gx_head_amp:in_0
pw-link -d alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FR alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FR
pw-link -d alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FL alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FL
pw-link -d alsa_input.pci-0000_00_1b.0.analog-stereo:capture_FR alsa_output.pci-0000_00_1b.0.analog-stereo:playback_FL



pw-link gx_head_fx:out_0 sooperlooper:common_in_1
pw-link gx_head_fx:out_1 sooperlooper:common_in_1


pw-link "jack-volume:output_1"   "alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:playback_FR"
pw-link "jack-volume:output_2"   "alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:playback_FR"
pw-link "jack-volume:output_3"   "alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:playback_FR"
pw-link "jack-volume:output_4"   "alsa_output.usb-Logitech_G935_Gaming_Headset-00.iec958-stereo:playback_FR"

#input
pw-link "${InputGuitarL}" "gx_head_amp:in_0"
pw-link "${InputGuitarR}" "gx_head_amp:in_0"
pw-link "gx_head_amp:out_0" "gx_head_fx:in_0" 
pw-link "alsa_input.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device-00.mono-fallback:capture_MONO" "gx_head_amp:in_0"
pw-link "alsa_input.usb-IK_Multimedia_iRig_HD_2-00.mono-fallback:capture_MONO" "gx_head_amp:in_0"
pw-link "alsa_input.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:capture_FL" "gx_head_amp:in_0"
pw-link "alsa_input.usb-NUX_NUX_USB_Audio_2.0-00.analog-stereo:capture_FR" "gx_head_amp:in_0"




# Guitarix out
pw-link "gx_head_fx:out_0" "jack-volume:input_1"
pw-link "gx_head_fx:out_1" "jack-volume:input_1"

# Qsynth
pw-link "qsynth:left" "jack-volume:input_2"
pw-link "qsynth:right" "jack-volume:input_2"


pw-link "Midi-Bridge:LiveMusic Output:(capture_1) Guitarix" "gx_head_amp:midi_in_1"


# USB-C
pw-link "Google Chrome:output_FL" "alsa_output.usb-0c76_USB_PnP_Audio_Device-00.analog-stereo:playback_FL"
pw-link "Google Chrome:output_FR" "alsa_output.usb-0c76_USB_PnP_Audio_Device-00.analog-stereo:playback_FR"
pw-link "Google Chrome:output_FL" "alsa_output.platform-snd_aloop.0.analog-stereo:playback_FL"
pw-link "Google Chrome:output_FR" "alsa_output.platform-snd_aloop.0.analog-stereo:playback_FR"








