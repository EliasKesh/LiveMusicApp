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
# formatting: yapf LiveMusicCharts.py >LiveMusicCharts1.py
#------------------------------------------------#
import os
import argparse
from PyPDF2 import PdfFileReader, PdfFileWriter

from wand.image import Image
import sys
from shutil import copyfile
import subprocess

#from icecream import ic
import logging


sPresets = []
sSetIndex = 0
sSetNow = []
sHREFFile = []
sHREFIndex = 0
SongMark = []
SongMarkIndex = 0
sTempo = 0
sDrumFile = ""
sLoopFile = ""
sIntroCount = 0
sBeatsPerMeasure = 0
sLoopLength = 0
sSolo = ""
sSrcIndex = 0
sSrcFile = []

sMP3File = []
sMP3Index = 0
sMidiFile = []
sMidiIndex = 0
sGPFile = []
sGPIndex = 0
sTitle = ""

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
    global sIntroCount  # open file in variable
    global SongMark
    global SongMarkIndex
#    PDFIndex = 0;

    theFile = open(fname, 'w+')
    # start writing the HTML header
    theFile.write("<html>\n<head>\n<style type=\"text/css\">\n")
    #  theFile.write("{text-align: left; width: 100%; height: 100%;}\n body {\n")

    theFile.write(
        "body {\nbackground-image: url(../background.png);padding-right: 20px;\nheight: 100%; }"
    )
    theFile.write("</style>\n</head>\n")

    # background-repeat:background-size: 200%;\n\
    # no-repeat;
    # background-color: #000000;\n\
    # height: 100%; }\n\

    theFile.write("<title>" + sTitle + "</title>\n")
    theFile.write("<p style=\"line-height:125%\">\n<body>\n<big>\n")

    # Add the preset and other liveMusic meta data.
    for x in range(1, 6):
        theFile.write("<meta name=\"Preset" + str(x) + "\" content=\"" +
                      sPresets[x] + "\">\n")
    for x in range(0, sSetIndex):
        theFile.write("<meta name=\"SetNow\" content=\"" + sSetNow[x] +
                      "\">\n")

    theFile.write("<meta name=\"Tempo\" content=\"" + str(sTempo) + "\">\n")
    theFile.write("<meta name=\"DrumFile\" content=\"" + sDrumFile + "\">\n")
    theFile.write("<meta name=\"LoopFile\" content=\"" + sLoopFile + "\">\n")
    theFile.write("<meta name=\"IntroCount\" content=\"" + str(sIntroCount) +
                  "\">\n")
    theFile.write("<meta name=\"BeatsPerMeasure\" content=\"" +
                  str(sBeatsPerMeasure) + "\">\n")
    theFile.write("<meta name=\"LoopLength\" content=\"" + str(sLoopLength) +
                  "\">\n")

    for x in range(0, SongMarkIndex):
        theFile.write("<meta name=\"SongMark\" content=\"" + SongMark[x] +
                      "\">\n")

    theFile.write("<font color=#880000>" + sTitle + "</font>\n")
    theFile.write("<font color=#8800FF>Solo:" + sSolo + "</font>\n")
    # The Logo
    theFile.write(
        "<img style=\"width:60px; height:50px\" SRC=\"../logo.jpg\" align=right>"
    )

    # Add links for the external media files.
    for x in range(0, sHREFIndex):
        FileName = sHREFFile[x]
        FileRef = sHREFFile[x]
        if (len(FileName) > 30):
            FileName = FileName[0:30]

        if (FileRef.find("mp3") > 0):
            theFile.write("<a style=\"color:red\" href=\"" + FileRef + "\">[" +
                          FileName + "]</a>\n")
        elif (FileRef.find("tg") > 0):
            theFile.write("<a style=\"color:blue\" href=\"" + FileRef +
                          "\">[" + FileName + "]</a>\n")
        elif (FileRef.find("mid") > 0):
            theFile.write("<a style=\"color:yellow\" href=\"" + FileRef +
                          "\">[" + FileName + "]</a>\n")
        elif (FileRef.find("mscz") > 0):
            theFile.write("<a style=\"color:orange\" href=\"" + FileRef +
                          "\">[" + FileName + "]</a>\n")
        else:
            theFile.write("<a style=\"color:lightgreen\" href=\"" + FileRef +
                          "\">[" + FileName + "]</a>\n")

    # Re-insert any user data
    ModString = sGlobalNotes.replace("\n\n", "\n")
    #  logger.debug(ModString)
    #  theFile.write("<br><LiveMusic><code>\n")
    theFile.write("<LiveMusic>")
    theFile.write(ModString)
    #  theFile.write("\n</code></LiveMusic>\n")
    theFile.write("</LiveMusic>\n")

    # Add the link for the charts
    for x in range(0, sSrcIndex):
        FileName = sSrcFile[x]

        # Look for a PDF file and see how many Pages.
        #    if (FileName.find(".pdf") > 0):
        if (FileName.endswith("pdf") > 0):
            logger.info("PDF Name: ", dirname + "/" + FileName)
            theFile.write("<a style=\"color:blue\" href=\"" + FileName +
                          "\">[" + FileName + "]</a>\n")

            #         pdf = PdfFileReader(open(dirname+"/"+FileName,'rb'))
            #         pdfPages=pdf.getNumPages()

            # #       a Bug in the evince browser plugin
            #         if (pdf.getNumPages() == 2):
            #           logger.debug ("Elias FIX: ",dirname+"/"+FileName )

            # If it's a two page PDF set side by side mode
            #        if (pdf.getNumPages() == 1):


#            theFile.write(
#                "<embed src=\"" + FileName +
#                "\"  zoommode=\"auto\" height=\"100%\" continuous=\"false\" width=\"100%\" >\n"
#            )
#       else:
#        theFile.write("<embed src=\""+FileName+"\"  zoommode=\"auto\" dual=\"true\" currentpage=\"2\"  continuous=\"false\" height=\"100%\" width=\"100%\" >\n")
        else:
            theFile.write("<img alt=\"\" src=\"" + FileName +
                          "\" width=\"100%\" >\n")
#       theFile.write("<img alt=\"\" src=\""+FileName+"\" width=\"75%\" >\n")
#       theFile.write("<img alt=\"\" src=\""+FileName+"\" >\n")
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

    sSetIndex = 0
    sPresets = [
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', ''
    ]
    sSetNow = [
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', ''
    ]
    sSetIndex = 0
    sHREFFile = [
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', ''
    ]
    sHREFIndex = 0
    SongMark = [
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', ''
    ]
    SongMarkIndex = 0
    sSrcFile = [
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', '', '', '', '', '', ''
    ]
    sSrcIndex = 0
    sGlobalNotes = ""

    sTempo = 10
    sDrumFile = ""
    sLoopFile = ""
    sIntroCount = 4
    sBeatsPerMeasure = 4
    sLoopLength = 4
    sSolo = "XXX"


# ------------------------------------------
def PrintVariables():
    global sSetIndex
    global SongMark
    global SongMarkIndex

    print("*************************")
    print("sPresets 1", sPresets[1])
    print("sPresets 2", sPresets[2])
    print("sPresets 3", sPresets[3])
    print("sPresets 4", sPresets[4])
    print("sPresets 5", sPresets[5])
    print("sTempo ", sTempo)
    print("sSetIndex ", sSetIndex)
    print("sSetNow 1 ", sSetNow[0])
    print("sIntroCount ", sIntroCount)
    print("sBeatsPerMeasure ", sBeatsPerMeasure)
    print("sDrumFile ", sDrumFile)
    print("sLoopFile ", sLoopFile)
    print("sLoopLength ", sLoopLength)
    print("sSolo ", sSolo)
    print("sHREFIndex ", sHREFIndex)
    for x in range(0, sHREFIndex):
        print("sHREFFile ", x, sHREFFile[x])
    for x in range(0, sSrcIndex):
        print("sSrcFile ", x, sSrcFile[x])
    for x in range(0, SongMarkIndex):
        print("Mark ", x, SongMark[x])

    print("Notes=", sGlobalNotes)


# Check for a valid file.
# ------------------------------------------
def ParseFile(fname, dirname):
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

    #  logger.debug("Parse Function %s %s",fname, dirname)
    userContStart = 0
    userContEnd = 0

    # open file in variable
    theHtmlFile = open(dirname + "/" + fname, 'r')
    theFileStr = theHtmlFile.read()

    #  Check to see if this is a LiveMusicApp HTML
    userContStart = theFileStr.find("<LiveMusic>")
    userContEnd = theFileStr.find("</LiveMusic>")
    if (userContStart < 0 or userContEnd < 0):
        logger.info("**** NO **** %s ",fname )
        theHtmlFile.close()
        return (1)

    sGlobalNotes = theFileStr[userContStart + 11:userContEnd]

    # Let's split the file into lines for parsing
    for theLine in theFileStr.splitlines():
        logger.debug("Line %s", theLine)

        # Look for embedded Preset
        contentRes = theLine.find("Preset")
        if (contentRes > 0):
            PreNumber = theLine[contentRes + 6]
            contentRes = theLine.find("content=")
            logger.debug ("contentRes %s %s", PreNumber, theLine[contentRes + 8:-1].replace("\"", ""))
            sPresets[int(PreNumber)] = theLine[contentRes + 8:-2].replace(
                "\"", "")

        # Look for embedded Preset
        contentRes = theLine.find("SongMark")
        if (contentRes > 0):
            contentRes = theLine.find("content=")
            HoldString = theLine[contentRes + 8:-2].replace("\"", "")
            logger.debug ("SongMark %s", SongMarkIndex, HoldString)
            SongMark[SongMarkIndex] = HoldString
            SongMarkIndex = SongMarkIndex + 1

        contentRes = theLine.find("Tempo")
        if (contentRes > 0):
            contentRes = theLine.find("content=")
            #        sTempo=int(contentRes)
            sTempo = theLine[contentRes + 8:-2].replace("\"", "")
            logger.debug ("Tempo %s", theLine[contentRes + 8:-1].replace("\"", ""))

        contentRes = theLine.find("SetNow")
        if (contentRes > 0):
            contentRes = theLine.find("content=")
            sSetNow[sSetIndex] = theLine[contentRes + 8:-2].replace("\"", "")
            sSetIndex = sSetIndex + 1
            logger.debug ("SetNow %d %s", sSetIndex, theLine[contentRes + 8:-1].replace("\"", ""))

        contentRes = theLine.find("IntroCount")
        if (contentRes > 0):
            contentRes = theLine.find("content=")
            sIntroCount = theLine[contentRes + 8:-2].replace("\"", "")
            logger.debug ("IntroCount %s", theLine[contentRes + 8:-1].replace("\"", ""))

        contentRes = theLine.find("BeatsPerMeasure")
        if (contentRes > 0):
            contentRes = theLine.find("content=")
            #        sBeatsPerMeasure=int(contentRes)
            sBeatsPerMeasure = theLine[contentRes + 8:-2].replace("\"", "")
            logger.debug ("BeatsPerMeasure %s ", theLine[contentRes + 8:-1].replace("\"", ""))

        contentRes = theLine.find("DrumFile")
        if (contentRes > 0):
            contentRes = theLine.find("content=")
            sDrumFile = theLine[contentRes + 8:-2].replace("\"", "")
            logger.debug ("DrumFile %s", theLine[contentRes + 8:-1].replace("\"", ""))

        contentRes = theLine.find("LoopFile")
        if (contentRes > 0):
            contentRes = theLine.find("content=")
            sLoopFile = theLine[contentRes + 8:-2].replace("\"", "")
            logger.debug ("LoopFile %s", theLine[contentRes + 8:-1].replace("\"", ""))

        contentRes = theLine.find("LoopLength")
        if (contentRes > 0):
            contentRes = theLine.find("content=")
            sLoopLength = theLine[contentRes + 8:-2].replace("\"", "")
            logger.debug ("LoopLength %s", theLine[contentRes + 8:-1].replace("\"", ""))

        contentRes = theLine.find("font color=")
        if (contentRes > 0):
            contentRes = theLine.find("Solo")
            if (contentRes > 0):
                theValue = theLine[contentRes + 5:-2].split("<")
                sSolo = theValue[0]

        contentRes = theLine.find("href=")
        logger.debug("HREF___ %s %s", contentRes, theLine)
        if (contentRes > 0):
            theValue = theLine[contentRes + 5:-2].split(">")
            sHREFFile[sHREFIndex] = theValue[0]
            sHREFIndex = sHREFIndex + 1

        contentRes = theLine.find("src=")
        if (contentRes > 0):
            logger.debug (theValue[0])
            theValue = theLine[contentRes + 5:-2].split("\"")
            sSrcFile[sSrcIndex] = theValue[0]
            sSrcIndex = sSrcIndex + 1

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

    logger.debug ("Load Variables %s",Files)
    sHREFIndex = 0
    sSrcIndex = 0

    Files.sort()
    for filename in Files:

        if (filename.endswith("mp3")):
            logger.debug ("MP3 %s", filename)
            sHREFFile[sHREFIndex] = filename
            sHREFIndex = sHREFIndex + 1

        if filename.endswith(".mp3"):
            Command = "mpck " + Root + "/" + filename + "> /dev/null"
            Mp3Problem=os.system(Command)

            if (Mp3Problem):
                Command = "echo " + Root + "/" + filename + ">> ~/MP3Errors.txt"
                logger.debug("*** MP3 Problem " + Command)
                os.system(Command)

        if (filename.endswith("mscz")):
            logger.debug ("Midi %s", filename)
            sHREFFile[sHREFIndex] = filename
            sHREFIndex = sHREFIndex + 1

        if (filename.endswith("mid")):
            logger.debug ("Midi %s", filename)
            sHREFFile[sHREFIndex] = filename
            sHREFIndex = sHREFIndex + 1

        if (filename.endswith("tg")):
            logger.debug ("tg %s", filename)
            sHREFFile[sHREFIndex] = filename
            sHREFIndex = sHREFIndex + 1

        if (filename.endswith("rg")):
            logger.debug ("rg %s", filename)
            sHREFFile[sHREFIndex] = filename
            sHREFIndex = sHREFIndex + 1

        if (filename.endswith("mkv")):
            logger.debug ("mkv %s", filename)
            sHREFFile[sHREFIndex] = filename
            sHREFIndex = sHREFIndex + 1

        if (filename.endswith("mp4")):
            logger.debug ("mp4 %s", filename)
            sHREFFile[sHREFIndex] = filename
            sHREFIndex = sHREFIndex + 1

        if (filename.endswith("png")):
            logger.debug ("png %s", filename)
            if (filename != "background.png"
                    and not filename.endswith(".spec.png")):
                sSrcFile[sSrcIndex] = filename
                sSrcIndex = sSrcIndex + 1

        if (filename.endswith("jpg")):
            logger.debug ("jpg %s", filename)
            if (filename != "logo.jpg"):
                sSrcFile[sSrcIndex] = filename
                sSrcIndex = sSrcIndex + 1

        if (filename.endswith("pdf")):
            logger.info ("pdf sSrc %s", filename)
            sSrcFile[sSrcIndex] = filename
            sSrcIndex = sSrcIndex + 1


#        if (filename.endswith("webm")):
#             logger.debug ("webm ", filename)
#            sHREFFile[sHREFIndex] = filename
#            sHREFIndex = sHREFIndex + 1


# Create the main index html page
# ------------------------------------------
def GenerateIndex(Base, List, Reference):
    MaxNameLength = 18
    Padding = "_____________________________________"

    IndexName = os.path.basename(os.getcwd())

    theFile = open(Base + "/" + IndexName + ".html", 'w+')
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
    theFile.write("<title>" + IndexName + "_Index</title>\n")
    theFile.write("<p style=\"line-height:135%\">\n<body>\n\n")
    theFile.write(
        "<body link=\"#ffd0a0\" vlink=\"#d08080\" alink=\"#FF0000\"><tr><tt>")

    #    logger.debug ("\n\n")
    logger.info("in GenerateIndex " + Base + "/" + IndexName + ".html")
    if (Reference):
        theFile.write(
            "<big><big><p id=\"#TOP\"></p><a href=\"#Aaaa\">[__A__]</a><a href=\"#Baaa\">[__B__]</a><a href=\"#Caaa\">[__C__]</a><a href=\"#Daaa\">[__D__]</a><a href=\"#Eaaa\">[__E__]</a><a href=\"#Faaa\">[__F__]</a><a href=\"#Gaaa\">[__G__]</a><a href=\"#Haaa\">[__H__]</a><a href=\"#Iaaa\">[__I__]</a><a href=\"#Jaaa\">[__J__]</a><a href=\"#Kaaa\">[__K__]</a><a href=\"#Laaa\">[__L__]</a><a href=\"#Maaa\">[__M__]</a><a href=\"#Naaa\">[__N__]</a><a href=\"#Oaaa\">[__O__]</a><a href=\"#Paaa\">[__P__]</a><a href=\"#Qaaa\">[__Q__]</a><a href=\"#Raaa\">[__R__]</a><a href=\"#Saaa\">[__S__]</a><a href=\"#Taaa\">[__T__]</a><a href=\"#Uaaa\">[__U__]</a><a href=\"#Vaaa\">[__V__]</a><a href=\"#Waaa\">[__W__]</a><a href=\"#Xaaa\">[__X__]</a><a href=\"#Yaaa\">[__Y__]</a><a href=\"#Zaaa\">[__Z__]</a><br>"
        )

    PreviousIndex = 'Z'
    for x in List:
        FileName = os.path.basename(x)
        DirName = os.path.basename(os.path.dirname(x))
        #        DirName=os.path.dirname(x)
        FileNoExt = os.path.splitext(FileName)[0]
        logger.debug (" **** " + DirName+" "+FileName)
        logger.debug (" ---- " + x)
        Length = len(FileNoExt)
        if (Length > MaxNameLength):
            Length = MaxNameLength
            FileNoExt = FileNoExt[0:MaxNameLength]

        PadLev = (MaxNameLength + 1) - int(Length)
        PadString = Padding[1:PadLev]
        # HERE ADD Section Index
        IndexLetter = FileName[0:1]
        CurrentIndex = IndexLetter

        if (CurrentIndex != PreviousIndex):
            if (Reference):
                theFile.write("<p id=\"" + CurrentIndex + "aaa\"></p>")
                theFile.write(
                    "<a style=\"color:red\" href=\"#TOP\">TOP</a><br>")

#        theFile.write("[<a href=./"+DirName+"/"+FileName+">"+FileNoExt+PadString+"</a>]\n")
        theFile.write("[<a href=" + x + ">" + FileNoExt + PadString +
                      "</a>]\n")
        PreviousIndex = CurrentIndex

    theFile.write(
        "<LiveMusic></LiveMusic></tr></tt></code></big>\n</code>\n</p>\n</body>\n</html>"
    )
    theFile.close()


# Create a new html from a folder with content.
# ------------------------------------------
def CreateNewHTML(fname, dirname, Files):
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

    logger.debug ("In CreateHTML %s in %s",fname,dirname)
    logger.debug ("Files %s", Files)

    ClearVariables()
    LoadVariables(Files)
    sGlobalNotes = "<code><pre>\n\
<font color=#ff8002>Chords:  </font>\n\
<font color=#88ffff>Structure:  </font>\n\
</pre></code>"

    sPresets = [
        '', 'Str_Pre', 'Juicy_A', 'Blues_Pre', 'Jazz_Pre', 'MidiTog', '', '',
        '', '', '', '', ''
    ]
    sSetNow = ['PlaceHolder', '', '', '', '', '', '', '', '', '', '', '', '']
    sSetIndex = 1
    WriteFile(dirname + "/" + fname + ".html", dirname)


def ExtractPDF(Files, dirname):
    logger.debug ("Load Variables %s",Files)
    for theFile in Files:
        #        if (theFile.endswith("pdf.conv")):
        if (theFile.endswith("pdf")):
            f = dirname + "/" + theFile
            if ( not isFullPdf(f)):
                logger.error("BAD PDF " + f)
                Command = "echo " + f + ">> ~/MP3Errors.txt"
                os.system(Command)

            try:
                with (Image(filename=f, resolution=200)) as source:
                    for i, image in enumerate(source.sequence):
                        #                   newfilename = f[:-9] + "_" + str(i + 1).zfill(3) + '.pdf.jpg'
                        # -4 is to remove the .pdf
                        newfilename = f[:-4] + "_" + str(i +
                                      1).zfill(3) + '.pdf.jpg'
                        Image(image).save(filename=newfilename)
                        logger.info("PDF to %s", newfilename)
            except:
                logger.error("Image Exception\n")
                return

            if (os.path.exists(f)):
                os.rename(f, f + ".conv")

def isFullPdf(f):
    logger.info("isFullPdf " + f)
    end_content = ''
    start_content = ''
    size = os.path.getsize(f)

    if size < 1024: 
        return False 

    try:
        with open(f, 'rb') as fin: 
            #start content 
            fin.seek(0, 0)
            start_content = fin.read(1024)
            start_content = start_content.decode("ascii", 'ignore' )
            fin.seek(-1024, 2)
            end_content = fin.read()
            end_content = end_content.decode("ascii", 'ignore' )
    except:
        logger.error("PDF Exception\n")
        return

    start_flag = False
    #%PDF
    if start_content.count('%PDF') > 0:
        start_flag = True
    
        
    if end_content.count('%%EOF') and start_flag > 0:
        return True

    eof = bytes([0])
    eof = eof.decode("ascii")
    if end_content.endswith(eof) and start_flag:
        return True

    return False


import os

indexTextStart = """<!DOCTYPE html>
<html>
<head><title>Index of {folderPath}</title></head>
<body>
    <h2>Index of {folderPath}</h2>
    <hr>
    <ul>
        <li>
            <a href='../'>../</a>
        </li>
"""
indexTextEnd = """
    </ul>
</body>
</html>
"""

def index_folder(folderPath):
    logger.info("Indexing: " + folderPath +'/')
    #Getting the content of the folder
    files = os.listdir(folderPath)
    files.sort()
    #If Root folder, correcting folder name
    root = folderPath
    if folderPath == '.':
        root = 'Root'
    indexText = indexTextStart.format(folderPath=root)
    for file in files:
        #Avoiding index.html files
        if file != 'index_dir.html':
            # if it's a directory then add the index
            if os.path.isdir(folderPath+'/'+file):
                indexText += "\t\t<li>\n\t\t\t<a href='" + file + "/index_dir.html'>" + file + "</a>\n\t\t</li>\n"
            else:
                if (file.endswith("mscz")):
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:orange\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                elif (file.endswith("mp3")):
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:red\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                elif (file.endswith("mid")):
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:orange\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                elif (file.endswith("Loops")):
                    pass
                elif (file.endswith("spec.png")):
                    pass
                elif (file[0] == '.'):
                    pass
#                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:yellow\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                elif (file.endswith("pdf")):
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:blue\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"
                else:
                    indexText += "\t\t<li>\n\t\t\t<a style=\"color:black\" href='" + file + "'>" + file + "</a>\n\t\t</li>\n"

        #Recursive call to continue indexing
        if os.path.isdir(folderPath+'/'+file):
            index_folder(folderPath + '/' + file)

    indexText += indexTextEnd
    #Create or override previous index.html
    index = open(folderPath+'/index_dir.html', "w")
    #Save indexed content to file
    index.write(indexText)



# Main
# ------------------------------------------
#
MySongList = []
FoundHTML = 0
global logger

parser = argparse.ArgumentParser(description='Usage: LiveMusicCharts.py . -a ')
parser.add_argument("BaseDir", help="Base Directory")
parser.add_argument("-v", action='store_true', help="Verify")
parser.add_argument("-f", action='store_true', help="Fix additional meta Data")
parser.add_argument("-i", action='store_true', help="Create index.html")
parser.add_argument("-l", action='store_true', help="Directory Level")
parser.add_argument("-c", action='store_true', help="Create HTML from folder")
parser.add_argument("-r", action='store_true', help="Reference on Create")
parser.add_argument("-p", action='store_true', help="Pdf to JPG")
parser.add_argument("-g", action='store_true', help="Gp? to mscz")
parser.add_argument("-n", action='store_true', help="Version Number")
parser.add_argument("-a", action='store_true', help="All Options")
args = parser.parse_args()

# updated by the version script do not change.
Version = "1.9.6"

logger = logging.getLogger(__name__)

# levels:logging.NOTSET,logging.DEBUG,logging.INFO,
# logging.WARNING, logging.ERROR, logging.CRITICAL
#MyLevel=logging.CRITICAL
MyLevel=logging.INFO
#MyLevel=logging.NOTSET

logger.setLevel(MyLevel)
logging.basicConfig(level=MyLevel)


logger.debug("Testing")

# if asking about version number.
if (args.n):
    print ("Version ",Version)
    exit(0)

# look for the base directory
if (args.BaseDir == "./"):
    BaseDir = os.getcwd()
else:
    BaseDir = args.BaseDir


# Make the options more readable.
ConvertPDF = args.p
ConvertGP = args.g
FixMetaData = args.f
CreateHTML = args.c
CreateIndexFIle = args.i
ReferenceCreate = args.r

# sometimes let's just make an index of the top
# level files, usually html
if (args.l):
    DirectoryLevel = 2
else:
    DirectoryLevel = 20 # should be enough

# Most common options
if (args.a):
    ConvertPDF = 1
    FixMetaData = 1
    CreateHTML = 1
    CreateIndexFIle = 1
    ReferenceCreate = 1
    ConvertGP = 1

# if we need to convert PDFs to jpg or
# Guitar Pro to MuseScore do that first so
# the new files can be found and added.
#-------------------------------------------------------
for Root, Dir, Files in os.walk(BaseDir):
    FoundHTML = 0
    Files.sort()
    logger.debug(BaseDir, Root, Dir, "\n")
    ClearVariables()
    for filename in Files:
        #        Check for an html file

        if (ConvertPDF):
            logger.debug("Convert PDF")
            if filename.endswith('.html') and not filename.startswith('.'):
                #            sys.stdout.write("\n 1-"+filename+" ")
                FoundHTML = 1
                #           walk thru a file and pull out meta data.
                if (ParseFile(filename, Root) == 0):
                    #                logger.debug("Filename %s",filename)
                    ExtractPDF(Files, Root)

        if (ConvertGP):
            if (filename.endswith(("gp", "gp1", "gp2", "gp3", "gp4", "gp5",
                                   "gp6", "gp7", "gpx"))):
                #           logger.debug ("Guitar Pro %s", filename)
                MuseFileName = os.path.splitext(filename)[0]
                MuseFileName = Root + "/" + MuseFileName + ".mscz"
                GPFileName = Root + "/" + filename
                logger.debug ("Guitar New Pro ", MuseFileName)
                #            Command="ConvertGPtoMScore.sh "+filename+" "+MuseFileName
                if (not os.path.exists(MuseFileName)):
                    #              logger.debug ("File Does not exist")
                    Command = "mscore3 " + Root + "/" + filename + " -o " + MuseFileName
                    logger.debug(Command)
                    os.system(Command)

                    # Command = "mv " + GPFileName + " " + GPFileName + ".conv"
                    # logger.debug(Command)
                    # os.system(Command)

                    Command = "mscore " + MuseFileName + " -o " + MuseFileName + ".musicxml"
                    logger.info(Command)
                    os.system(Command)

                    Command = "ChordsFromXML.py " + MuseFileName + ".musicxml > " + Root + "/Chords.txt"
                    logger.info(Command)
                    os.system(Command)
                else:
                    logger.debug("File " + MuseFileName + " Does exist")



# This returns file in one directry
# for Files in os.listdir(BaseDir):
# This is recursive
#-------------------------------------------------------
for Root, Dir, Files in os.walk(BaseDir, followlinks=True, topdown=False):
    Files = [f for f in Files if not f[0] == '.']
    Dir[:] = [d for d in Dir if not d[0] == '.']
    #    logger.debug (Root, Dir, Files)
    # Get the Current Dir Name.
    sTitle = os.path.basename(Root)
    Files.sort()

    FoundHTML = 0

    base_depth = Root.rstrip("/").count("/")
    if (base_depth < DirectoryLevel):
        for filename in Files:
            if (filename.endswith(".mp3") or filename.endswith(".mp4")
                    or filename.endswith(".webm")):
                logger.debug("Found File %s", filename)
                if (not os.path.exists(Root + "/" + filename + ".spec.png")):
                    logger.debug("Running Subprocess")
                    subprocess.run([
                        "ffmpeg", "-i", Root + "/" + filename, "-lavfi",
                        "showspectrumpic=s=1395x60:mode=combined:color=plasma:scale=5thrt:fscale=log:legend=off",
                        Root + "/" + filename + ".spec.png"
                    ])

            # Check for an html file
            if filename.endswith('.html') and (FoundHTML == 0):

                FoundHTML = 1
                ClearVariables()

                # walk thru a file and pull out meta data.
                if (ParseFile(filename, Root) == 0
                        and not filename.startswith('.')):

                    # add the html to the main index list
                    # but don't add ourselves
                    if (Root != "."):
                        logger.info("Adding to list %s %s", filename, Root)
                        MySongList.append(Root + "/" + filename)
                    else:
                        logger.info("*** Not Added %s %s", filename, Root)

                    if (FixMetaData):
                        LoadVariables(Files)
                        WriteFile(Root + "/" + filename, Root)

                # Check for Background and logo images.
                if (not os.path.exists(Root + "/background.png")):
                    logger.info(Root + "/background.png")
                    copyfile("/home/MySongs/background.png",
                             Root + "/background.png")
                    copyfile("/home/MySongs/logo.jpg", Root + "/logo.jpg")

        # If we did find a valid html and were asked to create one.
        if (CreateHTML and FoundHTML == 0 and not sTitle.startswith('.')):
            logger.info("Create file from Directory ", sTitle)
            #        ClearVariables()
            CreateNewHTML(sTitle, Root, Files)
            MySongList.append(Root + "/" + sTitle + ".html")


if (CreateIndexFIle):
    MySongList.sort()
    logger.debug("List ---------------- %d",len(MySongList))
    logger.debug(MySongList)
    GenerateIndex(BaseDir, MySongList, ReferenceCreate)


# Indexing root directory (Script position)
index_folder(BaseDir)

logger.info("Version %s",Version)
#logger.info(Version)

# find ./ -iname \*mp3 -exec normalize-mp3 -b {} \;

# logger.debug("Done")

# jpdfbookmarks.jar for modify pages

# Remove special char
# rename 's/#//g; s/\A-*//' *
# rename 's/ //g; s/\A-*//' *
# find ./ -type d -exec rename 's/ //g; s/\A-*//' {}"/"* \
# mscore3 Azra.gp4 -o Azra.mscz
# find ./ -iname \*.gp? -exec mscore3 {} -o {}".mscz" \;
# # lame --scale 4 BKContigo.mp3 BKContigo1.mp3
#
# ffmpeg -i ahf1_icecold_vid.mp4  -lavfi showspectrumpic=s=800x400:mode=separate spectrogram.png
# 
# Remove Generate PDF
# find ./ -iname \*.pdf.jpg -exec rm {} \;
# 
# Remove extension
# find ./ -name \*.conv -print0 | xargs -0 rename 's/.conv$//'
# 
# 
