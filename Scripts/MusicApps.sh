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


export PIPEWIRE_AUTOCONNECT=false 
export PIPEWIRE_JACK_SERVER_GAIN=10.0
export PIPEWIRE_PROPS='{ node.autoconnect = false }'

    
PipeActive=$(ps aux | grep pipewire | wc -l)
echo $PipeActive
if [[ $PipeActive > 0 ]]; then
    echo "Pipewire found"
    PipeStart="pw-jack -s 96000 "
    # PipeOption=" -w "
    PipeOption=" "
else
    echo "Pipewire Not found"
    PipeStart=" "
    PipeOption=" "
fi

if [ "${1}" == "LiveMusicApp" ]; then
    ButtonSize=140
    if [ $(hostname) == "LiveMusic" ]; then
        ButtonSize=88
    fi

    if [ $(hostname) == "KeshT16" ]; then
        ButtonSize=100
    fi

    if [ $(hostname) == "KeshZen" ]; then
        ButtonSize=160
    fi

    GDK_DPI_SCALE=1.25 \
        GTK_THEME=LiveMusicTheme \
        nice -19 ${PipeStart} \
        LiveMusicApp -c 0x0a \
        -v -d 0x220 $PipeOption \
        -f $ButtonSize &
fi

if [ "${1}" == "MidiProcessor" ]; then
    echo "Running Midi Program"

#     $PipeStart qsampler /home/MySongs/Prefs/Sampler/LSPLiveMusic5.lscp &
# 
#     echo "Starting PipeStart "$PipeStart | tee $LogFile
#     nice -15 $PipeStart `QT_STYLE_OVERRIDE=QtCurve ${PipeStart} qsynth -g 1.0  -o  synth.midi-bank-select=mma` >> $LogFile 2>&1 &
#     sfizz --jack  --load piano.sfz 
# 
#    ${PipeStart}
# echo "XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR"
# echo "PIPEWIRE_REMOTE=$PIPEWIRE_REMOTE"
# echo "DBUS_SESSION_BUS_ADDRESS=$DBUS_SESSION_BUS_ADDRESS"

#     systemd-run --user --scope "python3 /home/elias/workspace/LiveMusicApp/Scripts/PCtoSFIZZ.py"
#    python3 /home/elias/workspace/LiveMusicApp/Scripts/StartPCtoSFIZZ.py &

#   /home/elias/workspace/LiveMusicApp/Scripts/Startsfizz.sh & 

exit 0
fi

if [ "${1}" == "EffectsProcessorApp" ]; then
    echo "Running guitarix"
    GDK_DPI_SCALE=2.0 \
        GTK_THEME="LiveMusicTheme" \
        nice -20 ${PipeStart} \
        guitarix --log-terminal 2>&1 >/dev/null &
    exit 0
fi

if [ "${1}" == "CarlaPost" ]; then
    QT_SCREEN_SCALE_FACTORS="1.75" nice -18 ${PipeStart} carla-jack-single /home/MySongs/CarlaConfig/CarlaMixer.carxp &
    exit 0
fi

if [ "${1}" == "CarlaGuitar" ]; then
    #    QT_SCREEN_SCALE_FACTORS=2.0 nice -18 ${PipeStart} carla-jack-multi --cnprefix "LveMus" /home/MySongs/CarlaConfig/NewGuitarix.carxp &
    # --cnprefix "LveMus"
    cp /home/MySongs/CarlaConfig/NewGuitarix.carxp /home/MySongs/CarlaConfig/NewGuitarix1.carxp
    # Remove the auto connection 
    awk '/<Connection>+$/,/<\/Connection>+$/{next}1' /home/MySongs/CarlaConfig/NewGuitarix1.carxp >/home/MySongs/CarlaConfig/NewGuitarix.carxp

#     export PIPEWIRE_AUTOCONNECT=false 
#     export PIPEWIRE_JACK_SERVER_GAIN=10.0
#     QT_SCREEN_SCALE_FACTORS="1.25;1.25;1.25" nice -19 ${PipeStart} /home/Misc/src/Music/LiveMusicBuilds/Carla/source/frontend/carla-jack-multi --cnprefix "Carla" /home/MySongs/CarlaConfig/NewGuitarix.carxp &
    
    export PIPEWIRE_AUTOCONNECT=false 
#    export PIPEWIRE_JACK_SERVER_GAIN=20.0
#    export QT_QPA_PLATFORM=xcb
 #   export QT_SCREEN_SCALE_FACTORS="1.25"
#    LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu/pipewire-0.3/jack/"
nice -19 ${PipeStart} /home/Misc/src/Music/LiveMusicBuilds/Carla/source/frontend/carla-jack-multi --with-libprefix=/usr/local --cnprefix "Carla" /home/MySongs/CarlaConfig/Holdsworth_335_Style.carxp &
    
#    Holdsworth_UD_Style.carxp &

#    Holdsworth_335_Style.carxp
#     QT_SCREEN_SCALE_FACTORS="1.25;1.25;1.25" nice -19 ${PipeStart} /home/Misc/src/Music/LiveMusicBuilds/Carla/source/frontend/carla-jack-multi --cnprefix "Carla" /home/MySongs/CarlaConfig/Holdsworth_Full.carxp &

#    QT_SCREEN_SCALE_FACTORS="1.25;1.25;1.25" nice -19 ${PipeStart} /usr/bin/carla-jack-multi --cnprefix "Carla" /home/MySongs/CarlaConfig/NewGuitarix.carxp &

#     export PIPEWIRE_AUTOCONNECT=false
#     QT_SCREEN_SCALE_FACTORS="1.25;1.25;1.25" nice -18 ${PipeStart} /usr/src/Music/LiveMusicBuilds/Carla/source/frontend/carla --cnprefix "Carla" /home/MySongs/CarlaConfig/NewGuitarix.carxp &
    
    exit 0
fi

if [ "${1}" == "html" ]; then
    gedit "${2}" &
    exit 0
fi

if [ "${1}" == "sng" ]; then
    pw-jack jjazzlab "${2}" &
    exit 0
fi

if [ "${1}" == "desktop" ]; then
    #    gtk-launch "${2}" &

    echo "desktop -> " "${2}" "${2%/*}/"

    #    kioclient5 exec "${2}" &
    kioclient5 exec "${2%/*}/" &
    exit 0
fi

if [ "${1}" == "DrumFile" ]; then
    exit 0
fi

if [ "${1}" == "mp3" ]; then
    # Handle it internal to the App
    #    smplayer "${2}" &
    # PIPEWIRE_PROPS='{ node.autoconnect = false }' smplayer
    exit 1
fi

if [ "${1}" == "mid" ]; then
    # Handle it internal to the App
    /opt/musescore/bin/mscore "${2}" &
    exit 1
fi

if [ "${1}" == "med" ]; then
    ${PipeStart} muse "${2}" &
    exit 0
fi

if [ "${1}" == "Score" ]; then
    ${PipeStart} /opt/musescore/bin/mscore "${2}" &
    exit 0
fi

if [ "${1}" == "MidPlay" ]; then
    ${PipeStart} muse "${2}" &
    exit 0
fi

if [ "${1}" == "rg" ]; then
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

if [ "${1}" == "gp3" ] ||
    [ "${1}" == "gp" ] ||
    [ "${1}" == "gpx" ] ||
    [ "${1}" == "gp4" ] ||
    [ "${1}" == "gp5" ] ||
    [ "${1}" == "gp7" ] ||
    [ "${1}" == "pt2" ] ||
    [ "${1}" == "ptb" ]; then

# QT_STYLE_OVERRIDE=kvantum ${PipeStart} powertabeditor "${2}"
${PipeStart} powertabeditor "${2}"
    exit 0
fi

echo "About to check gp5"

if [ "${1}" == "mscz" ]; then
    echo "Muse Score ${1} ${2} nice -15 ${PipeStart}"

    QT_PLUGIN_PATH="" QT_SCREEN_SCALE_FACTORS="1.5;1.5;1.5" nice -15 ${PipeStart} /opt/musescore/bin/mscore "${2}" &>/dev/null &

    #     QT_PLUGIN_PATH="" QT_SCREEN_SCALE_FACTORS=1.4 nice -15 ${PipeStart} /usr/src/Music/LiveMusicBuilds/MuseScore-Studio-4.3.2.241630832-x86_64.appimage "${2}" &>/dev/null &
    #    /usr/src/LiveMusicBuilds/MuseScore-4.0.0-x86_64.appimage "${2}" &>/dev/null &
    # /usr/src/LiveMusicBuilds/MuseScore-3.6.2.548021370-x86_64.AppImage "${2}" &>/dev/null &
    # Convert from pdf to sheet music
    # magick convert scan_fullcolour1.png -colorspace gray grayscale_page_1.png
    # magick convert scan_fullcolour2.png -colorspace gray grayscale_page-2.png
    # magick grayscale_page_* grayscale.pdf

    # upload grayscale.pdf to https://musescore.com/import
    exit 0
fi

if [ "${1}" == "Looper" ]; then
    echo "Running Looper"
    ss --kill state listening src :9951
    sleep 1
    GDK_DPI_SCALE=1.25 GDK_SCALE=1.25  nice -18 ${PipeStart} slgui -L /home/MySongs/GuitarSound/GuitarSound.slsess -P=9951 --never-timeout &
    # --never-timeout &

    exit 0
fi

if [ "${1}" == "guitarix" ]; then
    echo "Running guitarix"
    GDK_DPI_SCALE=1.5 \
        GTK_THEME="LiveMusicTheme" \
        nice -20 ${PipeStart} \
        guitarix --log-terminal 2>&1 >/dev/null &
    exit 0
fi

if [ "${1}" == "guitarixNew" ]; then
    echo "Running guitarix"
    GDK_DPI_SCALE=1.5 \
        GTK_THEME="LiveMusicTheme" \
        GDK_BACKEND=x11 nice -15 \
        ${PipeStart} guitarix \
        -p 7000 \
        --log-terminal &
    exit 0
fi

# A PDF file.
if [ "${1}" == "xopp" ]; then
    xournalpp "${2}" &
    exit 0
fi

# A PDF file.
if [ "${1}" == "pdf" ]; then
    okular "${2}" &
    exit 0
fi

# A PDF file with a page number offset..
if grep -q "pdf" <<<"$1"; then
    theURL="${3:7}"
    PDFPage="${theURL//#/ --}"
    okular ${PDFPage} &
    exit 0
fi

if [ "${1}" == "connect" ]; then
    GuitarEffects -c
    exit 0mask
fi

if [ "${1}" == "Reindex" ]; then
    ReIndex.sh &
    exit 0
fi

exit 1

# QT_STYLE_OVERRIDE=Fusion
# GTK_THEME=Breeze ./powertabeditor 
# QT_STYLE_OVERRIDE=kvantum powertabeditor
