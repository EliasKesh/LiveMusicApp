#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	RenameGiga
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Fri Dec 26 10:58:40 EET 2025
#
#	Copyright : 	2025 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
#!/bin/bash


# GM500 Names
gm_names=(
    "Acoustic Grand Piano"  "Bright Acoustic Piano"  "Electric Grand Piano" 
    "Honky-tonk Piano"  "Electric Piano 1"  "Electric Piano 2" 
    "Harpsichord"  "Clavinet"  "Celesta"  "Glockenspiel" 
    "Music Box"  "Vibraphone"  "Marimba"  "Xylophone" 
    "Tubular Bells"  "Dulcimer"  "Drawbar Organ"  "Percussive Organ" 
    "Rock Organ"  "Church Organ"  "Reed Organ"  "Accordion" 
    "Harmonica"  "Tango Accordion"  "Acoustic Guitar (nylon)" 
    "Acoustic Guitar (steel)"  "Electric Guitar (jazz)"  "Electric Guitar (clean)" 
    "Electric Guitar (muted)"  "Overdriven Guitar"  "Distortion Guitar" 
    "Guitar Harmonics"  "Acoustic Bass"  "Electric Bass (finger)" 
    "Electric Bass (pick)"  "Fretless Bass"  "Slap Bass 1"  "Slap Bass 2" 
    "Synth Bass 1"  "Synth Bass 2"  "Violin"  "Viola"  "Cello" 
    "Contrabass"  "Tremolo Strings"  "Pizzicato Strings"  "Orchestral Harp" 
    "Timpani"  "String Ensemble 1"  "String Ensemble 2"  "Synth Strings 1" 
    "Synth Strings 2"  "Choir Aahs"  "Voice Oohs"  "Synth Voice" 
    "Orchestra Hit"  "Trumpet"  "Trombone"  "Tuba"  "Muted Trumpet" 
    "French Horn"  "Brass Section"  "Synth Brass 1"  "Synth Brass 2" 
    "Soprano Sax"  "Alto Sax"  "Tenor Sax"  "Baritone Sax"  "Oboe" 
    "English Horn"  "Bassoon"  "Clarinet"  "Piccolo"  "Flute" 
    "Recorder"  "Pan Flute"  "Blown Bottle"  "Shakuhachi"  "Whistle" 
    "Ocarina"  "Lead 1 (square)"  "Lead 2 (sawtooth)"  "Lead 3 (calliope)" 
    "Lead 4 (chiff)"  "Lead 5 (charang)"  "Lead 6 (voice)" 
    "Lead 7 (fifths)"  "Lead 8 (bass + lead)"  "Pad 1 (new age)" 
    "Pad 2 (warm)"  "Pad 3 (polysynth)"  "Pad 4 (choir)"  "Pad 5 (bowed)" 
    "Pad 6 (metallic)"  "Pad 7 (halo)"  "Pad 8 (sweep)"  "FX 1 (rain)" 
    "FX 2 (soundtrack)"  "FX 3 (crystal)"  "FX 4 (atmosphere)" 
    "FX 5 (brightness)"  "FX 6 (goblins)"  "FX 7 (echoes)"  "FX 8 (sci-fi)" 
    "Sitar"  "Banjo"  "Shamisen"  "Koto"  "Kalimba"  "Bag pipe" 
    "Fiddle"  "Shanai"  "Tinkle Bell"  "Agogo"  "Steel Drums" 
    "Woodblock"  "Taiko Drum"  "Melodic Tom"  "Synth Drum" 
    "Reverse Cymbal"  "Guitar Fret Noise"  "Breath Noise"  "Seashore" 
    "Bird Tweet"  "Telephone Ring"  "Helicopter"  "Applause"  "Gunshot"
)


# Array of General MIDI instrument names (0-127)
gm_names=(
"Acoustic Grand Piano" "Bright Acoustic Piano" "Electric Grand Piano" "Honky-tonk Piano"
"Electric Piano 1" "Electric Piano 2" "Harpsichord" "Clavinet"
"Celesta" "Glockenspiel" "Music Box" "Vibraphone"
"Marimba" "Xylophone" "Tubular Bells" "Dulcimer"
"Drawbar Organ" "Percussive Organ" "Rock Organ" "Church Organ"
"Reed Organ" "Accordion" "Harmonica" "Tango Accordion"
"Acoustic Guitar (nylon)" "Acoustic Guitar (steel)" "Electric Guitar (jazz)" "Electric Guitar (clean)"
"Electric Guitar (muted)" "Overdriven Guitar" "Distortion Guitar" "Guitar Harmonics"
"Acoustic Bass" "Electric Bass (finger)" "Electric Bass (pick)" "Fretless Bass"
"Slap Bass 1" "Slap Bass 2" "Synth Bass 1" "Synth Bass 2"
"Violin" "Viola" "Cello" "Contrabass"
"Tremolo Strings" "Pizzicato Strings" "Orchestral Harp" "Timpani"
"String Ensemble 1" "String Ensemble 2" "SynthStrings 1" "SynthStrings 2"
"Choir Aahs" "Voice Oohs" "Synth Voice" "Orchestra Hit"
"Trumpet" "Trombone" "Tuba" "Muted Trumpet"
"French Horn" "Brass Section" "SynthBrass 1" "SynthBrass 2"
"Soprano Sax" "Alto Sax" "Tenor Sax" "Baritone Sax"
"Oboe" "English Horn" "Bassoon" "Clarinet"
"Piccolo" "Flute" "Recorder" "Pan Flute"
"Blown Bottle" "Shakuhachi" "Whistle" "Ocarina"
"Lead 1 (square)" "Lead 2 (sawtooth)" "Lead 3 (calliope)" "Lead 4 (chiff)"
"Lead 5 (charang)" "Lead 6 (voice)" "Lead 7 (fifths)" "Lead 8 (bass + lead)"
"Pad 1 (new age)" "Pad 2 (warm)" "Pad 3 (polysynth)" "Pad 4 (choir)"
"Pad 5 (bowed)" "Pad 6 (metallic)" "Pad 7 (halo)" "Pad 8 (sweep)"
"FX 1 (rain)" "FX 2 (soundtrack)" "FX 3 (crystal)" "FX 4 (atmosphere)"
"FX 5 (brightness)" "FX 6 (goblins)" "FX 7 (echoes)" "FX 8 (sci-fi)"
"Sitar" "Banjo" "Shamisen" "Koto"
"Kalimba" "Bagpipe" "Fiddle" "Shanai"
"Tinkle Bell" "Agogo" "Steel Drums" "Woodblock"
"Taiko Drum" "Melodic Tom" "Synth Drum" "Reverse Cymbal"
"Guitar Fret Noise" "Breath Noise" "Seashore" "Bird Tweet"
"Telephone Ring" "Helicopter" "Applause" "Gunshot"
)


# Loop over decimal numbers, format with leading zeros in the filename
for i in $(seq 0 128); do
    old_file=$(printf "%03d.gig" "$(($i+1))")  # formats 0 → 000.gig, 8 → 008.gig, 128 → 128.gig

    if [ ! -f "$old_file" ]; then
        echo "Warning: $old_file not found!"
        continue
    fi

    if [ "$i" -lt 128 ]; then
        name="${gm_names[$i]}"
        Sub=$(printf "%03d" "$i")
        safe_name="${name// /_}.gig"
        mv "$old_file" $Sub"_$safe_name"
        echo "Renamed $old_file -> $safe_name"
    else
        echo "Skipping $old_file (no GM name for 128)"
    fi
done

exit

# Check that exactly 129 files exist (000.gig to 128.gig)
for i in $(seq -w 0 128); do
    old_file="$i.gig"
    if [ ! -f "$old_file" ]; then
        echo "Warning: $old_file not found!"
        continue
    fi

    # Only rename if i < 128 (because GM instruments are 0-127)
    if [ "$i" -lt 128 ]; then
        name="${gm_names[$i]}"
        # Replace spaces with underscores for filename
        safe_name="${name// /_}.gig"
        mv "$old_file" "$safe_name"
        echo "Renamed $old_file -> $safe_name"
    else
        echo "Skipping $old_file (no GM name for 128)"
    fi
done


