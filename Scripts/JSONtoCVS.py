#!/usr/bin/python
#---------------------------------------------------------------------
#
#	File: 	JSONtoCSV
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
import csv

def handler(signal_received, frame):
    # Handle any cleanup here
    print('SIGINT or CTRL-C detected. Exiting gracefully')
    exit(0)


signal(SIGINT, handler)

SourceDir="/home/Music/Charts/FakeBooks/ScannedBooks"
DestDir="/home/Music/Charts/FakeBooksOCR/ScannedBooks"
print("Starting")
NewFile=open("/home/Music/Charts/FakeBooksOCR/Master.csv", "w+")
#OutFile = csv.writer(NewFile,delimiter=',',
#    quotechar='|', quoting=csv.QUOTE_NONE)


# for Root, Dir, Files in os.walk(SourceDir, followlinks=True, topdown=False):
for Root, Dir, Files in os.walk(DestDir, topdown=True):
    Files = [f for f in Files if not f[0] == '.']
    Dir[:] = [d for d in Dir if not d[0] == '.']
#    CurrentDir=os.path.basename(Root)
    CurrentDir=os.path.relpath(Root, start=SourceDir)

    for filename in Files:
        if (filename.endswith(".json")):
#            if (CurrentDir != "."):
            print("Filename: "+filename)
            
            NewSource=(DestDir+"/"+CurrentDir+"/"+filename)
            NewDest=(DestDir+"/"+CurrentDir+"/"+filename)
            NewDestCSV=os.path.splitext(NewDest)[0]+".csv"
            LocalFile=CurrentDir+"/"+filename
            filenamePDF=os.path.splitext(LocalFile)[0]+".pdf"
#            if (os.path.exists(NewDestCSV)):
#                continue

            print("Open "+NewSource+"  "+filenamePDF)
            NewFile=open(NewDestCSV, "w+")
            with open(NewSource, newline='') as csvfile:
                TOCData = csv.reader(csvfile, delimiter=',', quotechar='|')
                for row in TOCData:
                    print(row[1:3], end = '')
                    print(","+filenamePDF)
                    TOCData=str(row[1:2])
                    TOCData=TOCData.replace('[','')
                    TOCData=TOCData.replace(']','')
                    TOCData=TOCData.replace('\'','')
                    TOCData=TOCData.replace('\"','')
                    TOCData=TOCData.replace('.png','')
                    TOCData=TOCData.replace('.agz','')
                    TOCNumber=str(row[2:3])
                    TOCNumber=TOCNumber.replace('[','')
                    TOCNumber=TOCNumber.replace(']','')
                    TOCNumber=TOCNumber.replace('\'','')
                    TOCNumber=TOCNumber.replace('\"','')
                    print(TOCNumber)

                    try:
                        PageNumber=int(TOCNumber)
                    except:
                        PageNumber=0

                    if (len(TOCData) and PageNumber > 0):
                        OutLine=TOCData+","+filenamePDF+","+TOCNumber+"\n"
                        NewFile.write(OutLine)
#                    OutFile.writerow(OutLine)

            csvfile.close()
#            exit()

NewFile.close()

            # CommandLine="ocrmypdf "+NewSource+" "+NewDest
            # RetStat=os.system(CommandLine)
            # print("ocrmypdf RetStat ",RetStat)

