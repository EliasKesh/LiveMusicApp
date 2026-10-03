#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	MidiLoopforSFIZZ
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Fri 2 Oct 17:27:46 EEST 2026
#
#	Copyright : 	2026 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

( f=/home/Music/GuitarSoundTest/GuitarMidi1.mid
  trap 'kill $pid 2>/dev/null; exit' INT TERM
  while true; do
    pw-cat -p -m -M midi "$f" & pid=$!
    until pw-link -o | grep -qx 'pw-cat:output_1'; do
      kill -0 $pid 2>/dev/null || { echo "pw-cat exited early"; exit 1; }
      sleep 0.05
    done
    pw-link pw-cat:output_1 sfizz:input
    # optional monitor: pw-link pw-cat:output_1 "Midi-Bridge:Protokol: (playback)"
    wait $pid
    sleep 0.2          # let the old pw-cat port disappear before the next round
  done )

