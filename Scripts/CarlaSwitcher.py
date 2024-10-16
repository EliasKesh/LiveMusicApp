#!/home/ebin/python3
#--------------------------------------------------------------
#
#	File: 	CarlaSwitcher
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


import liblo
import mido
import sys

from enum import auto, Enum     # for enum34, or the stdlib version

class Effect(Enum):
        ABGate = 0
        Compressor = auto()
        PreAmp = auto()
        Distortion = PreAmp
        Mojo = auto()
        Chorus = auto()
        Wah = auto()
        Reverb = auto()
#        Stereo = auto()
#        Lush = auto()
        Squish = auto()
        Mixer = auto()
 
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

# for testing.
# oscdump 1234
# Chttps://github.com/brummer10/SpecMatch.git
#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def foo_bar_callback(path, args):
    i, f = args
    print( "received message '%s' with arguments '%d' and '%f'" % (path, i, f))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def foo_baz_callback(path, args, types, src, data):
    print( "received message '%s'" % path)
    print( "blob contains %d bytes, user data was '%s'" % (len(args[0]), data))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def fallback(path, args, types, src):
    print( "got unknown message '%s' from '%s'" % (path, src.url))
    for a, t in zip(args, types):
        print( "argument of type '%s': %s" % (t, a))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def DistortionControl(level):

    # 0.0 to 1.0
    InitLevel=level
    EffectString = "/Carla/%d/set_volume" % (Effect.Distortion.value)
    Value=rescaleMidi(InitLevel, 0.80, 0.20)
#                        print(EffectString," Vol ",Value)
    liblo.send(target, EffectString, Value )
#    print("DistortionValue ", Value)

    # Dist 0 - 100
    Value=rescaleMidi(InitLevel, 0.0, 100.0)
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion.value)
    liblo.send(target, EffectString, 2, (Value))
#    print("DistortionDist ", Value)

    # Drive 0 - 1
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion.value)
    liblo.send(target, EffectString, 3, (Value))
#    print("DistortionDrive ", Value)

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

def GateControl(level):
    InitLevel=level
    EffectString = "/Carla/%d/set_drywet" % (Effect.ABGate.value)
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    liblo.send(target, EffectString, Value)

    Value=rescaleMidi(InitLevel,-0, 2000)
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
    EffectString = "/Carla/%d/set_drywet" % (Effect.Compressor.value)
    Value=rescaleMidi(InitLevel, 0.0, 1.0)
    liblo.send(target, EffectString, Value)

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def ResetToDefault():
    DistortionControl(0)
    WahControl(0)
    ChorusControl(15)
#    GateControl(100)
#    CompressorControl(100)
    EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
    liblo.send(target, EffectString, 0.0)
    print(EffectString)

    EffectString = "/Carla/%d/set_drywet" % (Effect.Mojo.value)
    print(EffectString)
    EffectString = "/Carla/%d/set_drywet" % (3)
    liblo.send(target, EffectString, 0.0)
    print(EffectString)


#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def rescale(val, in_min, in_max, out_min, out_max):
    return out_min + (val - in_min) * ((out_max - out_min) / (in_max - in_min))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def rescaleMidi(val, out_min, out_max):
    return out_min + (val) * ((out_max - out_min) / (127))

#--------------------------------------------------------------
# --- 
#--------------------------------------------------------------
def main():
    global target
    global server

    try:
        target = liblo.Address(22752)
    except (liblo.AddressError, err):
        print(str(err))
        sys.exit()

#    liblo.send(target, "/foo/message1", 123, 456.789, "test")
    try:
        server = liblo.Server(1234)
    except (liblo.ServerError, err):
        print(str(err))
        sys.exit()

    # register method taking an int and a float
    server.add_method("/foo/bar", 'if', foo_bar_callback)

    # register method taking a blob, and passing user data to the callback
    server.add_method("/foo/baz", 'b', foo_baz_callback, "blah")

    # register a fallback for unhandled messages
    server.add_method(None, None, fallback)

    # loop and dispatch messages every 100ms
#    while True:
#        server.recv(100)

    port = mido.open_output('SwitchOut', virtual=True, client_name='LvSwitch')
    msg = mido.Message('note_on', note=45, velocity=90, channel=1)
    msg = mido.Message('program_change', program=45, channel=1)
    port.send(msg)

    # Print the effects list.
    print(Effect.ABGate.value, Effect.Compressor.value, Effect.PreAmp.value, Effect.Distortion.value)
    print(Effect.Mojo.value, Effect.Wah.value, Effect.Chorus.value, Effect.Reverb.value)
    print(Effect.Mixer.value)

#    liblo.send(target, "/Carla/3/set_drywet", 1.0 )
#    liblo.send(target, "/Carla/3/set_program", 5 )
     
#    exit
#    ResetToDefault()


    with mido.open_input('SwitchIn', virtual=True, client_name='LvSwitch') as inport:

        for msg in inport:
            print(msg)
#            print(msg.type)

            if (msg.type == "program_change"):
#                print(msg.program)
                ResetToDefault()

                match msg.program:
                    case 0:
                        print("Jazz")
                        ChorusControl(127)
                        DistortionControl(0)

                    case 1:
                        print("Straight")
                        ChorusControl(25)
                        DistortionControl(0)

                    case 2:
                        print("Blues")
                        DistortionControl(75)
                        ChorusControl(25)

                    case 3:
                        print("Acoustic")

                    case 4:
                        print("Juicy")

                    case 5:
                        print("Funky")
                        EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
                        Value=rescaleMidi(InitLevel, 0.0, 1.0)
                        liblo.send(target, EffectString, 1.0)

                    case 6:
                        print("FlangeD_Pre")

                    case 7:
                        print("Clean")

                    case 8:
                        print("AutoWah")
                        EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
                        liblo.send(target, EffectString, 1.0)

                    case 9:
                        print("Lead")

                    case 10:
                        print("EchoLead")

                    case 11:
                        print("Holdsworth")

                    case 12:
                        print("ThickDist")

                    case _:
                        print("Found Nothing")

            if (msg.type == "control_change"):
#                print(msg.value)
                InitLevel=msg.value
                match msg.control:
                    case 2:     
                        print("Distortion")
                        DistortionControl(InitLevel)

                        # oscsend localhost 22752 /Carla/3/set_parameter_value if 3 90.0
                        # EffectString = "/Carla/%d/set_volume" % (Effect.Distortion.value)

                        # Value=rescaleMidi(InitLevel, 0.75, 0.25)
#                        print(EffectString," Vol ",Value)
                        # liblo.send(target, EffectString, Value )

                        # # Dist 0 - 100
                        # EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion.value)
                        # Value=rescaleMidi(InitLevel, 0.0, 90)
                        # liblo.send(target, EffectString, 2, Value)
#                        print(EffectString)

                        # Drive 0 - 1
                        # EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion.value)
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




# EffectString = "/Carla/%d/set_volume" % (Effect.Distortion.value)
# liblo.send(target, EffectString, (1.0 - (msg.value/200)) )
# EffectString = "/Carla/%d/set_parameter_value" % (Effect.Distortion.value)
# liblo.send(target, EffectString, 2, (msg.value * 0.9))
# EffectString = "/Carla/%d/set_drywet" % (Effect.Wah.value)
# liblo.send(target, EffectString, 1.0)


