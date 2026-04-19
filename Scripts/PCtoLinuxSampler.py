#!/usr/bin/python
#---------------------------------------------------------------------
#
#	File: 	PCtoLinuxSampler
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Fri Apr 10 22:47:15 EEST 2026
#
#	Copyright : 	2026 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
import mido
import socket

# -----------------------------
# CONFIG
# -----------------------------

LSCP_HOST = "localhost"
LSCP_PORT = 8888
MIDI_INPUT_DEVICE = "Launchkey"


# 🎛️ Choose MIDI input device here (IMPORTANT)
MIDI_INPUT_DEVICE = "Virtual Raw MIDI 4-0:VirMIDI 4-0 32:0"
# set to exact name, or None = first available

CHANNEL = 0

# Program Change → instrument mapping
INSTRUMENTS = {
    0: ("/home/SoundFonts/FluidR3GM.sf2", "SFZ"),
    1: ("/home/SoundFonts/Elias2025.sf2", "SFZ"),
    2: ("/home/SoundFonts/FatBoy-v0.786.sf2", "SFZ"),
    3: ("/home/SoundFonts/Ibanez_Soundgear.gig", "GIG"),
}

# -----------------------------
# LSCP COMMUNICATION
# -----------------------------

def send_lscp(cmd):
    print(cmd)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((LSCP_HOST, LSCP_PORT))
        s.sendall((cmd + "\r\n").encode())
        return s.recv(4096).decode()


def load_instrument(path):
    cmd = f'LOAD INSTRUMENT "{path}" {CHANNEL}'
    print("LSCP:", cmd)
    print(send_lscp(cmd))


def reset_channel():
    # NOTE: LSCP does NOT support engine switching directly,
    # so we recreate channel to avoid state issues
    send_lscp(f"REMOVE CHANNEL {CHANNEL}")
    send_lscp("ADD CHANNEL")
    send_lscp(f"SET CHANNEL AUDIO_OUTPUT_DEVICE 0")
    send_lscp(f"SET CHANNEL MIDI_INPUT_DEVICE 0")


# -----------------------------
# MIDI HANDLING
# -----------------------------

def handle_program_change(program):
    if program not in INSTRUMENTS:
        print("No mapping for program:", program)
        return

    path, engine = INSTRUMENTS[program]

    print(f"\nProgram Change → {program}")
    print(f"Loading → {path}")

    reset_channel()
    load_instrument(path)


def choose_midi_device():
    inputs = mido.get_input_names()

    if not inputs:
        print("No MIDI devices found")
        return None

    print("\nAvailable MIDI devices:")
    for i, name in enumerate(inputs):
        print(f"{i}: {name}")

    if MIDI_INPUT_DEVICE:
        for name in inputs:
            if MIDI_INPUT_DEVICE in name:
                print(f"\nUsing configured device: {name}")
                return name
        print("\nConfigured device not found, using first.")
        return inputs[0]

    print(f"\nUsing default: {inputs[0]}")
    return inputs[0]


def main():
    print("Listening for MIDI Program Change...")

    device = choose_midi_device()
    if not device:
        return

    with mido.open_input(device) as port:
        for msg in port:
            if msg.type == "program_change":
                handle_program_change(msg.program)


if __name__ == "__main__":
    main()


