#!/bin/bash
#------------------------------------------------
#
#   File:   LiveMusicApps
#
#   Contains:
#
#
#   Written By:     Elias Keshishoglou on Wed 17 Jun 2020 04:52:30 PM PDT
#
#   Copyright ©:    2020 Elias Keshishoglou all rights reserved.
#
#
#------------------------------------------------#
echo "MusicApp -> ${1}  ${2} ${3}"

PipeActive=`ps aux | grep pipewire | wc -l`
if [[ $PipeActive > 1 ]];  then
echo "Pipe found"
PipeStart="pw-jack -s 96000" 
else 
echo "Pipe Not found"
PipeStart=" " 
fi

if [ "${1}" == "html" ] ; then
    gedit "${2}" &
    exit 0
fi

if [ "${1}" == "desktop" ] ; then
#    gtk-launch "${2}" &

    echo "desktop -> " "${2}" "${2%/*}/"

#    kioclient5 exec "${2}" &
    kioclient5 exec "${2%/*}/" &
    exit 0
fi

if [ "${1}" == "DrumFile" ] ; then
    exit 0
fi

if [ "${1}" == "mp4" ] ; then
# Handle it internal to the App
exit 1
fi

if [ "${1}" == "mp3" ] ; then
# Handle it internal to the App
exit 1
fi

if [ "${1}" == "mid" ] ; then
# Handle it internal to the App
 #   mscore4  "${2}" &
    exit 1
fi

if [ "${1}" == "med" ] ; then
${PipeStart} muse "${2}" &
    exit 0
fi

if [ "${1}" == "Score" ] ; then
    ${PipeStart} mscore3 "${2}" &
    exit 0
fi

if [ "${1}" == "MidPlay" ] ; then
    ${PipeStart} muse "${2}" &
    exit 0
fi

if [ "${1}" == "rg" ] ; then
    ${PipeStart} rosegarden "${2}" &
    sleep 1
#    aconnect -x "rosegarden":0
#    aconnect -x "rosegarden":1
#        
#    aconnect "TriplePlay Connect":0 "rosegarden":0
#    aconnect "Fishman TriplePlay":0 "rosegarden":0
#    aconnect "LiveMusic Output":3 "rosegarden":2
#    aconnect "rosegarden":3 "FLUID Synth (qsynth)":0

    aconnect "TriplePlay Connect":0 "rosegarden:record in       "
    aconnect "Fishman TriplePlay":0 "rosegarden:record in       "
    aconnect "LiveMusic Output":3 "rosegarden":2
    aconnect "rosegarden:out 1 - General MIDI Device" "FLUID Synth (qsynth)":0
    exit 0
fi

# https://ftp.osuosl.org/pub/musescore-nightlies/linux/4x/nightly/?C=M;O=D

echo "About to check gp5"

if [ "${1}" == "gp3" ] ||
[ "${1}" == "mscz" ] ||
[ "${1}" == "gp" ] ||
[ "${1}" == "gpx" ] ||
[ "${1}" == "gp4" ] ||
[ "${1}" == "gp5" ] ||
[ "${1}" == "gp6" ] ||
[ "${1}" == "ptb" ] ; then
    echo "Muse Score ${1} ${2} nice -15 ${PipeStart}"
     QT_PLUGIN_PATH="" QT_SCREEN_SCALE_FACTORS=1.4 nice -15 ${PipeStart} mscore "${2}" &>/dev/null &
#    /usr/src/LiveMusicBuilds/MuseScore-4.0.0-x86_64.appimage "${2}" &>/dev/null &
    # /usr/src/LiveMusicBuilds/MuseScore-3.6.2.548021370-x86_64.AppImage "${2}" &>/dev/null &
# Convert from pdf to sheet music
# magick convert scan_fullcolour1.png -colorspace gray grayscale_page_1.png
# magick convert scan_fullcolour2.png -colorspace gray grayscale_page-2.png
# magick grayscale_page_* grayscale.pdf

# upload grayscale.pdf to https://musescore.com/import
    exit 0
fi

if [ "${1}" == "Looper" ] ; then
    echo "Running Looper"
        ss --kill state listening src :9951
        sleep 1
        GDK_SCALE=1 GDK_DPI_SCALE=0.8 nice -18 ${PipeStart} slgui -L /home/MySongs/GuitarSound/GuitarSound.slsess -P=9951  --never-timeout &
        # --never-timeout &

    exit 0
fi

if [ "${1}" == "guitarix" ] ; then
    echo "Running guitarix"
    GDK_DPI_SCALE=1.5 \
    GTK_THEME="LiveMusicTheme"  \
    nice -20 ${PipeStart}  \
    guitarix --log-terminal 2>&1 >/dev/null &
    exit 0
fi

if [ "${1}" == "EffectsProcessorApp" ] ; then
    echo "Running guitarix"
    GDK_DPI_SCALE=1.5 \
    GTK_THEME="LiveMusicTheme" \
    nice -20 ${PipeStart} \
    guitarix --log-terminal  2>&1 >/dev/null &

    # flatpak run org.guitarix.Guitarix
#    rsync -avrx .config/guitarix  .var/app/org.guitarix.Guitarix/config/
    # /usr/src/AppImageApps/AppImages/guitarix-0.39-x86_64.AppImage \
    # -p 7000 &
   
#    guitarix \
#    /AppImages/guitarix-0.39-x86_64.AppImage \

# QT_SCREEN_SCALE_FACTORS=1.25 pw-jack carla-rack /home/MySongs/Prefs/CarGuitar.carxp
# carla-single possibly.

# pw-jack carla-single win64 vst2 /home/elias/.vst/ValhallaDelay_x64.dll First 0001

# /home/elias/.wine/drive_c/Program Files/Common Files/VST3/Acon Digital
# pw-jack carla-single win64 vst3 ./Multiply.vst3

# /home/elias/.wine/drive_c/Program Files/Common Files/VST3
# pw-jack carla-single win64 vst3 ./ValhallaUberMod.vst3

# PIPEWIRE_LOG_SYSTEMD=false PIPEWIRE_DEBUG=5 carla-jack-multi
# WIREPLUMBER_DEBUG=D
# ps -eo pid,ppid,ni,comm

# LV2 plugins 
# https://www.youtube.com/watch?v=51eHCA4oCEI&list=PLkuRaNsK2AJ0D8uhRIjftgmqVW0yvDfMx

    exit 0
fi

if [ "${1}" == "guitarixNew" ] ; then
    echo "Running guitarix"
    GDK_DPI_SCALE=1.5 \
    GTK_THEME="LiveMusicTheme"  \
    GDK_BACKEND=x11 nice -15 \
    ${PipeStart} guitarix \
    -p 7000 \
    --log-terminal &
    exit 0
fi

# A PDF file.
if [ "${1}" == "xopp" ] ; then
     xournalpp "${2}"  &
     exit 0
fi

# A PDF file.
if [ "${1}" == "pdf" ] ; then
     okular  "${2}"  &
     exit 0
fi

# A PDF file with a page number offset..
if grep -q "pdf" <<< "$1"; then
    theURL="${3:7}"
    PDFPage="${theURL//#/ --}"
    okular ${PDFPage} &
    exit 0
fi

if [ "${1}" == "connect" ] ; then
     GuitarEffects -c
     exit 0
fi

if [ "${1}" == "Reindex" ] ; then
     ReIndex.sh  &
     exit 0
fi



exit 1
