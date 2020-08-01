#! /bin/sh
#---------------------------------------------------------------------
#
#	File: 	ReIndexAll
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sat 18 Jul 2020 06:11:53 PM PDT
#
#	Copyright ©: 	2020 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

cd /home/Music/
chown elias: Learn  -R
chmod 700 Learn

cd /home/Music/Learn/
find ./ -type d -exec chmod 700 {} \;
find ./ -type f -exec chmod 600 {} \;

cd /home/Music/Learn/50RockLicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/FlatFiv
LiveMusicCharts.py . -a
cd /home/Music/Learn/FusionPractice
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarMaster
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarPlayback
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral
LiveMusicCharts.py . -a
cd /home/Music/Learn/MBonus
LiveMusicCharts.py . -a

# cd /home/Music/Practice
# LiveMusicCharts.py . -a

cd /home/elias/
chown elias: MySongs  -R
chmod 700 MySongs

cd /home/elias/MySongs
find ./ -type d -exec chmod 700 {} \;
find ./ -type f -exec chmod 600 {} \;

cd /home/elias/MySongs/Solo
LiveMusicCharts.py . -a

cd /home/elias/MySongs/ExtraSongs
LiveMusicCharts.py . -a

cd /home/elias/MySongs/TOC
LiveMusicCharts.py . -a

cd /home/elias/MySongs/Tony
LiveMusicCharts.py . -a

cd /home/elias/MySongs/PoblanoCharts
LiveMusicCharts.py . -a

cd /home/elias/MySongs/NewScores
LiveMusicCharts.py . -a

cd /home/elias/MySongs/FusionBlue
LiveMusicCharts.py . -a



