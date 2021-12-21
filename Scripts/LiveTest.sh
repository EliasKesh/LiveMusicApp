#!/bin/bash
#---------------------------------------------------------------------
#
#   File:   LiveTest
#
#   Contains:
#
#
#   Written By:     Elias Keshishoglou on Sun 18 Aug 2019 11:09:52 AM PDT
#
#   Copyright ©:    2019 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#


Ramp() {
    echo "---DEB--------> Ramp Start "$RampCTR
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 0
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 10
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 20
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 30
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 40
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 50
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 60
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 70
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 80
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 90
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 100
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 120
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 128
    sleep .5
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 110
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 100
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 90
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 80
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 70
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 60
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 50
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 40
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 30
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 20
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 10
    sleep $RDly; sendmidi dev "LiveMusic input" cc $RampCTR 0
    echo "---DEB--------> Ramp Stop"
}

DoAll() {
    echo "---DEB--------> DoAll "$RampCTR
    sleep $RDly; sendmidi dev "LiveMusic input" pc 00
    sleep $RDly; sendmidi dev "LiveMusic input" pc 01
    sleep $RDly; sendmidi dev "LiveMusic input" pc 02
    sleep $RDly; sendmidi dev "LiveMusic input" pc 03
    sleep $RDly; sendmidi dev "LiveMusic input" pc 04
    sleep $RDly; sendmidi dev "LiveMusic input" pc 05
    sleep $RDly; sendmidi dev "LiveMusic input" pc 06
    sleep $RDly; sendmidi dev "LiveMusic input" pc 07
    sleep $RDly; sendmidi dev "LiveMusic input" pc 08
    sleep $RDly; sendmidi dev "LiveMusic input" pc 09
    
    sleep $RDly; sendmidi dev "LiveMusic input" pc 10
    sleep $RDly; sendmidi dev "LiveMusic input" pc 11
    sleep $RDly; sendmidi dev "LiveMusic input" pc 12
    sleep $RDly; sendmidi dev "LiveMusic input" pc 13
    sleep $RDly; sendmidi dev "LiveMusic input" pc 14
    sleep $RDly; sendmidi dev "LiveMusic input" pc 15
    sleep $RDly; sendmidi dev "LiveMusic input" pc 16
    sleep $RDly; sendmidi dev "LiveMusic input" pc 17
    sleep $RDly; sendmidi dev "LiveMusic input" pc 18
    sleep $RDly; sendmidi dev "LiveMusic input" pc 19
    
    sleep $RDly; sendmidi dev "LiveMusic input" pc 20
    sleep $RDly; sendmidi dev "LiveMusic input" pc 21
    sleep $RDly; sendmidi dev "LiveMusic input" pc 22
    sleep $RDly; sendmidi dev "LiveMusic input" pc 23
    #sleep $RDly; sendmidi dev "LiveMusic input" pc 24
    #sleep $RDly; sendmidi dev "LiveMusic input" pc 25
    #sleep $RDly; sendmidi dev "LiveMusic input" pc 26
    #sleep $RDly; sendmidi dev "LiveMusic input" pc 27
    sleep $RDly; sendmidi dev "LiveMusic input" pc 28
    sleep $RDly; sendmidi dev "LiveMusic input" pc 29
    
    sleep $RDly; sendmidi dev "LiveMusic input" pc 30
    sleep $RDly; sendmidi dev "LiveMusic input" pc 31
    sleep $RDly; sendmidi dev "LiveMusic input" pc 32
    sleep $RDly; sendmidi dev "LiveMusic input" pc 33
    sleep $RDly; sendmidi dev "LiveMusic input" pc 34
    sleep $RDly; sendmidi dev "LiveMusic input" pc 35
    sleep $RDly; sendmidi dev "LiveMusic input" pc 36
    sleep $RDly; sendmidi dev "LiveMusic input" pc 37
    sleep $RDly; sendmidi dev "LiveMusic input" pc 38
    sleep $RDly; sendmidi dev "LiveMusic input" pc 39
    
    #sleep $RDly; sendmidi dev "LiveMusic input" pc 40
    #sleep $RDly; sendmidi dev "LiveMusic input" pc 41
    #sleep $RDly; sendmidi dev "LiveMusic input" pc 42
    sleep $RDly; sendmidi dev "LiveMusic input" pc 43
    sleep $RDly; sendmidi dev "LiveMusic input" pc 44
    sleep $RDly; sendmidi dev "LiveMusic input" pc 45
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 46
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 47
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 48
    sleep $RDly; sendmidi dev "LiveMusic input" pc 49
    
    sleep $RDly; sendmidi dev "LiveMusic input" pc 50
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 51
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 52
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 53
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 54
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 55
    sleep .2; sendmidi dev "LiveMusic input" pc 56
    sleep $SDly; sendmidi dev "LiveMusic input" pc 57
    sleep .4; sendmidi dev "LiveMusic input" pc 58
    sleep $SDly; sendmidi dev "LiveMusic input" pc 59
    
    sleep $SDly; sendmidi dev "LiveMusic input" pc 60
    # sleep $SDly; sendmidi dev "LiveMusic input" pc 61
    sleep $SDly; sendmidi dev "LiveMusic input" pc 62
    sleep $LDly; sendmidi dev "LiveMusic input" pc 63
    sleep $RDly; sendmidi dev "LiveMusic input" pc 64
    sleep $RDly; sendmidi dev "LiveMusic input" pc 65
    sleep $RDly; sendmidi dev "LiveMusic input" pc 66
    sleep $RDly; sendmidi dev "LiveMusic input" pc 67
    sleep $RDly; sendmidi dev "LiveMusic input" pc 68
    sleep $RDly; sendmidi dev "LiveMusic input" pc 69
    
    sleep $RDly; sendmidi dev "LiveMusic input" pc 70
    sleep $RDly; sendmidi dev "LiveMusic input" pc 71
    sleep $RDly; sendmidi dev "LiveMusic input" pc 72
    sleep $RDly; sendmidi dev "LiveMusic input" pc 73
    sleep $RDly; sendmidi dev "LiveMusic input" pc 74
    sleep $RDly; sendmidi dev "LiveMusic input" pc 75
    sleep $RDly; sendmidi dev "LiveMusic input" pc 76
    sleep $RDly; sendmidi dev "LiveMusic input" pc 77
    sleep $RDly; sendmidi dev "LiveMusic input" pc 78
    sleep .6; sendmidi dev "LiveMusic input" pc 79
    sleep .6; sendmidi dev "LiveMusic input" pc 79
    sleep .6; sendmidi dev "LiveMusic input" pc 79
    sleep .6; sendmidi dev "LiveMusic input" pc 79
    sleep 2
    
    echo "---DEB--------> DoAll Stop "$RampCTR
    
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 80
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 81
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 82
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 83
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 84
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 85
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 86
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 87
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 88
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 89
    
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 90
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 91
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 92
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 93
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 94
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 95
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 96
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 97
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 98
    # sleep $RDly; sendmidi dev "LiveMusic input" pc 99
    
}

# amidi -p hw:1,1 -S 'B0 00 00 C0 07'
# amidi -p hw:1,0 --recieve

#sleep $RDly; sendmidi dev "LiveMusic input" ch 1

#sleep $RDly; sendmidi dev "LiveMusic input" cc 4 1

#sleep $RDly; sendmidi dev "LiveMusic input" pc 3

# Program Change triggers one more.

GetMidiVal() {
    MExtra=""
    MValue=""
    MCommand=""

    MExtra=`tail -n 1  ${MD_Test_File} | awk '{ print $5 }'`
    MValue=`tail -n 1  ${MD_Test_File} | awk '{ print $4 }'`
    MCommand=`tail -n 1  ${MD_Test_File} | awk '{ print $3 }'`
    echo "Return_Midi "$MCommand"  "$MValue" "$MExtra
}

CheckGuitarixPort() {
    receivemidi dev GuitarEffects > ${MD_Test_File} &
    sendmidi dev "LiveMusic input" pc 7
    sleep 1
    GetMidiVal
    echo "Got Guitarix Midi "$MCommand"  "$MValue
    
    if [[ $MCommand == "program-change" ]]; then
        echo "Found "$MValue
    else
        echo "Guitarix Not Found "$MCommand" "$MValue >> ${OutputFile}
    fi
    
    killall receivemidi
}

CheckMidiPort() {
#   Check for note pass-thru
    receivemidi dev MidiSynth > ${MD_Test_File} &
    sleep 0.5
    sendmidi dev "LiveMusic input" off 48 120
    sleep 0.5
    GetMidiVal
    echo "Got Synth Note Midi "$MCommand"  "$MValue
    
    if [[ $MCommand == "note-off" ]]; then
        echo "Found "$MValue
    else
        echo "Midi Not Found "$MCommand" "$MValue >> ${OutputFile}
    fi

#   Program Change
    receivemidi dev MidiSynth > ${MD_Test_File} &
    sleep 0.5
    sendmidi dev "LiveMusic input" pc 78
    sleep 0.5
    GetMidiVal
    echo "Got Synth PC Midi "$MCommand"  "$MValue
    
    if [[ $MCommand == "program-change" ]]; then
        echo "Found "$MValue
    else
        echo "Midi Not Found "$MCommand" "$MValue >> ${OutputFile}
    fi
        
#   Check for midi control Expression
    sendmidi dev "LiveMusic input" pc 20
    sendmidi dev "LiveMusic input" cc 07 40
    sendmidi dev "LiveMusic input" cc 07 127
    sleep 2.0
    GetMidiVal
    echo "Got Synth Expression Midi "$MCommand"  "$MValue

    if [[ $MValue == "7" ]]; then
        echo "Found "$MValue
    else
        echo "Midi Not Found "$MCommand" "$MValue >> ${OutputFile}
    fi



    killall receivemidi
}

CheckPedalPort() {
    receivemidi dev Pedal > ${MD_Test_File} &
#    sendmidi dev "LiveMusic input" pc 7
    sleep 2
    GetMidiVal
    echo "Got Pedal Midi "$MCommand"  "$MValue
    
    if [[ $MCommand == "control-change" ]]; then
        echo "Found "$MValue
    else
        echo "Pedal Not Found "$MCommand" "$MValue >> ${OutputFile}
    fi
    
    killall receivemidi
}

CheckDAWPort() {
    receivemidi dev DAW > ${MD_Test_File} &
#    sendmidi dev "LiveMusic input" pc 7
    sleep 2
    GetMidiVal
    echo "Got DAW Midi "$MCommand"  "$MValue
    
    if [[ $MCommand == "control-change" ]]; then
        echo "Found "$MValue
    else
        echo "DAW Not Found "$MCommand" "$MValue >> ${OutputFile}
    fi
    
    killall receivemidi
}

#MD_G_File=/home/elias/SysDocs/MidiGuitarTest.txt
#MD_C_File=/home/elias/SysDocs/MidiClickTest.txt
#MD_P_File=/home/elias/SysDocs/MidiPedalTest.txt

MD_Test_File=/home/elias/SysDocs/MidiTest.txt
OutputFile=/home/elias/SysDocs/LiveTest.log
echo "LiveMusicApp Log "`date` > ${OutputFile}

killall receivemidi

pid=`pgrep -i LiveMusicApp`
if [ -z $pid ]
then
    ./LiveMusicApp -v 0x200 &
    sleep 3
fi

#receivemidi dev GuitarEffects > ${MD_G_File} &
#receivemidi dev Click > ${MD_C_File} &
#receivemidi dev Pedal > ${MD_P_File} &

SDly=.3
RDly=.1
LDly=1
ulimit -c unlimited



LoopCount=0
while true
do
    echo `date`" Count "$LoopCount >> ${OutputFile}
    
    LoopCount=$((LoopCount+1))

    CheckGuitarixPort
    CheckMidiPort
    CheckPedalPort
    CheckDAWPort

    echo "---DEB--------> Patch "
    sendmidi dev "LiveMusic input" pc 55
    sleep $SDly;
    
    echo "---DEB--------> Live "
    sendmidi dev "LiveMusic input" pc 21
    sleep .6
    
    RampCTR=01
    Ramp
    RampCTR=02
    Ramp
    RampCTR=03
    Ramp
    RampCTR=07
    
    # Master Volume
    echo "---DEB--------> Master Volume"
    sleep $SDly;
    sendmidi dev "LiveMusic input" pc 21
    Ramp
    
    echo "---DEB--------> Midi Volume"
    sleep $SDly;
    sendmidi dev "LiveMusic input" pc 20
    Ramp
    
    echo "---DEB--------> Guitar Volume"
    sleep $SDly;
    sendmidi dev "LiveMusic input" pc 19
    Ramp
    
    echo "---DEB--------> Expression Dist"
    sleep $SDly;
    sendmidi dev "LiveMusic input" pc 16
    Ramp
    
    echo "---DEB--------> Expression Wah"
    sleep $SDly;
    sendmidi dev "LiveMusic input" pc 17
    Ramp
    
    echo "---DEB--------> Expression Volume"
    sleep $SDly;
    sendmidi dev "LiveMusic input" pc 18
    Ramp
    
    # Tempo Change
    sendmidi dev "LiveMusic input" cc 04 80
    
    # Back to patch screen.
    
    echo "---DEB--------> Chords"
    sendmidi dev "LiveMusic input" pc 57
    sleep .6
    
    echo "---DEB--------> Player"
    sendmidi dev "LiveMusic input" pc 59
    sleep 1
    
    echo "---DEB--------> Score"
    sendmidi dev "LiveMusic input" pc 58
    sleep 1
    
    echo "---DEB--------> Patch "
    sendmidi dev "LiveMusic input" pc 60
    sleep 1
    
    echo "---DEB--------> Looper "
    sendmidi dev "LiveMusic input" pc 27
    sleep .6
    
    echo "---DEB--------> Drums "
    sendmidi dev "LiveMusic input" pc 26
    sleep .6
    
    echo "---DEB--------> analog "
    sendmidi dev "LiveMusic input" pc 25
    sleep .6
    
    echo "---DEB--------> Live "
    sendmidi dev "LiveMusic input" pc 24
    sleep .6
    
    # Tempo Change
    sendmidi dev "LiveMusic input" cc 04 90
    
    DoAll
    
done
exit


