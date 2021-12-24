#!/bin/bash
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

# ReIndex.sh
set -x

BaseDir=/home/Music/Learn
cd $BaseDir

cd /home/Music/EliasOriginals
LiveMusicCharts.py . -vci

CurDir=$BaseDir/TrueFire
cd $CurDir
echo `pwd`
LiveMusicCharts.py 50BluesLicks -vgcf
LiveMusicCharts.py 50RockLicks -vgcf
LiveMusicCharts.py Jazz_Anatomy -vgcf
LiveMusicCharts.py Take_5_Jazz_Blues_Soloing -vgcf
LiveMusicCharts.py Smart_Practice_For_Guitar -vcfgp
LiveMusicCharts.py 10_Guitar_Licks_You_Must_Know -vcfgp
LiveMusicCharts.py . -vficzl 2

CurDir=$CurDir/50RockLicks
cd $CurDir
echo `pwd`
cd $CurDir/BonusMaterial
LiveMusicCharts.py . -vfcp
cd $CurDir/JamTracks
LiveMusicCharts.py . -vfcp
cd $CurDir/PowerTabFiles
LiveMusicCharts.py . -vfcp
cd $CurDir
LiveMusicCharts.py . -vficzl 2
cd ..
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/JamTrackCentral
cd $CurDir
echo `pwd`
cd $CurDir/ah_20TastyDominantLicks
LiveMusicCharts.py . -vfcp
cd $CurDir/ah_A7b5Licks
LiveMusicCharts.py . -vcfirpg
cd $CurDir/ah_customfusion1
LiveMusicCharts.py . -vcfirpg
cd $CurDir/ah_customfusion2
LiveMusicCharts.py . -vcfirpg
cd $CurDir/ah_customfusion3
LiveMusicCharts.py . -vcfirpg
cd $CurDir/ah_bluesextensions
LiveMusicCharts.py . -vcfirpg
cd $CurDir/ah_jazzyjamscomplete
LiveMusicCharts.py . -vcfirpg
cd $CurDir/ks_ultimateguitarworkout
LiveMusicCharts.py . -vcfir
cd $CurDir/LatinLicks
LiveMusicCharts.py . -vcfirpg
cd $CurDir/lm_legatomasterclass_intermediate
LiveMusicCharts.py . -vcfirpg
cd $CurDir/dg_20outsidelicks
LiveMusicCharts.py . -vcfirpg
cd $CurDir/gg_ford
LiveMusicCharts.py . -vcfirpg
cd $CurDir/gg_funkfusion
LiveMusicCharts.py . -vcfirpg
cd $CurDir/ii_VLicks
LiveMusicCharts.py . -vcfirpg
cd $CurDir/RG_20NeoSoulFusionLicks
LiveMusicCharts.py . -vcfirpg
cd $CurDir/RockLicks
LiveMusicCharts.py . -vcfirpg
cd $CurDir/TomQuayle
LiveMusicCharts.py . -vcfirpg
cd $CurDir/WestCoastLicks
LiveMusicCharts.py . -vcfirpg
cd $CurDir/JJ_WarmUpJamtracks
LiveMusicCharts.py . -vcfirpg
cd $CurDir/ah_sophisticatedblues
LiveMusicCharts.py . -vcfirpg
cd $CurDir/MO_Electric_Fusion_Soloing
LiveMusicCharts.py . -vcfirpg
cd $CurDir/MM_20LicksNLFingerstyle
LiveMusicCharts.py . -vcfirpg
cd $CurDir/MO_20FunkedUpFusionLicks
LiveMusicCharts.py . -vcfirpg
cd $CurDir/lb_fingerpickingmasterclass
LiveMusicCharts.py . -vcfirpg
cd $CurDir/JM_21stCen_JFC
LiveMusicCharts.py . -vcfirpg
cd $CurDir
LiveMusicCharts.py . -vficzl 2

# find ./ -type d -mindepth 4

CurDir=$BaseDir/GuitarLab
cd $CurDir
echo `pwd`
cd $CurDir/20_Sax_Licks_For_Guitar
LiveMusicCharts.py . -vfcp
cd $CurDir/Altered
LiveMusicCharts.py . -vfcp
cd $CurDir/Diminished
LiveMusicCharts.py . -vfcp
cd $CurDir/Dorian
LiveMusicCharts.py . -vfcp
cd $CurDir/20BluesFusionLicks
LiveMusicCharts.py . -vfcp
cd $CurDir/FunkFusionBack
LiveMusicCharts.py . -vfcp
cd $CurDir/FunkFusionLicks
LiveMusicCharts.py . -vfcp
cd $CurDir/JohnScofield
LiveMusicCharts.py . -vfcp
cd $CurDir/LydianDominant
LiveMusicCharts.py . -vfcp
cd $CurDir/Mixolydian
LiveMusicCharts.py . -vfcp
cd $CurDir/Phrasing
LiveMusicCharts.py . -vfcp
cd $CurDir/Robben_Ford
LiveMusicCharts.py . -vfcp
cd $CurDir/ScottHenderson
LiveMusicCharts.py . -vfcp
cd $CurDir/Spain_20_Fusion_Guitar_Licks
LiveMusicCharts.py . -vfcp
cd $CurDir/The_Chicken_20_Licks_Part1
LiveMusicCharts.py . -vfcp
cd $CurDir
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/PeopleLearn
cd $CurDir
echo `pwd`
cd $CurDir/AlDiMeola
LiveMusicCharts.py . -vfcp
cd $CurDir/Carlton
LiveMusicCharts.py . -vfcp
cd $CurDir/DavidLeeRoth
LiveMusicCharts.py . -vfcp
cd $CurDir/FrankGambale
LiveMusicCharts.py . -vfcp
cd $CurDir/Holdsworth
# too many pdfs and jpgs for just leave them.
LiveMusicCharts.py . -vfc
cd $CurDir/TimothyPedonAdvancedHarmonicConcepts
# too many pdfs and jpgs for just leave them.
LiveMusicCharts.py . -vcfirg

\cd $CurDir/SteveMorse
LiveMusicCharts.py . -vfcp
cd $CurDir
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/FlatFiv
cd $CurDir
echo `pwd`
cd $CurDir/FusionSolos
LiveMusicCharts.py . -vfcp
cd $CurDir/Tapping
LiveMusicCharts.py . -vfcp
cd $CurDir/CoolLegatoPhrasesTranscriptionFiles
LiveMusicCharts.py . -vfcp
cd $CurDir/DarrylPentatonics
LiveMusicCharts.py . -vfcp
cd $CurDir/FITDemoVersion
LiveMusicCharts.py . -vfcp
cd $CurDir
LiveMusicCharts.py . -vficzl 2


CurDir=$BaseDir/GuitarMaster
cd $CurDir
echo `pwd`
cd $CurDir/GuitarMasterTapping
LiveMusicCharts.py . -vfcp
cd $CurDir/JazzFusionLicks
LiveMusicCharts.py . -vfcp
cd $CurDir/Sweep
LiveMusicCharts.py . -vfcp
cd $CurDir
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/CoffeeBreakGroove
cd $CurDir
echo `pwd`
cd $CurDir/JazzFusionPremium4
LiveMusicCharts.py . -vcfirp
cd $CurDir
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/GuitarPlayback
cd $CurDir
echo `pwd`
cd $CurDir/30OutsideDorian
LiveMusicCharts.py . -vfcp
cd $CurDir/GPB_Backing_Track_Sampler
LiveMusicCharts.py . -vfcp
cd $CurDir/AeolianIslandGroove
LiveMusicCharts.py . -vfcp
cd $CurDir/AcidFusionModulation
LiveMusicCharts.py . -vfcp
cd $CurDir/JazzFusionModulation
LiveMusicCharts.py . -vfcp
cd $CurDir/JazzRockAmbientPads
LiveMusicCharts.py . -vfcp
cd $CurDir/JazzRockFusionGroove
LiveMusicCharts.py . -vfcp
cd $CurDir/DorianLaidBackFeel
LiveMusicCharts.py . -vfcp
cd $CurDir/FunkInB
LiveMusicCharts.py . -vfcp
cd $CurDir/FunkyMinorModes
LiveMusicCharts.py . -vfcp
cd $CurDir/FusionFunkMaster
LiveMusicCharts.py . -vfcp
cd $CurDir/FusionPads
LiveMusicCharts.py . -vfcp
cd $CurDir/JazzRockPhrasing
LiveMusicCharts.py . -vfcp
cd $CurDir/MellowFuse
LiveMusicCharts.py . -vfcp
cd $CurDir/PersonalCourse
LiveMusicCharts.py . -vfcp
cd $CurDir/SoloInMultipleKeys
LiveMusicCharts.py . -vfcp
cd $CurDir/FusionScales
LiveMusicCharts.py . -vfcp
cd $CurDir/FusionPads
LiveMusicCharts.py . -vfcp
cd $CurDir/
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/MBonus
cd $CurDir
echo `pwd`
#cd $CurDir/Adv_StringSkipping
#LiveMusicCharts.py . -vfcp
cd $CurDir/GuitarChords
LiveMusicCharts.py . -vfcp
#cd $CurDir/BasicJazzChords
#LiveMusicCharts.py . -vfcp
cd $CurDir/FingerstyleGuitar
LiveMusicCharts.py . -vfcp
cd $CurDir/HardRockTapping
LiveMusicCharts.py . -vfcp
cd $CurDir/StringSkipping
LiveMusicCharts.py . -vfcp
cd $CurDir/SweepPicking
LiveMusicCharts.py . -vfcp
cd $CurDir
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/MyMasterClass
cd $CurDir
echo `pwd`
cd $CurDir/ScottHenderson
LiveMusicCharts.py . -vfcp
cd $CurDir
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/DLP/Augmented
cd $CurDir
echo `pwd`
cd $CurDir/AugmentedScale
LiveMusicCharts.py . -vfcp
cd $CurDir/Licks
LiveMusicCharts.py . -vfcp
cd $CurDir/PeopleRiffs
LiveMusicCharts.py . -vfcp
cd $CurDir
LiveMusicCharts.py . -vficzl 2
cd $CurDir
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/FusionPractice
cd $CurDir
echo `pwd`
cd $CurDir/OutsideDorianLicks
LiveMusicCharts.py . -vfcp
cd $CurDir/PentaTrick
LiveMusicCharts.py . -vfcp
cd $CurDir/Tapping
LiveMusicCharts.py . -vfcp
cd $CurDir
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir/Theory
cd $CurDir
echo `pwd`
cd $CurDir/Theory1
LiveMusicCharts.py . -vfcz
cd $CurDir/Theory2
LiveMusicCharts.py . -vfcz
cd $CurDir/Theory3
LiveMusicCharts.py . -vfcz
cd $CurDir/Theory4
LiveMusicCharts.py . -vficzl 2
cd $CurDir
LiveMusicCharts.py . -vficzl 2

CurDir=$BaseDir
cd $CurDir
echo `pwd`
LiveMusicCharts.py RickBeato -vcf
# LiveMusicCharts.py JazzGuitar -vcf
#cd $CurDir/MyMasterClass
#LiveMusicCharts.py . -vcf
#cd $CurDir/Material
#LiveMusicCharts.py . -vcf

LiveMusicCharts.py MusicNotes -vcfz
LiveMusicCharts.py P4Tuning -vcfz
LiveMusicCharts.py Slonimsky -vcfz
# LiveMusicCharts.py . -vc

cd $BaseDir
echo `pwd`
# LiveMusicCharts.py . -vficzl 2

cd /home/Music/Practice
LiveMusicCharts.py . -a

echo "*** MP3 Files with Errors ***"
cat ~/MP3Errors.txt

# Generate the Directory listings.
cd /home/Music/BackingTracks
LiveMusicCharts.py . -vg
DirectoryToHtml.py

cd /home/Music/MidiDrumLoops
LiveMusicCharts.py . -vg
DirectoryToHtml.py

cd /home/Music/Practice
LiveMusicCharts.py . -vg
DirectoryToHtml.py

cd /home/Music/Learn
# LiveMusicCharts.py . -vg
DirectoryToHtml.py

# find ./ -type d -exec chmod 700 {} \;
# find ./ -type f -exec chmod 600 {} \;

cd $BaseDir
chown elias: *  -R
chmod 700 *

# =================================================================
# Normalize the audio output
# find ./ -iname \*mp3 -exec normalize-mp3 -b {} \;

# Mix seperate MP3 files into one
# ffmpeg -i mtjpsf-020-drums.mp3 -i mtjpsf-020-bass.mp3 -i mtjpsf-020-rg.mp3 -filter_complex amerge=inputs=3 -ac 2 Output.mp3


# find ./ -iname \*.pdf.jpg -exec rm {} \;
# find ./ -name \*.conv -print0 | xargs -0 rename 's/.conv$//'
#