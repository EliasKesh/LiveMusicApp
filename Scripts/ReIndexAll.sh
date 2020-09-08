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
cd /home/Music/Learn/GuitarMaster
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarPlayback
LiveMusicCharts.py . -a
cd /home/Music/Learn/MBonus
LiveMusicCharts.py . -a


cd /home/Music/Learn/JamTrackCentral/ah_20TastyDominantLicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/ah_A7b5Licks
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/ah_customfusion1
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/ah_customfusion2
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/ah_customfusion3
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/ks_ultimateguitarworkout
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/LatinLicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/lm_legatomasterclass_intermediate
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/dg_20outsidelicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/lm_legatomasterclass_intermediate
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/dg_20outsidelicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/gg_ford
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/gg_funkfusion
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/ii-VLicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/RG_20NeoSoulFusionLicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/RockLicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/TomQuayle
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/WestCoastLicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/JamTrackCentral/
LiveMusicCharts.py . -a


cd /home/Music/Learn/GuitarLab/Altered
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/Diminished
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/Dorian
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/FunkFusionBack
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/FunkFusionLicks
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/JohnScofield
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/LydianDominant
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/Mixolydian
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/Phrasing
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/Robben_Ford
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/ScottHenderson
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/Spain_20_Fusion_Guitar_Licks
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab/The_Chicken_20_Licks_Part1
LiveMusicCharts.py . -a
cd /home/Music/Learn/GuitarLab
LiveMusicCharts.py . -a



cd /home/Music/Learn/PeopleLearn
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



