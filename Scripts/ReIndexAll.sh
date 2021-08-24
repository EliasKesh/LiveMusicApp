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
set -x

cd /home/Music/
chown elias: Learn  -R
chmod 700 Learn

BaseDir=/home/Music/Learn/
cd $BaseDir

find ./ -type d -exec chmod 700 {} \;
find ./ -type f -exec chmod 600 {} \;

cd /home/Music/EliasOriginals
LiveMusicCharts.py . -vci

CurDir=$BaseDir/TrueFire/
cd $CurDir
echo `pwd`
LiveMusicCharts.py 10_Guitar_Licks_You_Must_Know -vgcfp
LiveMusicCharts.py Smart_Practice_For_Guitar -vcfgp
LiveMusicCharts.py . -il

CurDir=$BaseDir/JamTrackCentral/
cd $CurDir
echo `pwd`
cd $CurDir/ah_20TastyDominantLicks
LiveMusicCharts.py . -vcfir
cd $CurDir/ah_A7b5Licks
LiveMusicCharts.py . -vcfir
cd $CurDir/ah_customfusion1
LiveMusicCharts.py . -vcfir
cd $CurDir/ah_customfusion2
LiveMusicCharts.py . -vcfir
cd $CurDir/ah_customfusion3
LiveMusicCharts.py . -vcfir
cd $CurDir/ah_bluesextensions
LiveMusicCharts.py . -vcfir
cd $CurDir/ks_ultimateguitarworkout
LiveMusicCharts.py . -vcfir
cd $CurDir/LatinLicks
LiveMusicCharts.py . -vcfir
cd $CurDir/lm_legatomasterclass_intermediate
LiveMusicCharts.py . -vcfir
cd $CurDir/dg_20outsidelicks
LiveMusicCharts.py . -vcfir
cd $CurDir/gg_ford
LiveMusicCharts.py . -vcfir
cd $CurDir/gg_funkfusion
LiveMusicCharts.py . -vcfir
cd $CurDir/ii-VLicks
LiveMusicCharts.py . -vcfir
cd $CurDir/RG_20NeoSoulFusionLicks
LiveMusicCharts.py . -vcfir
cd $CurDir/RockLicks
LiveMusicCharts.py . -vcfir
cd $CurDir/TomQuayle
LiveMusicCharts.py . -vcfir
cd $CurDir/WestCoastLicks
LiveMusicCharts.py . -vcfir
cd $CurDir/JJ_WarmUpJamtracks
LiveMusicCharts.py . -vcfir
cd $CurDir/ah_sophisticatedblues
LiveMusicCharts.py . -vcfir
cd $CurDir/MO_Electric_Fusion_Soloing
LiveMusicCharts.py . -vcfir
cd $CurDir/MM_20LicksNLFingerstyle
LiveMusicCharts.py . -vcfir
cd $CurDir
LiveMusicCharts.py . -il

CurDir=$BaseDir/GuitarLab/
cd $CurDir
echo `pwd`
cd $CurDir/20_Sax_Licks_For_Guitar
LiveMusicCharts.py . -vcfir
cd $CurDir/Altered
LiveMusicCharts.py . -vcfir
cd $CurDir/Diminished
LiveMusicCharts.py . -vcfir
cd $CurDir/Dorian
LiveMusicCharts.py . -vcfir
cd $CurDir/20BluesFusionLicks
LiveMusicCharts.py . -vcfir
cd $CurDir/FunkFusionBack
LiveMusicCharts.py . -vcfir
cd $CurDir/FunkFusionLicks
LiveMusicCharts.py . -vcfir
cd $CurDir/JohnScofield
LiveMusicCharts.py . -vcfir
cd $CurDir/LydianDominant
LiveMusicCharts.py . -vcfir
cd $CurDir/Mixolydian
LiveMusicCharts.py . -vcfir
cd $CurDir/Phrasing
LiveMusicCharts.py . -vcfir
cd $CurDir/Robben_Ford
LiveMusicCharts.py . -vcfir
cd $CurDir/ScottHenderson
LiveMusicCharts.py . -vcfir
cd $CurDir/Spain_20_Fusion_Guitar_Licks
LiveMusicCharts.py . -vcfir
cd $CurDir/The_Chicken_20_Licks_Part1
LiveMusicCharts.py . -vcfir
cd $CurDir
LiveMusicCharts.py . -il

CurDir=$BaseDir/PeopleLearn/
cd $CurDir
echo `pwd`
cd $CurDir/AlDiMeola
LiveMusicCharts.py . -vcfir
cd $CurDir/Carlton
LiveMusicCharts.py . -vcfir
cd $CurDir/DavidLeeRoth
LiveMusicCharts.py . -vcfir
cd $CurDir/FrankGambale
LiveMusicCharts.py . -vcfir
cd $CurDir/Holdsworth
LiveMusicCharts.py . -vcfir
cd $CurDir/mingus_examples
LiveMusicCharts.py . -vcfir
cd $CurDir/SteveMorse
LiveMusicCharts.py . -vcfir
cd $CurDir
LiveMusicCharts.py . -il

CurDir=$BaseDir/FlatFiv/
cd $CurDir
echo `pwd`
cd $CurDir/AndroidTab
LiveMusicCharts.py . -vcfir
cd $CurDir/FusionSolos
LiveMusicCharts.py . -vcfir
cd $CurDir/Tapping
LiveMusicCharts.py . -vcfir
cd $CurDir
LiveMusicCharts.py . -il

CurDir=$BaseDir/GuitarMaster/
cd $CurDir
echo `pwd`
cd $CurDir/GuitarMasterTapping
LiveMusicCharts.py . -vcfir
cd $CurDir/JazzFusionLicks
LiveMusicCharts.py . -vcfir
cd $CurDir/Sweep
LiveMusicCharts.py . -vcfir
cd $CurDir/CoolLegatoPhrasesTranscriptionFiles
LiveMusicCharts.py . -vcfir
cd $CurDir/DarrylPentatonics
LiveMusicCharts.py . -vcfir
cd $CurDir/FITDemoVersion
LiveMusicCharts.py . -vcfir
cd $CurDir
LiveMusicCharts.py . -il

CurDir=$BaseDir/CoffeeBreakGroove/
cd $CurDir
echo `pwd`
cd $CurDir/JazzFusionPremium4
LiveMusicCharts.py . -vcfir
cd $CurDir
LiveMusicCharts.py . -il

CurDir=$BaseDir/GuitarPlayback/
cd $CurDir
echo `pwd`
cd $CurDir/30OutsideDorian
LiveMusicCharts.py . -vcfir
cd $CurDir/GPB_Backing_Track_Sampler
LiveMusicCharts.py . -vcfir
cd $CurDir/AeolianIslandGroove
LiveMusicCharts.py . -vcfir
cd $CurDir/AcidFusionModulation
LiveMusicCharts.py . -vcfir
cd $CurDir/JazzFusionModulation
LiveMusicCharts.py . -vcfir
cd $CurDir/JazzRockAmbientPads
LiveMusicCharts.py . -vcfir
cd $CurDir/JazzRockFusionGroove
LiveMusicCharts.py . -vcfir
cd $CurDir/DorianLaidBackFeel
LiveMusicCharts.py . -vcfir
cd $CurDir/FunkInB
LiveMusicCharts.py . -vcfir
cd $CurDir/FunkyMinorModes
LiveMusicCharts.py . -vcfir
cd $CurDir/FusionFunkMaster
LiveMusicCharts.py . -vcfir
cd $CurDir/FusionPads
LiveMusicCharts.py . -vcfir
cd $CurDir/JazzRockPhrasing
LiveMusicCharts.py . -vcfir
cd $CurDir/MellowFuse
LiveMusicCharts.py . -vcfir
cd $CurDir/PersonalCourse
LiveMusicCharts.py . -vcfir
cd $CurDir/SoloInMultipleKeys
LiveMusicCharts.py . -vcfir
cd $CurDir/FusionScales
LiveMusicCharts.py . -vcfir
cd $CurDir/FusionPads
LiveMusicCharts.py . -vcfir
cd $CurDir/
LiveMusicCharts.py . -il

CurDir=$BaseDir/MBonus/
cd $CurDir
echo `pwd`
cd $CurDir/Adv_StringSkipping
LiveMusicCharts.py . -vcfir
cd $CurDir/GuitarChords
LiveMusicCharts.py . -vcfir
cd $CurDir/BasicJazzChords
LiveMusicCharts.py . -vcfir
cd $CurDir/FingerstyleGuitar
LiveMusicCharts.py . -vcfir
cd $CurDir/HardRockTapping
LiveMusicCharts.py . -vcfir
cd $CurDir/StringSkipping
LiveMusicCharts.py . -vcfir
cd $CurDir/SweepPicking
LiveMusicCharts.py . -vcfir
cd $CurDir
LiveMusicCharts.py . -il

CurDir=$BaseDir/DLP/
cd $CurDir
echo `pwd`
cd $CurDir/Example
LiveMusicCharts.py . -vcfir
cd $CurDir/Licks
LiveMusicCharts.py . -vcfir
cd $CurDir/Sample
LiveMusicCharts.py . -vcfir
cd..
LiveMusicCharts.py . -il

CurDir=$BaseDir/50RockLicks/
cd $CurDir
echo `pwd`
cd $CurDir/BonusMaterial
LiveMusicCharts.py . -vcfir
cd $CurDir/JamTracks
LiveMusicCharts.py . -vcfir
cd $CurDir/PowerTabFiles
LiveMusicCharts.py . -vcfir
cd $CurDir
LiveMusicCharts.py . -il

CurDir=$BaseDir
cd $CurDir
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

cd /home/Music/Practice
LiveMusicCharts.py . -a

echo "*** MP3 Files with Errors ***"
cat ~/MP3Errors.txt

# Generate the Directory listings.
cd /home/Music/BackingTracks/
LiveMusicCharts.py . -vg
DirectoryToHtml.py

cd /home/Music/MidiDrumLoops/
LiveMusicCharts.py . -vg
DirectoryToHtml.py

cd /home/Music/Practice/
LiveMusicCharts.py . -vg
DirectoryToHtml.py

cd /home/Music/Learn/
# LiveMusicCharts.py . -vg
DirectoryToHtml.py

# Normalize the audio output
# find ./ -iname \*mp3 -exec normalize-mp3 -b {} \;

# Mix seperate MP3 files into one
# ffmpeg -i mtjpsf-020-drums.mp3 -i mtjpsf-020-bass.mp3 -i mtjpsf-020-rg.mp3 -filter_complex amerge=inputs=3 -ac 2 Output.mp3


# find ./ -iname \*.pdf.jpg -exec rm {} \;
# find ./ -name \*.conv -print0 | xargs -0 rename 's/.conv$//'
#