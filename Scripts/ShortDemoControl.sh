#! /bin/sh
#---------------------------------------------------------------------
#
#	File: 	ShortDemoControl
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Fri 06 Sep 2019 05:02:59 PM PDT
#
#	Copyright ©: 	2019 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#

Ramp() {

sleep .02; sendmidi dev "LiveMusic input" cc 07 0
sleep .02; sendmidi dev "LiveMusic input" cc 07 10
sleep .02; sendmidi dev "LiveMusic input" cc 07 20
sleep .02; sendmidi dev "LiveMusic input" cc 07 30
sleep .02; sendmidi dev "LiveMusic input" cc 07 40
sleep .02; sendmidi dev "LiveMusic input" cc 07 50
sleep .02; sendmidi dev "LiveMusic input" cc 07 60
sleep .02; sendmidi dev "LiveMusic input" cc 07 70
sleep .02; sendmidi dev "LiveMusic input" cc 07 80
sleep .02; sendmidi dev "LiveMusic input" cc 07 90
sleep .02; sendmidi dev "LiveMusic input" cc 07 100
sleep .02; sendmidi dev "LiveMusic input" cc 07 100
sleep .02; sendmidi dev "LiveMusic input" cc 07 120
sleep .02; sendmidi dev "LiveMusic input" cc 07 110
sleep .02; sendmidi dev "LiveMusic input" cc 07 100
sleep .02; sendmidi dev "LiveMusic input" cc 07 90
sleep .02; sendmidi dev "LiveMusic input" cc 07 80
sleep .02; sendmidi dev "LiveMusic input" cc 07 70
sleep .02; sendmidi dev "LiveMusic input" cc 07 60
sleep .02; sendmidi dev "LiveMusic input" cc 07 50
sleep .02; sendmidi dev "LiveMusic input" cc 07 40
sleep .02; sendmidi dev "LiveMusic input" cc 07 30
sleep .02; sendmidi dev "LiveMusic input" cc 07 20
sleep .02; sendmidi dev "LiveMusic input" cc 07 10
sleep .02; sendmidi dev "LiveMusic input" cc 07 0
}

# Demo Start
sendmidi dev "LiveMusic input" pc 50
sleep .5
sendmidi dev "LiveMusic input" pc 54
sleep .5
echo "Midi Volume"
sendmidi dev "LiveMusic input" pc 17
sleep .5
sendmidi dev "LiveMusic input" cc 07 0
sleep .5

# Master Volume
sendmidi dev "LiveMusic input" pc 18
sleep .5
sendmidi dev "LiveMusic input" cc 07 70
sleep .5

sleep 30

echo "Analog"
sendmidi dev "LiveMusic input" pc 48
sleep 11

echo "Live"
sendmidi dev "LiveMusic input" pc 50
sleep 8
wmctrl -a Hydrogen
sleep 8 
wmctrl -a catia
sleep 15

echo "Live"
sendmidi dev "LiveMusic input" pc 50
sleep 9

# Guitar Volume
sendmidi dev "LiveMusic input" pc 16
sleep .3
sendmidi dev "LiveMusic input" cc 07 80

# Master Volume
sendmidi dev "LiveMusic input" pc 18
sleep .3
sendmidi dev "LiveMusic input" cc 07 70
sleep 2

echo "Jazz"
sendmidi dev "LiveMusic input" pc 05
sleep 5

echo "Echo"
sendmidi dev "LiveMusic input" pc 06
sleep 6

echo "Midi Volume"
sendmidi dev "LiveMusic input" pc 17
sendmidi dev "LiveMusic input" cc 07 100

echo "Breath"
sendmidi dev "LiveMusic input" pc 12
sleep 1
pmidi /home/elias/SoftwareVideo/FirstOverview/GirlMel.mid -p 131:0
sleep 1

echo "Synth"
sendmidi dev "LiveMusic input" pc 11
sleep 1
pmidi /home/elias/SoftwareVideo/FirstOverview/FunkMid.mid -p 131:0
sleep 3.0

sendmidi dev "LiveMusic input" cc 07 100
sleep .3
sendmidi dev "LiveMusic input" pc 05

# Master Volume
echo "Master Volume"
sleep .3
sendmidi dev "LiveMusic input" pc 18
Ramp

echo "Midi Volume"
sleep .3
sendmidi dev "LiveMusic input" pc 17
Ramp

echo "Guitar Volume"
sleep .3
sendmidi dev "LiveMusic input" pc 16
Ramp
# sleep 2.5

echo "Expression Dist"
sleep .3
sendmidi dev "LiveMusic input" pc 15
Ramp

echo "Expression Wah"
sleep .3
sendmidi dev "LiveMusic input" pc 14
Ramp

# Master Volume
echo "Master Volume"
sleep .3
sendmidi dev "LiveMusic input" pc 18
Ramp
sendmidi dev "LiveMusic input" cc 07 70

# Guitar Volume.
sendmidi dev "LiveMusic input" pc 16
sleep .3
sendmidi dev "LiveMusic input" cc 07 80

sleep	11

echo "Wah"
sendmidi dev "LiveMusic input" pc 19
sleep .3

sleep	4

echo "Analog"
sendmidi dev "LiveMusic input" pc 48
sleep 8

echo "Live"
sendmidi dev "LiveMusic input" pc 50
sleep 25

echo "*** Do Ctrl Demo"

sleep 17 
echo "Charts"
sendmidi dev "LiveMusic input" pc 55
sleep 20

echo "*** Open Chicken"
echo "*** Open Midi"
sleep 8
echo "*** Open MP4"
sleep 10
echo "*** Open Tux"
sleep 10
echo "*** Open MP3"

#echo "Charts"
#sendmidi dev "LiveMusic input" pc 56




sleep sleep 12











