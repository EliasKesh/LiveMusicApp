/*  File:   PrefsFile
 *
 *  Contains:
 *
 *
 *  Written By:     Elias Keshishoglou on Tue Dec 25 17:28:58 PST 2012
 *
 *  Copyright �:  2012 Elias Keshishoglou all rights reserved.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 *------------------------------------------------*/


#define PrefsFile_c

/*
 * Place Includes Here
 */
#include "LiveMusicApp.h"
#include "PrefsFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include "xmltok/xmlparse.h"
#include "SooperOSC.h"
#include <libxml/xmlreader.h>

int TopLevelParse;

// Use relative index not from the XML file.
#define DontIndexFromXML 1

typedef struct {
    char Valid;
    char name[250];
    char value[250];
    int Level;
    int Type;
} ParseData;

#define MaxLevels 10

// /usr/include/libxml2/
/*
 * Place defines and Typedefs here
 */

#define PREFSFILENAME "LiveMusic.xml"
#define DefinePrefsFile "./src/GenPrefs.h"

/*
 * Place Local prototypes here
 */

void WritePrefs(void);
int PostProcessPrefs(LiveMusicInfo *MyInfo);

/*
 * Place Static variables here
 */

//#include "DefPrefs.h"
//#include "GenPrefs.h"

/*--------------------------------------------------------------------
 * Function:        InitPref
 *
 * Description:     <Description/Comments>
 *
 *------------------------------------------------*/
void
InitPref(void) {
    int Count;

    memset(&gMyInfo, 0, sizeof(LiveMusicInfo));
    //  memcpy (&gMyInfo, &GlobalInfo, sizeof (LiveMusicInfo));

    /* If we can not read the XML file load up the defaults.
    */
#if 1
    //    if (NewInstall) {
    //        printd(LogDebug, "New Install Copy Pref\n");
    //        strcpy(gMyInfo.BasePath, GetResourceDir("index.html", FileLocTunes));
    //       memcpy(&gMyInfo, &GlobalInfo, sizeof(LiveMusicInfo));
    //    }
    //    else {
    printd(LogDebug, "Read xml File\n");
    ReadPrefs();
    //    }

    // if (ReadPrefs()) {
    //     memcpy(&gMyInfo, &GlobalInfo, sizeof(LiveMusicInfo));
    //     printd(LogDebug, "ReadFrefs Failed. \n");
    //     NewInstall = 1;
    // }

    //        PrintDataStructure(&gMyInfo, NULL);
    //        exit(0);
#else
    memcpy(&gMyInfo, &GlobalInfo, sizeof(LiveMusicInfo));
#endif

    printd(LogDebug, "Prefs file %s\n", gMyInfo.BasePath);

    /* Clear some of the basic user variables.
    */
    gMyInfo.TempoMax = 8;

    for (Count = 0; Count < MaxPresetButtons; Count++) {
        gMyInfo.WebPresets.thePreset[Count] = -1;
    }

    // WritePrefs();

    /* If we wish to generate an include file from the
    existing preferences.
    */
    if (GenerateHFile) {
        PrintDataStructure(&gMyInfo, DefinePrefsFile);
    }

    /* Setup some parameters that require preferences values.
    */
    PostProcessPrefs(&gMyInfo);
    gMyInfo.MidiThresholdLevel = 30;

    //  exit(0);
}

/*--------------------------------------------------------------------
 * Function:        PostProcessPrefs
 *
 * Description:     Make any assignments not included in the prefs
 *
 *------------------------------------------------*/
int PostProcessPrefs(LiveMusicInfo *MyInfo) {
    int Index;

    for (Index = 0; Index < Max_Patches; Index++) {
        if (MyInfo->MyPatchInfo[Index].CustomCommand == cmdHardSlider)
            if (MyInfo->MyPatchInfo[Index].Channel < MaxHardSliders) {
                MyInfo->HardSlider[MyInfo->MyPatchInfo[Index].Channel] = Index;
                printd(LogDebug, "PostProcessPrefs %d %d\n", Index, MyInfo->MyPatchInfo[Index].Channel);
            }
    }
}

/*--------------------------------------------------------------------
 * Function:        PrintDataStructure
 *
 * Description:     Print out the data structure in an "#include" format
 *
 *------------------------------------------------*/
void PrintDataStructure(LiveMusicInfo * myInfo, char *PrefsRef) {
    int Loop;
    int Loop1;
    PortsInfo *thePorts;
    FILE *PrefsFile = NULL;
    char SFName[100];
    const char *padding = "                                ";
    int     StringLen;

    printd(LogTest, "Main Information\n");

    if (PrefsRef != NULL) {
        PrefsFile = fopen(PrefsRef, "w+");
        printd(LogTest, "Prefs File %s %x\n", PrefsRef, PrefsFile);
    }

    if (PrefsFile) {
        fprintf(PrefsFile, "LiveMusicInfo GlobalInfo = {\n \t{\n");
    }

    for (Loop = 0; Loop < Max_Patches; Loop++) {
        if ((Loop % 10) == 0 && PrefsFile) {
            fprintf(PrefsFile, "/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ \n");
        }

        if (myInfo->MyPatchInfo[Loop].CustomCommand == NoCustom) {
            //          if (myInfo->MyPatchInfo[Loop].BankSelect <= MaxSoundFonts)
            //              strcpy(SFName, SoundFontBankNames[myInfo->MyPatchInfo[Loop].BankSelect]);
            //          else
            strcpy(SFName, "NoBank");
        }
        else
            if (myInfo->MyPatchInfo[Loop].BankSelect <= MaxSoundFonts) {
                sprintf(SFName, "0x%02x", myInfo->MyPatchInfo[Loop].BankSelect);
            }
            else {
                strcpy(SFName, "NoBank");
            }

        StringLen = strlen(myInfo->MyPatchInfo[Loop].Name);
        printd(LogTest, "ID=%d %-15s %-10s %d %-12s %d %-10s %-10s\n", Loop,
               myInfo->MyPatchInfo[Loop].Name,

               SFName, // SoundFontBankNames
               myInfo->MyPatchInfo[Loop].Patch,
               gMyInfo.OutPortName[myInfo->MyPatchInfo[Loop].OutPort],
               //              CustomPorts[myInfo->MyPatchInfo[Loop].OutPort],
               myInfo->MyPatchInfo[Loop].Channel,
               CustomCommands[myInfo->MyPatchInfo[Loop].CustomCommand],
               myInfo->MyPatchInfo[Loop].Chain);

        /* Patch definitions.
        */
        if (PrefsFile)
            fprintf(PrefsFile, "/* %3d */ {\"%s\",%*.*s %-15s, %3d, %-15s, %3d, %-10s, \"%s\" },\n", Loop,
                    myInfo->MyPatchInfo[Loop].Name,
                    15 - StringLen, 15 - StringLen,
                    padding,
                    SFName,
                    myInfo->MyPatchInfo[Loop].Patch,
                    gMyInfo.OutPortName[myInfo->MyPatchInfo[Loop].OutPort],
                    //                   CustomPorts[myInfo->MyPatchInfo[Loop].OutPort],
                    myInfo->MyPatchInfo[Loop].Channel,
                    CustomCommands[myInfo->MyPatchInfo[Loop].CustomCommand],
                    myInfo->MyPatchInfo[Loop].Chain);

    }

    if (PrefsFile) {
        fprintf(PrefsFile, "},\n");

        /* Base HTML File Name.
        */
        fprintf(PrefsFile, "\"%s\",\n", myInfo->BasePath);
        fprintf(PrefsFile, "%d, \n { \n", myInfo->NumOutPorts);

        /* LiveMusic Ports.
        */
        for (Loop = 0; Loop < myInfo->NumOutPorts; Loop++) {
            fprintf(PrefsFile, "\"%s\",\n", myInfo->OutPortName[Loop]);
        }
        fprintf(PrefsFile, "\"%s\"\n", myInfo->OutPortName[Loop]);

        fprintf(PrefsFile, "},\n{\n");

        for (Loop = 0; Loop < MaxApps - 1; Loop++) {
            fprintf(PrefsFile, "{ \"%s\", %d },\n", myInfo->Apps[Loop].Name,
                    myInfo->Apps[Loop].PortID);
        }
        fprintf(PrefsFile, "{ \"%s\", %d }\n", myInfo->Apps[Loop].Name,
                myInfo->Apps[Loop].PortID);

        fprintf(PrefsFile, "},\n");
        /* Middle Numbers.
        */
        fprintf(PrefsFile, " %d, /* TempoMax  */\n", myInfo->TempoMax);
        fprintf(PrefsFile, " %d, /* Metronome On  */\n", myInfo->MetronomeOn);
        fprintf(PrefsFile, " %d, /* Drum down beat, midi code  */\n", myInfo->Drum1);
        fprintf(PrefsFile, " %d, /* Drum Other beats  */\n", myInfo->DrumRest);
        fprintf(PrefsFile, " %d, /* Base Midi Note for switching  */\n", myInfo->MidiBaseNote);
        fprintf(PrefsFile, " %d, /* Tempo   */\n", myInfo->Tempo);
        fprintf(PrefsFile, " %d, /* CountInBeats   */\n", myInfo->CountInBeats);
        fprintf(PrefsFile, " %d, /* LoopRecBeats   */\n", myInfo->LoopRecBeats);
        fprintf(PrefsFile, " %d, /* MidiPassThru   */\n", myInfo->MidiPassThru);
        fprintf(PrefsFile, " %d, /* MidiPassLevel   */\n", myInfo->MidiPassLevel);
        fprintf(PrefsFile, " %d, /* BeatsPerMeasure   */\n", myInfo->BeatsPerMeasure);

        fprintf(PrefsFile, " %d, /* AnalogVolume   */\n", myInfo->AnalogVolume);
        fprintf(PrefsFile, " %d, /* MidiVolume   */\n", myInfo->MidiVolume);
        fprintf(PrefsFile, " 0x%x, /* StatusTextColor   */\n", (unsigned int)myInfo->StatusTextColor);
        fprintf(PrefsFile, " 0x%x, /* ButtonTextColor   */\n", (unsigned int)myInfo->ButtonTextColor);

        fprintf(PrefsFile, " \"%s\", /* OSCIPAddress   */\n", myInfo->OSCIPAddress);
        fprintf(PrefsFile, " \"%s\", /* OSCPortNumLooper   */\n", myInfo->OSCPortNumLooper);
        fprintf(PrefsFile, " \"%s\", /* OSCPortNumJackVol   */\n", myInfo->OSCPortNumJackVol);
        fprintf(PrefsFile, " \"%s\", /* OSCPortNumCarla   */\n", myInfo->OSCPortNumCarla);
        fprintf(PrefsFile, " \"%s\", /* OSCPortNumHydrogen   */\n", myInfo->OSCPortNumHydrogen);

        fprintf(PrefsFile, " %d, /* Number of Strings   */\n {", myInfo->NumberOfStrings);


        //  printf (" %d, /* Number of Strings   */\n {", myInfo->NumberOfStrings);

        for (Loop = 0; Loop < TotalMaxStrings; Loop++) {
            fprintf(PrefsFile, "%d,\n", myInfo->BaseStringName[Loop]);
            //      printf ("%d,\n", myInfo->BaseStringName[Loop]);
        }

        fprintf(PrefsFile, "    },\n");
    }
    printd(LogTest, "BaseName %s\n", myInfo->BasePath);

    printd(LogTest, "Num Ports %d\n", myInfo->NumOutPorts);
    for (Loop = 0; Loop < myInfo->NumOutPorts; Loop++) {
        printd(LogTest, "Ports %s\n", myInfo->OutPortName[Loop]);
    }

    /* LiveMusic Apps.
    */
    for (Loop = 0; Loop < MaxApps; Loop++) {
        printd(LogTest, "Apps %s %d\n", myInfo->Apps[Loop].Name,
               myInfo->Apps[Loop].PortID);
    }

    thePorts = &theInPorts;
    for (Loop = 0; Loop < thePorts->NumDevices; Loop++) {
        printd(LogTest, "In Name [%s]\n", thePorts->Devices[Loop].Name);
        for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
            printd(LogTest, "In SName %d, [%s]\n", Loop1,
                   thePorts->Devices[Loop].Ports[Loop1].Name);
        }
    }

    thePorts = &theOutPorts;
    for (Loop = 0; Loop < thePorts->NumDevices; Loop++) {
        printd(LogTest, "Out Name [%s]\n", thePorts->Devices[Loop].Name);
        for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
            printd(LogTest, "Out SName %d, [%s]\n", Loop1,
                   thePorts->Devices[Loop].Ports[Loop1].Name);
        }
    }

    /* Controller prefs.
    */
    if (PrefsFile) {
        fprintf(PrefsFile, "{ // Controller mapping \n");
    }

    for (Loop = 0; Loop < MaxControllers; Loop++) {
        printd(LogTest, "Cont=%d %d\n", Loop,
               gMyInfo.ControlRoute[Loop].OutPort);

        if (PrefsFile)
            fprintf(PrefsFile, "{ /* %3d */  %d, %d },\n", Loop,
                    gMyInfo.ControlRoute[Loop].OutPort,
                    gMyInfo.ControlRoute[Loop].OutControl);
    }

    if (PrefsFile) {
        fprintf(PrefsFile, "}, \n");
    }

    if (PrefsFile) {
        fprintf(PrefsFile, "{ \n ");
    }

    /* Layouts .
    */
    for (Loop = 0; gMyInfo.LayoutPresets[Loop].Name[0] != 0; Loop++) {
        printd(LogTest, "Lay=%d %s\n", Loop,
               gMyInfo.LayoutPresets[Loop].Name);

        if (PrefsFile) {
            fprintf(PrefsFile, "\t{\n \"%s\", \n{\n", gMyInfo.LayoutPresets[Loop].Name);
        }


        for (Loop1 = 0; gMyInfo.LayoutPresets[Loop].Presets[Loop1][0] != 0; Loop1++) {
            printd(LogTest, "%d %s \n", Loop1, gMyInfo.LayoutPresets[Loop].Presets[Loop1]);

            if (PrefsFile) {
                fprintf(PrefsFile, "/* %3d */  \"%s\", \n", Loop1, gMyInfo.LayoutPresets[Loop].Presets[Loop1]);
            }
        }

        if (PrefsFile) {
            fprintf(PrefsFile, "},}, \n");
        }
    }

    if (PrefsFile) {
        fprintf(PrefsFile, "{\n \"\", { },\t},\n },\n}; \n");
        fflush(PrefsFile);
    }

    printd(LogTest, "Metronome Base %d On %d %d %d\n", myInfo->TempoMax,
           myInfo->MetronomeOn, myInfo->Drum1, myInfo->DrumRest);
    printd(LogTest, "Midi Base Note %d\n", myInfo->MidiBaseNote);
}

/*--------------------------------------------------------------------
 * Function:        WritePref
 *
 * Description:     <Description/Comments>
 *
 *------------------------------------------------*/
void WritePrefs(void) {
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node0 = NULL, node1 = NULL, node2 = NULL; /* node pointers */
    xmlDtdPtr dtd = NULL;       /* DTD pointer */
    char buff[256];
    int Loop;
    int PatchLoop;;

    //  LIBXML_TEST_VERSION;

    printd(LogDebug, "In Write Prefs\n");
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST XML_BASIC_SETTINGS);
    xmlDocSetRootElement(doc, root_node);

    xmlNewChild(root_node, NULL, BAD_CAST "SongPath",
                BAD_CAST gMyInfo.BasePath);


    sprintf(buff, "%03d", gMyInfo.TempoMax);
    xmlNewChild(root_node, NULL, BAD_CAST "TempoMax", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.MetronomeOn);
    xmlNewChild(root_node, NULL, BAD_CAST "MetroOn", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.Drum1);
    xmlNewChild(root_node, NULL, BAD_CAST "Click1", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.DrumRest);
    xmlNewChild(root_node, NULL, BAD_CAST "ClickRest", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.MidiBaseNote);
    xmlNewChild(root_node, NULL, BAD_CAST "MidiBaseNote", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.Tempo);
    xmlNewChild(root_node, NULL, BAD_CAST "Tempo", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.CountInBeats);
    xmlNewChild(root_node, NULL, BAD_CAST "CountInBeats", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.LoopRecBeats);
    xmlNewChild(root_node, NULL, BAD_CAST "LoopRecBeats", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.MidiPassThru);
    xmlNewChild(root_node, NULL, BAD_CAST "MidiPassThru", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.MidiPassLevel);
    xmlNewChild(root_node, NULL, BAD_CAST "MidiPassLevel", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.BeatsPerMeasure);
    xmlNewChild(root_node, NULL, BAD_CAST "BeatsPerMeasure", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.AnalogVolume);
    xmlNewChild(root_node, NULL, BAD_CAST "AnalogVolume", BAD_CAST buff);

    sprintf(buff, "%03d", gMyInfo.MidiVolume);
    xmlNewChild(root_node, NULL, BAD_CAST "MidiVolume", BAD_CAST buff);

    sprintf(buff, "0x%x", (unsigned int)gMyInfo.StatusTextColor);
    xmlNewChild(root_node, NULL, BAD_CAST "StatusTextColor", BAD_CAST buff);

    sprintf(buff, "0x%x", (unsigned int)gMyInfo.ButtonTextColor);
    xmlNewChild(root_node, NULL, BAD_CAST "ButtonTextColor", BAD_CAST buff);

    sprintf(buff, "%s", gMyInfo.OSCIPAddress);
    xmlNewChild(root_node, NULL, BAD_CAST "OSCIPAddress", BAD_CAST buff);

    sprintf(buff, "%s", gMyInfo.OSCPortNumLooper);
    xmlNewChild(root_node, NULL, BAD_CAST "OSCPortNumLooper", BAD_CAST buff);

    sprintf(buff, "%s", gMyInfo.OSCPortNumJackVol);
    xmlNewChild(root_node, NULL, BAD_CAST "OSCPortNumJackVol", BAD_CAST buff);

    sprintf(buff, "%s", gMyInfo.OSCPortNumCarla);
    xmlNewChild(root_node, NULL, BAD_CAST "OSCPortNumCarla", BAD_CAST buff);

    sprintf(buff, "%s", gMyInfo.OSCPortNumHydrogen);
    xmlNewChild(root_node, NULL, BAD_CAST "OSCPortNumHydrogen", BAD_CAST buff);

    /*
     * Write out the Output Ports.
     */
    sprintf(buff, "%03d", gMyInfo.NumOutPorts);
    xmlNewChild(root_node, NULL, BAD_CAST "NumOutPorts", BAD_CAST buff);
    printd(LogDebug, "In Write Prefs Output Ports\n");

    node1 = xmlNewChild(root_node, NULL, BAD_CAST "OutPorts", NULL);
    for (Loop = 0; Loop < MaxOutPorts; Loop++) {
        sprintf(buff, "Port%03d", Loop);
        node = xmlNewChild(node1, NULL, buff, NULL);
        xmlSetProp(node, "Name", gMyInfo.OutPortName[Loop]);
    }


    /*
     * Write out the String Names.
     */
    sprintf(buff, "%03d", gMyInfo.NumberOfStrings);
    xmlNewChild(root_node, NULL, BAD_CAST "NumberOfStrings", BAD_CAST buff);

    node1 = xmlNewChild(root_node, NULL, BAD_CAST "Strings", NULL);
    for (Loop = 0; Loop < TotalMaxStrings; Loop++) {
        sprintf(buff, "StrNum%03d", Loop);
        node = xmlNewChild(node1, NULL, buff, NULL);
        sprintf(buff, "%02d", gMyInfo.BaseStringName[Loop]);
        xmlSetProp(node, "Value", buff);
    }

    /*
     * Write out the Patches
     */
    node1 = xmlNewChild(root_node, NULL, BAD_CAST "MainButtons", NULL);
    for (Loop = 0; Loop < Max_Patches; Loop++) {
        //              doc->children = xmlNewDocNode(doc, NULL, gMyInfo.MyPatchInfo[Loop].Button, NULL);
        sprintf(buff, "Preset%03d", Loop);
        node = xmlNewChild(node1, NULL, buff, NULL);
        //      printf("%d  %s\n", Loop, gMyInfo.MyPatchInfo[Loop].Name);
        xmlSetProp(node, "Name", gMyInfo.MyPatchInfo[Loop].Name);
        sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].Channel);
        xmlSetProp(node, "Channel", buff);
        sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].BankSelect);
        xmlSetProp(node, "Controller", buff);
        sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].OutPort);
        xmlSetProp(node, "OutPort", buff);
        sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].Patch);
        xmlSetProp(node, "Patch", buff);
        sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].CustomCommand);
        xmlSetProp(node, "Custom", buff);
        //              sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].Chain);
        xmlSetProp(node, "Chain", gMyInfo.MyPatchInfo[Loop].Chain);
    }

#if 1
    /* Application names.
    */
    node1 = xmlNewChild(root_node, NULL, BAD_CAST "AppNames", NULL);
    for (Loop = 0; Loop < MaxApps; Loop++) {
        sprintf(buff, "App%03d", Loop);
        node = xmlNewChild(node1, NULL, buff, NULL);
        xmlSetProp(node, "Name", gMyInfo.Apps[Loop].Name);
        sprintf(buff, "%03d", gMyInfo.Apps[Loop].PortID);
        xmlSetProp(node, "PortID", buff);
    }

    /* Controller mapping.
    */
    node1 = xmlNewChild(root_node, NULL, BAD_CAST "ControllerMap", NULL);
    for (Loop = 0; Loop < MaxControllers; Loop++) {
        sprintf(buff, "App%03d", Loop);
        node = xmlNewChild(node1, NULL, buff, NULL);
        sprintf(buff, "%03d", gMyInfo.ControlRoute[Loop].OutPort);
        xmlSetProp(node, "Port", buff);
        sprintf(buff, "%03d", gMyInfo.ControlRoute[Loop].OutControl);
        xmlSetProp(node, "Midi", buff);
    }
#endif

    /*
     * Dump the layouts to the XML File.
     */
    printd(LogDebug, "In Write Prefs layouts\n");
    Loop = 0;
    node = xmlNewChild(root_node, NULL, BAD_CAST "Layouts", NULL);
    while (gMyInfo.LayoutPresets[Loop].Name[0]) {
        sprintf(buff, "Layout%03d", Loop);
        node1 = xmlNewChild(node, NULL, buff, NULL);

        xmlSetProp(node1, "LayoutName", gMyInfo.LayoutPresets[Loop].Name);

        for (PatchLoop = 0; PatchLoop < MaxLayoutPatches; PatchLoop++) {
            sprintf(buff, "Patch%03d", PatchLoop);
            node2 = xmlNewChild(node1, NULL, buff, NULL);
            xmlSetProp(node2, "PatchName",
                       gMyInfo.LayoutPresets[Loop].Presets[PatchLoop]);
        }
        Loop++;
    }

    /*
     * Dumping document to stdio or file
     */
    printd(LogDebug, "In Write Prefs %s\n", GetResourceDir(PREFSFILENAME, FileLocConfig));
    xmlSaveFormatFileEnc(GetResourceDir(PREFSFILENAME, FileLocConfig), doc, "UTF-8", 1);

    xmlFreeDoc(doc);

    xmlCleanupParser();

    if (GenerateHFile) {
        PrintDataStructure(&gMyInfo, DefinePrefsFile);
    }
}

/*--------------------------------------------------------------------
** Reading from here.
--------------------------------------------------------------------*/

#if 1
#define MaxElement      10
#define ElementNameMax  100
static char ElementName[MaxElement][ElementNameMax];
static int elementLevel;

/*--------------------------------------------------------------------
 * Function:        XMLParseFileElement
 *
 * Description:     Given an XML node get the data and
 * sote it in the main data structure gMyInfo
 *
 *------------------------------------------------*/
int XMLParseFileElement(int Level, xmlNode *theNode) {
    int theValue;
    int theIndex;
    char *Element;
    char *Content;
    char *value;
    //    char Value[100];

    Element = (char *)theNode->name;
    Content = (char *)theNode->content;
#if 0
    printf("node type: Element, name: %d 0-%s 1-%s 2-%s 3-%s %s\n",
           Level,
           ElementName[0],
           ElementName[1],
           ElementName[2],
           ElementName[3],
           Element);
    //    Print_Node("SongPath", theNode);
    //    Print_Node("SongPath", theNode->children);
#endif

    switch (Level) {

    case 0:
        break;

    case 1:
        value = theNode->children->content;
        if (!strcmp(Element, "SongPath")) {
            printd(LogDebug, "*** SongPath %s\n", theNode->children->content);
            strncpy(gMyInfo.BasePath, value, 255);
        }

        if (!strcmp(Element, "TempoMax")) {
            printd(LogDebug, "*** TempoMax %s\n", theNode->children->content);
            gMyInfo.TempoMax = atoi(value);
        }

        if (!strcmp(Element, "MetroOn")) {
            printd(LogDebug, "*** MetroOn %s\n", theNode->children->content);
            gMyInfo.MetronomeOn = atoi(value);
        }

        if (!strcmp(Element, "Click1")) {
            printd(LogDebug, "*** Click1 %s\n", theNode->children->content);
            gMyInfo.Drum1 = atoi(value);
        }

        if (!strcmp(Element, "ClickRest")) {
            printd(LogDebug, "*** ClickRest %s\n", theNode->children->content);
            gMyInfo.DrumRest = atoi(value);
        }

        if (!strcmp(Element, "MidiBaseNote")) {
            printd(LogDebug, "*** MidiBaseNote %s\n", theNode->children->content);
            gMyInfo.MidiBaseNote = atoi(value);
        }

        if (!strcmp(Element, "Tempo")) {
            printd(LogDebug, "*** Tempo %s\n", theNode->children->content);
            gMyInfo.Tempo = atoi(value);
        }

        if (!strcmp(Element, "CountInBeats")) {
            printd(LogDebug, "*** CountInBeats %s\n", theNode->children->content);
            gMyInfo.CountInBeats = atoi(value);
        }

        if (!strcmp(Element, "LoopRecBeats")) {
            printd(LogDebug, "*** LoopRecBeats %s\n", theNode->children->content);
            gMyInfo.LoopRecBeats = atoi(value);
        }

        if (!strcmp(Element, "NumberOfStrings")) {
            printd(LogDebug, "*** NumberOfStrings %s\n", theNode->children->content);
            gMyInfo.NumberOfStrings = atoi(value);
        }

        if (!strcmp(Element, "MidiPassThru")) {
            printd(LogDebug, "*** MidiPassThru %s\n", theNode->children->content);
            gMyInfo.MidiPassThru = atoi(value);
        }

        if (!strcmp(Element, "MidiPassLevel")) {
            printd(LogDebug, "*** MidiPassLevel %s\n", theNode->children->content);
            gMyInfo.MidiPassLevel = atoi(value);
        }

        if (!strcmp(Element, "BeatsPerMeasure")) {
            printd(LogDebug, "*** BeatsPerMeasure %s\n", theNode->children->content);
            gMyInfo.BeatsPerMeasure = atoi(value);
        }

        if (!strcmp(Element, "AnalogVolume")) {
            printd(LogDebug, "*** AnalogVolume %s\n", theNode->children->content);
            gMyInfo.AnalogVolume = atoi(value);
        }

        if (!strcmp(Element, "MidiVolume")) {
            printd(LogDebug, "*** MidiVolume %s\n", theNode->children->content);
            gMyInfo.MidiVolume = atoi(value);
        }

        if (!strcmp(Element, "ButtonTextColor")) {
            printd(LogDebug, "*** ButtonTextColor %s\n", theNode->children->content);
            sscanf(value, "%x", (unsigned int *)&gMyInfo.ButtonTextColor);
        }

        if (!strcmp(Element, "StatusTextColor")) {
            printd(LogDebug, "*** StatusTextColor %s\n", theNode->children->content);
            sscanf(value, "%x", (unsigned int *)&gMyInfo.StatusTextColor);
        }

        if (!strcmp(Element, "OSCIPAddress")) {
            printd(LogDebug, "*** OSCIPAddress %s\n", theNode->children->content);
            strncpy(gMyInfo.OSCIPAddress, value, sizeof(gMyInfo.OSCIPAddress));
        }

        if (!strcmp(Element, "OSCPortNumLooper")) {
            printd(LogDebug, "*** OSCPortNumLooper %s\n", theNode->children->content);
            strncpy(gMyInfo.OSCPortNumLooper, value, sizeof(gMyInfo.OSCPortNumLooper));
        }

        if (!strcmp(Element, "OSCPortNumJackVol")) {
            printd(LogDebug, "*** OSCPortNumJackVol %s\n", theNode->children->content);
            strncpy(gMyInfo.OSCPortNumJackVol, value, sizeof(gMyInfo.OSCPortNumJackVol));
        }

        if (!strcmp(Element, "OSCPortNumCarla")) {
            printd(LogDebug, "*** OSCPortNumCarla %s\n", theNode->children->content);
            strncpy(gMyInfo.OSCPortNumCarla, value, sizeof(gMyInfo.OSCPortNumCarla));
        }
        
        if (!strcmp(Element, "OSCPortNumHydrogen")) {
            printd(LogDebug, "*** OSCPortNumHydrogen %s\n", theNode->children->content);
            strncpy(gMyInfo.OSCPortNumHydrogen, value, sizeof(gMyInfo.OSCPortNumHydrogen));
        }

        if (!strcmp(Element, "NumOutPorts")) {
            printd(LogDebug, "*** NumOutPorts %s\n", theNode->children->content);
            gMyInfo.NumOutPorts = atoi(value);
        }
        break;

    case 2:
        if (!strcmp(ElementName[1], "Strings")) {
            if (!strncmp(Element, "StrNum", 6)) {
                theValue = atoi(&Element[6]);
                printd(LogDebug, "*** String Value %d %s\n", theValue,
                       xmlGetProp(theNode, "Value"));
                gMyInfo.BaseStringName[theValue] = atoi(xmlGetProp(theNode, "Value"));
            }
        }

        if (!strcmp(ElementName[1], "OutPorts")) {
            if (!strncmp(Element, "Port", 4)) {
                theValue = atoi(&Element[4]);
                printd(LogDebug, "*** OutPorts Preset %d %s\n", theValue,
                       xmlGetProp(theNode, "Name"));
                strcpy(gMyInfo.OutPortName[theValue], xmlGetProp(theNode, "Name"));
            }
        }

        if (!strcmp(ElementName[1], "MainButtons")) {
            if (!strncmp(Element, "Preset", 6)) {
                theValue = atoi(&Element[6]);
                printd(LogDebug, "*** Main Preset %d %s %s\n", theValue,
                       xmlGetProp(theNode, "Name"),
                       xmlGetProp(theNode, "Chain")
                      );
                strcpy(gMyInfo.MyPatchInfo[theValue].Name,
                       xmlGetProp(theNode, "Name"));

                gMyInfo.MyPatchInfo[theValue].Channel =
                    atoi(xmlGetProp(theNode, "Channel"));

                gMyInfo.MyPatchInfo[theValue].BankSelect =
                    atoi(xmlGetProp(theNode, "Controller"));

                gMyInfo.MyPatchInfo[theValue].OutPort =
                    atoi(xmlGetProp(theNode, "OutPort"));

                gMyInfo.MyPatchInfo[theValue].Patch =
                    atoi(xmlGetProp(theNode, "Patch"));

                gMyInfo.MyPatchInfo[theValue].CustomCommand =
                    atoi(xmlGetProp(theNode, "Custom"));

                gMyInfo.MyPatchInfo[theValue].OutPort =
                    atoi(xmlGetProp(theNode, "OutPort"));

                strcpy(gMyInfo.MyPatchInfo[theValue].Chain,
                       xmlGetProp(theNode, "Chain"));
            }
        }

        if (!strcmp(ElementName[1], "AppNames")) {
            if (!strncmp(Element, "App", 3)) {
                theValue = atoi(&Element[3]);
                printd(LogDebug, "*** App %d %s\n", theValue,
                       xmlGetProp(theNode, "Name"));
                strcpy(gMyInfo.Apps[theValue].Name, xmlGetProp(theNode, "Name"));
                gMyInfo.Apps[theValue].PortID = atoi(xmlGetProp(theNode, "PortID"));
            }
        }

        if (!strcmp(ElementName[1], "ControllerMap")) {
            if (!strncmp(Element, "App", 3)) {
                theValue = atoi(&Element[3]);
                printd(LogDebug, "*** CApp %d %s %s\n", theValue,
                       xmlGetProp(theNode, "Port"),
                       xmlGetProp(theNode, "Midi"));
                gMyInfo.ControlRoute[theValue].OutPort =
                    atoi(xmlGetProp(theNode, "Port"));
                gMyInfo.ControlRoute[theValue].OutControl =
                    atoi(xmlGetProp(theNode, "Midi"));
            }
        }

        if (!strncmp(ElementName[1], "Layouts", 7)) {
            printd(LogDebug, "Layout in 2\n");
            if (!strncmp(Element, "Layout", 6)) {
                theValue = atoi(&Element[6]);
                strcpy(gMyInfo.LayoutPresets[theValue].Name,
                       xmlGetProp(theNode, "LayoutName"));
                printd(LogDebug, "*** Layout %d Name %s \n", theValue,
                       xmlGetProp(theNode, "LayoutName"));
            }
        }

    case 3:
        if (!strncmp(ElementName[2], "Layout", 6)) {
            theIndex = atoi(&ElementName[2][6]);

            if (!strncmp(Element, "Patch", 5)) {
                theValue = atoi(&Element[5]);

                printd(LogDebug, "*** Lay %d Patch %d %s \n", theIndex, theValue,
                       xmlGetProp(theNode, "PatchName"));
                strcpy(gMyInfo.LayoutPresets[theIndex].Presets[theValue],
                       xmlGetProp(theNode, "PatchName"));
            }
        }
    }
}

/*--------------------------------------------------------------------
 * Function:        print_element_names
 *
 * Description:     <Description/Comments>
 *
 *------------------------------------------------*/
static void print_element_names(xmlNode * a_node) {
    xmlNode *cur_node = NULL;

    // Walk through all of the nodes.
    for (cur_node = a_node; cur_node; cur_node =
             cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            XMLParseFileElement(elementLevel, cur_node);
        }

        // Save the previous Name so we know where we are.
        strcpy(ElementName[elementLevel], cur_node->name);
        elementLevel++;

        // Go Recursion
        print_element_names(cur_node->children);

        // Remove a level of Hierarchy
        elementLevel--;
        ElementName[elementLevel][0] = 0;
    }
}

/*--------------------------------------------------------------------
 * Function:        ReadPrefs
 *
 * Description:     <Description/Comments>
 *
 *------------------------------------------------*/
int ReadPrefs() {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    printd(LogDebug, "----------------------\n");
    printd(LogDebug, "Reading prefs file\n");
    printd(LogDebug, "----------------------\n");

    /*parse the file and get the DOM */
    doc = xmlReadFile(GetResourceDir(PREFSFILENAME, FileLocConfig), NULL, 0);
    if (doc == NULL) {
        exit(-1);
    }

    // Start a level Zero
    elementLevel = 0;
    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    // Let's get it started
    print_element_names(root_element);
    xmlFreeDoc(doc);       // free document
    xmlCleanupParser();    // Free globals
    return 0;
}
#endif
