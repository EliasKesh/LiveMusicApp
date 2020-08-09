#! /bin/sh
#---------------------------------------------------------------------
#
#	File: 	ReplacePatch
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sat 08 Aug 2020 09:06:03 PM PDT
#
#	Copyright : 	2020 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
find ./ -iname \*html -exec sed -i  "s/${1}/${2}/g" {} \;

