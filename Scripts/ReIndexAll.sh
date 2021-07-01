#! /bin/sh
#------------------------------------------------
#
#   File:   ReIndexAll
#
#   Contains:
#
#
#   Written By:     Elias Keshishoglou on Sat 18 Jul 2020 06:11:53 PM PDT
#
#   Copyright ©:    2020 Elias Keshishoglou all rights reserved.
#
#
#------------------------------------------------#


ReIndex.sh

cd /home/Music/
chown elias: Learn  -R
chmod 700 Learn

cd /home/Music/Learn/
find ./ -type d -exec chmod 700 {} \;
find ./ -type f -exec chmod 600 {} \;

cd /home/Music/EliasOriginals
LiveMusicCharts.py . -vci

cd /home/Music/Learn/JamTrackCentral/
echo `pwd`
LiveMusicCharts.py ah_20TastyDominantLicks -vcfp
LiveMusicCharts.py ah_A7b5Licks -vcfp
LiveMusicCharts.py ah_customfusion1 -vcfp
LiveMusicCharts.py ah_customfusion2 -vcfp
LiveMusicCharts.py ah_customfusion3 -vcfp
LiveMusicCharts.py ah_bluesextensions -vcfp
LiveMusicCharts.py ks_ultimateguitarworkout -vcfp
LiveMusicCharts.py LatinLicks -vcfp
LiveMusicCharts.py lm_legatomasterclass_intermediate -vcfp
LiveMusicCharts.py dg_20outsidelicks -vcfp
LiveMusicCharts.py gg_ford -vcfp
LiveMusicCharts.py gg_funkfusion -vcfp
LiveMusicCharts.py ii-VLicks -vcfp
LiveMusicCharts.py RG_20NeoSoulFusionLicks -vcfp
LiveMusicCharts.py RockLicks -vcfp
LiveMusicCharts.py TomQuayle -vcfp
LiveMusicCharts.py WestCoastLicks -vcfp
LiveMusicCharts.py JJ_WarmUpJamtracks -vcfp
LiveMusicCharts.py ah_sophisticatedblues -vcfp

LiveMusicCharts.py . -il

cd /home/Music/Learn/GuitarLab/
echo `pwd`
LiveMusicCharts.py 20_Sax_Licks_For_Guitar_ -vfc
LiveMusicCharts.py Altered -vfc
LiveMusicCharts.py Diminished -vfc
LiveMusicCharts.py Dorian -vfc
LiveMusicCharts.py 20BluesFusionLicks -vfc
LiveMusicCharts.py FunkFusionBack -vfc
LiveMusicCharts.py FunkFusionLicks -vfc
LiveMusicCharts.py JohnScofield -vfc
LiveMusicCharts.py LydianDominant -vfc
LiveMusicCharts.py Mixolydian -vfc
LiveMusicCharts.py Phrasing -vfc
LiveMusicCharts.py Robben_Ford -vfc
LiveMusicCharts.py ScottHenderson -vfc
LiveMusicCharts.py Spain_20_Fusion_Guitar_Licks -vfc
LiveMusicCharts.py The_Chicken_20_Licks_Part1 -vfc
LiveMusicCharts.py . -il


cd /home/Music/Learn/PeopleLearn/
echo `pwd`
LiveMusicCharts.py AlDiMeola -vcfp
LiveMusicCharts.py Carlton -vcfp
LiveMusicCharts.py DavidLeeRoth -vcfp
LiveMusicCharts.py FrankGambale -vcfp
LiveMusicCharts.py Holdsworth -vcfp
LiveMusicCharts.py mingus_examples -vcfp
LiveMusicCharts.py SteveMorse -vcfp
LiveMusicCharts.py . -il

cd /home/Music/Learn/FlatFiv
echo `pwd`
LiveMusicCharts.py AndroidTab -vcfp
LiveMusicCharts.py FusionSolos -vcfp
LiveMusicCharts.py Tapping -vcfp
LiveMusicCharts.py . -il


cd /home/Music/Learn/GuitarMaster
echo `pwd`
LiveMusicCharts.py GuitarMasterTapping -vcfp
LiveMusicCharts.py JazzFusionLicks -vcfp
LiveMusicCharts.py Sweep -vcfp
LiveMusicCharts.py . -il

cd /home/Music/Learn/CoffeeBreakGroove/
echo `pwd`
LiveMusicCharts.py JazzFusionPremium4 -vcfp
LiveMusicCharts.py . -il


cd /home/Music/Learn/GuitarPlayback
echo `pwd`
LiveMusicCharts.py 30OutsideDorian -vcfp
LiveMusicCharts.py GPB_Backing_Track_Sampler -vcfp
LiveMusicCharts.py AeolianIslandGroove -vcfp
LiveMusicCharts.py AcidFusionModulation -vcfp
LiveMusicCharts.py JazzFusionModulation -vcfp
LiveMusicCharts.py JazzRockAmbientPads -vcfp
LiveMusicCharts.py JazzRockFusionGroove -vcfp
LiveMusicCharts.py DorianLaidBackFeel -vcfp
LiveMusicCharts.py FunkInB -vcfp
LiveMusicCharts.py FunkyMinorModes -vcfp
LiveMusicCharts.py FusionFunkMaster -vcfp
LiveMusicCharts.py FusionPads -vcfp
LiveMusicCharts.py JazzRockPhrasing -vcfp
LiveMusicCharts.py MellowFuse -vcfp
LiveMusicCharts.py PersonalCourse -vcfp
LiveMusicCharts.py SoloInMultipleKeys -vcfp
LiveMusicCharts.py FusionScales -vcfp
LiveMusicCharts.py FusionPads -vcfp
LiveMusicCharts.py . -il

cd /home/Music/Learn/MBonus
echo `pwd`
LiveMusicCharts.py Adv_StringSkipping -vcfp
LiveMusicCharts.py GuitarChords -vcfp
LiveMusicCharts.py BasicJazzChords -vcfp
LiveMusicCharts.py FingerstyleGuitar -vcfp
LiveMusicCharts.py HardRockTapping -vcfp
LiveMusicCharts.py StringSkipping -vcfp
LiveMusicCharts.py SweepPicking -vcfp
LiveMusicCharts.py . -il

cd /home/Music/Learn/DLP
echo `pwd`
LiveMusicCharts.py Example -vcfp
LiveMusicCharts.py Licks -vcfp
LiveMusicCharts.py Sample -vcfp
LiveMusicCharts.py . -il

cd /home/Music/Learn/50RockLicks
echo `pwd`
LiveMusicCharts.py BonusMaterial -vcfp
LiveMusicCharts.py JamTracks -vcfp
LiveMusicCharts.py PowerTabFiles -vcfp
LiveMusicCharts.py . -il

cd /home/Music/Learn/
echo `pwd`
# LiveMusicCharts.py RickBeato -vcf
LiveMusicCharts.py Tapping -vcfp
LiveMusicCharts.py FusionPractice -vcfp
#  LiveMusicCharts.py JazzGuitar -vcfp
LiveMusicCharts.py PentaTrick -vcfp
# LiveMusicCharts.py Material -vcfp
LiveMusicCharts.py Material -il


cd /home/Music/Learn/
echo `pwd`
LiveMusicCharts.py . -il

#cd /home/Music/BackingTracks

cd /home/Music/Practice
LiveMusicCharts.py . -a

echo "*** MP3 Files with Errors ***"
cat ~/MP3Errors.txt

# find ./ -iname \*mp3 -exec normalize-mp3 -b {} \;

