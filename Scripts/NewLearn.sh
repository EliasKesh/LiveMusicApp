#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	NewLearn
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sat Feb 11 10:09:13 AM PST 2023
#
#	Copyright : 	2023 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
FixFileNames.sh
FixFileNames.sh

/* Convert Guitar Pro into Muse
*/
find . -regextype posix-extended -regex '.*\.(gp.?)$ --exec /usr/src/LiveMusicBuilds/MuseScore-3.6.2.548021370-x86_64.AppImage {} -S EliasTab1 -o {}".mscz" \;'

# find ./ -iname \*mp4 -exec ffmpeg -i {} {}.mp3 \;
find ./ -iname \*wma -exec ffmpeg -i {} {}.mp3 \;
#find ./ -iname \*rtf -exec ffmpeg -i {} {}.mp3 \; -exec libreoffice --headless convert-to pdf {} \;

find ./ -iname \*.pdf -exec pdftoppm -jpeg {} {}".pdf.jpg" \;
find ./ -iname \*.pdf -exec mv {} {}".conv" \;

