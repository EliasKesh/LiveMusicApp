#!/usr/bin/python3
#--------------------------------------------------------------
#
#	File: 	CarlaPresets.py
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Wed Jun 12 11:19:53 EEST 2024
#
#	Copyright : 	2024 Elias Keshishoglou all rights reserved.
#   
# 
#-------------------------------------------------------------#
# pip3 install python-rtmidi --user

# Import required libraries for MIDI and OSC communication
import pyliblo3 as liblo          # OSC communication library
import mido           # MIDI I/O library
import sys           # System functions
import math          # Mathematical operations

# Define Effect enum class for mapping effects to their rack positions
from enum import auto, Enum
class Effect(Enum):
        ABGate = 0          # Noise gate effect
#        Compressor2 = auto() # Dynamic range compressor
        LSPCompressor = auto()
        Equalizer = auto()    # Equalizer
        Distortion2 = auto()    # Tube screamer
        PreAmp = auto()      # Amplifier preamp simulation
        Distortion1 = PreAmp  # Alias for PreAmp
#        Conditioner = auto()      # Amplifier preamp simulation
#        Toob = auto()      # Amplifier preamp simulation
        Mojo = auto()        # Tone shaping effect
        Waylo = auto()
        TwinPitch = auto()
        Wah = Mojo          # Alias for Mojo (Wah effect)
        Reverb = auto()      # Reverb effect
#        Chorus = auto()      # Modulation effect
#        Squish = auto()      # Compression effect
        Loudness = auto()
        Mixer = auto()       # Audio mixer
        Tuner = auto()       # Guitar tuner
        MidiConvert = auto()
#        GxDelay = auto()     # Delay effect
#        DubDelay = auto()    # Additional delay
#        FlangeWaylo = auto() # Combined flanger/chorus

GuitarMidiVar=1
# for testing.
# oscdump 1234
# Chttps://github.com/brummer10/SpecMatch.git
#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
# OSC callback functions
def foo_bar_callback(path, args):
    # Handle basic OSC messages with integer and float arguments
    i, f = args
    print( "received mes4sage '%s' with arguments '%d' and '%f'" % (path, i, f))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def foo_baz_callback(path, args, types, src, data):
    # Handle OSC messages containing blob data
    print( "received message '%s'" % path)
    print( "blob contains %d bytes, user data was '%s'" % (len(args[0]), data))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def fallback(path, args, types, src):
    # Default handler for unrecognized OSC messages
    print( "got unknown message '%s' from '%s'" % (path, src.url))
    for a, t in zip(args, types):
        print( "argument of type '%s': %s" % (t, a))

#--------------------------------------------------------------
# --- 
#-------------------------------------------------           -------------
# Effect control functions
def MidiGuitar(switch):
    global GuitarMidiVar 


    print("MidiGuitar ", switch, GuitarMidiVar)
    theswitch = switch
    if (switch == 0):
        if (GuitarMidiVar == 1):
            theswitch = 2
        elif (GuitarMidiVar == 2):
            theswitch = 1
        else:
            theswitch = 1
            GuitarMidiVar = 1


    # Guitar on, Midi Off
    if (theswitch == 1):
        GuitarMidiVar = 1
        print("MidiGuitar 1")
        EffectString = "/Carla/%d/set_parameter_value" % (Effect.Mixer.value)
        liblo.send(target, EffectString, 11, 0.0)
        EffectString = "/Carla/%d/set_parameter_value" % (Effect.Mixer.value)
        liblo.send(target, EffectString, 20, 1.0)
        EffectString = "/Carla/%d/set_active" % (Effect.MidiConvert.value)
        liblo.send(target, EffectString, 0)

    # Guitar off, Midi On
    if (theswitch == 2):
        GuitarMidiVar = 2
        print("MidiGuitar 2")
        EffectString = "/Carla/%d/set_parameter_value" % (Effect.Mixer.value)
        liblo.send(target, EffectString, 11, 1.0)
        EffectString = "/Carla/%d/set_parameter_value" % (Effect.Mixer.value)
        liblo.send(target, EffectString, 20, 0.0)
        EffectString = "/Carla/%d/set_active" % (Effect.MidiConvert.value)
        liblo.send(target, EffectString, 1)

    if (theswitch == 3):
        GuitarMidiVar = 1
        EffectString = "/Carla/%d/set_parameter_value" % (Effect.Mixer.value)
        liblo.send(target, EffectString, 11, 0.0)
        EffectString = "/Carla/%d/set_parameter_value" % (Effect.Mixer.value)
        liblo.send(target, EffectString, 20, 0.0)
        EffectString = "/Carla/%d/set_active" % (Effect.MidiConvert.value)
        liblo.send(target, EffectString, 1)



def DistortionControl(level):
    # Control Distortion parameters based on input level (0-127)
    InitLevel=level
    
    # Set Distortion volume
    EffectString = "/Carla/%d/set_volume" % (Effect.Distortion1.value)
#    Value=(0.7 * rescaleMidiL(InitLevel, 1.0, 0.20)) + 0.2
    Value=rescale(InitLevel, 128.0, 0.0, 0.20, 1.25)
    liblo.send(target, EffectString, Value )

    # Set Distortion amount (0-100)
    Value=rescaleMidi(InitLevel, 0.0, 100.0)
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion1.value)
    liblo.send(target, EffectString, 2, (Value))

    # Set drive level (0-1)
    Value=rescaleMidi(InitLevel, 0.0, 0.5)
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion1.value)
    liblo.send(target, EffectString, 3, (Value))

    # Set bass level (0.6-0.85)
    Value=rescaleMidi(InitLevel, 0.6, 0.85)
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
    liblo.send(target, EffectString, 5, (Value))

    # Value=rescaleMidi(InitLevel, 0.35, 0.85)
    # EffectString = "/Carla/%d/set_drywet" % (Effect.Conditioner.value)
    # liblo.send(target, EffectString, 0.65)

    # increase the bass
    # Value=rescaleMidi(InitLevel, 0.5, 0.85)
    # EffectString = "/Carla/%d/set_parameter_value" % (Effect.Toob.value)
    # liblo.send(target, EffectString, 1, Value)

    # Tube Distortion2
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    EffectString = "/Carla/%d/set_drywet" % (Effect.Distortion2.value)
    liblo.send(target, EffectString, Value)

#  Parameter Value 0 Master Out
#  Parameter Value 1 Pregain

def Dist1Setup(Mode):
    print("Dist1Setup " + Mode)

    Dist1Bass=0.6
    Dist1Mid=0.5499
    Dist1Treble=0.47
    Dist1Presence=10.0
    Dist1PreGain=0.0
    Dist1Master=-18.0
    Dist1Distortion=1.0
    Dist1Drive=0.0
    
    if (Mode == "FunkyStrat"):
        Dist1Bass=0.4
        Dist1Mid=0.35
        Dist1Treble=0.68
        Dist1Presence=14.0
        Dist1PreGain=0.0
        Dist1Master=-18.0
        Dist1Drive=0.0
        Dist1Distortion=0.0

    # Set drive level (0-1)
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion1.value)
    liblo.send(target, EffectString, 3, Dist1Drive)

    # Distortion 1 - 100
    liblo.send(target, EffectString, 2, Dist1Distortion)

    # Set bass level (0.6-0.85)
    liblo.send(target, EffectString, 5, Dist1Bass)
 
    # Mid 
    liblo.send(target, EffectString, 4,Dist1Mid)
    # Treble 
    liblo.send(target, EffectString, 6, Dist1Treble)

    # Presence  
    liblo.send(target, EffectString, 8, Dist1Presence)

        # PreGain   
    liblo.send(target, EffectString, 1, 0.0)

        # Master   
    liblo.send(target, EffectString, 0, -18.0)


#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def WahControl(level):

    # 0.0 to 1.0
    InitLevel=level
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
    liblo.send(target, EffectString, Value)
    print("WahControl ", Value)

def WahSetup(Mode):
    print("WahSetup " + Mode)

    WahResonace=62.0
    WahBypass=0
    WahSmooth=105
    WahSense=-24.0
    WahWetDry=0.0
    
    if (Mode == "FunkyStrat"):
        WahResonace=78.0
        WahBypass=0
        WahSmooth=55
        WahSense=-34.0
        WahWetDry=1.0

    # Resonance 0 - 127
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Wah.value)
    liblo.send(target, EffectString, 2, WahResonace)

    # Sensitivity -64 to 64
    liblo.send(target, EffectString, 8, WahSense)

    # Smoothing 0 - 127
    liblo.send(target, EffectString, 10, WahSmooth)

    # Bypass
    liblo.send(target, EffectString, 0, WahBypass)

    EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
    liblo.send(target, EffectString, WahWetDry )

def ChorusControl(level):
    
    InitLevel=level
    EffectString = "/Carla/%d/set_drywet" % (Effect.Waylo.value)
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    liblo.send(target, EffectString, Value)

    # Set Distortion volume
#     EffectString = "/Carla/%d/set_volume" % (Effect.Waylo.value)
# #    Value=(0.7 * rescaleMidiL(InitLevel, 1.0, 0.20)) + 0.2
#     Value=rescale(InitLevel, 128.0, 0.0, 0.40, 1.25)
#     liblo.send(target, EffectString, Value )


    # InitLevel=level
    # EffectString = "/Carla/%d/set_drywet" % (Effect.FlangeWaylo.value)
    # Value=rescaleMidi(InitLevel, 0.0, 1.0)
    # liblo.send(target, EffectString, Value)

def ReverbSetup(Mode):
    print("ReverbSetup " + Mode)

    ReverbWetDry=1
    ReverbLevel=-1.157
    
    if (Mode == "FunkyStrat"):
        ReverbLevel=-6
        ReverbWetDry=1

    # Reverb level is volume; dry/wet is a separate parameter.
#    EffectString = "/Carla/%d/set_volume" % (Effect.Reverb.value)
#    liblo.send(target, EffectString, ReverbLevel)

    EffectString = "/Carla/%d/set_drywet" % (Effect.Reverb.value)
    liblo.send(target, EffectString, ReverbWetDry)


def GateControl(level):
    InitLevel=level
    EffectString = "/Carla/%d/set_drywet" % (Effect.ABGate.value)
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    liblo.send(target, EffectString, Value)

    Value=rescaleMidi(InitLevel, 0, 2000)
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.ABGate.value)
    liblo.send(target, EffectString, 8, (Value))

    # Threshold
    # Value=rescaleMidi(InitLevel,-46 , 4)
    # EffectString = "/Carla/%d/set_parameter_value" % (Effect.ABGate.value)
    # liblo.send(target, EffectString, 1, (Value))

    # # Hold
    # Value=rescaleMidi(InitLevel, 0, 3000)
    # EffectString = "/Carla/%d/set_parameter_value" % (Effect.ABGate.value)
    # liblo.send(target, EffectString, 3, (Value))

    # # Rati0
    # Value=rescaleMidi(InitLevel, 1, 100)
    # EffectString = "/Carla/%d/set_parameter_value" % (Effect.ABGate.value)
    # liblo.send(target, EffectString, 3, (Value))


def CompressorControl(Mode):
    print("CompressorControl:", Mode)

    EffectString = "/Carla/%d/set_parameter_value" % (Effect.LSPCompressor.value)

    if (Mode == "Off"):
        # Disable compressor
        liblo.send(target, EffectString, 6, 0)
        return

    # ----------------------------------------------------------
    # Enable compressor
    # ----------------------------------------------------------
    liblo.send(target, EffectString, 6, 1)

    # Down compression
    liblo.send(target, EffectString, 23, 0)

    # Input / output / makeup at unity
    liblo.send(target, EffectString, 7, 1.0)
    liblo.send(target, EffectString, 8, 1.0)
    liblo.send(target, EffectString, 33, 1.0)

    # Soft knee
    liblo.send(target, EffectString, 30, 0.50)

    # No hold
    liblo.send(target, EffectString, 28, 0.0)

    # Dry/Wet blend
    liblo.send(target, EffectString, 34, 0.65)   # Dry
    liblo.send(target, EffectString, 35, 1.0)    # Wet
    liblo.send(target, EffectString, 36, 35.0)   # 35% wet

    if (Mode == "Jazz"):
        # Gentle, smooth compression
        liblo.send(target, EffectString, 24, 0.25)   # Attack threshold
        liblo.send(target, EffectString, 25, 15.0)   # Attack
        liblo.send(target, EffectString, 26, 0.0)    # Release threshold
        liblo.send(target, EffectString, 27, 150.0)  # Release
        liblo.send(target, EffectString, 29, 2.0)    # 2:1

    elif (Mode == "Hollowbody"):
        # Slightly more control and sustain
        liblo.send(target, EffectString, 24, 0.20)   # Attack threshold
        liblo.send(target, EffectString, 25, 20.0)   # Attack
        liblo.send(target, EffectString, 26, 0.0)    # Release threshold
        liblo.send(target, EffectString, 27, 180.0)  # Release
        liblo.send(target, EffectString, 29, 2.5)    # 2.5:1

    else:
        print("Unknown compressor mode:", Mode)
        
        
def WayLo(Mode):

    print("WayLo " + Mode)
    WayChorusFunk = [ 
        0.0316, # 1 Delay time
        0.85, # Gain 
        0.75, # 1 Gain
        0.11, # 1 Depth
        0.47, # 1 Rate
        0.00, # 1 Feedback

        0.0226, # 2 Delay time
        0.75, # 2 Gain
        0.11, # 2 Depth
        0.48, # 2 Rate
        0.00, # 2 Feedback
        
        0.04, # 3 Delay time
        0.75, # 3 Gain
        0.90, # 3 Depth
        0.62, # 3 Rate
        0.00, # 3 Feedback
        
        0.048, # 4 Delay time
        0.75, # 4 Gain
        0.90, # 4 Depth
        0.71, # 4 Rate
        0.00, # 4 Feedback
        
        0.25, # 5 Delay time
        0.375, # 5 Gain
        0.10, # 5 Depth
        0.33, # 5 Rate
        0.50, # 5 Feedback
        
        0.30, # 6 Delay time
        0.375, # 6 Gain
        0.104, # 6 Depth
        0.42, # 6 Rate
        0.40, # 6 Feedback
        
        0.30, # 7 Delay time
        0.375, # 7 Gain
        0.104, # 7 Depth
        0.35, # 7 Rate
        0.10, # 7 Feedback
        
        0.35, # 8 Delay time
        0.375, # 8 Gain
        0.14, # 8 Depth
        0.35, # 8 Rate
        0.30, # 8 Feedback
        
        1.00, # Wet Gain
        1.00, # Global Rate
        1.00 # Global Depth
        ]
        
    WayLead4 = [ 
        0.0316, # 1 Delay time
        0.85, # Gain 
        0.85, # 1 Gain
        0.00, # 1 Depth
        0.00, # 1 Rate
        0.00, # 1 Feedback

        0.045, # 2 Delay time
        0.85, # 2 Gain
        0.00, # 2 Depth
        0.00, # 2 Rate
        0.00, # 2 Feedback
        
        0.13, # 3 Delay time
        0.425, # 3 Gain
        0.0, # 3 Depth
        0.0, # 3 Rate
        0.0, # 3 Feedback
        
        0.00, # 4 Delay time
        0.00, # 4 Gain
        0.00, # 4 Depth
        0.00, # 4 Rate
        0.00, # 4 Feedback
        
        0.30, # 5 Delay time
        0.34, # 5 Gain
        0.00, # 5 Depth
        0.00, # 5 Rate
        0.13, # 5 Feedback
        
        0.40, # 6 Delay time
        0.34, # 6 Gain
        0.00, # 6 Depth
        0.00, # 6 Rate
        0.10, # 6 Feedback
        
        0.355, # 7 Delay time
        0.34, # 7 Gain
        0.00, # 7 Depth
        0.00, # 7 Rate
        0.08, # 7 Feedback
        
        0.455, # 8 Delay time
        0.34, # 8 Gain
        0.00, # 8 Depth
        0.00, # 8 Rate
        0.05, # 8 Feedback
        
        1.00, # Wet Gain
        1.00, # Global Rate
        1.00 # Global Depth
        ]

    WayChorus8 = [ 
        0.0316, # 1 Delay time
        0.85, # Gain 
        0.75, # 1 Gain
        0.11, # 1 Depth
        0.47, # 1 Rate
        0.00, # 1 Feedback

        0.0226, # 2 Delay time
        0.75, # 2 Gain
        0.11, # 2 Depth
        0.48, # 2 Rate
        0.00, # 2 Feedback
        
        0.040, # 3 Delay time
        0.73, # 3 Gain
        0.90, # 3 Depth
        0.62, # 3 Rate
        0.0, # 3 Feedback
        
        0.048, # 4 Delay time
        0.75, # 4 Gain
        0.90, # 4 Depth
        0.71, # 4 Rate
        0.00, # 4 Feedback
        
        0.212, # 5 Delay time
        0.375, # 5 Gain
        0.10, # 5 Depth
        0.38, # 5 Rate
        0.50, # 5 Feedback
        
        0.225, # 6 Delay time
        0.375, # 6 Gain
        0.104, # 6 Depth
        0.42, # 6 Rate
        0.40, # 6 Feedback
        
        0.226, # 7 Delay time
        0.375, # 7 Gain
        0.104, # 7 Depth
        0.35, # 7 Rate
        0.10, # 7 Feedback
        
        0.240, # 8 Delay time
        0.375, # 8 Gain
        0.104, # 8 Depth
        0.35, # 8 Rate
        0.30, # 8 Feedback
        
        1.00, # Wet Gain
        0.20, # Global Rate
        0.20 # Global Depth
        ]
        
    theArray=WayChorus8

    if (Mode == "FunkyStrat"):
        theArray=WayChorusFunk

    if (Mode == "Jazz"):
        theArray=WayChorus8

    if (Mode == "WayLead4"):
        theArray=WayChorus8

    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Waylo.value)
    Loop = 0
    for index in theArray:
        liblo.send(target, EffectString, Loop, index)
        print("WayLo ", index, Loop)
        Loop = Loop + 1
        

def Distortion2(Mode):
    print("Distortion2 " + Mode)
    
    DistLevel=-3.22
    DistTone=314.0
    DistDrive=0.43
    DistByPass=1

    if (Mode == "FunkyStrat"):
 #       theArray = FunkyStrat
        DistLevel=-1.5
        DistTone=550.0
        DistDrive=0.08
        DistByPass=0

    if (Mode == "Boston"):
        DistLevel=4.0
        DistTone=560.0
        DistDrive=0.12
        DistByPass=0
        
        
    # Level -20 to 4 
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion2.value)
    liblo.send(target, EffectString, 0, DistLevel)

    # Tone 100 - 1000
    liblo.send(target, EffectString, 1, DistTone)

    # Drive 0 - 1 
    liblo.send(target, EffectString, 2, DistDrive)

    # Bypass
    liblo.send(target, EffectString, 3, DistByPass)



def EQControl(Mode):
    print("EQControl " + Mode)
    
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Equalizer.value)
#                        16   25     40     63  100    160   250  400   630   1000  1600  2500  4000  6300  10000 16000 
    WayloValues =     [ 1.00, 1.00, 1.00, 1.00, 0.70, 1.00, 1.00, 1.00, 2.10, 1.30, 0.44, 0.44, 0.80, 0.98, 0.97, 1.00 ]
    HigherValues =     [ 1.00, 1.00, 1.00, 1.00, 0.70, 1.00, 1.00, 1.00, 2.10, 1.30, 0.80, 0.80, 0.80, 0.98, 0.97, 1.00 ]
    NormalValues =     [ 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 ]
    AcousticValues =   [ 0.53, 0.74, 1.00, 0.71, 1.40, 0.98, 0.98, 0.88, 0.40, 0.33, 0.78, 1.00, 1.50, 2.33, 2.68, 2.50 ]
    TestValues =       [ 1.05, 1.10, 1.15, 1.20, 1.25, 1.30, 1.35, 1.40, 1.45, 1.50, 1.45, 1.40, 1.35, 1.30, 1.25, 1.20 ]
    Normal1Values =    [ 1.00, 1.00, 1.00, 1.00, 0.70, 1.30, 1.35, 0.50, 0.50, 0.65, 0.63, 0.55, 0.80, 1.45, 1.20, 1.00 ]

    MellowValues = [ 
        1.00, # 16
        1.00, # 25
        1.00, # 40
        1.00, # 63
        1.50, # 100
        1.56, # 160
        1.28, # 250 
        1.00, # 400 
        0.67, # 630
        0.20, # 1000
        0.21, # 1600
        0.23, # 2500
        0.22, # 4000
        0.40, # 6300
        0.70, # 10000
        0.68  # 16000 
        ]

    MellowValues2 = [ 
        1.00, # 16
        1.00, # 25
        1.00, # 40
        1.00, # 63
        1.50, # 100
        1.33, # 160
        1.28, # 250 
        1.00, # 400 
        0.67, # 630
        0.50, # 1000
        0.46, # 1600
        0.46, # 2500
        0.44, # 4000
        0.45, # 6300
        0.48, # 10000
        1.05  # 16000 
        ]

    MellowValues1 = [ 
        1.00, # 16
        1.00, # 25
        1.00, # 40
        1.00, # 63
        1.50, # 100
        1.50, # 160
        1.00, # 250 
        0.81, # 400 
        0.75, # 630
        0.50, # 1000
        0.40, # 1600
        0.34, # 2500
        0.46, # 4000
        0.60, # 6300
        0.83, # 10000
        1.20  # 16000 
        ]

    HollowbodyValues = [
        1.00, # 16
        1.00, # 25
        1.00, # 40
        1.00, # 63
        1.45, # 100 - body
        1.55, # 160 - warmth
        1.35, # 250 - hollowbody low mids
        1.10, # 400 - body
        0.82, # 630 - keep mud controlled
        0.55, # 1000
        0.42, # 1600 - soften pick attack
        0.36, # 2500 - soften attack/presence
        0.40, # 4000
        0.50, # 6300
        0.62, # 10000
        0.55  # 16000 - rolled off
        ]

    MellowValuesBright = [ 
        1.00, # 16
        1.00, # 25
        1.00, # 40
        1.00, # 63
        1.50, # 100
        1.50, # 160
        1.00, # 250 
        0.81, # 400 
        0.75, # 630
        0.50, # 1000
        0.40, # 1600
        0.40, # 2500
        0.65, # 4000
        0.83, # 6300
        1.9, # 10000
        1.50  # 16000 
        ]

    FunkyStrat = [ 
        1.00, # 16
        0.70, # 25
        0.80, # 40
        0.85, # 63
        0.90, # 100
        0.85, # 160
        0.75, # 250 
        0.65, # 400 
        0.80, # 630
        1.30, # 1000
        1.50, # 1600
        1.40, # 2500
        1.15, # 4000
        1.00, # 6300
        0.95, # 10000
        0.90  # 16000 
        ]

    BostonValues = [ 
        0.90, # 16
        0.92, # 25
        0.94, # 40
        0.97, # 63
        1.02, # 100
        1.06, # 160
        1.00, # 250
        0.92, # 400
        0.96, # 630
        1.08, # 1000
        1.18, # 1600
        1.28, # 2500
        1.22, # 4000
        1.10, # 6300
        0.90, # 10000
        0.72  # 16000
        ]
    
    JazzBellValues = [
        1.00,  # 16 Hz
        1.00,  # 25 Hz
        1.00,  # 40 Hz
        1.00,  # 63 Hz
        1.45,  # 100 Hz
        1.50,  # 160 Hz
        1.25,  # 250 Hz
        1.00,  # 400 Hz
        0.75,  # 630 Hz
        0.55,  # 1000 Hz
        0.65,  # 1600 Hz
        0.72,  # 2500 Hz
        1.00,  # 4000 Hz
        1.00,  # 6300 Hz
        1.00,  # 10000 Hz
        1.00   # 16000 Hz
    ]

    if (Mode == "Normal"):
        theArray = Normal1Values
        print(Mode)

    if (Mode == "JazzBell"):
        theArray = JazzBellValues
        print(Mode)
    
    if (Mode == "Acoustic"):
        theArray = AcousticValues
        print(Mode)

    if (Mode == "Waylo"):
        theArray = WayloValues
        print(Mode)

    if (Mode == "Jazz"):
        theArray = MellowValues1
        print(Mode)

    if (Mode == "Hollowbody"):
        theArray = HollowbodyValues
        print(Mode)

    if (Mode == "Blues"):
        theArray = HigherValues
        print(Mode)

    if (Mode == "FunkyStrat"):
        theArray = FunkyStrat
        print(Mode)

    if (Mode == "Boston"):
        theArray = BostonValues
        print(Mode)
        
        
    Loop = 0
    for index in theArray:
        liblo.send(target, EffectString, (17 + (Loop * 5)), index)
        print("EQ ", index,  (17 + (Loop * 5)))
        Loop = Loop + 1

    # Make zero
#    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Compressor2.value)
#    liblo.send(target, EffectString, 62, (Value))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def ResetToDefault():
    # Reset all effects to their default values
    RestoreGateCompressor()
    DistortionControl(0)
    WahControl(0)
    ChorusControl(15)

    # Reset wah effect
    EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
    liblo.send(target, EffectString, 0.0)
    print(EffectString)

    # Set default EQ values
    # Bass
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
    liblo.send(target, EffectString, 5, 0.6)

    # Mid
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
    liblo.send(target, EffectString, 4, 0.54)

    # Treble
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
    liblo.send(target, EffectString, 6, 0.47)

#    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Toob.value)
#    liblo.send(target, EffectString, 1, 0.5)

    # Dry Wet for Gate
 #   EffectString = "/Carla/%d/set_parameter_value" % (Effect.ABGate.value)
 #   liblo.send(target, EffectString, 30, 1.0)

    # Dry Wet for Compressor
#    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Compressor2.value)
#    liblo.send(target, EffectString, 6, 1.0)

    # Set amp characteristics
#    EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
#    liblo.send(target, EffectString, 9, 10.0)  # pre 12aux7 FeedPush 
#    liblo.send(target, EffectString, 9, 10.0)  # pre 12aux7 FeedPush #    liblo.send(target, EffectString, 10, 12.0)  #  JTM-45
#    liblo.send(target, EffectString, 11, 18.0)  # Mesa Style

#    EffectString = "/Carla/%d/set_drywet" % (Effect.Mojo.value)
#    print(EffectString)
#    EffectString = "/Carla/%d/set_drywet" % (3)
#    liblo.send(target, EffectString, 0.0)
#    print(EffectString)


def WayLoBoston():
    print("WayLo Boston Solo")

    # ----------------------------------------------------------
    # Boston first-album style guitar doubler / harmony width
    #
    # Short delays + very small modulation.
    # The idea is multiple guitar voices rather than obvious
    # "chorus pedal" modulation.
    #
    # Each voice:
    #   Delay
    #   Gain
    #   Depth
    #   Rate
    #   Feedback
    # ----------------------------------------------------------

    WayBoston = [
        # Voice 1 - very close first double
        0.026,       # Delay ~26 ms
        0.82,        # Gain
        0.035,       # Depth
        0.30,        # Rate
        0.00,        # Feedback

        # Voice 2 - opposite side / slightly later
        0.038,       # Delay ~38 ms
        0.72,        # Gain
        0.045,       # Depth
        0.38,        # Rate
        0.00,        # Feedback

        # Voice 3 - adds width
        0.055,       # Delay ~55 ms
        0.48,        # Gain
        0.030,       # Depth
        0.25,        # Rate
        0.00,        # Feedback

        # Voice 4 - very subtle
        0.070,
        0.32,
        0.025,
        0.32,
        0.00,

        # Voices 5-8 - essentially disabled
        0.00, 0.00, 0.00, 0.00, 0.00,
        0.00, 0.00, 0.00, 0.00, 0.00,
        0.00, 0.00, 0.00, 0.00, 0.00,
        0.00, 0.00, 0.00, 0.00, 0.00
    ]

    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Waylo.value)

    Loop = 0
    for value in WayBoston:
        liblo.send(target, EffectString, Loop, value)
        Loop += 1

    # Bring the effect in
    EffectString = "/Carla/%d/set_drywet" % (Effect.Waylo.value)
    liblo.send(target, EffectString, 0.32)

    # Unity-ish output level
#    EffectString = "/Carla/%d/set_volume" % (Effect.Waylo.value)
#    liblo.send(target, EffectString, 0.90)

def BostonSetup():
    print("BostonSetup")

    # ----------------------------------------------------------
    # Tom Scholz / Boston-style amp
    #
    # GxAmplifier-X parameters:
    # 0 MasterGain
    # 1 PreGain
    # 2 Distortion
    # 3 Drive
    # 4 Middle
    # 5 Bass
    # 6 Treble
    # 8 Presence
    # 10 Tonestack Model
    # 12 BYPASS
    # ----------------------------------------------------------

    EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)

    liblo.send(target, EffectString, 0, -15.0)   # Master
    liblo.send(target, EffectString, 1, -4.0)    # PreGain
    liblo.send(target, EffectString, 2, 35.0)    # Distortion
    liblo.send(target, EffectString, 3, 0.32)    # Drive
    liblo.send(target, EffectString, 4, 0.63)    # Mid
    liblo.send(target, EffectString, 5, 0.46)    # Bass
    liblo.send(target, EffectString, 6, 0.66)    # Treble
    liblo.send(target, EffectString, 8, 7.0)     # Presence
    liblo.send(target, EffectString, 10, 16)     # Tonestack Model
    liblo.send(target, EffectString, 12, 0)      # Amp ON

    # ----------------------------------------------------------
    # Tube Screamer
    # ----------------------------------------------------------

    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion2.value)

    liblo.send(target, EffectString, 0, 4.0)     # Level
    liblo.send(target, EffectString, 1, 560.0)   # Tone
    liblo.send(target, EffectString, 2, 0.12)    # Drive
    liblo.send(target, EffectString, 3, 0)       # Bypass OFF

    # ----------------------------------------------------------
    # Disable the MuTroMojo / auto-wah
    # ----------------------------------------------------------

    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Mojo.value)
    liblo.send(target, EffectString, 0, 1)

    EffectString = "/Carla/%d/set_drywet" % (Effect.Mojo.value)
    liblo.send(target, EffectString, 0.0)

    # No chorus/modulation for the basic Boston sound
    ChorusControl(0)
    
    
#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
# Utility functions for value scaling
def rescale(val, in_min, in_max, out_min, out_max):
    # Linear scaling function
    return out_min + (val - in_min) * ((out_max - out_min) / (in_max - in_min))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def rescaleMidi(val, out_min, out_max):
    # Scale MIDI value (0-127) to target range
    return out_min + (val) * ((out_max - out_min) / (127))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def rescaleMidiL(val, out_min, out_max):
    # Scale MIDI value with logarithmic curve
    NormValue = rescaleMidi(val, out_min, out_max)
    LogValue = NormValue / (1 + (1 - NormValue) * 2)
    print("Norm ", NormValue, "Log ",LogValue)
    return (LogValue)
    
#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
   
def TwinPitch(preset, wet_dry):          
    print("TwinPitch: preset=%d wet/dry=%.2f" %
          (preset, wet_dry))

    # TwinPitch uses LV2 Atom controls rather than normal
    # Carla numeric parameters, so select the factory preset.
    #
    # TwinPitch factory preset:
    #   "Harmonizer - 4th-6th" = program 15
    #
    # tone and scale are retained in the function signature so
    # the preset interface can be extended later if needed.
    #
    # tone:
    #   0 = C
    #   1 = C#
    #   2 = D
    #   ...
    #   11 = B
    #
    # scale:
    #   0 = Major
    #   1 = Minor
    #   2 = Harmonic Minor


    EffectString = "/Carla/%d/set_program" % (Effect.TwinPitch.value)

    # Select TwinPitch factory preset
    liblo.send(target, EffectString, preset)

    EffectString = "/Carla/%d/set_drywet" % (Effect.TwinPitch.value)
    liblo.send(target, EffectString, wet_dry)

    
def RestoreGateCompressor():
    """Restore the current/default gate and compressor settings.

    This restores the values currently used by CarlaPresets.py:
      Gate:
        dry/wet = 100%
        parameter 8 = 0 ms

       Compressor:
        dry/wet = 28%
    """
    print("RestoreGateCompressor")

    # ----------------------------------------------------------
    # Gate
    # ----------------------------------------------------------
    EffectString = "/Carla/%d/set_drywet" % (Effect.ABGate.value)
    liblo.send(target, EffectString, 1.0)

    # Restore the current GateControl() parameter-8 value.
    # GateControl(0) maps MIDI 0 -> 0 ms.
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.ABGate.value)
    liblo.send(target, EffectString, 8, 0.0)

    # ----------------------------------------------------------
    #  Compressor
    # ----------------------------------------------------------
    CompressorControl(28)   # ON
    
def HollowbodySetup():
    print("HollowbodySetup")

    ResetToDefault()
    DistortionControl(0)

    CompressorControl("Hollowbody")

    EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
    liblo.send(target, EffectString, 0, -20.0)
    liblo.send(target, EffectString, 1, -7.5)
    liblo.send(target, EffectString, 2, 0.0)
    liblo.send(target, EffectString, 3, 0.0)
    liblo.send(target, EffectString, 4, 0.60)
    liblo.send(target, EffectString, 5, 0.66)
    liblo.send(target, EffectString, 6, 0.38)
    liblo.send(target, EffectString, 8, 6.0)

    ChorusControl(45)
    EQControl("Hollowbody")
    WayLo("Jazz")
    
def main():
    global target
    global server
    global err
    global GuitarMidiVar

    GuitarMidiVar = 1
    # Initialize OSC communication
    try:
        target = liblo.Address(22752)
    except liblo.AddressError as err:
        print("opening 22752", str(err))
        sys.exit()

#    liblo.send(target, "/foo/message1", 123, 456.789, "test")
    # Setup OSC server
    try:
        server = liblo.Server(1234)
    except liblo.ServerError as err:
        print("opening 1234", str(err))
        sys.exit()

    # Register OSC message handlers
    # register method taking an int and a float
    server.add_method("/foo/bar", 'if', foo_bar_callback)

    # register method taking a blob, and passing user data to the callback
    server.add_method("/foo/baz", 'b', foo_baz_callback, "blah")

    # register a fallback for unhandled messages
    server.add_method(None, None, fallback)

    # loop and dispatch messages every 100ms
#    while True:
#        server.recv(100)

    # Setup virtual MIDI port
    port = mido.open_output('SwitchOut', virtual=True, client_name='LvSwitch')
    msg = mido.Message('note_on', note=45, velocity=90, channel=1)
    msg = mido.Message('program_change', program=45, channel=1)
    port.send(msg)

    # Print the effects list.
    print(Effect.ABGate.value, Effect.Equalizer.value, Effect.PreAmp.value)
    print(Effect.Distortion1.value, Effect.Distortion2.value, )
    print(Effect.Wah.value, Effect.Waylo.value, Effect.Reverb.value)
    print(Effect.Mixer.value, Effect.Tuner.value, Effect.MidiConvert.value)

#    ResetToDefault()
#    CompressorControl(1)
    MidiGuitar(1)

    EffectString = "/Carla/%d/set_active" % (Effect.Tuner.value)
    liblo.send(target, EffectString, 0)

    EffectString = "/Carla/%d/set_active" % (Effect.MidiConvert.value)
    liblo.send(target, EffectString, 0)

    # Main program loop - handle MIDI input
    with mido.open_input('SwitchIn', virtual=True, client_name='LvSwitch') as inport:

        for msg in inport:
            print("**** msg *****************")
#            print(msg, msg.type, msg.program)

            # Handle program changes (preset switches)
            if (msg.type == "program_change"):

                match msg.program:
                    case 0:
                        print("Jazz")
                        ResetToDefault()
                        CompressorControl("Jazz")
                        ChorusControl(127)
                        DistortionControl(0)
                        EQControl("JazzBell")
                        WayLo("Jazz")
                        TwinPitch(9, 0.50)   
                        
                        # ChorusControl(127)
                        # DistortionControl(0)
                        # EQControl("JazzBell")
                        # WayLo("Jazz")
                        # TwinPitch(9, 0.35)

                    case 1:
                        print("Straight")
                        ResetToDefault()
                        ChorusControl(25)
                        DistortionControl(0)
                        EQControl("Normal")

                    case 2:
                        print("Blues")
                        ResetToDefault()
                        DistortionControl(70)
                        ChorusControl(25)
                        EQControl("Waylo")

                    case 3:
                        print("Acoustic")
                        ResetToDefault()
                        
                        # Bass 0 - 1
                        Value=rescale(0.75, 0.0, 1.0, 0.0, 1.0)
                        EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
                        liblo.send(target, EffectString, 5, (Value))
                        print(EffectString, Value)
                        EQControl("Acoustic")

                        # Mid 0 - 1
                        Value=rescale(0.0, 0.0, 1.0, 0.0, 1.0)
                        EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
                        liblo.send(target, EffectString, 4, (Value))

                        # Treble 0 - 1
                        Value=rescale(1.0, 0.0, 1.0, 0.0, 1.0)
                        EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
                        liblo.send(target, EffectString, 6, (Value))


                    case 4:
                        print("Juicy")
                        ResetToDefault()

                    case 5:
                        print("FunkyStrat")
                        ResetToDefault()
                        EQControl("FunkyStrat")
                        Distortion2("FunkyStrat")                    
                        WahSetup("FunkyStrat")
                        ReverbSetup("FunkyStrat")
                        Dist1Setup("FunkyStrat")
                        WayLo("Jazz")
#                        WayLo("FunkyStrat")


                    # case 5:
                    #     print("Funky")
                    #     ResetToDefault()                        
                    #     EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
                    #     Value=rescale(1.0, 0.0, 1.0, 0.0, 1.0)
                    #     liblo.send(target, EffectString, (Value) )

                    case 6:
                        print("FlangeD_Pre")

                    case 7:
                        print("Clean")

                    case 8:
                        print("AutoWah")
                        # EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
                        # liblo.send(target, EffectString, 1.0)
                        WahControl(100)
                        # EffectString = "/Carla/%d/set_parameter_value" % (Effect.Toob.value)
                        # liblo.send(target, EffectString, 1, 0.2)

                    case 9:
                        print("Lead")

                    case 10:
                        print("EchoLead")

                    case 11:
                        print("Holdsworth")

                    case 12:
                        print("ThickDist")
                        DistortionControl(96)
                        ChorusControl(60)
                        WayLo("ThickDist")
                        EQControl("Boston")


                    case 13:
                        print("GuiarMidi13")
                        MidiGuitar(0)

                    case 14:
                        print("GuiarMidi14")
                        MidiGuitar(3)

                    case 15:
                        print("Waylo")
                        ResetToDefault()
                        ChorusControl(127)
                        DistortionControl(0)
                        EQControl("Waylo")
                    
                    case 16:
                        print("Boston")

                        ResetToDefault()

                        BostonSetup()
                        EQControl("Boston")
                        Distortion2("Boston")
                        WayLoBoston()
                        TwinPitch(15,1.0)
                        ReverbSetup("Boston")
                        
                        # Make absolutely sure wah is bypassed
                        EffectString = "/Carla/%d/set_parameter_value" % (Effect.Wah.value)
                        liblo.send(target, EffectString, 0, 1)
                        
                        EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
                        liblo.send(target, EffectString, 0.0)
    

                    case 17:
                        print("Hollowbody")
                        HollowbodySetup()

                    case _:
                        print("Found Nothing")

            # Handle control changes (continuous controllers)
            if (msg.type == "control_change"):
#                print(msg.value)
                InitLevel=msg.value
                match msg.control:
                    case 2:     
                        print("Distortion1")
                        DistortionControl(InitLevel)

                        # oscsend localhost 22752 /Carla/3/set_parameter_value if 3 90.0
                        # EffectString = "/Carla/%d/set_volume" % (Effect.Distortion1.value)

                        # Value=rescaleMidi(InitLevel, 0.75, 0.25)
#                        print(EffectString," Vol ",Value)
                        # liblo.send(target, EffectString, Value )

                        # # Dist 0 - 100
                        # EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion1.value)
                        # Value=rescaleMidi(InitLevel, 0.0, 90)
                        # liblo.send(target, EffectString, 2, Value)
#                        print(EffectString)

                        # Drive 0 - 1
                        # EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion1.value)
                        # Value=rescaleMidi(InitLevel, 0.0, 1.0)
                        # liblo.send(target, EffectString, 3, Value)
#                        print(EffectString)

                    case 7:     
                        print("Volume")

                    case 11:     
                        print("Wah ",Effect.Wah.value,  InitLevel)
                        WahControl(InitLevel)

                    case 12:     
                        print("Corus ",Effect.Waylo.value,  InitLevel)
                        ChorusControl(InitLevel)

                    case _:
                        print("Found Nothing")

    # liblo.send(target, "/foo/message1", 123, 456.789, "test")




if __name__ == '__main__':
    main()
