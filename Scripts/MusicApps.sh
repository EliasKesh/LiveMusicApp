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
 #   mscore4  "${2}" &
    exit 1
fi

if [ "${1}" == "med" ] ; then
    mscore4  "${2}" &
    exit 0
fi

# https://ftp.osuosl.org/pub/musescore-nightlies/linux/4x/nightly/?C=M;O=D
if [ "${1}" == "mscz" ] ; then
    mscore4 "${2}" &>/dev/null &
    exit 0
fi

if [ "${1}" == "gp3" ] |
[ "${1}" == "gp4" ] |
[ "${1}" == "gp5" ] |
[ "${1}" == "gp6" ] |
[ "${1}" == "ptb" ] ; then
    echo "${1}  ${2}"
    mscore3  "${2}"  &>/dev/null &
    exit 0
fi

if [ "${1}" == "guitarix" ] ; then
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
    /AppImages/guitarix-0.39-x86_64.AppImage \
    -p 7000 &
    exit 0
fi

if [ "${1}" == "EffectsProcessorApp" ] ; then
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
    /AppImages/guitarix-0.39-x86_64.AppImage \
    -p 7000 &

#    /AppImages/guitarix-0.39-x86_64.AppImage \

    exit 0
fi

if [ "${1}" == "guitarixNew" ] ; then
    echo "Running guitarix"
    GDK_BACKEND=x11 nice -15 \
    guitarix \
    -p 7000 &
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
