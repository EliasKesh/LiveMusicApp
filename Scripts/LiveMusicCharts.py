#!/usr/bin/python
#---------------------------------------------------------------------
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
#---------------------------------------------------------------------#
import os
import argparse
from PyPDF2 import PdfFileReader, PdfFileWriter
from wand.image import Image
import sys

sPresets=[]
sSetIndex=0
sSetNow=[]
sHREFFile=[]
sHREFIndex=0
SongMark=[]
SongMarkIndex=0
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

# Write out the new HTML file 
# ------------------------------------------
def WriteFile(fname, dirname):
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
  global sIntroCount # open file in variable
  global SongMark
  global SongMarkIndex


  theFile = open(fname, 'w+')
  # start writing the HTML header
  theFile.write("<html>\n<head>\n<style type=\"text/css\">\n")
#  theFile.write("{text-align: left; width: 100%; height: 100%;}\n body {\n")


  theFile.write("body {\nbackground-image: url(../background.png);\nheight: 100%; }")
  theFile.write("</style>\n</head>\n")

# background-repeat:background-size: 200%;\n\
# no-repeat;
# background-color: #000000;\n\
# height: 100%; }\n\

  theFile.write("<title>"+sTitle+"</title>\n")
  theFile.write("<p style=\"line-height:125%\">\n<body>\n<big>\n")

  # Add the preset and other liveMusic meta data.
  for x in range(1, 6):
    theFile.write("<meta name=\"Preset"+str(x)+"\" content=\""+sPresets[x]+"\">\n")
  for x in range(0, sSetIndex):
    theFile.write("<meta name=\"SetNow\" content=\""+sSetNow[x]+"\">\n")
      
  theFile.write("<meta name=\"Tempo\" content=\""+str(sTempo)+"\">\n")
  theFile.write("<meta name=\"DrumFile\" content=\""+sDrumFile+"\">\n")
  theFile.write("<meta name=\"LoopFile\" content=\""+sLoopFile+"\">\n")
  theFile.write("<meta name=\"IntroCount\" content=\""+str(sIntroCount)+"\">\n")
  theFile.write("<meta name=\"BeatsPerMeasure\" content=\""+str(sBeatsPerMeasure)+"\">\n")
  theFile.write("<meta name=\"LoopLength\" content=\""+str(sLoopLength)+"\">\n")

  for x in range(0, SongMarkIndex):
    theFile.write("<meta name=\"SongMark\" content=\""+SongMark[x]+"\">\n")

  theFile.write("<font color=#880000>"+sTitle+"</font>\n")
  theFile.write("<font color=#8800FF>Solo:"+sSolo+"</font>\n")
  # The Logo
  theFile.write("<img style=\"width:60px; height:50px\" SRC=\"../logo.jpg\" align=right>")

  # Add links for the external media files.
  for x in range(0, sHREFIndex):
    FileName=sHREFFile[x]
    FileRef=sHREFFile[x]
    if (len(FileName) > 12):
        FileName=FileName[0:12]

    if (FileRef.find("mp3") > 0):
        theFile.write("<a style=\"color:red\" href=\""+FileRef+"\">["+FileName+"]</a>\n")
    elif (FileRef.find("tg") > 0):
        theFile.write("<a style=\"color:blue\" href=\""+FileRef+"\">["+FileName+"]</a>\n")
    elif (FileRef.find("mid") > 0):
        theFile.write("<a style=\"color:yellow\" href=\""+FileRef+"\">["+FileName+"]</a>\n")
    elif (FileRef.find("mscz") > 0):
        theFile.write("<a style=\"color:orange\" href=\""+FileRef+"\">["+FileName+"]</a>\n")
    else:    
        theFile.write("<a style=\"color:lightgreen\" href=\""+FileRef+"\">["+FileName+"]</a>\n")

# Re-insert any user data
  ModString=sGlobalNotes.replace("\n\n","\n")
#  print(ModString)
#  theFile.write("<br><LiveMusic><code>\n")
  theFile.write("<br>\n<LiveMusic>")
  theFile.write(ModString)
#  theFile.write("\n</code></LiveMusic>\n")
  theFile.write("</LiveMusic>")

# Add the link for the charts 
  for x in range(0, sSrcIndex):
    FileName=sSrcFile[x]

# Look for a PDF file and see how many Pages.
#    if (FileName.find(".pdf") > 0):
    if (FileName.endswith("pdf") > 0):
        print ("PDF Name: ",dirname+"/"+FileName )
        pdf = PdfFileReader(open(dirname+"/"+FileName,'rb'))
        pdfPages=pdf.getNumPages()

#       a Bug in the evince browser plugin
        if (pdf.getNumPages() == 2):
          print ("Elias FIX: ",dirname+"/"+FileName )

        # If it's a two page PDF set side by side mode
        if (pdf.getNumPages() == 1):
          theFile.write("<embed src=\""+FileName+"\"  zoommode=\"auto\" height=\"100%\" continuous=\"false\" width=\"100%\" >\n")
        else:
          theFile.write("<embed src=\""+FileName+"\"  zoommode=\"auto\" dual=\"true\" currentpage=\"2\"  continuous=\"false\" height=\"100%\" width=\"100%\" >\n")
    else:
        theFile.write("<img alt=\"\" src=\""+FileName+"\"  height=\"100%\" >\n")
 #       theFile.write("<img alt=\"\" src=\""+FileName+"\" height=\"100%\" width=\"100%\" >\n")

# Options for evince browser plugin
#  ZoomMode "none"  "fit-page" "fit-width"  "auto") == 0)
# Use pdfmod to insert a dummy front page.
#from PyPDF2 import PdfFileReader
#pdf = PdfFileReader(open('path/to/file.pdf','rb'))
#pdf.getNumPages()

  theFile.write("</body>\n</html>\n")
  theFile.close()

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
  global SongMark
  global SongMarkIndex

  sSetIndex=0
  sPresets=['','','','','','','','','','','','','']
  sSetNow=['','','','','','','','','','','','','']
  sSetIndex=0
  sHREFFile=['','','','','','','','','','','','','']
  sHREFIndex=0
  SongMark=['','','','','','','','','','','','','']
  SongMarkIndex=0
  sSrcFile=['','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','']
  sSrcIndex=0
  sGlobalNotes=""

  sTempo=10
  sDrumFile=""
  sLoopFile=""
  sIntroCount=4
  sBeatsPerMeasure=4
  sLoopLength=4
  sSolo="XXX"
  

# ------------------------------------------
def PrintVariables():
  global sSetIndex
  global SongMark
  global SongMarkIndex

  print ("*************************")
  print ("sPresets 1", sPresets[1])
  print ("sPresets 2", sPresets[2])
  print ("sPresets 3", sPresets[3])
  print ("sPresets 4", sPresets[4])
  print ("sPresets 5", sPresets[5])
  print ("sTempo ", sTempo)
  print ("sSetIndex ", sSetIndex)
  print ("sSetNow 1 ", sSetNow[0])
  print ("sIntroCount ", sIntroCount)
  print ("sBeatsPerMeasure ", sBeatsPerMeasure)
  print ("sDrumFile ", sDrumFile)
  print ("sLoopFile ", sLoopFile)
  print ("sLoopLength ", sLoopLength)
  print ("sSolo ", sSolo)
  print ("sHREFIndex ", sHREFIndex)
  for x in range(0, sHREFIndex):
    print ("sHREFFile ",x, sHREFFile[x])
  for x in range(0, sSrcIndex):
    print ("sSrcFile ",x, sSrcFile[x])
  for x in range(0, SongMarkIndex):
    print ("Mark ",x, SongMark[x])

  print ("Notes=",sGlobalNotes)



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
  global SongMark
  global SongMarkIndex

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

    # Look for embedded Preset
    contentRes = theLine.find("SongMark")
    if (contentRes > 0):
      contentRes = theLine.find("content=")
      HoldString=theLine[contentRes + 8:-2].replace("\"", "")
#      print ("SongMark ", SongMarkIndex, HoldString)
      SongMark[SongMarkIndex]=HoldString
      SongMarkIndex=SongMarkIndex+1

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
def LoadVariables(Files):
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
    global SongMarkIndex
    global SongMark

#    print ("Load Variables ",Files)
    sHREFIndex=0
    sSrcIndex=0

    Files.sort()
    for filename in Files:

        if (filename.endswith("mp3")):
#            print ("MP3 ", filename)
            sHREFFile[sHREFIndex]=filename
            sHREFIndex=sHREFIndex+1

        if (filename.endswith("mscz")):
#            print ("Midi ", filename)
            sHREFFile[sHREFIndex]=filename
            sHREFIndex=sHREFIndex+1

        if (filename.endswith("mid")):
#            print ("Midi ", filename)
            sHREFFile[sHREFIndex]=filename
            sHREFIndex=sHREFIndex+1

        if (filename.endswith("tg")):
#            print ("tg ", filename)
            sHREFFile[sHREFIndex]=filename
            sHREFIndex=sHREFIndex+1

        if (filename.endswith("mkv")):
 #           print ("mkv ", filename)
            sHREFFile[sHREFIndex]=filename
            sHREFIndex=sHREFIndex+1

        if (filename.endswith("mp4")):
  #          print ("mp4 ", filename)
            sHREFFile[sHREFIndex]=filename
            sHREFIndex=sHREFIndex+1

        if (filename.endswith("png")):
 #           print ("png ", filename)
            sSrcFile[sSrcIndex]=filename
            sSrcIndex=sSrcIndex+1

        if (filename.endswith("jpg")):
 #           print ("jpg ", filename)
            sSrcFile[sSrcIndex]=filename
            sSrcIndex=sSrcIndex+1

 #       if (filename.endswith("pdf")):
 #           print ("pdf ", filename)
 #           sSrcFile[sSrcIndex]=filename
 #           sSrcIndex=sSrcIndex+1

# Create the main index html page
# ------------------------------------------
def GenerateIndex(Base, List,Reference):
    MaxNameLength=18
    Padding="_____________________________________"
    theFile = open(Base+"/"+"index.html", 'w+')
    theFile.write
    theFile.write("<html>\n<head>\n<style type=\
\"text/css\">\n\
{text-align: left; \
width: 100%; height: 100%;}\
\n body {\n")
    theFile.write("background-image: url(../background.png);\n\
background-repeat: no-repeat;\n\
background-size: 100%;\n\
color: blue;\n\
background-color: #000000;\n\
height: 100%; }\n\
</style>\n\
</head>\n")
    theFile.write("<title>MainIndex</title>\n")
    theFile.write("<p style=\"line-height:135%\">\n<body>\n\n")
    theFile.write("<body link=\"#ffd0a0\" vlink=\"#d08080\" alink=\"#FF0000\"><tr><tt>")

    if (Reference):
      theFile.write("<big><p id=\"#TOP\"></p><a href=\"#Aaaa\">[ __A__ ]</a><a href=\"#Baaa\">[ __B__ ]</a><a href=\"#Caaa\">[ __C__ ]</a><a href=\"#Daaa\">[ __D__ ]</a><a href=\"#Eaaa\">[ __E__ ]</a><a href=\"#Faaa\">[ __F__ ]</a><a href=\"#Gaaa\">[ __G__ ]</a><a href=\"#Haaa\">[ __H__ ]</a><a href=\"#Iaaa\">[ __I__ ]</a><a href=\"#Jaaa\">[ __J__ ]</a><a href=\"#Kaaa\">[ __K__ ]</a><a href=\"#Laaa\">[ __L__ ]</a><a href=\"#Maaa\">[ __M__ ]</a><a href=\"#Naaa\">[ __N__ ]</a><a href=\"#Oaaa\">[ __O__ ]</a><a href=\"#Paaa\">[ __P__ ]</a><a href=\"#Qaaa\">[ __Q__ ]</a><a href=\"#Raaa\">[ __R__ ]</a><a href=\"#Saaa\">[ __S__ ]</a><a href=\"#Taaa\">[ __T__ ]</a><a href=\"#Uaaa\">[ __U__ ]</a><a href=\"#Vaaa\">[ __V__ ]</a><a href=\"#Waaa\">[ __W__ ]</a><a href=\"#Xaaa\">[ __X__ ]</a><a href=\"#Yaaa\">[ __Y__ ]</a><a href=\"#Zaaa\">[ __Z__ ]</a><br>")

    PreviousIndex='Z'
    for x in List:
        FileName=os.path.basename(x)
        DirName=os.path.basename(os.path.dirname(x))
        FileNoExt=os.path.splitext(FileName)[0]
#        print(DirName, FileName)
        Length=len(FileNoExt)
        if (Length > MaxNameLength):
            Length=MaxNameLength
            FileNoExt=FileNoExt[0:MaxNameLength]

        PadLev=(MaxNameLength+1)-int(Length)
        PadString=Padding[1:PadLev]
        # HERE ADD Section Index
        IndexLetter=FileName[0:1]
        CurrentIndex=IndexLetter

        if (CurrentIndex != PreviousIndex):
          if (Reference):
             theFile.write("<p id=\""+CurrentIndex+"aaa\"></p>")
             theFile.write("<a style=\"color:red\" href=\"#TOP\">TOP</a><br>")

        theFile.write("[<a href=./"+DirName+"/"+FileName+">"+FileNoExt+PadString+"</a>]\n")

        PreviousIndex=CurrentIndex
    theFile.write("</tr></tt></code></big>\n</code>\n</p>\n</body>\n</html>")
    theFile.close()

# Create a new html from a folder with content.
# ------------------------------------------
def CreateNewHTML(fname,dirname,Files):
  global sGlobalNotes
  global sTempo
  global sSolo
  global sDrumFile
  global sLoopFile
  global sSrcIndex
  global sGlobalNotes
  global sBeatsPerMeasure
  global sLoopLength
  global sIntroCount
  global sPresets
  global sSetNow
  global sSetIndex
  global SongMark
  global SongMarkIndex

#  print ("In CreateHTML ",fname," in ",dirname)
#  print ("Files ", Files)

  ClearVariables()
  LoadVariables(Files)
  sGlobalNotes="\n<code>\n\
<font color=#ff8002>Chords:  </font> <br>\n\
<font color=#88ffff>Structure:  </font> <br>\n\
</code>\n"
  sPresets=['','Jazz_Pre','Expre Dist','Funky_Pre','','','','','','','','','']
  sSetNow=['Jazz_Pre','','','','','','','','','','','','']
  sSetIndex=1
  WriteFile(dirname+"/"+fname+".html", dirname)

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
parser.add_argument("-p", action='store_true', help="Pdf to JPG")
parser.add_argument("-a", action='store_true', help="All Options")
args = parser.parse_args()
#print args.accumulate(args.integers)

# look for the base directory
if (args.BaseDir == "./"):
    BaseDir=os.getcwd() 
#    BaseDir="/mnt/Personal/ChartsHTML"
else:
    BaseDir=args.BaseDir

ConvertPDF=args.p
FixMetaData=args.f
CreateHTML=args.c
CreateIndexFIle=args.i
ReferenceCreate=args.r

if (args.a):
  ConvertPDF=1
  FixMetaData=1
  CreateHTML=1
  CreateIndexFIle=1
  ReferenceCreate=1

# print ("Base Dir = ", BaseDir)

# if we need to convert PDFs to jpg do that first.
if (ConvertPDF):
  for Root, Dir, Files in os.walk(BaseDir):
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


# This returns file in one directry
# for Files in os.listdir(BaseDir):
# This is recursive
for Root, Dir, Files in os.walk(BaseDir):
#    print (Root, Dir, Files)
    # Get the Current Dir Name.
    sTitle=os.path.basename(Root)

    FoundHTML=0
    for filename in Files:
#        Check for an html file
        if filename.endswith('.html'):
            sys.stdout.write("\n"+filename+" ")

            FoundHTML=1
            ClearVariables()
#           walk thru a file and pull out meta data.
            if (ParseFile(filename,Root) == 0):
#               print ("Adding to list ", filename)

#               add the html to the main index list
                MySongList.append(Root+"/"+filename)
                sys.stdout.write("Added ")

                if (FixMetaData):
                    LoadVariables(Files)
                    WriteFile(Root+"/"+filename,Root)
                    sys.stdout.write("Write ")


#   If we did find a valid html and were asked to create one.
    if (CreateHTML and FoundHTML == 0):
#      print ("Create file from Directory ", sTitle)
      CreateNewHTML(sTitle,Root,Files)
      MySongList.append(Root+"/"+sTitle+".html")

if (CreateIndexFIle):
    MySongList.sort()
#    print("List ----------------",len(MySongList))
    GenerateIndex(BaseDir, MySongList,ReferenceCreate)

# print("Done")



# jpdfbookmarks.jar for modify pages