#!/usr/bin/env python3
# Modified from ShockingSimpleMetronome.py
# https://askubuntu.com/questions/814759/where-can-i-find-a-metronome-for-music-practice

import gi
gi.require_version('Gtk', '3.0')
#from gi.repository import Gtk
from gi.repository import Gtk, Gdk, GdkPixbuf, Pango
import sys
import subprocess
import time
from threading import Thread
import os
import logging

path = os.path.dirname(os.path.realpath(__file__))

CurrentBeat=1

class MetroWindow(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self, title="Training Metronome")
        self.speed = 70
        self.Endspeed = 70
        self.run = False
        # maingrid
        maingrid = Gtk.Grid()
        maingrid.set_column_homogeneous(True)
        maingrid.set_row_homogeneous(False)
        maingrid.set_border_width(30)
        maingrid.set_row_spacing(30)
        self.add(maingrid)

        # icon
        image = Gtk.Image(xalign=0)
        print(path)
#        image.set_from_file(os.path.join(path, "icon.png"))
        image.set_from_file("/usr/share/icons/ToneLib-Jam.png")
        maingrid.attach(image, 0, 1, 1, 1)

        self.CurrentTempoLabel = Gtk.Label(
            label=70
        )
        self.CurrentTempoLabel.modify_font(Pango.FontDescription("sans 56"))
        self.CurrentTempoLabel.set_justify(Gtk.Justification.RIGHT)
        maingrid.attach(self.CurrentTempoLabel, 0, 0, 1, 1)
#        self.CurrentTempoLabel.set_size_request(10,300)

        self.CurrentBeatLabel = Gtk.Label(
            label=1
        )
        self.CurrentBeatLabel.modify_font(Pango.FontDescription("sans 56"))
        self.CurrentBeatLabel.set_justify(Gtk.Justification.RIGHT)
        maingrid.attach(self.CurrentBeatLabel, 3, 0, 1, 1)


# ------------- Start
        self.Startlabel = Gtk.Label(
            label="Start"
        )
        self.Startlabel.set_justify(Gtk.Justification.RIGHT)
        maingrid.attach(self.Startlabel,1, 0, 1, 1)

        # vertical slider,  initial value, min, max, step, page, psize
        self.StartSpeed = Gtk.Scale(
            orientation=Gtk.Orientation.VERTICAL,
            adjustment=Gtk.Adjustment.new(self.speed, 10, 240, 1, 0, 0)
            )
        self.StartSpeed.set_vexpand(True)
        self.StartSpeed.set_digits(0)
        self.StartSpeed.connect("value-changed", self.scale_moved)
        self.StartSpeed.set_size_request(10,300)
        maingrid.attach(self.StartSpeed, 1, 1, 1, 1)

 
# ------------- End
        self.Endlabel = Gtk.Label(
            label="End"
        )
        self.Endlabel.set_justify(Gtk.Justification.RIGHT)
        maingrid.attach(self.Endlabel, 2, 0, 1, 1)

       # vertical slider,  initial value, min, max, step, page, psize
        self.EndSpeed = Gtk.Scale(
            orientation=Gtk.Orientation.VERTICAL,
            adjustment=Gtk.Adjustment.new(self.Endspeed, 10, 240, 1, 0, 0)
            )
        self.EndSpeed.set_vexpand(True)
        self.EndSpeed.set_digits(0)
        self.EndSpeed.connect("value-changed", self.scaleE_moved)
        self.EndSpeed.set_size_request(10,300)
        maingrid.attach(self.EndSpeed, 2, 1, 1, 1)


        self.togglebutton = Gtk.Button("_Run", use_underline=True)
        self.togglebutton.connect("clicked", self.time_out)
        self.togglebutton.set_size_request(70,20)
        maingrid.attach(self.togglebutton, 0, 2, 1, 1)

        self.quitbutton = Gtk.Button("Quit", use_underline=True)
        self.quitbutton.connect("clicked", self.my_quit)
        self.quitbutton.set_size_request(70,20)
        maingrid.attach(self.quitbutton, 3, 2, 1, 1)

        # start the thread
        self.update = Thread(target=self.run_metro, args=[])
        self.update.setDaemon(True)
        self.update.start()

    def my_quit(self, *args):
        exit(0)

    def scale_moved(self, event):
        self.speed = int(self.StartSpeed.get_value())
        print("Start ", self.speed)

    def scaleE_moved(self, event):
        self.Endspeed = int(self.EndSpeed.get_value())
        print("End ", self.Endspeed)

    def time_out(self, *args):
        if self.run == True:
            self.run = False
            self.togglebutton.set_label("Run")
        else:
            self.run = True
            self.togglebutton.set_label("Pause")

    def Pause(self):
        return 60/self.speed

    def run_metro(self):
        global CurrentBeat
        CurrentTempo=self.speed

        soundfile = "/usr/share/sounds/ubuntu/stereo/button-pressed.mp3"
        while True:
            if self.run == True:
                CurrentBeat=CurrentBeat+1
  
                if (CurrentBeat == 4):
                    CurrentBeat = 0
                    soundfile = "/usr/share/sounds/ubuntu/stereo/bell.mp3"

                    if (int(self.Endspeed) > CurrentTempo):
                        CurrentTempo=CurrentTempo+5

                else:
                    soundfile = "/usr/share/sounds/ubuntu/stereo/button-pressed.mp3"

                self.CurrentTempoLabel.set_label(str(CurrentTempo))
                self.CurrentBeatLabel.set_label(str(CurrentBeat+1))

                subprocess.Popen([
                    "mpg123", 
                    soundfile],
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL)
            else:
                CurrentTempo=self.speed

            time.sleep(60/CurrentTempo)

def run_gui():

    logger = logging.getLogger(__name__)

    logger.setLevel(logging.NOTSET)
#    logger.setLevel(logging.CRITICAL)

    window = MetroWindow()
    window.connect("delete-event", Gtk.main_quit)
    window.set_resizable(False)
    window.show_all()
    Gtk.main()

run_gui()

