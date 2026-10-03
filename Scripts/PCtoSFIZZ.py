#!/usr/bin/python
#---------------------------------------------------------------------
#
#	File: 	PCtoSFIZZ
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Sat Apr 11 09:05:20 EEST 2026
#
#	Copyright : 	2026 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
import subprocess
import time
import rtmidi
import os
import threading
import os

# -----------------------------
# CONFIG
# -----------------------------

# Choose your real MIDI input device here
MIDI_INPUT_PORT_NAME = "LiveMusic Output:Fluid"
SFIZZ_INPUT_PORT_NAME = "Carla.0/MIDI Guitar 2:event-out"
AUDIO_TARGET_L = "Carla.0/LSP Mixer x4 Stereo:Audio input left 2"
AUDIO_TARGET_R = "Carla.0/LSP Mixer x4 Stereo:Audio input right 2"

# MIDI_INPUT_PORT_NAME = "Midi Through:Midi Through Port-0 14:0"
# SFIZZ_INPUT_PORT_NAME = "LiveMusic Output:Fluid"
# AUDIO_TARGET_L = "bluez_output.7C_E9_13_2C_34_24.1:playback_FL"
# AUDIO_TARGET_R = "bluez_output.7C_E9_13_2C_34_24.1:playback_FR"


sfizz_process = None


# BANK → PROGRAM → SFZ FILE




# -----------------------------
# STATE
# -----------------------------

current_bank = 0
current_program = 0
previous_program = 0
previous_bank = 0


SFZ_BANKS = {
0: {
0: "/home/SoundFonts/SFZFonts/0 vibes/tachoir's vibraphone.sfz", # Good
1: "/home/SoundFonts/SFZFonts/Baritone_Sax/Baritone Sax/Baritone Sax.sfz", # Not 
2: "/home/SoundFonts/SFZFonts/Classical_Acoustic_Guitar/Classical Acoustic Guitar/Classical Acoustic.sfz", # Good guitar
3: "/home/SoundFonts/SFZFonts/DamiensFunkyGuitar/Damiens funky guitar.sfz", # Nice Guitar sound
4: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/001-Acoustic Grand Piano.sfz", # Great piano
5: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/002-Bright Acoustic Piano.sfz",
6: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/003-Electric Grand Piano.sfz",
7: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/005-Electric Piano 1.sfz", # Rhodes
8: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/005-Electric Piano 1/_GM3-looped(1).sfz",
9: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/005-Electric Piano 1/_GM3-looped.sfz",
10: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/026-Acoustic Guitar (steel).sfz", # OK acoustic Guitar
11: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/026-Acoustic Guitar (steel)/_MartinGM2-loop-sw.sfz",
12: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/034-Electric Bass (finger).sfz",
13: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/045-Tremolo Strings.sfz", #powerfull strings
14: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/046-Pizzicato Strings.sfz", # crash
15: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/105-Sitar.sfz", # Good Sitar
16: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/105-Sitar/Sitar.sfz",
17: "/home/SoundFonts/SFZFonts/Ixox.Flute/Ixox Flute.sfz",
18: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Alto Sax (NL).sfz",
19: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Alto Sax.sfz",
20: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Baritone Sax (NL).sfz",
21: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Baritone Sax.sfz",
22: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Soprano Sax (NL).sfz",
23: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Soprano Sax.sfz",
24: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Tenor Sax (NL).sfz",
25: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Tenor Sax.sfz",
26: "/home/SoundFonts/SFZFonts/Marimba/Marimba/Marimba.sfz",# Check the ARPS manually
27: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_01.sfz",
28: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_02.sfz",
29: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_03.sfz",
30: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_04.sfz",
31: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_05.sfz",
32: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_06.sfz",
33: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_07.sfz",
34: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_08.sfz",
36: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_10.sfz",
37: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_11.sfz",
38: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_12.sfz",
39: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_17.sfz",
40: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_18.sfz",
41: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_19.sfz",
42: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_20.sfz",
43: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_21.sfz",
44: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_22.sfz",
45: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_23.sfz",
46: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_24.sfz",
47: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_25.sfz",
48: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_26.sfz",
49: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_27.sfz",
50: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_28.sfz",
51: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_33.sfz",
52: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_34.sfz",
53: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_35.sfz",
54: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_36.sfz",
55: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_37.sfz",
56: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_38.sfz",
57: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_39.sfz",
58: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_40.sfz",
59: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_41.sfz",
60: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_42.sfz",
61: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_43.sfz",
62: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_44.sfz",
63: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_01.sfz",
64: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_02.sfz",
65: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_03.sfz",
66: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_04.sfz",
67: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_05.sfz",
68: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_06.sfz",
69: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_07.sfz",
70: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_08.sfz",
71: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_09.sfz",
72: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_10.sfz",
73: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_11.sfz",
74: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_12.sfz",
75: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_13.sfz",
76: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_14.sfz",
77: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_15.sfz",
78: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_16.sfz",
79: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_17.sfz",
80: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_18.sfz",
81: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_19.sfz",
82: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_20.sfz",
83: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_21.sfz",
84: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_22.sfz",
85: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_23.sfz",
86: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_24.sfz",
87: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_25.sfz",
88: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_26.sfz",
89: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_27.sfz",
90: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_28.sfz",
91: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_33.sfz",
92: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_34.sfz",
93: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_35.sfz",
94: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_36.sfz",
95: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_37.sfz",
96: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_38.sfz",
97: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_39.sfz",
98: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_40.sfz",
99: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_41.sfz",
},
1: {
0: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_42.sfz",
1: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_43.sfz",
2: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_44.sfz",
3: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_45.sfz",
4: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_46.sfz",
5: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_47.sfz",
6: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_48.sfz",
7: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_49.sfz",
8: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_50.sfz",
9: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_51.sfz",
10: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_52.sfz",
11: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_53.sfz",
12: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_54.sfz",
13: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_55.sfz",
14: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_56.sfz",
15: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_57.sfz",
16: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_58.sfz",
17: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_59.sfz",
18: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_60.sfz",
19: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_61.sfz",
20: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_01.sfz",
21: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_02.sfz",
22: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_03.sfz",
23: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_04.sfz",
24: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_05.sfz",
25: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_06.sfz",
26: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_07.sfz",
27: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_08.sfz",
28: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_09.sfz",
29: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_10.sfz",
30: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_11.sfz",
31: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_12.sfz",
32: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_13.sfz",
33: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_14.sfz",
34: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_15.sfz",
35: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_16.sfz",
36: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_17.sfz",
37: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_18.sfz",
38: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_19.sfz",
39: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_20.sfz",
40: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_21.sfz",
41: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_23.sfz",
42: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_24.sfz",
43: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_25.sfz",
44: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_26.sfz",
45: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_65.sfz",
46: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_66.sfz",
47: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_67.sfz",
48: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_68.sfz",
49: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_69.sfz",
50: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_70.sfz",
51: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_71.sfz",
52: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_72.sfz",
53: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_73.sfz",
54: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_74.sfz",
55: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_75.sfz",
56: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_76.sfz",
57: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_77.sfz",
58: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_78.sfz",
59: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_79.sfz",
60: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_80.sfz",
61: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_89.sfz",
62: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_100.sfz",
63: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_101.sfz",
64: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_102.sfz",
65: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_103.sfz",
66: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_104.sfz",
67: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_105.sfz",
68: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_106.sfz",
69: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_107.sfz",
70: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_108.sfz",
71: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_109.sfz",
72: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_110.sfz",
73: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_111.sfz",
74: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_112.sfz",
75: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_113.sfz",
76: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_114.sfz",
77: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_115.sfz",
78: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_116.sfz",
79: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_117.sfz",
80: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_118.sfz",
81: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_121.sfz",
82: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_97.sfz",
83: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_98.sfz",
84: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_99.sfz",
85: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_001.sfz",
86: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_002.sfz",
87: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_003.sfz",
88: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_004.sfz",
89: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_005.sfz",
90: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_006.sfz",
91: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_007.sfz",
92: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_008.sfz",
93: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_009.sfz",
94: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_010.sfz",
95: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_011.sfz",
96: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_012.sfz",
97: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_013.sfz",
98: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_014.sfz",
99: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_015.sfz",
},
2: {

0: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_016.sfz",
1: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_017.sfz",
2: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_018.sfz",
3: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_019.sfz",
4: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_020.sfz",
5: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_021.sfz",
6: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_022.sfz",
7: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_023.sfz",
8: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_024.sfz",
9: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_025.sfz",
10: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_026.sfz",
11: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_027.sfz",
12: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_028.sfz",
13: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_029.sfz",
14: "/home/SoundFonts/SFZFonts/OldSynth/LFO/MP-LFO_01.sfz",
15: "/home/SoundFonts/SFZFonts/OldSynth/LFO/MP-LFO_02.sfz",
16: "/home/SoundFonts/SFZFonts/OldSynth/LFO/MP-LFO_03.sfz",
17: "/home/SoundFonts/SFZFonts/OldSynth/LFO/MP-LFO_04.sfz",
18: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_01.sfz",
19: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_02.sfz",
20: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_03.sfz",
21: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_04.sfz",
22: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_05.sfz",
23: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_06.sfz",
24: "/home/SoundFonts/SFZFonts/OldSynth/Orgues/MP-ORG_01.sfz",
25: "/home/SoundFonts/SFZFonts/OldSynth/Orgues/MP-ORG_02.sfz",
26: "/home/SoundFonts/SFZFonts/OldSynth/Orgues/MP-ORG_03.sfz",
27: "/home/SoundFonts/SFZFonts/OldSynth/RevNoise/MP-RNS_01.sfz",
28: "/home/SoundFonts/SFZFonts/OldSynth/RevNoise/MP-RNS_02.sfz",
29: "/home/SoundFonts/SFZFonts/OldSynth/RevNoise/MP-RNS_03.sfz",
30: "/home/SoundFonts/SFZFonts/OvationGuitar/Ovation Guitar (mw1 for 12-string).sfz",
31: "/home/SoundFonts/SFZFonts/Terkelsen.Marimba/Terkelsen's Marimba - xfade.sfz",
32: "/home/SoundFonts/SFZFonts/Terkelsen.Marimba/Terkelsen's Marimba.sfz",
33: "/home/SoundFonts/SFZFonts/Violin SFZ/Violin.sfz",
34: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_01.sfz",
35: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_02.sfz",
36: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_03.sfz",
37: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_04.sfz",
38: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_05.sfz",
39: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_06.sfz",
40: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_07.sfz",
41: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_08.sfz",
42: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_09.sfz",
43: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_10.sfz",
44: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_11.sfz",
45: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_12.sfz",
46: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_13.sfz",
47: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_14.sfz",
48: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_15.sfz",
49: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_16.sfz",
50: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_17.sfz",
51: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_18.sfz",
52: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_19.sfz",
53: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_20.sfz",
54: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_21.sfz",
55: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_22.sfz",
56: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_23.sfz",
57: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_24.sfz",
58: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_25.sfz",
59: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_26.sfz",
60: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_27.sfz",
61: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_28.sfz",
62: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_29.sfz",
63: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_30.sfz",
64: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_31.sfz",
65: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_32.sfz",
66: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_33.sfz",
67: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_34.sfz",
68: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_35.sfz",
69: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_36.sfz",
70: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_37.sfz",
71: "/home/SoundFonts/SFZFonts/basic-harmonica/harmonica solo.sfz",
72: "/home/SoundFonts/SFZFonts/glass/Glass-20191227.sfz",
73: "/home/SoundFonts/SFZFonts/jlearman.SteelDrum/_jSteelDrum-flac-noxfade.sfz",
74: "/home/SoundFonts/SFZFonts/jlearman.SteelDrum/_jSteelDrum-flac.sfz",
75: "/home/SoundFonts/SFZFonts/jlearman.SteelDrum/jSteelDrum-flac-sfz/_jSteelDrum-flac-noxfade.sfz",
76: "/home/SoundFonts/SFZFonts/jlearman.SteelDrum/jSteelDrum-flac-sfz/_jSteelDrum-flac.sfz",
77: "/home/SoundFonts/SFZFonts/jlearman.jRhodes3c/jRhodes3c-looped-flac-sfz/_jRhodes-both-looped.sfz",
78: "/home/SoundFonts/SFZFonts/jlearman.jRhodes3c/jRhodes3c-looped-flac-sfz/_jRhodes-mono-looped.sfz",
79: "/home/SoundFonts/SFZFonts/jlearman.jRhodes3c/jRhodes3c-looped-flac-sfz/_jRhodes-stereo-looped.sfz",
80: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/1-solo-mono.sfz",
81: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/2-solo-poly.sfz",
82: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/3-duo-mono.sfz",
83: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/4-duo-poly.sfz",
84: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/5-bearcussion.sfz",
85: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/6-bearborg.sfz",
86: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_basic.sfz",
87: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_chords.sfz",
88: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_chords_wide.sfz",
89: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_clean.sfz",
90: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_five_voice.sfz",
91: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_three_voice.sfz",
92: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/Blackheart.sfz",
93: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/Ironface.sfz",
94: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/Zinc.sfz",
95: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_Master.sfz",
96: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_five.sfz",
97: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_five_sc.sfz",
98: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_noise.sfz",
99: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_one.sfz",
},
3: {

1: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_one_sc.sfz",
2: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_three.sfz",
3: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_three_sc.sfz",
4: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_Master.sfz",
5: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_clean.sfz",
6: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_distorted.sfz",
7: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_sc.sfz",
8: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_sc_dist.sfz",
9: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_Master.sfz",
10: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_one_shift.sfz",
11: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_one_shift_sc.sfz",
12: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_three_shift.sfz",
13: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_three_shift_sc.sfz",
14: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_two_shift.sfz",
15: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_two_shift_sc.sfz",
16: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_noise.sfz",
17: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_one_voice.sfz",
18: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_one_voice_sc.sfz",
19: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_one_shift.sfz",
20: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_one_shift_sc.sfz",
21: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_three_shift.sfz",
22: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_three_shift_sc.sfz",
23: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_two_shift.sfz",
24: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_two_shift_sc.sfz",
25: "/home/SoundFonts/SFZFonts/ocarina1/Ocarina 20241002.sfz",
26: "/home/SoundFonts/SFZFonts/spanish-classical-guitar/SpanishClassicalGuitar-20190618.sfz",
27: "/home/SoundFonts/SFZFonts/synth-brass-2/SynthBrass2 20240610.sfz",
28: "/home/SoundFonts/SFZFonts/synth-calliope/SynthCalliope 20200512.sfz",
29: "/home/SoundFonts/SFZFonts/synth-soundtrack/SynthSoundtrack 20200521.sfz",
30: "/home/SoundFonts/SFZFonts/synth-strings-1/SynthStrings1 20200528.sfz",
31: "/home/SoundFonts/SFZFonts/synth-strings-2/SynthStrings2 20200528.sfz",
32: "/home/SoundFonts/SFZFonts/tubular-bells1/TubularBells 20241130.sfz",
33: "/home/SoundFonts/SFZFonts/tubular-bells1/TubularBells small 20241130.sfz",
},
}

# -----------------------------
# SFIZZ CONTROL
# -----------------------------

def kill_sfizz():
    global sfizz_process
    if sfizz_process:
        print("Killing sfizz...")
        sfizz_process.terminate()
        try:
            sfizz_process.wait(timeout=2)
        except subprocess.TimeoutExpired:
            sfizz_process.kill()
        sfizz_process = None

def read_sfizz_output(proc):
    for line in proc.stdout:
        print("[sfizz]", line.strip())

def auto_connect():
    print("def auto_connect", AUDIO_TARGET_Ls)
    subprocess.run(["pw-link", "sfizz:output_1", AUDIO_TARGET_L], check=False)
    subprocess.run(["pw-link", "sfizz:output_2", AUDIO_TARGET_R], check=False)

def wait_for_port(port_name, timeout=10):
    """Wait until a PipeWire port is visible."""
    import time
    deadline = time.time() + timeout
    while time.time() < deadline:
        result = subprocess.run(
            ["pw-link", "-o"],  # list output ports
            capture_output=True, text=True
        )
        if port_name in result.stdout:
            return True
        time.sleep(0.3)
    return False

def launch_sfizz(sfz_path):
    global sfizz_process

    print(f"Launching sfizz_jack: {sfz_path}")

    env = os.environ.copy()

#    subprocess.run(["/home/elias/workspace/LiveMusicApp/Scripts/sfizz_launch", sfz_path, "sfizz_jack"])

    sfizz_process = subprocess.Popen(
#        ["pw-link", "-s", "9600", "pw-jack", "sfizz_jack", sfz_path],
        ["/home/elias/workspace/LiveMusicApp/Scripts/sfizz_launch", sfz_path, "sfizz_jack"],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        env=env
    )

    # 🔥 DO NOT BLOCK MAIN THREAD
    threading.Thread(
        target=read_sfizz_output,
        args=(sfizz_process,),
        daemon=True
    ).start()

    time.sleep(2)
    print("def auto_connect2")
    subprocess.run(["pw-link", "sfizz:output_1", AUDIO_TARGET_L], check=False)
    subprocess.run(["pw-link", "sfizz:output_2", AUDIO_TARGET_R], check=False)
    subprocess.run(["pw-link", SFIZZ_INPUT_PORT_NAME, "sfizz:input"], check=False)

#    auto_connect()
    
def launch_sfizz2(sfz_path):
    global sfizz_process

    print(f"Launching sfizz_jack: {sfz_path}")

    env = os.environ.copy()

    sfizz_process = subprocess.Popen(
        ["pw-jack", "sfizz_jack", sfz_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        env=env
    )

    # Print errors immediately (VERY IMPORTANT)
    for line in sfizz_process.stderr:
        print("[sfizz ERR]", line.strip())

        # stop early if crash happens
        if line:
            break

    auto_connect()

def launch_sfizz1(sfz_path):
    global sfizz_process

    print(f"Launching sfizz: {sfz_path}")

    sfizz_process = subprocess.Popen(
        ["sfizz", "--load", sfz_path],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )

    print("def auto_connect1")
    time.sleep(4)
    print("def auto_connect3")
    auto_connect()

def load_from_state():
    global current_bank, current_program
    global previous_program, previous_bank

    bank = SFZ_BANKS.get(current_bank)

    print("Bank ", current_bank)
    if current_bank == 101:
        print("force Kill sfizz")
        kill_sfizz()
    
    if not bank:
        print(f"Bank not found: {current_bank}")
        return

    sfz_path = bank.get(current_program)
    print("bank.get ", sfz_path)

    if not sfz_path:
        print(f"Program not found: {current_program} in bank {current_bank}")
        return

    print("Pre ", previous_program, " curr ", current_program)
    if previous_program == current_program:
        print("Match")
        previous_program = current_program
        return
    previous_program = current_program
    previous_bank = current_bank

    print("Kill sfizz")
    kill_sfizz()
    print("Launch sfizz")
    launch_sfizz(sfz_path)

# -----------------------------
# MIDI HANDLER
# -----------------------------

def handle_midi(event, data=None):
    print("🔥 RAW:", event, flush=True)
    global current_bank, current_program

    message, _ = event
    status = message[0] & 0xF0

    # Debug (optional)
    # print("MIDI:", message)

    # -------------------------
    # BANK SELECT (CC0 / CC32)
    # -------------------------
    if status == 0xB0:
        cc = message[1]
        value = message[2]

        if cc == 0:
            current_bank = value
            print(f"Bank MSB: {value}")

        elif cc == 32:
            current_bank = value
            print(f"Bank LSB: {value}")

    # -------------------------
    # PROGRAM CHANGE
    # -------------------------
    elif status == 0xC0:
        program = message[1]
        current_program = program

        print(f"Program Change: {program}")
        load_from_state()

# -----------------------------
# MIDI SETUP (REAL DEVICE MODE)
# -----------------------------
def open_midi_port():
    import rtmidi

    midi_in = rtmidi.MidiIn()
    ports = midi_in.get_ports()

    print("MIDI PORTS:")
    for i, p in enumerate(ports):
        print(i, p)

    # 🔥 FORCE OPEN Midi Through (the working one)
    for i, name in enumerate(ports):
        if MIDI_INPUT_PORT_NAME in name:
            midi_in.open_port(i)
            print("Opened:", name)
            return midi_in

    print("No matching port found")
    return None

def open_midi_port_old():
    midi_in = rtmidi.MidiIn()
    ports = midi_in.get_ports()

    if not ports:
        print("No MIDI ports found")
        return None

    print("Available MIDI ports:")
    for i, p in enumerate(ports):
        print(f"{i}: {p}")

    for i, name in enumerate(ports):
        if MIDI_INPUT_PORT_NAME in name:
            midi_in.open_port(i)
            print(f"Opened MIDI port: {name}")
            return midi_in

    print("MIDI port not found")
    return None

# -----------------------------
# MAIN
# -----------------------------

def main():
    print("SFZ Bank + Program Change switcher starting...")
    
    midi_in = open_midi_port()
    if not midi_in:
        return

    midi_in.set_callback(handle_midi)

    print("Listening for MIDI Program Change...")

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Exiting...")
        kill_sfizz()

if __name__ == "__main__":
    main()
    
    
    


