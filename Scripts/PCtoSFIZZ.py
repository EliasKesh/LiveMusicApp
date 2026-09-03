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
SFZ_BANKS = {
0: {
0: "/home/SoundFonts/SFZFonts/jlearman.jRhodes3c/jRhodes3c-looped-flac-sfz",
1: "/home/SoundFonts/SFZFonts/jlearman.jRhodes3c/jRhodes3c-looped-flac-sfz/_jRhodes-both-looped.sfz", # Nice Rhodes
2: "/home/SoundFonts/SFZFonts/jlearman.jRhodes3c/jRhodes3c-looped-flac-sfz/_jRhodes-mono-looped.sfz", 
3: "/home/SoundFonts/SFZFonts/jlearman.jRhodes3c/jRhodes3c-looped-flac-sfz/_jRhodes-stereo-looped.sfz", # Clean Rhodes
4: "/home/SoundFonts/SFZFonts/ocarina1/Ocarina 20241002.sfz", # Remove
5: "/home/SoundFonts/SFZFonts/synth-strings-1/SynthStrings1 20200528.sfz", # OK
6: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_one_shift.sfz",
7: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_one_shift.sfz",
8: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_one_shift_sc.sfz",
9: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_clean.sfz", # OK
10: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_Master.sfz",# Synth mellow
11: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_midi_cc_setup.sfz",
12: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_one_voice.sfz", # sythn tight
13: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_noise.sfz",
14: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_Master.sfz",
15: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_sc_dist.sfz",
16: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_one.sfz",
17: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_midi_cc_setup.sfz",
18: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_three.sfz",
19: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_two_shift.sfz",
20: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/Ironface.sfz",
21: "/home/SoundFonts/SFZFont/karoryfer.string-cyborgs/Programs/zinc_Master.sfz", # Funk Synth
22: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_three_shift.sfz",                                                    
23: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_distorted.sfz",                                                              
24: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/Blackheart.sfz",
25: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_five_sc.sfz",
26: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_three_shift.sfz",
27: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_two_shift.sfz",
28: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_midi_cc_setup.sfz",
29: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_one_shift_sc.sfz",
30: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_three_shift_sc.sfz",
31: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/Zinc.sfz",
32: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_two_shift_sc.sfz",
33: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_one_sc.sfz",
34: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_five.sfz",
35: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/ironface_sc.sfz",
36: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_one_voice_sc.sfz",
37: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_three_voice_three_shift_sc.sfz",
38: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_three_sc.sfz",
39: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/zinc_five_voice_two_shift_sc.sfz",
40: "/home/SoundFonts/SFZFonts/karoryfer.string-cyborgs/Programs/blackheart_noise.sfz",
41: "/home/SoundFonts/SFZFonts/OvationGuitar/Ovation Guitar (mw1 for 12-string).sfz", # Good Guitar
42: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_48.sfz",
43: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_47.sfz", # Bounce Sythn
44: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_34.sfz",
45: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_42.sfz",
46: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_61.sfz", # Yaz Synth
47: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_33.sfz", # Pop clean  Synth
48: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_54.sfz", # Brught Synth
49: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_38.sfz",
50: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_40.sfz", # Media Bounce
51: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_57.sfz",
52: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_55.sfz",
53: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_36.sfz",
54: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_51.sfz", # Clean Bounce
55: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_46.sfz",
56: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_41.sfz",
57: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_50.sfz",
58: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_39.sfz",
59: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_45.sfz", # Deel Synth
60: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_53.sfz", # Funk Sync
61: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_52.sfz",
62: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_60.sfz",
63: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_56.sfz",
64: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_37.sfz",
65: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_58.sfz",
66: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_44.sfz", # Crackle Synth
67: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_49.sfz",
68: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_43.sfz",
69: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_35.sfz",
70: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PW/MP-BSQ_59.sfz",
71: "/home/SoundFonts/SFZFonts/OldSynth/LFO/MP-LFO_01.sfz",
72: "/home/SoundFonts/SFZFonts/OldSynth/LFO/MP-LFO_03.sfz",
73: "/home/SoundFonts/SFZFonts/OldSynth/LFO/MP-LFO_02.sfz",
74: "/home/SoundFonts/SFZFonts/OldSynth/LFO/MP-LFO_04.sfz",
75: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_113.sfz",
76: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_102.sfz",
77: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_101.sfz",
78: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_106.sfz",
79: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_121.sfz",
80: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_100.sfz",
81: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_112.sfz",
82: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_108.sfz",
83: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_115.sfz",
84: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_99.sfz",
85: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_103.sfz",
86: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_109.sfz",
87: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_98.sfz",
88: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_97.sfz",
89: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_111.sfz",
90: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_110.sfz",
91: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_114.sfz",
92: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_117.sfz",
93: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_118.sfz",
94: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_116.sfz",
95: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_104.sfz",
96: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_105.sfz",
97: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Tri/MP-BSQ_107.sfz",
98: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_025.sfz",
99: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_019.sfz",
},
1: {
0: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_001.sfz",
1: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_015.sfz",
2: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_029.sfz",
3: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_020.sfz",
4: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_014.sfz",
5: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_005.sfz",
6: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_013.sfz",
7: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_018.sfz",
8: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_028.sfz",
9: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_016.sfz",
10: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_017.sfz",
11: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_026.sfz",
12: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_004.sfz",
13: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_022.sfz",
14: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_011.sfz",
15: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_007.sfz",
16: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_021.sfz",
17: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_003.sfz",
18: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_009.sfz",
19: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_010.sfz",
20: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_023.sfz",
21: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_027.sfz",
22: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_008.sfz",
23: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_024.sfz",
24: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_012.sfz",
25: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_002.sfz",
26: "/home/SoundFonts/SFZFonts/OldSynth/BrassChords/MP-BRS_006.sfz",
27: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_13.sfz",
28: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_15.sfz",
29: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_25.sfz",
30: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_26.sfz",
31: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_10.sfz",
32: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_08.sfz",
33: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_24.sfz",
34: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_02.sfz",
35: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_04.sfz",
36: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_09.sfz",
37: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_16.sfz",
38: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_17.sfz",
39: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_05.sfz",
40: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_19.sfz",
41: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_21.sfz",
42: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_20.sfz",
43: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_23.sfz",
44: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_18.sfz",
45: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_14.sfz",
46: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_03.sfz",
47: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_07.sfz",
48: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_01.sfz",
49: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_06.sfz",
50: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_12.sfz",
51: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-PWM/MP-BSQ_11.sfz",
52: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_68.sfz",
53: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_73.sfz",
54: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_72.sfz",
55: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_80.sfz",
56: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_66.sfz",
57: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_77.sfz",
58: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_75.sfz",
59: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_74.sfz",
60: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_89.sfz",
61: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_67.sfz",
62: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_76.sfz",
63: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_79.sfz",
64: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_70.sfz",
65: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_65.sfz",
66: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_71.sfz",
67: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_69.sfz",
68: "/home/SoundFonts/SFZFonts/OldSynth/BassSeq-Saw/MP-BSQ_78.sfz",
69: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_19.sfz",
70: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_27.sfz",
71: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_28.sfz",
72: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_10.sfz",
73: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_18.sfz",
74: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_25.sfz",
75: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_05.sfz",
76: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_34.sfz",
77: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_35.sfz",
78: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_41.sfz",
79: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_01.sfz",
80: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_36.sfz",
81: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_03.sfz",
82: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_12.sfz",
83: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_42.sfz",
84: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_37.sfz",
85: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_22.sfz",
86: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_06.sfz",
87: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_39.sfz",
88: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_26.sfz",
89: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_09.sfz",
90: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_43.sfz",
91: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_33.sfz",
92: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_23.sfz",
93: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_17.sfz",
94: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_24.sfz",
95: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_07.sfz",
96: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_21.sfz",
97: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_04.sfz",
98: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_40.sfz",
99: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_08.sfz",
},
2: {
0: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_11.sfz",
1: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_44.sfz",
2: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_38.sfz",
3: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_02.sfz",
4: "/home/SoundFonts/SFZFonts/OldSynth/Arp/MP-ARP_20.sfz",
5: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_17.sfz",
6: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_04.sfz",
7: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_26.sfz",
8: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_18.sfz",
9: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_10.sfz",
10: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_20.sfz",
11: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_22.sfz",
12: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_19.sfz",
13: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_03.sfz",
14: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_08.sfz",
15: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_11.sfz",
16: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_05.sfz",
17: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_06.sfz",
18: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_12.sfz",
19: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_23.sfz",
20: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_13.sfz",
21: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_25.sfz",
22: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_16.sfz",
23: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_27.sfz",
24: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_28.sfz",
25: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_14.sfz",
26: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_24.sfz",
27: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_07.sfz",
28: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_09.sfz",
29: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_21.sfz",
30: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_15.sfz",
31: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_02.sfz",
32: "/home/SoundFonts/SFZFonts/OldSynth/BassLead/MP-BSLD_01.sfz",
33: "/home/SoundFonts/SFZFonts/OldSynth/Orgues/MP-ORG_03.sfz",
34: "/home/SoundFonts/SFZFonts/OldSynth/Orgues/MP-ORG_02.sfz",
35: "/home/SoundFonts/SFZFonts/OldSynth/Orgues/MP-ORG_01.sfz",
36: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_06.sfz",
37: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_02.sfz",
38: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_04.sfz",
39: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_05.sfz",
40: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_01.sfz",
41: "/home/SoundFonts/SFZFonts/OldSynth/Noise/MP-NS_03.sfz",
42: "/home/SoundFonts/SFZFonts/OldSynth/RevNoise/MP-RNS_01.sfz",
43: "/home/SoundFonts/SFZFonts/OldSynth/RevNoise/MP-RNS_03.sfz",
44: "/home/SoundFonts/SFZFonts/OldSynth/RevNoise/MP-RNS_02.sfz",
45: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Discord GM Bank - Melodic-test.sfz", # Good Clean Paino
46: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Discord GM Bank - Drums.sfz",
47: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/017-Power Kit.sfz",
48: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/001-Standard Kit.sfz",
49: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/033-Jazz Kit.sfz",
50: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/027-TR-909 Kit.sfz",
51: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/049-Orchestra Kit.sfz",
52: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/026-TR-808 Kit.sfz",
53: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/041-Brush Kit.sfz",
54: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/025-Electronic Kit.sfz",
55: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/057-SFX Kit.sfz",
56: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Drums/009-Room Kit.sfz",
57: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/035-Electric Bass (pick).sfz",
58: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/027-Electric Guitar (jazz).sfz",
59: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/128-Gunshot.sfz",
60: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/104-FX 8 (sci-fi).sfz",
61: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/105-Sitar/SitarSliding.sfz",
62: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/105-Sitar/Sitar.sfz",
63: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/014-Xylophone.sfz",
64: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/090-Pad 2 (warm).sfz",
65: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/011-Music Box.sfz",
66: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/019-Rock Organ.sfz",
67: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/095-Pad 7 (halo).sfz",
68: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/096-Pad 8 (sweep).sfz",
69: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/085-Lead 5 (charang).sfz",
70: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/117-Taiko Drum.sfz",
71: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/048-Timpani.sfz",
72: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/047-Orchestral Harp.sfz",
73: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/082-Lead 2 (sawtooth).sfz",
74: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/105-Sitar.sfz",
75: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/125-Telephone Ring.sfz",
76: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/055-Synth Choir.sfz",
77: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/072-Clarinet.sfz",
78: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/068-Baritone Sax.sfz",
79: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/040-Synth Bass 2.sfz",
80: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/087-Lead 7 (fifths).sfz",
81: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/112-Shanai/Shanai.sfz",
82: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/062-Brass Section.sfz",
83: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/001-Acoustic Grand Piano.sfz",
84: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/058-Trombone.sfz",
85: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/126-Helicopter.sfz",
86: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/116-Woodblock.sfz",
87: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/103-FX 7 (echoes).sfz",
88: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/044-Contrabass.sfz",
89: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/043-Cello.sfz",
90: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/028-Electric Guitar (clean).sfz",
91: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/038-Slap Bass 2.sfz",
92: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/119-Synth Drum.sfz",
93: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/102-FX 6 (goblins).sfz",
94: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/106-Banjo.sfz",
95: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/015-Tubular Bells.sfz",
96: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/012-Vibraphone.sfz",
97: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/032-Guitar Harmonics.sfz",
98: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/051-Synth Strings 1.sfz",
99: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/108-Koto.sfz",
},
3: {
0: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/005-Electric Piano 1/_GM3-looped.sfz",
1: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/005-Electric Piano 1/_GM3-looped(1).sfz",
2: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/054-Voice Oohs.sfz",
3: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/061-French Horn.sfz",
4: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/004-Honky-tonk Piano.sfz",
5: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/018-Percussive Organ.sfz",
6: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/057-Trumpet.sfz",
7: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/003-Electric Grand Piano.sfz",
8: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/074-Flute.sfz",
9: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/013-Marimba.sfz",
10: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/042-Viola.sfz",
11: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/124-Bird Tweet.sfz",
12: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/099-FX 3 (crystal).sfz",
13: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/029-Electric Guitar (muted).sfz",
14: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/031-Distortion Guitar.sfz",
15: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/071-Bassoon.sfz",
16: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/009-Celesta.sfz",
17: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/045-Tremolo Strings.sfz",
18: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/098-FX 2 (soundtrack).sfz",
19: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/021-Reed Organ.sfz",
20: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/046-Pizzicato Strings.sfz",
21: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/080-Ocarina.sfz",
22: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/115-Steel Drums.sfz",
23: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/016-Dulcimer.sfz",
24: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/100-FX 4 (atmosphere).sfz",
25: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/113-Tinkle Bell.sfz",
26: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/089-Pad 1 (new age).sfz",
27: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/036-Fretless Bass.sfz",
28: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/101-FX 5 (brightness).sfz",
29: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/110-Bagpipe.sfz",
30: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/049-String Ensemble 1.sfz",
31: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/037-Slap Bass 1.sfz",
32: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/073-Piccolo.sfz",
33: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/112-Shanai.sfz",
34: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/064-Synth Brass 2.sfz",
35: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/127-Applause.sfz",
36: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/097-FX 1 (rain).sfz",
37: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/060-Muted Trumpet.sfz",
38: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/002-Bright Acoustic Piano.sfz",
39: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/052-Synth Strings 2.sfz",
40: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/050-String Ensemble 2.sfz",
41: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/008-Clavinet.sfz",
42: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/093-Pad 5 (bowed).sfz",
43: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/020-Church Organ.sfz",
44: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/023-Harmonica.sfz",
45: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/109-Kalimba.sfz",
46: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/026-Acoustic Guitar (steel)/_MartinGM2-loop-sw.sfz",
47: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/007-Harpsichord.sfz",
48: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/118-Melodic Tom.sfz",
49: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/059-Tuba.sfz",
50: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/122-Breath Noise.sfz",
51: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/079-Whistle.sfz",
52: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/076-Pan Flute.sfz",
53: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/053-Choir Aahs.sfz",
54: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/067-Tenor Sax.sfz",
55: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/083-Lead 3 (calliope).sfz",
56: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/123-Seashore.sfz",
57: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/034-Electric Bass (finger).sfz",
58: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/107-Shamisen.sfz",
59: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/114-Agogo.sfz",
60: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/030-Overdriven Guitar.sfz",
61: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/084-Lead 4 (chiff).sfz",
62: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/086-Lead 6 (voice).sfz",
63: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/063-Synth Brass 1.sfz",
64: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/025-Acoustic Guitar (nylon).sfz",
65: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/092-Pad 4 (choir).sfz",
66: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/111-Fiddle.sfz",
67: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/065-Soprano Sax.sfz",
68: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/022-Accordion.sfz",
69: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/081-Lead 1 (square).sfz",
70: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/077-Blown bottle.sfz",
71: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/006-Electric Piano 2.sfz",
72: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/066-Alto Sax.sfz",
73: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/017-Drawbar Organ.sfz",
74: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/078-Shakuhachi.sfz",
75: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/041-Violin.sfz",
76: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/121-Guitar Fret Noise.sfz",
77: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/094-Pad 6 (metallic).sfz",
78: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/039-Synth Bass 1.sfz",
79: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/069-Oboe.sfz",
80: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/120-Reverse Cymbal.sfz",
81: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/005-Electric Piano 1.sfz",
82: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/024-Tango Accordion.sfz",
83: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/056-Orchestra Hit.sfz",
84: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/010-Glockenspiel.sfz",
85: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/075-Recorder.sfz",
86: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/033-Acoustic Bass.sfz",
87: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/070-English Horn.sfz",
88: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/088-Lead 8 (bass + lead).sfz",
89: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/026-Acoustic Guitar (steel).sfz",
90: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Melodic/091-Pad 3 (polysynth).sfz",
91: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Discord GM Bank-test.sfz",
92: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Discord GM Bank - Melodic.sfz",
93: "/home/SoundFonts/SFZFonts/Discord-SFZ-GM-Bank/Discord GM/Discord GM Bank.sfz",
94: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_chords_wide.sfz",
95: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_five_voice.sfz",
96: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_clean.sfz",
97: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_three_voice.sfz",
98: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_basic.sfz",
99: "/home/SoundFonts/SFZFonts/karoryfer.emilyguitar/emily_chords.sfz",
},
4: {
0: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz",
1: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/000-007 Piano/006_Dist Guitar Pluck.sfz",
2: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/000-007 Piano/005_Distorted Guitar 2.sfz",
3: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/000-007 Piano/001_Brass Porta Up.sfz",
4: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/000-007 Piano/007_Dist Guitar Pluck 2.sfz",
5: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/000-007 Piano/004_Distorted Guitar.sfz",
6: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/000-007 Piano/002_Chime.sfz",
7: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/000-007 Piano/003_Chime 2.sfz",
8: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/000-007 Piano/000_Brass.sfz",
9: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/040-047 Strings/047_Triangle Pad 2.sfz",
10: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/040-047 Strings/041_Polysynth Pad.sfz",
11: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/040-047 Strings/043_Slap Bass.sfz",
12: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/040-047 Strings/040_Pluck 2.sfz",
13: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/040-047 Strings/045_Tremolo Strings.sfz",
14: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/040-047 Strings/046_Triangle Pad.sfz",
15: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/040-047 Strings/042_Weird Onehit Saw.sfz",
16: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/040-047 Strings/044_Synth Bass.sfz",
17: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/016-023 Organ/019_Guitar Chord 8.sfz",
18: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/016-023 Organ/021_Guitar Chord 10.sfz",
19: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/016-023 Organ/016_Guitar Chord 5.sfz",
20: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/016-023 Organ/022_Guitar Chord 11.sfz",
21: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/016-023 Organ/017_Guitar Chord 6.sfz",
22: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/016-023 Organ/018_Guitar Chord 7.sfz",
23: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/016-023 Organ/020_Guitar Chord 9.sfz",
24: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/016-023 Organ/023_Guitar Chord 12.sfz",
25: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/008-015 Chromatic percussion/014_Guitar Chord 3.sfz",
26: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/008-015 Chromatic percussion/013_Guitar Chord 2.sfz",
27: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/008-015 Chromatic percussion/010_Fingered Bass.sfz",
28: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/008-015 Chromatic percussion/008_Electric Piano.sfz",
29: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/008-015 Chromatic percussion/012_Guitar Chord 1.sfz",
30: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/008-015 Chromatic percussion/011_FM Bass.sfz",
31: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/008-015 Chromatic percussion/009_Fantasia.sfz",
32: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/008-015 Chromatic percussion/015_Guitar Chord 4.sfz",
33: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/024-031 Guitar/028_Nylon Guitar.sfz",
34: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/024-031 Guitar/025_Guitar Riff 1.sfz",
35: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/024-031 Guitar/024_Guitar Chord 13.sfz",
36: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/024-031 Guitar/030_Orchestra Hit.sfz",
37: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/024-031 Guitar/027_Honkey-Tonk Piano.sfz",
38: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/024-031 Guitar/029_Nylon Guitar 2.sfz",
39: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/024-031 Guitar/031_Orchestra Hit 2.sfz",
40: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/024-031 Guitar/026_Harpischord.sfz",
41: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/048-055 Ensemble/048_Xylophone.sfz",
42: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/032-039 Bass/038_Piano.sfz",
43: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/032-039 Bass/034_Organ.sfz",
44: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/032-039 Bass/037_Pad.sfz",
45: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/032-039 Bass/036_Overdrive Guitar.sfz",
46: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/032-039 Bass/032_Orchestra Hit 3.sfz",
47: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/032-039 Bass/033_Orchestra Hit 4.sfz",
48: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/032-039 Bass/039_Pluck.sfz",
49: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/032-039 Bass/035_Organ 2.sfz",
50: "/home/SoundFonts/SFZFonts/DAYTONA USA SOUNDS/Daytona USA sfz/120-127 Sound effects/127_Drumkit.sfz",
51: "/home/SoundFonts/SFZFonts/Classical_Acoustic_Guitar/Classical Acoustic Guitar/Classical Acoustic.sfz",
52: "/home/SoundFonts/SFZFonts/DamiensFunkyGuitar/Damiens funky guitar.sfz",
53: "/home/SoundFonts/SFZFonts/Ixox.Flute/Ixox Flute.sfz",
54: "/home/SoundFonts/SFZFonts/spanish-classical-guitar/SpanishClassicalGuitar-20190618.sfz",
55: "/home/SoundFonts/SFZFonts/SalamanderGrandPianoV3Retuned.sfz",
56: "/home/SoundFonts/SFZFonts/SalamanderGrandPianoV3.sfz",
57: "/home/SoundFonts/SFZFonts/synth-soundtrack/SynthSoundtrack 20200521.sfz",
58: "/home/SoundFonts/SFZFonts/glass/Glass-20191227.sfz",
59: "/home/SoundFonts/SFZFonts/basic-harmonica/harmonica solo.sfz",
60: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Alto Sax.sfz",
61: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Soprano Sax.sfz",
62: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Baritone Sax (NL).sfz",
63: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Tenor Sax (NL).sfz",
64: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Soprano Sax (NL).sfz",
65: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Tenor Sax.sfz",
66: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Alto Sax (NL).sfz",
67: "/home/SoundFonts/SFZFonts/MTG.SoloSax/MTG Solo Saxophones/MTG Baritone Sax.sfz",
68: "/home/SoundFonts/SFZFonts/tubular-bells1/TubularBells small 20241130.sfz",
69: "/home/SoundFonts/SFZFonts/tubular-bells1/TubularBells 20241130.sfz",
70: "/home/SoundFonts/SFZFonts/synth-brass-2/SynthBrass2 20240610.sfz",
71: "/home/SoundFonts/SFZFonts/synth-strings-2/SynthStrings2 20200528.sfz",
72: "/home/SoundFonts/SFZFonts/Marimba/Marimba/Marimba.sfz",
73: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/electric_three.sfz",
74: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/acoustic_noises.sfz",
75: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/acoustic_four.sfz",
76: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/acoustic_three.sfz",
77: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/electric_two.sfz",
78: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/electric_four.sfz",
79: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/electric_five.sfz",
80: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/electric_release.sfz",
81: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/acoustic_one.sfz",
82: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/electric_one.sfz",
83: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/acoustic_two.sfz",
84: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/acoustic.sfz",
85: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/main.sfz",
86: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/acoustic_release.sfz",
87: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/acoustic_five.sfz",
88: "/home/SoundFonts/SFZFonts/karoryfer.shinyguitar/Programs/electric_noises.sfz",
89: "/home/SoundFonts/SFZFonts/jlearman.SteelDrum/_jSteelDrum-flac-noxfade.sfz",
90: "/home/SoundFonts/SFZFonts/jlearman.SteelDrum/_jSteelDrum-flac.sfz",
91: "/home/SoundFonts/SFZFonts/jlearman.SteelDrum/jSteelDrum-flac-sfz",
92: "/home/SoundFonts/SFZFonts/jlearman.SteelDrum/jSteelDrum-flac-sfz/_jSteelDrum-flac-noxfade.sfz",
93: "/home/SoundFonts/SFZFonts/jlearman.SteelDrum/jSteelDrum-flac-sfz/_jSteelDrum-flac.sfz",
94: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/dynfade_legato_first_map.sfz",
95: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/noises_map.sfz",
96: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/marcato_short_map.sfz",
97: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/dynfade_marcato_long_map.sfz",
98: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/dynfade_marcato_second_map.sfz",
99: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/breath.sfz",
},

5: {
0: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/staccato_map.sfz",
1: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/dynfade_legato_second_map.sfz",
2: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/growl_map.sfz",
3: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/legato/sub_map.sfz",
4: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_poly/dynfade_map.sfz",
5: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_poly/marcato_short_map.sfz",
6: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_poly/dynfade_marcato_long_map.sfz",
7: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_poly/staccato_map.sfz",
8: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/poly/noises_map.sfz",
9: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/poly/dynfade_map.sfz",
10: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/poly/marcato_short_map.sfz",
11: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/poly/dynfade_marcato_long_map.sfz",
12: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/poly/breath.sfz",
13: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/poly/staccato_map.sfz",
14: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/poly/growl_map.sfz",
15: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/poly/sub_map.sfz",
16: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/5-bearcussion.sfz",
17: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/3-duo-mono.sfz",
18: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/2-solo-poly.sfz",
19: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/soft_center.sfz",
20: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/soft_center_mono.sfz",
21: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/single_cycle_sides.sfz",
22: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/medium_sides_mono.sfz",
23: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/soft_sides.sfz",
24: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/single_cycle_center.sfz",
25: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/noise.sfz",
26: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/noise_mono.sfz",
27: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/medium_sides.sfz",
28: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/single_cycle_center_mono.sfz",
29: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/medium_center_mono.sfz",
30: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/medium_center.sfz",
31: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/soft_sides_mono.sfz",
32: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/bearborg/single_cycle_sides_mono.sfz",
33: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_legato/dynfade_legato_first_map.sfz",
34: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_legato/marcato_short_map.sfz",
35: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_legato/dynfade_marcato_long_map.sfz",
36: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_legato/dynfade_marcato_second_map.sfz",
37: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_legato/staccato_map.sfz",
38: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/duo_legato/dynfade_legato_second_map.sfz",
39: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/1-solo-mono.sfz",
40: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/4-duo-poly.sfz",
41: "/home/SoundFonts/SFZFonts/karoryfer.bear-sax/Programs/6-bearborg.sfz",
42: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_13.sfz",
43: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_01.sfz",
44: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_26.sfz",
45: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_18.sfz",
46: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_35.sfz",
47: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_20.sfz",
48: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_21.sfz",
49: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_25.sfz",
50: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_16.sfz",
51: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_19.sfz",
52: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_29.sfz",
53: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_36.sfz",
54: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_03.sfz",
55: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_08.sfz",
56: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_07.sfz",
57: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_05.sfz",
58: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_09.sfz",
59: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_22.sfz",
60: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_33.sfz",
61: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_32.sfz",
62: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_04.sfz",
63: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_11.sfz",
64: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_28.sfz",
65: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_14.sfz",
66: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_24.sfz",
67: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_06.sfz",
68: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_17.sfz",
69: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_34.sfz",
70: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_31.sfz",
71: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_15.sfz",
72: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_02.sfz",
73: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_30.sfz",
74: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_27.sfz",
75: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_10.sfz",
76: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_12.sfz",
77: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_23.sfz",
78: "/home/SoundFonts/SFZFonts/Xmod/MP-XM_37.sfz",
79: "/home/SoundFonts/SFZFonts/Violin SFZ",
80: "/home/SoundFonts/SFZFonts/Violin SFZ/Violin.sfz",
81: "/home/SoundFonts/SFZFonts/0 vibes/tachoir's vibraphone.sfz",
82: "/home/SoundFonts/SFZFonts/Baritone_Sax/Baritone Sax/Baritone Sax.sfz",
83: "/home/SoundFonts/SFZFonts/sfzinstruments.github.io/macros/includes/sfz",
84: "/home/SoundFonts/SFZFonts/sfzinstruments.github.io/data/sfz",
85: "/home/SoundFonts/SFZFonts/sfzinstruments.github.io/layout/sfz",
86: "/home/SoundFonts/SFZFonts/synth-calliope/SynthCalliope 20200512.sfz",  # nice with lead
87: "/home/SoundFonts/SFZFonts/Terkelsen.Marimba/Terkelsen's Marimba - xfade.sfz",
88: "/home/SoundFonts/SFZFonts/Terkelsen.Marimba/Terkelsen's Marimba.sfz", # Great
},
}


# -----------------------------
# STATE
# -----------------------------

current_bank = 0
current_program = 0
previous_program = 0
previous_bank = 0

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
    
    
    


