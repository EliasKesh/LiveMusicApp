#!/usr/bin/python
#---------------------------------------------------------------------
#
#	File: 	OCRMusicBooksIndex
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Wed May 5 04:40:33 PM PDT 2021
#
#	Copyright : 	2021 Elias Keshishoglou all rights reserved.
#
#
#---------------------------------------------------------------------#
import os
import argparse
from signal import signal, SIGINT
from sys import exit


def handler(signal_received, frame):
    # Handle any cleanup here
    print('SIGINT or CTRL-C detected. Exiting gracefully')
    exit(0)




SourceDir="/home/Music/Charts/FakeBooks/ScannedBooks"
DestDir="/home/Music/Charts/FakeBooksOCR/ScannedBooks"

signal(SIGINT, handler)

# for Root, Dir, Files in os.walk(SourceDir, followlinks=True, topdown=False):
for Root, Dir, Files in os.walk(DestDir, topdown=True):
    Files = [f for f in Files if not f[0] == '.']
    Dir[:] = [d for d in Dir if not d[0] == '.']
#    CurrentDir=os.path.basename(Root)
    CurrentDir=os.path.relpath(Root, start=SourceDir)

    for filename in Files:

        if (filename.endswith(".pdf")):
#            if (CurrentDir != "."):
#            
            NewSource=(SourceDir+"/"+CurrentDir+"/"+filename)
            NewDest=(DestDir+"/"+CurrentDir+"/"+filename)
            NewDestOCR=os.path.splitext(NewDest)[0]+".ocr"
            NewDestTBC=os.path.splitext(NewDest)[0]+".json"
            print(NewDest)
  
            if (os.path.exists(NewDest)):
                CommandLine="ocrmypdf "+NewDest+" "+NewDest
                RetStat=os.system(CommandLine)
                print("ocrmypdf RetStat ",RetStat)

                # If OCR didn't find text already
                if (RetStat == 0):
                    print("PDFtoTest ",NewDest)
                    CommandLine="pdftotext "+NewDest+" "+NewDestOCR
                    os.system(CommandLine)
#        else:
#                print("Copy ",NewDest)
     #           CommandLine="cp "+NewDest+" "+NewDest
     #           os.system(CommandLine)

            print("pdf-toc ",NewDest)
            CommandLine="pdf-toc --show-toc json "+NewDest+" > "+NewDestTBC
            RetStat=os.system(CommandLine)

            # print("echo ",NewDest)
            # CommandLine="echo "+filename+" >> "+NewDestTBC
            # RetStat=os.system(CommandLine)

            # if (RetStat == 0):
            #     try:
            #         fp = open(NewDestTBC, 'r')
            #         for line in fp.readlines():
            #     finally:
            #         fp.close()



            # print(SourceDir+"/"+CurrentDir+"/"+filename)
            # print(DestDir+"/"+CurrentDir+"/"+filename)

            # print(SourceDir+"/"+CurrentDir+"/"+filename)
            # print(DestDir+"/"+CurrentDir+"/"+filename)
            # print("-----------------");

# ocrmypdf TheloniousMonk.pdf TheloniousMonkOCR.pdf
# pdftotext TheloniousMonkOCR.pdf TheloniousMonkOCR.txt
# find FakeBooks -type d -exec mkdir FakeBooksOCR/{} \;