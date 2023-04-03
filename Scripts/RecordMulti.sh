#!/bin/bash
#---------------------------------------------------------------------
#
#	File: 	RecordMulti
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sat Jan 22 10:12:08 AM PST 2022
#
#	Copyright : 	2022 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
KillAll jack-plumbing

jack_connect  "system:capture_3" "rosegarden:record in 2 L"
jack_connect  "system:capture_4" "rosegarden:record in 2 R"
jack_connect  "system:capture_1" "rosegarden:record in 1 L"
jack_connect  "system:capture_2" "rosegarden:record in 1 R"
