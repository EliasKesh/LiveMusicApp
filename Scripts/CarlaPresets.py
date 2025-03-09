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
        Compressor2 = auto() # Dynamic range compressor
        Equalizer = auto()    # Tube screamer
        PreAmp = auto()      # Amplifier preamp simulation
        Distortion1 = PreAmp  # Alias for PreAmp
#        Conditioner = auto()      # Amplifier preamp simulation
#        Toob = auto()      # Amplifier preamp simulation
        Distortion2 = auto()    # Tube screamer
        Mojo = auto()        # Tone shaping effect
        Wah = Mojo          # Alias for Mojo (Wah effect)
        Chorus = auto()      # Modulation effect
        Reverb = auto()      # Reverb effect
        Squish = auto()      # Compression effect
        Mixer = auto()       # Audio mixer
        Tuner = auto()       # Guitar tuner
        GxDelay = auto()     # Delay effect
        DubDelay = auto()    # Additional delay
        FlangeChorus = auto() # Combined flanger/chorus

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
    print( "received message '%s' with arguments '%d' and '%f'" % (path, i, f))

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
        print( "argument of type '%s': %s" % (t, LSPMixerx4Stereoa))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
# Effect control functions
def DistortionControl(level):
    # Control Distortion parameters based on input level (0-127)
    InitLevel=level
    
    # Set Distortion volume
    EffectString = "/Carla/%d/set_volume" % (Effect.Distortion1.value)
#    Value=(0.7 * rescaleMidiL(InitLevel, 1.0, 0.20)) + 0.2
    Value=rescale(InitLevel, 128.0, 0.0, 0.6, 1.25)
    liblo.send(target, EffectString, Value )

    # Set Distortion amount (0-100)
    Value=rescaleMidi(InitLevel, 0.0, 100.0)
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion1.value)
    liblo.send(target, EffectString, 2, (Value))

    # Set drive level (0-1)
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
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
    Value=rescaleMidi(InitLevel, 0.0, 0.75)
    EffectString = "/Carla/%d/set_drywet" % (Effect.Distortion2.value)
    liblo.send(target, EffectString, Value)
    print("WahControl ", Value)

#  Parameter Value 0 Master Out
#  Parameter Value 1 Pregain


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

def ChorusControl(level):
    InitLevel=level
    EffectString = "/Carla/%d/set_drywet" % (Effect.Chorus.value)
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    liblo.send(target, EffectString, Value)

    InitLevel=level
    EffectString = "/Carla/%d/set_drywet" % (Effect.FlangeChorus.value)
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    liblo.send(target, EffectString, Value)

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

def CompressorControl(level):
    InitLevel=level
    EffectString = "/Carla/%d/set_drywet" % (Effect.Compressor2.value)
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    liblo.send(target, EffectString, 0)

    EffectString = "/Carla/%d/set_parameter_value" %  (Effect.Compressor2.value)
    liblo.send(target, EffectString, 23, 55.0)
    EffectString = "/Carla/%d/set_parameter_value" %  (Effect.Compressor2.value)
    liblo.send(target, EffectString, 25, 170.0)
    EffectString = "/Carla/%d/set_parameter_value" %  (Effect.Compressor2.value)
    liblo.send(target, EffectString, 27, 608.0)
    EffectString = "/Carla/%d/set_parameter_value" %  (Effect.Compressor2.value)
    liblo.send(target, EffectString, 29, 1976.0)

    InitLevel = 60.0
    BaseFreq=36
    EffectString = "/Carla/%d/set_parameter_value" %  (Effect.Compressor2.value)
    liblo.send(target, EffectString, (BaseFreq+6), 0.04211)
    liblo.send(target, EffectString, (BaseFreq+20), InitLevel)
    liblo.send(target, EffectString, (BaseFreq+24), 1.02)

    BaseFreq=64
    liblo.send(target, EffectString, (BaseFreq+6), 1.165)
    liblo.send(target, EffectString, (BaseFreq+20), InitLevel)
    liblo.send(target, EffectString, (BaseFreq+24), 0.35)

    BaseFreq=92
    liblo.send(target, EffectString, (BaseFreq+6), 0.244)
    liblo.send(target, EffectString, (BaseFreq+20), InitLevel)
    liblo.send(target, EffectString, (BaseFreq+24), 0.23)

    BaseFreq=120
    liblo.send(target, EffectString, (BaseFreq+6), 0.24)
    liblo.send(target, EffectString, (BaseFreq+20), InitLevel)
    liblo.send(target, EffectString, (BaseFreq+24), 0.42)

    BaseFreq=148
    liblo.send(target, EffectString, (BaseFreq+6), 0.04)
    liblo.send(target, EffectString, (BaseFreq+20), InitLevel)
    liblo.send(target, EffectString, (BaseFreq+24), 1.9)



    # Make zero
#    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Compressor2.value)
#    liblo.send(target, EffectString, 62, (Value))


#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def ResetToDefault():
    # Reset all effects to their default values
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
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.ABGate.value)
    liblo.send(target, EffectString, 30, 1.0)

    # Dry Wet for Compressor
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Compressor2.value)
    liblo.send(target, EffectString, 6, 1.0)

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
def main():
    global target
    global server
    global err

    # Initialize OSC communication
    try:
        target = liblo.Address(22752)
    except (liblo.AddressError, err):
        print(str(err))
        sys.exit()

#    liblo.send(target, "/foo/message1", 123, 456.789, "test")
    # Setup OSC server
    try:
        server = liblo.Server(1234)
    except (liblo.ServerError, err):
        print(str(err))
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
    print(Effect.ABGate.value, Effect.Compressor2.value, Effect.PreAmp.value, Effect.Distortion1.value)
    print(Effect.Mojo.value, Effect.Wah.value, Effect.Chorus.value, Effect.Reverb.value)
    print(Effect.Mixer.value)

#    liblo.send(target, "/Carla/3/set_drywet", 1.0 )
#    liblo.send(target, "/Carla/3/set_program", 5 )
     
#    exit
#    ResetToDefault()

#    CompressorControl(1)

    # Main program loop - handle MIDI input
    with mido.open_input('SwitchIn', virtual=True, client_name='LvSwitch') as inport:

        for msg in inport:
            print(msg)
#            print(msg.type)

            # Handle program changes (preset switches)
            if (msg.type == "program_change"):
#                print(msg.program)
                ResetToDefault()

                match msg.program:
                    case 0:
                        print("Jazz")
                        ChorusControl(127)
                        DistortionControl(0)

                        # increase the bass
                        # EffectString = "/Carla/%d/set_parameter_value" % (Effect.Toob.value)
                        # liblo.send(target, EffectString, 1, 0.65)


                    case 1:
                        print("Straight")
                        ChorusControl(25)
                        DistortionControl(0)

                    case 2:
                        print("Blues")
                        DistortionControl(82)
                        ChorusControl(25)

                    case 3:
                        print("Acoustic")
                        
                        # Bass 0 - 1
                        Value=rescale(0.75, 0.0, 1.0, 0.0, 1.0)
                        EffectString = "/Carla/%d/set_parameter_value" % (Effect.PreAmp.value)
                        liblo.send(target, EffectString, 5, (Value))
                        print(EffectString, Value)

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

                    case 5:
                        print("Funky")
                        EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
                        Value=rescale(1.0, 0.0, 1.0, 0.0, 1.0)
                        liblo.send(target, EffectString, (Value) )

                    case 6:
                        print("FlangeD_Pre")

                    case 7:
                        print("Clean")

                    case 8:
                        print("AutoWah")
                        EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
                        liblo.send(target, EffectString, 1.0)
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
                        print("Corus ",Effect.Chorus.value,  InitLevel)
                        ChorusControl(InitLevel)

                    case _:
                        print("Found Nothing")

    # liblo.send(target, "/foo/message1", 123, 456.789, "test")




if __name__ == '__main__':
    main()



# Parse Carla File.
# sed -n '/\<Info/,/\/Info/{//!p;}' NewGuitarix.carxp | grep -oP '(?<=<Name>).*(?=</Name)' | tr -d '[:blank:]'
# line=line.strip()
# print(MyEnum[line].value)

# GT10QM
# GxCompressor
# GxAmplifier-Stereo-X
# rkrMuTroMojo
# GxChorus-Stereo
# rkrWahWah
# GxZita_rev1-Stereo
# theinfamouslushlife
# PurestSquish
# LSPMixerx4Stereo
# PreAmps
# LSPProfilerStereo
# MultivoiceChorus




# EffectString = "/Carla/%d/set_volume" % (Effect.Distortion1.value)
# liblo.send(target, EffectString, (1.0 - (msg.value/200)) )
# EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion1.value)
# liblo.send(target, EffectString, 2, (msg.value * 0.9))
# EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
# liblo.send(target, EffectString, 1.0)

# oscsend localhost 22752 /Carla/1/set_parameter_value if 43 3
# oscsend localhost 22752 /Carla/1/set_drywet f 1.0



