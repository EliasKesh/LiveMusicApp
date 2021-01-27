#!/usr/bin/python
#------------------------------------------------
#
#	File: 	LiveMusicCharts
#
#	Contains: 	
#
#
#	Written By: 	Elias Keshishoglou on Wed 29 Jan 2020 09:32:02 PM PST
#
#	Copyright: 	2020 Elias Keshishoglou all rights reserved.
#
#
#------------------------------------------------#
import os
import argparse
from PyPDF2 import PdfFileReader, PdfFileWriter
# from pdf2image import convert_from_path, convert_from_bytes
from wand.image import Image
import sys

sPresets=[]
sSetIndex=0
sSetNow=[]
sHREFFile=[]
sHREFIndex=0
sTempo=0
sDrumFile=""
sLoopFile=""
sIntroCount=0
sBeatsPerMeasure=0
sLoopLength=0
sSolo=""
sSrcIndex=0
sSrcFile=[]

sMP3File=[]
sMP3Index=0
sMidiFile=[]
sMidiIndex=0
sGPFile=[]
sGPIndex=0
sTitle=""

# ------------------------------------------
def ClearVariables():
  global sSetIndex
  global sHREFIndex
  global sPresets
  global sTempo
  global sSolo
  global sDrumFile
  global sLoopFile
  global sSrcIndex
  global sGlobalNotes
  global sBeatsPerMeasure
  global sLoopLength
  global sIntroCount
  global sSetNow
  global sHREFFile
  global sSrcFile

  sSetIndex=0
  sPresets=['','','','','','','','','','','','','']
  sSetNow=['','','','','','','','','','','','','']
  sSetIndex=0
  sHREFFile=['','','','','','','','','','','','','']
  sHREFIndex=0
  sSrcFile=['','','','','','','','','','','','','']
  sSrcIndex=0
  sGlobalNotes=""

  sTempo=10
  sDrumFile=""
  sLoopFile=""
  sIntroCount=4
  sBeatsPerMeasure=4
  sLoopLength=4
  sSolo="XXX"
  


# Check for a valid file.
# ------------------------------------------
def ParseFile(fname,dirname):
  global sSetIndex
  global sHREFIndex
  global sTempo
  global sSolo
  global sDrumFile
  global sLoopFile
  global sSrcIndex
  global sGlobalNotes
  global sBeatsPerMeasure
  global sLoopLength
  global sIntroCount

#  print("Parse Function ",fname, dirname)
  userContStart = 0;
  userContEnd = 0;

  # open file in variable
  theHtmlFile = open(dirname+"/"+fname, 'r')
  theFileStr = theHtmlFile.read()

#  Check to see if this is a LiveMusicApp HTML
  userContStart = theFileStr.find("<LiveMusic>")
  userContEnd = theFileStr.find("</LiveMusic>")
  if (userContStart < 0 or userContEnd < 0 ):
    sys.stdout.write("**** NO ****")
    theHtmlFile.close()
    return (1)

  sGlobalNotes=theFileStr[userContStart+11:userContEnd]

#    print("Found ", result)

    # Let's split the file into lines for parsing
  for theLine in theFileStr.splitlines():
#    print("Line ", theLine)

    # Look for embedded Preset
    contentRes = theLine.find("Preset")
    if (contentRes > 0):
      PreNumber = theLine[contentRes + 6]
      contentRes = theLine.find("content=")
    #        print ("contentRes ", PreNumber, theLine[contentRes + 8:-1].replace("\"", ""))
      sPresets[int(PreNumber)]=theLine[contentRes + 8:-2].replace("\"", "")

    contentRes = theLine.find("Tempo")
    if (contentRes > 0):
      contentRes = theLine.find("content=")
    #        sTempo=int(contentRes)
      sTempo=theLine[contentRes + 8:-2].replace("\"", "")
    #        print ("Tempo ", theLine[contentRes + 8:-1].replace("\"", ""))

    contentRes = theLine.find("SetNow")
    if (contentRes > 0):
      contentRes = theLine.find("content=")
      sSetNow[sSetIndex]=theLine[contentRes + 8:-2].replace("\"", "")
      sSetIndex=sSetIndex+1
    #        print ("SetNow ", sSetIndex, theLine[contentRes + 8:-1].replace("\"", ""))

    contentRes = theLine.find("IntroCount")
    if (contentRes > 0):
      contentRes = theLine.find("content=")
      sIntroCount=theLine[contentRes + 8:-2].replace("\"", "")
    #        print ("IntroCount ", theLine[contentRes + 8:-1].replace("\"", ""))

    contentRes = theLine.find("BeatsPerMeasure")
    if (contentRes > 0):
      contentRes = theLine.find("content=")
    #        sBeatsPerMeasure=int(contentRes)
      sBeatsPerMeasure=theLine[contentRes + 8:-2].replace("\"", "")
    #        print ("BeatsPerMeasure ", theLine[contentRes + 8:-1].replace("\"", ""))

    contentRes = theLine.find("DrumFile")
    if (contentRes > 0):
      contentRes = theLine.find("content=")
      sDrumFile=theLine[contentRes + 8:-2].replace("\"", "")
    #     print ("DrumFile ", theLine[contentRes + 8:-1].replace("\"", ""))

    contentRes = theLine.find("LoopFile")
    if (contentRes > 0):
      contentRes = theLine.find("content=")
      sLoopFile=theLine[contentRes + 8:-2].replace("\"", "")
    #        print ("LoopFile ", theLine[contentRes + 8:-1].replace("\"", ""))

    contentRes = theLine.find("LoopLength")
    if (contentRes > 0):
      contentRes = theLine.find("content=")
      sLoopLength=theLine[contentRes + 8:-2].replace("\"", "")
    #       print ("LoopLength ", theLine[contentRes + 8:-1].replace("\"", ""))

    contentRes = theLine.find("font color=")
    if (contentRes > 0):
      contentRes = theLine.find("Solo")
      if (contentRes > 0):
        theValue = theLine[contentRes + 5:-2].split("<")
        sSolo=theValue[0]

    contentRes = theLine.find("href=")
#    print("HREF___", contentRes, theLine)
    if (contentRes > 0):
      theValue = theLine[contentRes + 5:-2].split(">")
      sHREFFile[sHREFIndex]=theValue[0]
      sHREFIndex=sHREFIndex+1

    contentRes = theLine.find("src=")
    if (contentRes > 0):
      theValue = theLine[contentRes+5:-2].split("\"")
      sSrcFile[sSrcIndex]=theValue[0]
      sSrcIndex=sSrcIndex+1

  theHtmlFile.close()
#  PrintVariables()
  return (0)

# reload the list of files based on what's in the
# directory
# ------------------------------------------
def ExtractPDF(Files, dirname):
#    print ("Load Variables ",Files)
    for theFile in Files:
        if (theFile.endswith("pdf")):
            f = dirname+"/"+theFile
            print ("pdf ", f)
            with(Image(filename=f, resolution=120)) as source: 
               for i, image in enumerate(source.sequence):
                   newfilename = f[:-4] + str(i + 1) + '.pdf.jpg'
                   Image(image).save(filename=newfilename)
                   print ("PDF to ", newfilename)
           
            if (os.path.exists(f)):
              os.rename(f,f+".conv")


# Main
# ------------------------------------------
#
MySongList=[]
FoundHTML=0
parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument("BaseDir", help="Base Directory")
parser.add_argument("-v", action='store_true', help="Verify")
parser.add_argument("-f", action='store_true', help="Fix additional meta Data")
parser.add_argument("-i", action='store_true', help="Create index.html")
parser.add_argument("-c", action='store_true', help="Create HTML from folder")
parser.add_argument("-r", action='store_true', help="Reference on Create")
args = parser.parse_args()
#print args.accumulate(args.integers)

# look for the base directory
if (args.BaseDir == ""):
#    BaseDir="/home/elias/MySongs/FusionBlue/"
    BaseDir="/mnt/Personal/"
else:
    BaseDir=args.BaseDir

# This returns file in one directry
# for Files in os.listdir(BaseDir):
# This is recursive
for Root, Dir, Files in os.walk(BaseDir):
#    print (Root, Dir, Files)
    # Get the Current Dir Name.
    sTitle=os.path.basename(Root)

    FoundHTML=0
    ClearVariables()
    for filename in Files:
#        Check for an html file
        if filename.endswith('.html'):
            sys.stdout.write("\n"+filename+" ")
            FoundHTML=1
#           walk thru a file and pull out meta data.
            if (ParseFile(filename,Root) == 0):
                ExtractPDF(Files,Root)

# jpdfbookmarks.jar for modify pages