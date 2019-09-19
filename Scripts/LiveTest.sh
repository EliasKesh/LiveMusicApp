#! /bin/sh
#---------------------------------------------------------------------
#
#	File: 	LiveTest
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sun 18 Aug 2019 11:09:52 AM PDT
#
#	Copyright ©: 	2019 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#


Ramp() {

sleep .1; sendmidi dev "LiveMusic input" cc 07 0
sleep .1; sendmidi dev "LiveMusic input" cc 07 10
sleep .1; sendmidi dev "LiveMusic input" cc 07 20
sleep .1; sendmidi dev "LiveMusic input" cc 07 30
sleep .1; sendmidi dev "LiveMusic input" cc 07 40
sleep .1; sendmidi dev "LiveMusic input" cc 07 50
sleep .1; sendmidi dev "LiveMusic input" cc 07 60
sleep .1; sendmidi dev "LiveMusic input" cc 07 70
sleep .1; sendmidi dev "LiveMusic input" cc 07 80
sleep .1; sendmidi dev "LiveMusic input" cc 07 90
sleep .1; sendmidi dev "LiveMusic input" cc 07 100
sleep .1; sendmidi dev "LiveMusic input" cc 07 100
sleep .1; sendmidi dev "LiveMusic input" cc 07 120
sleep .1; sendmidi dev "LiveMusic input" cc 07 110
sleep .1; sendmidi dev "LiveMusic input" cc 07 100
sleep .1; sendmidi dev "LiveMusic input" cc 07 90
sleep .1; sendmidi dev "LiveMusic input" cc 07 80
sleep .1; sendmidi dev "LiveMusic input" cc 07 70
sleep .1; sendmidi dev "LiveMusic input" cc 07 60
sleep .1; sendmidi dev "LiveMusic input" cc 07 50
sleep .1; sendmidi dev "LiveMusic input" cc 07 40
sleep .1; sendmidi dev "LiveMusic input" cc 07 30
sleep .1; sendmidi dev "LiveMusic input" cc 07 20
sleep .1; sendmidi dev "LiveMusic input" cc 07 10
sleep .1; sendmidi dev "LiveMusic input" cc 07 0
}


# amidi -p hw:1,1 -S 'B0 00 00 C0 07'
# amidi -p hw:1,0 --recieve

#sleep .1; sendmidi dev "LiveMusic input" ch 1

#sleep .1; sendmidi dev "LiveMusic input" cc 4 1

#sleep .1; sendmidi dev "LiveMusic input" pc 3


# Master Volume
echo "Master Volume"
sleep .3
while true
do
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

echo "Expression Dist"
sleep .3
sendmidi dev "LiveMusic input" pc 15
Ramp

echo "Expression Wah"
sleep .3
sendmidi dev "LiveMusic input" pc 14
Ramp

echo "Expression Volume"
sleep .3
sendmidi dev "LiveMusic input" pc 13
Ramp

# Back to patch screen.

echo "Chords"
sendmidi dev "LiveMusic input" pc 57
sleep .6

echo "Player"
sendmidi dev "LiveMusic input" pc 56
sleep .6

echo "Score"
sendmidi dev "LiveMusic input" pc 55
sleep .6

echo "Patch "
sendmidi dev "LiveMusic input" pc 54
sleep .6


echo "Looper "
sendmidi dev "LiveMusic input" pc 24
sleep .6

echo "Drums "
sendmidi dev "LiveMusic input" pc 23
sleep .6

echo "analog "
sendmidi dev "LiveMusic input" pc 22
sleep .6

echo "Live "
sendmidi dev "LiveMusic input" pc 21
sleep .6


done
exit
