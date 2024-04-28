#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	ReIndex
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sat Mar 13 10:47:29 AM PST 2021
#
#	Copyright : 	2021 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
rm ~/MP3Errors.txt

set -x 
cd /home/MySongs/LaidBack
LiveMusicCharts.py . -a

cd /home/MySongs/Charts
LiveMusicCharts.py . -a

cd /home/MySongs/Solo
LiveMusicCharts.py . -a

cd /home/MySongs/OtherTunes
LiveMusicCharts.py . -a

cd /home/MySongs/NewScores
LiveMusicCharts.py . -a

cd /home/MySongs/Alternates
LiveMusicCharts.py . -a

cd /home/MySongs/TOC
LiveMusicCharts.py . -a

cd /home/MySongs/GregChambers
LiveMusicCharts.py . -a

cd /home/MySongs/Reference
LiveMusicCharts.py . -ig

for dir in $(find /home/MySongs/Reference -depth -type d )
do
    cd $dir
    # check for .mscbackup
    echo "Dir  "$dir
    # Create Index Convert GPx to Muse
    LiveMusicCharts.py . -ig
done


echo "*** MP3 Files with Errors ***"
cat ~/MP3Errors.txt

cd /home/
chown elias: MySongs  -R
chmod 700 MySongs -R
