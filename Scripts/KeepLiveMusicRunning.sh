#!/bin/bash
#------------------------------------------------
#
#	File:		KeepLiveAppRunning.sh
#
#	Contains:	Keeps LiveMusicApp and Carla running (relaunches
#			them if they quit), and keeps a handful of desktop
#			apps (Java, MPlayer, MuseScore, sfizz) routed into
#			the Carla mixer.
#
#	Written By:	Elias Keshishoglou on Thu Jun 25 20:49:03 PDT 2009
#
#	Copyright:	2015 Elias Keshishoglou
#
#------------------------------------------------#

#=======================================================================
# CONFIGURATION - all connection points live here
#=======================================================================

# --- Processes / launcher ---------------------------------------------
AppLiveMusic="LiveMusicApp"
AppCarlaProc="Carla"
LaunchScript="MusicApps.sh"
CarlaGuitarProfile="CarlaGuitar"
EffectsCli="GuitarEffects"

# --- Carla mixer prefix (same convention as EffectsConnect.sh) --------
PreFixMix="Carla.0/"
FxMixer="LSP Mixer x4 Stereo"

# --- Headset/interface match strings, in fallback priority order ------
# (whichever of these is actually connected becomes MainOutL/R - see
# FindMainOutput below). Channel suffixes differ per device (most use
# FL/FR, the Scarlett's PipeWire node uses AUX0/AUX1), so each device
# carries its own pair.
MainOutDevices=("Logitech_G935"          "SteelSeries_Arctis_Nova_7"  "Focusrite_Scarlett")
MainOutSuffixL=("playback_FL"            "playback_FL"                "playback_AUX0")
MainOutSuffixR=("playback_FR"            "playback_FR"                "playback_AUX1")

# --- Desktop app source ports (may or may not be running) -------------
AppJava="alsa_playback.java"
AppMPlayer="MPlayer"
AppMscore="mscore"
AppSfizz="sfizz"

# --- Midi-Bridge ports (looked up dynamically - see FindPort) ---------
KwLiveMusicOutFluid=("LiveMusic" "Fluid")
KwRtMidiIn=("RtMidiIn")

# --- Screensaver inhibit ------------------------------------------------
DbusScreensaverService="org.freedesktop.ScreenSaver"
DbusScreensaverPath="/ScreenSaver"

#=======================================================================
# FUNCTIONS
#=======================================================================

#---------------------------------------------------------------------#
#*** FindPort
#---------------------------------------------------------------------#
# FindPort: Resolve a live port name from loose keywords instead of a
# hardcoded literal (same helper as in EffectsConnect.sh - PipeWire
# naming punctuation has changed on us before).
#   $1        direction: "out" = capture/source ports (pw-link -o,
#             cached in $DeviceList) | "in" = playback/sink ports
#             (pw-link -i, cached in $DeviceListI)
#   $2, $3... substrings that must ALL appear in the port name
#             (case-insensitive, ANDed together); first match wins.
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
#*** FindMainOutput
#---------------------------------------------------------------------#
# FindMainOutput: The original script set MainOutL/MainOutR three
# times in a row (G935, then Arctis, then Scarlett) - each assignment
# silently overwrote the last, so only the Scarlett lines ever actually
# took effect, regardless of what was plugged in. This picks whichever
# device in $MainOutDevices is actually present, in priority order.
function FindMainOutput {
    local i dev sufL sufR l r
    for i in "${!MainOutDevices[@]}"; do
        dev="${MainOutDevices[$i]}"
        sufL="${MainOutSuffixL[$i]}"
        sufR="${MainOutSuffixR[$i]}"
        l=$(FindPort in "$dev" "$sufL")
        r=$(FindPort in "$dev" "$sufR")
        if [ -n "$l" ] && [ -n "$r" ]; then
            MainOutL="$l"
            MainOutR="$r"
            return 0
        fi
    done
    echo "FindMainOutput: no known output device found" >&2
    MainOutL=""
    MainOutR=""
    return 1
}

#---------------------------------------------------------------------#
#*** SafeLink / SafeUnlink
#---------------------------------------------------------------------#
# Thin wrappers so call sites don't all repeat ">/dev/null 2>&1" -
# these routes are opportunistic (the source app may not be running).
function SafeLink {
    pw-link "$1" "$2" >/dev/null 2>&1
}

function SafeUnlink {
    pw-link -d "$1" "$2" >/dev/null 2>&1
}

#=======================================================================
# MAIN
#=======================================================================

echo "Waiting"
sleep 1

while true
do
    # Refresh port lists each pass - devices/apps come and go.
    DeviceList="$(pw-link -o)"
    DeviceListI="$(pw-link -i)"
    FindMainOutput    
    
    # --- Keep LiveMusicApp running ---
    pid=$(pgrep -i "$AppLiveMusic")
    if [ -z "$pid" ]; then
        echo "Launching LiveMusic"
        "$LaunchScript" "$AppLiveMusic"
        sleep 3
        "$EffectsCli" -c
        "$EffectsCli" -b
    fi

    # --- Keep Carla running ---
    pid=$(pgrep -i "$AppCarlaProc" | head -n 1)
    if [ -z "$pid" ]; then
        echo "Launching Carla"
        "$LaunchScript" "$CarlaGuitarProfile" &
        sleep 3
        "$EffectsCli" -c
    fi

    # --- (disabled) Keep PCtoSFIZZ.py running ---
    # pid=$(pgrep -fi PCtoSFIZZ.py | head -n 1)
    # if [ -z "$pid" ]; then
    #     echo "Launching PCtoSFIZZ.py"
    #     pw-jack bash -c "exec python3 /home/elias/workspace/LiveMusicApp/Scripts/PCtoSFIZZ.py"
    # fi
    sleep 5
    qdbus "$DbusScreensaverService" "$DbusScreensaverPath" SimulateUserActivity

# FluidSynth:output_1
# FluidSynth:output_2
    # --- Java audio -> Carla mixer input 2 ---
    PwAppJava=$(FindPort out "FluidSynth")
    PwAppJavaBase=${PwAppJava%:*}
    SafeUnlink "${PwAppJavaBase}:output_1" "$MainOutL"
    SafeUnlink "${PwAppJavaBase}:output_2" "$MainOutR"
    SafeLink   "${PwAppJavaBase}:output_1" "${PreFixMix}${FxMixer}:Audio input left 3"
    SafeLink   "${PwAppJavaBase}:output_2" "${PreFixMix}${FxMixer}:Audio input right 3"
    
    # --- Java audio -> Carla mixer input 2 ---
    PwAppJava=$(FindPort out "$AppJava")
    PwAppJavaBase=${PwAppJava%:*}
    SafeUnlink "${PwAppJavaBase}:output_FL" "$MainOutL"
    SafeUnlink "${PwAppJavaBase}:output_FR" "$MainOutR"
    SafeLink   "${PwAppJavaBase}:output_FL" "${PreFixMix}${FxMixer}:Audio input left 2"
    SafeLink   "${PwAppJavaBase}:output_FR" "${PreFixMix}${FxMixer}:Audio input right 2"

    # --- MPlayer -> Carla mixer input 3 ---
    PwAppMPlayer=$(FindPort out "$AppMPlayer")
    PwAppMPlayerBase=${PwAppMPlayer%:*}
    SafeUnlink "${PwAppMPlayerBase}:output_FL" "$MainOutL"
    SafeUnlink "${PwAppMPlayerBase}:output_FR" "$MainOutR"
    SafeLink   "${PwAppMPlayerBase}:output_FL" "${PreFixMix}${FxMixer}:Audio input left 3"
    SafeLink   "${PwAppMPlayerBase}:output_FR" "${PreFixMix}${FxMixer}:Audio input right 3"

    # --- MuseScore -> Carla mixer input 3 ---
	PwAppMscore=$(FindPort out "mscore")
    PwAppMscoreBase=${PwAppMscore%:*}
    SafeUnlink "${PwAppMscoreBase}:output_FL" "$MainOutL"
    SafeUnlink "${PwAppMscoreBase}:output_FR" "$MainOutR"
    SafeLink   "${PwAppMscoreBase}:output_FL" "${PreFixMix}${FxMixer}:Audio input left 3"
    SafeLink   "${PwAppMscoreBase}:output_FR" "${PreFixMix}${FxMixer}:Audio input right 3"

    # --- sfizz <-> Carla mixer / MIDI Guitar 2 ---
    timeout 1 pw-link "${AppSfizz}:output_1" "${PreFixMix}${FxMixer}:Audio input left 2" >/dev/null 2>&1; true
    timeout 1 pw-link "${AppSfizz}:output_2" "${PreFixMix}${FxMixer}:Audio input right 2" >/dev/null 2>&1; true
    timeout 1 pw-link "${PreFixMix}MIDI Guitar 2:event-out" "${AppSfizz}:input" >/dev/null 2>&1; true

    # --- LiveMusic Fluid output -> RtMidiIn (looked up, not literal) ---
    PortLiveMusicFluid=$(FindPort out "${KwLiveMusicOutFluid[@]}")
    PortRtMidiIn=$(FindPort in "${KwRtMidiIn[@]}")
    SafeLink "$PortLiveMusicFluid" "$PortRtMidiIn"
done
