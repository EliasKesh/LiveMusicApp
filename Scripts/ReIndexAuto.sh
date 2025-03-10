#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	ReIndexAuto
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Tue Dec 21 10:38:14 PM PST 2021
#
#	Copyright : 	2021 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
# sudo pip3 install PyPDF2
# sudo pip3 install wand
#
# sudo pip3 install PyQt5
# sudo pip3 install qtpy
# sudo pip3 install setuptools
# sudo pip3 install cython
# sudo pip3 install mido
# build pyliblo
# sudo pip3 install python-rtmidi



BaseDir=/home/Music/Learn
cd $BaseDir

find $BaseDir -iname \*.mscbackup -exec rm {} -rf \;
find $BaseDir -iname \*html\#\* -exec rm {} \;

# Change Muse extension


# alias current_keyboard_layout_chart=$'gkbd-keyboard-display -l $(localectl status | awk \'/X11 Layout/ {print $3}\')$\'\t\'$(localectl status | awk \'/X11 Variant/ {print $3}\')'

# 
# Rename pdf Files back to pdf

# find ./ -name \*.conv -print0 | xargs -0 rename 's/.conv$//'
# find ./ -iname \*.pdf.jpg -exec rm {} \;

# find . -regextype posix-extended -regex '.*\.(gp.?)$'
# find . -regextype posix-extended -regex '.*\.(gp|gpx)$'
# libreoffice --headless --convert-to pdf *.rtf
# find ./ -iname \*wma -exec ffmpeg -i {} {}.mp3 \;
# find ./ -iname \*mp4 -exec ffmpeg -i {} {}.mp3 \;

# find ./ -iname \*mp3 -exec mp3gain {} \;
# find . -name "*.gp5.mscz" -exec rename -f  's/\.gp5.mscz$/.mscz/' '{}' +
# find ./ -iname \*.gp? -exec mscore -f {} -S EliasTab -o {}".mscz" \;

# append Prefix
# for file in *; do mv "$file" "F24$file"; done;

# Convert to MScore
# find /home/Music/Learn  -iname \*mp3 -iname \*backing\* -exec ln -s {} /home/Music/BackingTracks/CourseBacking/ \;

# rename -v -f 's/Holdsworth-//g;' *

# pdftoppm UserGuide-JazzFusionPremium4VideoGuitarLesson.pdf UserGuide-JazzFusionPremium4VideoGuitarLesson.png  -png
# find ./ -iname \*.pdf -exec pdftoppm -png {} {}".png" \;
# find ./ -iname \*.pdf -exec pdftoppm -jpeg {} {}".pdf.jpg" \;
# find ./ -iname \*.pdf -exec mv {} {}".conv" \;

for dir in $(find /home/Music/Learn/Courses -depth -type d )
do
    cd $dir
    echo "Dir  "$dir
    # Create Index Convert GPx to Muse
    LiveMusicCharts.py . -ig
done

for dir in $(find /home/Music/Learn/Reading -depth -type d )
do
    cd $dir
    echo "Dir  "$dir
    # Create Index Convert GPx to Muse
    LiveMusicCharts.py . -ig
done

for dir in $(find /home/Music/Learn/MuseLearn -depth -type d )
do
    cd $dir
    echo "Dir  "$dir
    # Create Index Convert GPx to Muse
    LiveMusicCharts.py . -ig
done

for dir in $(find /home/Music/Learn/Transcriptions -depth -type d )
do
    cd $dir
    echo "Dir  "$dir
    # Create Index Convert GPx to Muse
    LiveMusicCharts.py . -ig
done

for dir in $(find /home/Music/BackingTracks -depth -type d )
do
    cd $dir
    echo "Dir  "$dir
    # Create Index Convert GPx to Muse
    LiveMusicCharts.py . -ig
done



# Generate the Directory listings.
cd /home/Music/BackingTracks
# LiveMusicCharts.py . -ivg
DirectoryToHtml.py

cd /home/Music/MidiDrumLoops
LiveMusicCharts.py . -vg
DirectoryToHtml.py

cd /home/Music/Practice
LiveMusicCharts.py . -ivg
DirectoryToHtml.py

# find ./ -type d -exec chmod 700 {} \;
# find ./ -type f -exec chmod 600 {} \;

cd $BaseDir
chown elias: *  -R
chmod 700 *

echo "*** MP3 Files with Errors ***"
cat ~/MP3Errors.txt
exit 

# =================================================================
# Normalize the audio output
# find ./ -iname \*mp3 -exec normalize-mp3 -b {} \;

# Mix seperate MP3 files into one
# ffmpeg -i mtjpsf-020-drums.mp3 -i mtjpsf-020-bass.mp3 -i mtjpsf-020-rg.mp3 -filter_complex amerge=inputs=3 -ac 2 Output.mp3


# find ./ -iname \*.pdf.jpg -exec rm {} \;
# find ./ -name \*.conv -print0 | xargs -0 rename 's/.conv$//'
#


Level=1
for Level in {1..6}
do
    echo " ---- Level "$Level" ----"
    for dir in $(find /home/Music/Learn -mindepth $Level -maxdepth $Level  -type d )
    do
        cd $dir
        # check for .mscbackup
        echo "Level "$Level"  "$dir
        LiveMusicCharts.py . -vfiz
    done
done

exit
