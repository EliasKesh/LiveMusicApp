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
BaseDir=/home/Music/Learn
cd $BaseDir

find $BaseDir -iname \*.mscbackup -exec rm {} -rf \;
find $BaseDir -iname \*html\#\* -exec rm {} \;

# Rename pdf Files back to pdf
# find ./ -name \*.conv -print0 | xargs -0 rename 's/.conv$//'
# find ./ -iname \*.pdf.jpg -exec rm {} \;

#find ./ -iname \*mp3 -exec normalize-mp3 -b {} \;
# find ./ -iname \*.gp? -exec mscore3 {} -o {}".mscz" \;

for dir in $(find /home/Music/Learn/Courses -type d )
do
    cd $dir
    # check for .mscbackup
    echo "Level "$Level"  "$dir
    LiveMusicCharts.py . -ig
done

for dir in $(find /home/Music/Learn/Reading -type d )
do
    cd $dir
    # check for .mscbackup
    echo "Level "$Level"  "$dir
    LiveMusicCharts.py . -ig
done

exit 

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
