#!/usr/bin/python3
import subprocess
import sys
import os

os.execvp("pw-jack", [
    "pw-jack", "-s", "96000",
    "python3",
    "/home/elias/workspace/LiveMusicApp/Scripts/PCtoSFIZZ.py"
])


