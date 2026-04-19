#!/usr/bin/python3
#--------------------------------------------------------------
#
#	File: 	CarlaPresets.py
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Jun 06, 2025
#
#	Copyright : 	2024 Elias Keshishoglou all rights reserved.
#   
# 
#-------------------------------------------------------------#
import re
import sys
import math

def convert_to_decibel(arr):

    if arr!=0:
        return(1 * (10**(arr / 20.0)) )

#        return 10 * math.log10(abs(arr))    
    else:
        return 1.0

def extract_gain_values(file_path):
    gain_pattern = re.compile(r"g_(\d+)\s*=\s*([-+]?[0-9]*\.?[0-9]+)")
    gain_values = {}

    with open(file_path, "r") as file:
        for line in file:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            match = gain_pattern.match(line)
            if match:
                index = int(match.group(1))
                value = float(match.group(2))
                gain_values[index] = convert_to_decibel(value)


    # Ensure values are sorted by index
    return [gain_values[i] for i in sorted(gain_values)]

if __name__ == "__main__":
#    path_to_file = "2025-0606MellowEQ.cfg"  # Update with your file path
    path_to_file = sys.argv[1]

    gains = extract_gain_values(path_to_file)
    print("[ ", end='')
    print(', '.join('{:0.2f}'.format(i) for i in gains), end='')
    print(" ]")

    #print ["%0.2f" % i for i in gains]

    #print("Extracted gain values:", gains)
