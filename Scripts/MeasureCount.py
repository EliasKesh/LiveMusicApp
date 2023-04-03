#!/usr/bin/python

import logging
import os
import sys
import threading

from halo import Halo
from pynput.keyboard import Key, Listener

matcher = 0
KeyCount = 0
gKeyCount = 0

def on_release(key):
    global matcher
    global KeyCount
    global gKeyCount
    
    key = str(key).strip("'")
#    logging.debug(f'Key pressed: {key}')
#    print('Key pressed: ', key)

    if key == 'v':
        print ("Verse ", KeyCount)
        KeyCount = 0

    if key == 'c':
        print ("Chorus ", KeyCount)
        KeyCount = 0

    if key == 'm':
        print ("Mark ", KeyCount)
        KeyCount = 0

    KeyCount = KeyCount + 1
    gKeyCount = gKeyCount + 1


# with Halo(text='Mark ', spinner='dots'):
try:
    with Listener(on_release=on_release) as listener:
        listener.join()
except KeyboardInterrupt:
    print('\nStopped', gKeyCount)

    sys.exit(0)

