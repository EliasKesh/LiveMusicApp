#! /bin/sh
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

if [ "${1}" == "html" ] ; then
    gedit "${2}" &
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
    muse "${2}" &
    exit 0
fi

if [ "${1}" == "Score" ] ; then
    mscore "${2}" &
    exit 0
fi

if [ "${1}" == "MidPlay" ] ; then
    muse "${2}" &
    exit 0
fi

if [ "${1}" == "rg" ] ; then
    rosegarden "${2}" &
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
    echo "Muse Score ${1}  ${2}"
/usr/src/LiveMusicBuilds/MuseScore-3.6.2.548021370-x86_64.AppImage "${2}" &>/dev/null &
    exit 0
fi

if [ "${1}" == "guitarix" ] ; then
    echo "Running guitarix"
    GTK_THEME="LiveMusicApp" nice -15 guitarix --log-terminal &
    exit 0
fi

if [ "${1}" == "EffectsProcessorApp" ] ; then
    echo "Running guitarix"
    GTK_THEME="LiveMusicApp" nice -15 guitarix --log-terminal &
    # /AppImages/guitarix-0.39-x86_64.AppImage \
    # -p 7000 &
   
#    guitarix \
#    /AppImages/guitarix-0.39-x86_64.AppImage \

    exit 0
fi

if [ "${1}" == "guitarixNew" ] ; then
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
    guitarix \
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

exit 1
