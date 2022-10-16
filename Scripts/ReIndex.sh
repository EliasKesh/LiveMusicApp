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
cd /home/MySongs/GregChambers
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

echo "*** MP3 Files with Errors ***"
cat ~/MP3Errors.txt

cd /home/
chown elias: MySongs  -R
chmod 700 MySongs -R
