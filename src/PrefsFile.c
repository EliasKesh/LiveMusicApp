/*	File: 	PrefsFile
 *
 *	Contains:
 *
 *
 *	Written By: 	Elias Keshishoglou on Tue Dec 25 17:28:58 PST 2012
 *
 *	Copyright �: 	2012 Elias Keshishoglou all rights reserved.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version 2
 *	of the License, or (at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 *---------------------------------------------------------------------*/

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

// /usr/include/libxml2/
/*
 * Place defines and Typedefs here
 */

#define PREFSFILENAME "LiveMusic.xml"
#define DefinePrefsFile "./src/GenPrefs.h"

/*
 * Place Local prototypes here
 */

void WritePrefs (void);
int PostProcessPrefs(LiveMusicInfo *MyInfo);

/*
 * Place Static variables here
 */

//#include "DefPrefs.h"
#include "GenPrefs.h"

/*--------------------------------------------------------------------
 * Function:		InitPref
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void
InitPref (void) {
	int Count;

	memset (&gMyInfo, 0, sizeof (LiveMusicInfo));
//	memcpy (&gMyInfo, &GlobalInfo, sizeof (LiveMusicInfo));

	/* If we can not read the XML file load up the defaults.
	*/

	if (ReadPrefs ()) {
		memcpy (&gMyInfo, &GlobalInfo, sizeof (LiveMusicInfo));
		printd(LogDebug, "ReadFrefs Failed. \n");
		NewInstall = 1;
	}

	/* If it's a new install set the Charts directory.
	*/
	if (NewInstall) {
		strcpy(gMyInfo.BasePath,GetResourceDir("index.html",FileLocTunes ));
	}

	/* Clear some of the basic user variables.
	*/
	gMyInfo.TempoMax = 8;

	for (Count = 0; Count < MaxPresetButtons; Count++)
		gMyInfo.WebPresets.thePreset[Count] = -1;

	gMyInfo.ControlRoute[0].OutPort = MaxOutPorts;
	gMyInfo.ControlRoute[0].OutControl = 0;
	gMyInfo.ControlRoute[1].OutPort = GuitarixPort;
	gMyInfo.ControlRoute[1].OutControl = 7;
	gMyInfo.ControlRoute[2].OutPort = LooperPort;
	gMyInfo.ControlRoute[2].OutControl = 8;
	gMyInfo.ControlRoute[3].OutPort = TransportPort;
	gMyInfo.ControlRoute[3].OutControl = 9;
	gMyInfo.ControlRoute[4].OutPort = TransportPort;
	gMyInfo.ControlRoute[4].OutControl = 10;
	gMyInfo.ControlRoute[5].OutPort = TransportPort;
	gMyInfo.ControlRoute[5].OutControl = 11;
	gMyInfo.ControlRoute[6].OutPort = TransportPort;
	gMyInfo.ControlRoute[6].OutControl = 12;


	// WritePrefs();

	/* If we wish to generate an include file from the
	existing preferences.
	*/
	if (GenerateHFile)
		PrintDataStructure(&gMyInfo, DefinePrefsFile);

	/* Setup some parameters that require preferences values.
	*/
	PostProcessPrefs(&gMyInfo);
}

/*--------------------------------------------------------------------
 * Function:		PostProcessPrefs
 *
 * Description:		Make any assignments
 *
 *---------------------------------------------------------------------*/
int PostProcessPrefs(LiveMusicInfo *MyInfo) {
	int Index;

	printf("Start PostProcessPrefs\n");
	for (Index = 0; Index < Max_Patches; Index++) {
		if (MyInfo->MyPatchInfo[Index].CustomCommand == cmdHardSlider)
			if (MyInfo->MyPatchInfo[Index].Channel < MaxHardSliders) {
				MyInfo->HardSlider[MyInfo->MyPatchInfo[Index].Channel] = Index;
				printd(LogDebug, "PostProcessPrefs %d %d\n", Index, MyInfo->MyPatchInfo[Index].Channel);
			}
	}

}

/*--------------------------------------------------------------------
 * Function:		PrintDataStructure
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void PrintDataStructure (LiveMusicInfo * myInfo, char *PrefsRef) {
	int Loop;
	int Loop1;
	PortsInfo *thePorts;
	FILE *PrefsFile = NULL;
	char SFName[100];
	const char *padding = "                                ";
	int 	StringLen;

	printd (LogInfo, "Main Information\n");

	if (PrefsRef != NULL) {
		PrefsFile = fopen(PrefsRef, "w+");
		printd (LogInfo, "Prefs File %s %x\n", PrefsRef, PrefsFile);
	}

	if (PrefsFile)
		fprintf (PrefsFile, "LiveMusicInfo GlobalInfo = {\n \t{\n");

	for (Loop = 0; Loop < Max_Patches; Loop++) {
		if ((Loop % 10) == 0 && PrefsFile)
			fprintf (PrefsFile, "/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ \n");

		if (myInfo->MyPatchInfo[Loop].CustomCommand == NoCustom) {
			if (myInfo->MyPatchInfo[Loop].BankSelect <= MaxSoundFonts)
				strcpy(SFName, SoundFontBankNames[myInfo->MyPatchInfo[Loop].BankSelect]);
			else
				strcpy(SFName, "NoBank");
		} else if (myInfo->MyPatchInfo[Loop].BankSelect <= MaxSoundFonts)
			sprintf(SFName, "0x%02x", myInfo->MyPatchInfo[Loop].BankSelect);
		else
			strcpy(SFName, "NoBank");


		StringLen = strlen(myInfo->MyPatchInfo[Loop].Name);
		printd (LogInfo, "ID=%d %-15s %-10s %d %-12s %d %-10s %-10s\n", Loop,
		        myInfo->MyPatchInfo[Loop].Name,

		        SFName, // SoundFontBankNames
		        myInfo->MyPatchInfo[Loop].Patch,
		        CustomPorts[myInfo->MyPatchInfo[Loop].OutPort],
		        myInfo->MyPatchInfo[Loop].Channel,
		        CustomCommands[myInfo->MyPatchInfo[Loop].CustomCommand],
		        myInfo->MyPatchInfo[Loop].Chain);


		/* Patch definitions.
		*/
		if (PrefsFile)
			fprintf (PrefsFile, "/* %3d */ {\"%s\",%*.*s %-15s, %3d, %-15s, %3d, %-10s, \"%s\" },\n", Loop,
			         myInfo->MyPatchInfo[Loop].Name,
			         15 - StringLen, 15 - StringLen,
			         padding,
			         SFName,
			         myInfo->MyPatchInfo[Loop].Patch,
			         CustomPorts[myInfo->MyPatchInfo[Loop].OutPort],
			         myInfo->MyPatchInfo[Loop].Channel,
			         CustomCommands[myInfo->MyPatchInfo[Loop].CustomCommand],
			         myInfo->MyPatchInfo[Loop].Chain);

	}

	if (PrefsFile) {
		fprintf (PrefsFile, "},\n");

		/* Base HTML File Name.
		*/
		fprintf (PrefsFile, "\"%s\",\n", myInfo->BasePath);
		fprintf (PrefsFile, "%d, \n { \n", myInfo->NumOutPorts);

		/* LiveMusic Ports.
		*/
		for (Loop = 0; Loop < myInfo->NumOutPorts; Loop++) {
			fprintf (PrefsFile, "\"%s\",\n", myInfo->OutPortName[Loop]);
		}
		fprintf (PrefsFile, "\"%s\"\n", myInfo->OutPortName[Loop]);

		fprintf (PrefsFile, "},\n{\n");

		for (Loop = 0; Loop < MaxApps - 1; Loop++) {
			fprintf (PrefsFile, "{ \"%s\", %d },\n", myInfo->Apps[Loop].Name,
			         myInfo->Apps[Loop].PortID);
		}
		fprintf (PrefsFile, "{ \"%s\", %d }\n", myInfo->Apps[Loop].Name,
		         myInfo->Apps[Loop].PortID);

		fprintf (PrefsFile, "},\n");
		/* Middle Numbers.
		*/
		fprintf (PrefsFile, " %d, /* TempoMax  */\n", myInfo->TempoMax);
		fprintf (PrefsFile, " %d, /* Metronome On  */\n", myInfo->MetronomeOn);
		fprintf (PrefsFile, " %d, /* Drum down beat, midi code  */\n", myInfo->Drum1);
		fprintf (PrefsFile, " %d, /* Drum Other beats  */\n", myInfo->DrumRest);
		fprintf (PrefsFile, " %d, /* Base Midi Note for switching  */\n", myInfo->MidiBaseNote);
		fprintf (PrefsFile, " %d, /* Tempo   */\n", myInfo->Tempo);
		fprintf (PrefsFile, " %d, /* CountInBeats   */\n", myInfo->CountInBeats);
		fprintf (PrefsFile, " %d, /* LoopRecBeats   */\n", myInfo->LoopRecBeats);
		fprintf (PrefsFile, " %d, /* MidiPassThru   */\n", myInfo->MidiPassThru);
		fprintf (PrefsFile, " %d, /* MidiPassLevel   */\n", myInfo->MidiPassLevel);
		fprintf (PrefsFile, " %d, /* BeatsPerMeasure   */\n", myInfo->BeatsPerMeasure);

		fprintf (PrefsFile, " %d, /* AnalogVolume   */\n", myInfo->AnalogVolume);
		fprintf (PrefsFile, " %d, /* MidiVolume   */\n", myInfo->MidiVolume);
		fprintf (PrefsFile, " 0x%x, /* StatusTextColor   */\n", myInfo->StatusTextColor);
		fprintf (PrefsFile, " 0x%x, /* ButtonTextColor   */\n", myInfo->ButtonTextColor);

		fprintf (PrefsFile, " \"%s\", /* OSCIPAddress   */\n", myInfo->OSCIPAddress);
		fprintf (PrefsFile, " \"%s\", /* OSCPortNumLooper   */\n", myInfo->OSCPortNumLooper);
		fprintf (PrefsFile, " \"%s\", /* OSCPortNumJackVol   */\n", myInfo->OSCPortNumJackVol);
		fprintf (PrefsFile, " \"%s\", /* OSCPortNumHydrogen   */\n", myInfo->OSCPortNumHydrogen);

	}

	fprintf (PrefsFile, " %d, /* Number of Strings   */\n {", myInfo->NumberOfStrings);
	printf (" %d, /* Number of Strings   */\n {", myInfo->NumberOfStrings);

	for (Loop = 0; Loop < TotalMaxStrings; Loop++) {
		fprintf (PrefsFile, "%d,\n", myInfo->BaseStringName[Loop]);
		printf ("%d,\n", myInfo->BaseStringName[Loop]);
	}

	fprintf (PrefsFile, "    },\n");

	printd (LogInfo, "BaseName %s\n", myInfo->BasePath);

	printd (LogInfo, "Num Ports %d\n", myInfo->NumOutPorts);
	for (Loop = 0; Loop < myInfo->NumOutPorts; Loop++) {
		printd (LogInfo, "Ports %s\n", myInfo->OutPortName[Loop]);
	}

	/* LiveMusic Apps.
	*/
	for (Loop = 0; Loop < MaxApps; Loop++) {
		printd (LogInfo, "Apps %s %d\n", myInfo->Apps[Loop].Name,
		        myInfo->Apps[Loop].PortID);
	}

	thePorts = &theInPorts;
	for (Loop = 0; Loop < thePorts->NumDevices; Loop++) {
		printd (LogInfo, "In Name [%s]\n", thePorts->Devices[Loop].Name);
		for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
			printd (LogInfo, "In SName %d, [%s]\n", Loop1,
			        thePorts->Devices[Loop].Ports[Loop1].Name);
		}
	}

	thePorts = &theOutPorts;
	for (Loop = 0; Loop < thePorts->NumDevices; Loop++) {
		printd (LogInfo, "Out Name [%s]\n", thePorts->Devices[Loop].Name);
		for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
			printd (LogInfo, "Out SName %d, [%s]\n", Loop1,
			        thePorts->Devices[Loop].Ports[Loop1].Name);
		}
	}
	if (PrefsFile)
		fprintf (PrefsFile, "{ \n ");

	/* Layouts .
	*/
	for (Loop = 0; gMyInfo.LayoutPresets[Loop].Name[0] != 0; Loop++) {
		printd (LogInfo, "Lay=%d %s\n", Loop,
		        gMyInfo.LayoutPresets[Loop].Name);

		if (PrefsFile)
			fprintf (PrefsFile, "\t{\n \"%s\", \n{\n", gMyInfo.LayoutPresets[Loop].Name);


		for (Loop1 = 0; gMyInfo.LayoutPresets[Loop].Presets[Loop1][0] != 0; Loop1++) {
			printd (LogInfo, "%d %s \n", Loop1, gMyInfo.LayoutPresets[Loop].Presets[Loop1]);

			if (PrefsFile)
				fprintf (PrefsFile, "/* %3d */  \"%s\", \n", Loop1, gMyInfo.LayoutPresets[Loop].Presets[Loop1]);
		}
		if (PrefsFile)
			fprintf (PrefsFile, "},}, \n", gMyInfo.LayoutPresets[Loop].Name);


	}
	if (PrefsFile) {
		fprintf (PrefsFile, "{\n \"\", { },\t},\n },\n}; \n");
		fflush(PrefsFile);
	}



	printd (LogInfo, "Metronome Base %d On %d %d %d\n", myInfo->TempoMax,
	        myInfo->MetronomeOn, myInfo->Drum1, myInfo->DrumRest);
	printd (LogInfo, "Midi Base Note %d\n", myInfo->MidiBaseNote);

}

/*--------------------------------------------------------------------
 * Function:		WritePref
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void WritePrefs (void) {
	xmlDocPtr doc = NULL;		/* document pointer */
	xmlNodePtr root_node = NULL, node = NULL, node0 = NULL, node1 = NULL, node2 = NULL;	/* node pointers */
	xmlDtdPtr dtd = NULL;		/* DTD pointer */
	char buff[256];
	int Loop;
	int PatchLoop;;

	LIBXML_TEST_VERSION;

	printd (LogDebug, "In Write Prefs\n");
	doc = xmlNewDoc (BAD_CAST "1.0");
	root_node = xmlNewNode (NULL, BAD_CAST XML_BASIC_SETTINGS);
	xmlDocSetRootElement (doc, root_node);

	xmlNewChild (root_node, NULL, BAD_CAST "SongPath",
	             BAD_CAST gMyInfo.BasePath);


	sprintf (buff, "%03d", gMyInfo.TempoMax);
	xmlNewChild (root_node, NULL, BAD_CAST "TempoMax", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.MetronomeOn);
	xmlNewChild (root_node, NULL, BAD_CAST "MetroOn", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.Drum1);
	xmlNewChild (root_node, NULL, BAD_CAST "Click1", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.DrumRest);
	xmlNewChild (root_node, NULL, BAD_CAST "ClickRest", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.MidiBaseNote);
	xmlNewChild (root_node, NULL, BAD_CAST "MidiBaseNote", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.Tempo);
	xmlNewChild (root_node, NULL, BAD_CAST "Tempo", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.CountInBeats);
	xmlNewChild (root_node, NULL, BAD_CAST "CountInBeats", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.LoopRecBeats);
	xmlNewChild (root_node, NULL, BAD_CAST "LoopRecBeats", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.MidiPassThru);
	xmlNewChild (root_node, NULL, BAD_CAST "MidiPassThru", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.MidiPassLevel);
	xmlNewChild (root_node, NULL, BAD_CAST "MidiPassLevel", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.BeatsPerMeasure);
	xmlNewChild (root_node, NULL, BAD_CAST "BeatsPerMeasure", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.AnalogVolume);
	xmlNewChild (root_node, NULL, BAD_CAST "AnalogVolume", BAD_CAST buff);

	sprintf (buff, "%03d", gMyInfo.MidiVolume);
	xmlNewChild (root_node, NULL, BAD_CAST "MidiVolume", BAD_CAST buff);

	sprintf (buff, "0x%x", gMyInfo.StatusTextColor);
	xmlNewChild (root_node, NULL, BAD_CAST "StatusTextColor", BAD_CAST buff);

	sprintf (buff, "0x%x", gMyInfo.ButtonTextColor);
	xmlNewChild (root_node, NULL, BAD_CAST "ButtonTextColor", BAD_CAST buff);

	sprintf (buff, "%s", gMyInfo.OSCIPAddress);
	xmlNewChild (root_node, NULL, BAD_CAST "OSCIPAddress", BAD_CAST buff);

	sprintf (buff, "%s", gMyInfo.OSCPortNumLooper);
	xmlNewChild (root_node, NULL, BAD_CAST "OSCPortNumLooper", BAD_CAST buff);

	sprintf (buff, "%s", gMyInfo.OSCPortNumJackVol);
	xmlNewChild (root_node, NULL, BAD_CAST "OSCPortNumJackVol", BAD_CAST buff);

	sprintf (buff, "%s", gMyInfo.OSCPortNumHydrogen);
	xmlNewChild (root_node, NULL, BAD_CAST "OSCPortNumHydrogen", BAD_CAST buff);

	/*
	 * Write out the Output Ports.
	 */
	sprintf (buff, "%03d", gMyInfo.NumOutPorts);
	xmlNewChild (root_node, NULL, BAD_CAST "NumOutPorts", BAD_CAST buff);
	printd (LogDebug, "In Write Prefs Output Ports\n");

	node1 = xmlNewChild (root_node, NULL, BAD_CAST "OutPorts", NULL);
	for (Loop = 0; Loop < MaxOutPorts; Loop++) {
		sprintf (buff, "Port%03d", Loop);
		node = xmlNewChild (node1, NULL, buff, NULL);
		xmlSetProp (node, "Name", gMyInfo.OutPortName[Loop]);
	}


	/*
	 * Write out the String Names.
	 */
	sprintf (buff, "%03d", gMyInfo.NumberOfStrings);
	xmlNewChild (root_node, NULL, BAD_CAST "NumberOfStrings", BAD_CAST buff);

	node1 = xmlNewChild (root_node, NULL, BAD_CAST "Strings", NULL);
	for (Loop = 0; Loop < TotalMaxStrings; Loop++) {
		sprintf (buff, "StrNum%03d", Loop);
		node = xmlNewChild (node1, NULL, buff, NULL);
		sprintf (buff, "%02d", gMyInfo.BaseStringName[Loop]);
		xmlSetProp (node, "Value", buff);
	}

	/*
	 * Write out the Patches
	 */
	node1 = xmlNewChild (root_node, NULL, BAD_CAST "MainButtons", NULL);
	for (Loop = 0; Loop < Max_Patches; Loop++) {
//              doc->children = xmlNewDocNode(doc, NULL, gMyInfo.MyPatchInfo[Loop].Button, NULL);
		sprintf (buff, "Preset%03d", Loop);
		node = xmlNewChild (node1, NULL, buff, NULL);
		xmlSetProp (node, "Name", gMyInfo.MyPatchInfo[Loop].Name);
		sprintf (buff, "%03d", gMyInfo.MyPatchInfo[Loop].Channel);
		xmlSetProp (node, "Channel", buff);
		sprintf (buff, "%03d", gMyInfo.MyPatchInfo[Loop].BankSelect);
		xmlSetProp (node, "Controller", buff);
		sprintf (buff, "%03d", gMyInfo.MyPatchInfo[Loop].OutPort);
		xmlSetProp (node, "OutPort", buff);
		sprintf (buff, "%03d", gMyInfo.MyPatchInfo[Loop].Patch);
		xmlSetProp (node, "Patch", buff);
		sprintf (buff, "%03d", gMyInfo.MyPatchInfo[Loop].CustomCommand);
		xmlSetProp (node, "Custom", buff);
//              sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].Chain);
		xmlSetProp (node, "Chain", gMyInfo.MyPatchInfo[Loop].Chain);
	}


	node1 = xmlNewChild (root_node, NULL, BAD_CAST "AppNames", NULL);
	for (Loop = 0; Loop < MaxApps; Loop++) {
		sprintf (buff, "App%03d", Loop);
		node = xmlNewChild (node1, NULL, buff, NULL);
		xmlSetProp (node, "Name", gMyInfo.Apps[Loop].Name);
		sprintf (buff, "%03d", gMyInfo.Apps[Loop].PortID);
		xmlSetProp (node, "PortID", buff);
	}

	/*
	 * Dump the layouts to the XML File.
	 */
	printd (LogDebug, "In Write Prefs layouts\n");
	Loop = 0;
	node = xmlNewChild (root_node, NULL, BAD_CAST "Layouts", NULL);
	while (gMyInfo.LayoutPresets[Loop].Name[0]) {
		sprintf (buff, "Layout%03d", Loop);
		node1 = xmlNewChild (node, NULL, buff, NULL);

		xmlSetProp (node1, "LayoutName", gMyInfo.LayoutPresets[Loop].Name);

		for (PatchLoop = 0; PatchLoop < Max_Patches; PatchLoop++) {
			sprintf (buff, "Patch%03d", PatchLoop);
			node2 = xmlNewChild (node1, NULL, buff, NULL);
			xmlSetProp (node2, "PatchName",
			            gMyInfo.LayoutPresets[Loop].Presets[PatchLoop]);
		}
		Loop++;
	}

	/*
	 * Dumping document to stdio or file
	 */

	printd (LogDebug, "In Write Prefs %s\n", GetResourceDir(PREFSFILENAME, FileLocConfig));
	xmlSaveFormatFileEnc (	GetResourceDir(PREFSFILENAME, FileLocConfig), doc, "UTF-8", 1);

	xmlFreeDoc (doc);

	xmlCleanupParser ();

	if (GenerateHFile)
		PrintDataStructure(&gMyInfo, DefinePrefsFile);
}

#include <libxml/xmlreader.h>
// NodeType: The node type,
// 1 for start element,
// 2 for attributes,
// 3 for text nodes,
// 4 for CData sections,
// 5 for entity references,
// 6 for entity declarations,
// 7 for PIs,
// 8 for comments,
// 9 for the document nodes,
// 10 for DTD/Doctype nodes,
// 11 for document fragment and
// 12 for notation nodes.
// 13 Whitespace
// 14 SignificantWhitespace
// 15 for end of element,
// 16 EndEntity
// XML_ELEMENT_NODE
//
//
enum { dTopLevelNone = 0, dTopLevelMainButtons, dTopLevelNumOutPorts,
       dTopLevelOutPorts, dTopLevelSongPath, dTopLevelAppName,
       dTopLevelMetronome, dTopLevelMetronomeOn, dTopLevelMidiBase,
       dTopLevelClick1, dTopLevelClickRest, dTopLevelLayouts,
       dTopLevelTempo, dTopLevelCountInBeats, dTopLevelLoopRecBeats, dTopLevelBeatsPerMeasure,
       dTopLevelAnalogVolume,  dTopLevelMidiVolume, dTopLevelStatusTextColor, dTopLevelButtonTextColor,
       dTopLevelOSCIPAddress, dTopLevelOSCPortNumLooper,
       dTopLevelOSCPortNumJackVol, dTopLevelOSCPortNumHydrogen,
       dTopMidiPassThru, dTopMidiPassLevel, dTopMaxStrings, dTopStrings
     };

int TopLevelParse;
int ParseCountL4;
int ParseCountL2;
int ParseCountL3;
int LayoutIndexXML;
int PatchIndexXML;

static void processNode (xmlTextReaderPtr reader, char Location) {
	xmlChar *name, *value;
	int NodeType;
	int Depth;
	char Buffer[100];
	int HoldIndex;


	name = xmlTextReaderName (reader);
	if (name == NULL)
		name = xmlStrdup (BAD_CAST "");

	value = xmlTextReaderValue (reader);

	NodeType = xmlTextReaderNodeType (reader);
	Depth = xmlTextReaderDepth (reader);
	//      if (NodeType != 14 && NodeType != 3) {
	printd (LogDebug, "%d D=%d T=%d N=[%s] V=[%s] E=%d Top=%d\n", Location,
	        Depth, NodeType, name, value, xmlTextReaderIsEmptyElement (reader), TopLevelParse);

	if (Depth == 2)
		ParseCountL2++;

	if (Depth == 3)
		ParseCountL3++;

	if (Depth == 4)
		ParseCountL4++;

	if (Depth == 1 && NodeType == 14) {
		printd (LogDebug, "\n*** Setting Top level to None\n");
		TopLevelParse = dTopLevelNone;
	}

	if (Depth == 1 && NodeType == 1) {
		printd (LogDebug, "\nTop Level Clear \n");
		TopLevelParse = dTopLevelNone;
		ParseCountL4 = 0;
		ParseCountL2 = 0;
		ParseCountL3 = 0;

		/* Let's start by looking for the highest level qualifier.
		 */
		if (!strcmp ("MainButtons", name) && NodeType == 1) {
			TopLevelParse = dTopLevelMainButtons;
		}

		if (!strcmp ("NumOutPorts", name) && NodeType == 1) {
			TopLevelParse = dTopLevelNumOutPorts;
		}

		if (!strcmp ("NumberOfStrings", name) && NodeType == 1) {
			TopLevelParse = dTopMaxStrings;
		}

		if (!strcmp ("Strings", name) && NodeType == 1) {
			TopLevelParse = dTopStrings;
		}

		if (!strcmp ("OutPorts", name) && NodeType == 1) {
			TopLevelParse = dTopLevelOutPorts;
		}

		if (!strcmp ("SongPath", name) && NodeType == 1) {
			TopLevelParse = dTopLevelSongPath;
		}

		if (!strcmp ("AppNames", name) && NodeType == 1) {
			TopLevelParse = dTopLevelAppName;
		}

		if (!strcmp ("TempoMax", name) && NodeType == 1) {
			TopLevelParse = dTopLevelMetronome;
		}

		if (!strcmp ("MetroOn", name) && NodeType == 1) {
			TopLevelParse = dTopLevelMetronomeOn;
		}

		if (!strcmp ("MidiBaseNote", name) && NodeType == 1) {
			TopLevelParse = dTopLevelMidiBase;
		}

		if (!strcmp ("Click1", name) && NodeType == 1) {
			TopLevelParse = dTopLevelClick1;
		}

		if (!strcmp ("ClickRest", name) && NodeType == 1) {
			TopLevelParse = dTopLevelClickRest;
			printd (LogInfo, "\nTop Level Set %d\n", TopLevelParse);
		}

		if (!strcmp ("Layouts", name) && NodeType == 1) {
			TopLevelParse = dTopLevelLayouts;
			printd (LogInfo, "Found Layouts\n");
		}

		if (!strcmp ("Tempo", name) && NodeType == 1) {
			TopLevelParse = dTopLevelTempo;
		}

		if (!strcmp ("MidiPassThru", name) && NodeType == 1) {
			TopLevelParse = dTopMidiPassThru;
		}

		if (!strcmp ("MidiPassLevel", name) && NodeType == 1) {
			TopLevelParse = dTopMidiPassLevel;
		}

		if (!strcmp ("CountInBeats", name) && NodeType == 1) {
			TopLevelParse = dTopLevelCountInBeats;
		}

		if (!strcmp ("LoopRecBeats", name) && NodeType == 1) {
			TopLevelParse = dTopLevelLoopRecBeats;
		}
		if (!strcmp ("BeatsPerMeasure", name) && NodeType == 1) {
			TopLevelParse = dTopLevelBeatsPerMeasure;
		}

		if (!strcmp ("AnalogVolume", name) && NodeType == 1) {
			TopLevelParse = dTopLevelAnalogVolume;
		}

		if (!strcmp ("MidiVolume", name) && NodeType == 1) {
			TopLevelParse = dTopLevelMidiVolume;
		}

		if (!strcmp ("StatusTextColor", name) && NodeType == 1) {
			TopLevelParse = dTopLevelStatusTextColor;
		}

		if (!strcmp ("ButtonTextColor", name) && NodeType == 1) {
			TopLevelParse = dTopLevelButtonTextColor;
		}

		if (!strcmp ("OSCIPAddress", name) && NodeType == 1) {
			TopLevelParse = dTopLevelOSCIPAddress;
		}

		if (!strcmp ("OSCPortNumLooper", name) && NodeType == 1) {
			TopLevelParse = dTopLevelOSCPortNumLooper;
		}

		if (!strcmp ("OSCPortNumJackVol", name) && NodeType == 1) {
			TopLevelParse = dTopLevelOSCPortNumJackVol;
		}

		if (!strcmp ("OSCPortNumHydrogen", name) && NodeType == 1) {
			TopLevelParse = dTopLevelOSCPortNumHydrogen;
		}
	}

	if (TopLevelParse == dTopLevelOutPorts) {
		/* Get the second element.
		 */
		printd(LogDebug, "dTopLevelOutPorts %d %d\n", Depth, NodeType );
		if (Depth == 2 && NodeType == 1) {
			sscanf (name, "Port%03d", &HoldIndex);
			ParseCountL2 = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {
			if (!strcmp ("Name", name))
				strcpy (gMyInfo.OutPortName[ParseCountL2], value);

		}
	}

	if (TopLevelParse == dTopStrings) {
		/* Get the second element.
		 */
		printd(LogDebug, "dTopStrings %d %d\n", Depth, NodeType );

		if (Depth == 2 && NodeType == 1) {
			sscanf (name, "StrNum%03d", &HoldIndex);
			printd(LogDebug, "String Number %d\n", HoldIndex );
			ParseCountL2 = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {
			printd(LogDebug, "String Value %d %d %s\n", HoldIndex, ParseCountL2, value );
			if (!strcmp ("Value", name))
				gMyInfo.BaseStringName[ParseCountL2] = atoi(value);

		}
	}

	if (TopLevelParse == dTopLevelAppName) {
		/* Get the second element.
		 */
		if (Depth == 2 && NodeType == 1) {
			sscanf (name, "App%03d", &HoldIndex);
			ParseCountL2 = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {
			if (!strcmp ("Name", name))
				strcpy (gMyInfo.Apps[ParseCountL2].Name, value);

			if (!strcmp ("PortID", name))
				gMyInfo.Apps[ParseCountL2].PortID = atoi (value);
		}
	}

	/* ********************************** */
	if (TopLevelParse == dTopLevelLayouts) {
		printd(LogInfo, "IN the TopLevel Layouts Parse\n");
		printd(LogInfo, "dTopLevelLayouts %d %d\n", LayoutIndexXML,  PatchIndexXML);

		if (Depth == 2 && NodeType == 1) {
			sscanf (name, "Layout%03d\n", &HoldIndex);

			/* This is the index for the Layouts. */
			LayoutIndexXML = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {
			/* Assign Names here.   */
			printd (LogInfo, "Layout Name Read %d %s\n", LayoutIndexXML, value);
			strcpy (gMyInfo.LayoutPresets[LayoutIndexXML].Name, value);
		}

		if (Depth == 3 && NodeType == 1) {
			sscanf (name, "Patch%03d\n", &HoldIndex);
			PatchIndexXML = HoldIndex;
		}

		if (Depth == 4 && NodeType == 2) {
			/* Assign Presets here. */
			strcpy (gMyInfo.
			        LayoutPresets[LayoutIndexXML].Presets[PatchIndexXML],
			        value);
		}
	}

	if (TopLevelParse == dTopLevelMainButtons) {
		/* Get the second element.
		 */
		if (Depth == 2 && NodeType == 1) {
			sscanf (name, "Preset%03d", &HoldIndex);
			ParseCountL2 = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {

			if (!strcmp ("Name", name))
				strcpy (gMyInfo.MyPatchInfo[ParseCountL2].Name, value);

			if (!strcmp ("Channel", name))
				gMyInfo.MyPatchInfo[ParseCountL2].Channel = atoi (value);

			if (!strcmp ("Controller", name))
				gMyInfo.MyPatchInfo[ParseCountL2].BankSelect = atoi (value);

			if (!strcmp ("OutPort", name))
				gMyInfo.MyPatchInfo[ParseCountL2].OutPort = atoi (value);

			if (!strcmp ("Patch", name))
				gMyInfo.MyPatchInfo[ParseCountL2].Patch = atoi (value);

			if (!strcmp ("Custom", name))
				gMyInfo.MyPatchInfo[ParseCountL2].CustomCommand = atoi (value);

			if (!strcmp ("Chain", name))
				strcpy (gMyInfo.MyPatchInfo[ParseCountL2].Chain, value);

		}

	}

	/* String names here.
	*/

	/* TopLevelParse == dTopLevelMainButtons      */
	if (Depth == 2 && NodeType == 3) {
		if (TopLevelParse == dTopLevelNumOutPorts) {
			gMyInfo.NumOutPorts = atoi (value);
			printd (LogDebug, "\n***Numout Ports %d\n", gMyInfo.NumOutPorts);
		}

		if (TopLevelParse == dTopMaxStrings) {
			gMyInfo.NumberOfStrings = atoi (value);
			printd (LogDebug, "\n***Number of Strings %d\n", gMyInfo.NumberOfStrings);
		}

		if (TopLevelParse == dTopLevelSongPath) {
			printd (LogDebug, "Song Path %s\n", value);
			strncpy (gMyInfo.BasePath, value, 255);
		}

		if (TopLevelParse == dTopLevelMetronome) {
			printd (LogDebug, "dTopLevelMetronome %s\n", value);
			gMyInfo.TempoMax = atoi (value);
		}

		if (TopLevelParse == dTopLevelMetronomeOn) {
			printd (LogDebug, "dTopLevelMetronomeOn %s\n", value);
			gMyInfo.MetronomeOn = atoi (value);
		}

		if (TopLevelParse == dTopLevelMidiBase) {
			printd (LogDebug, "dTopLevelMidiBase %s\n", value);
			gMyInfo.MidiBaseNote = atoi (value);
		}

		if (TopLevelParse == dTopLevelClick1) {
			printd (LogDebug, "dTopLevelClick1 %s\n", value);
			gMyInfo.Drum1 = atoi (value);
		}

		if (TopLevelParse == dTopLevelClickRest) {
			printd (LogDebug, "dTopLevelClickRest %s\n", value);
			gMyInfo.DrumRest = atoi (value);
		}

		if (TopLevelParse == dTopLevelTempo) {
			printd (LogDebug, "dTopLevelTempo %s\n", value);
			gMyInfo.Tempo = atoi (value);
		}

		if (TopLevelParse == dTopLevelCountInBeats) {
			printd (LogDebug, "dTopLevelCountInBeats %s\n", value);
			gMyInfo.CountInBeats = atoi (value);
		}

		if (TopLevelParse == dTopLevelLoopRecBeats) {
			printd (LogDebug, "dTopLevelLoopRecBeats %s\n", value);
			gMyInfo.LoopRecBeats = atoi (value);
		}

		if (TopLevelParse == dTopLevelBeatsPerMeasure) {
			printd (LogDebug, "dTopLevelBeatsPerMeasure %s\n", value);
			gMyInfo.BeatsPerMeasure = atoi (value);
		}

		if (TopLevelParse == dTopLevelAnalogVolume) {
			printd (LogDebug, "dTopLevelAnalogVolume %s\n", value);
			gMyInfo.AnalogVolume = atoi (value);
		}

		if (TopLevelParse == dTopLevelMidiVolume) {
			printd (LogDebug, "dTopLevelMidiVolume %s\n", value);
			gMyInfo.MidiVolume = atoi (value);
		}

		if (TopLevelParse == dTopMidiPassLevel) {
			printd (LogDebug, "dTopMidiPassLevel %s\n", value);
			gMyInfo.MidiPassLevel = atoi (value);
		}

		if (TopLevelParse == dTopMidiPassThru) {
			printd (LogDebug, "dTopMidiPassThru %s\n", value);
			gMyInfo.MidiPassThru = atoi (value);
		}

		if (TopLevelParse == dTopLevelOSCIPAddress) {
			printd (LogDebug, "dTopLevelOSCIPAddress %s\n", value);
			strncpy (gMyInfo.OSCIPAddress, value, sizeof (gMyInfo.OSCIPAddress));
		}

		if (TopLevelParse == dTopLevelOSCPortNumJackVol) {
			printd (LogDebug, "dTopLevelOSCPortNumJackVol %s\n", value);
			strncpy (gMyInfo.OSCPortNumJackVol, value, sizeof (gMyInfo.OSCPortNumJackVol));
		}

		if (TopLevelParse == dTopLevelOSCPortNumLooper) {
			printd (LogDebug, "dTopLevelOSCPortNumLooper %s\n", value);
			strncpy (gMyInfo.OSCPortNumLooper, value, sizeof (gMyInfo.OSCPortNumLooper));
		}

		if (TopLevelParse == dTopLevelOSCPortNumHydrogen) {
			printd (LogDebug, "dTopLevelOSCPortNumHydrogen %s\n", value);
			strncpy (gMyInfo.OSCPortNumHydrogen, value, sizeof (gMyInfo.OSCPortNumHydrogen));
		}

		if (TopLevelParse == dTopLevelStatusTextColor) {
			printd (LogDebug, "dTopLevelStatusTextColor %s\n", value);
			sscanf(value, "%x", &gMyInfo.StatusTextColor);
		}


		if (TopLevelParse == dTopLevelButtonTextColor) {
			printd (LogDebug, "dTopLevelButtonTextColor %s\n", value);
			sscanf(value, "%x", &gMyInfo.ButtonTextColor);
		}

	}

	/* Free memory.
	*/
	if (value != NULL)
		xmlFree (value);

	if (NodeType == 1)
		xmlFree (name);
}

int ReadPrefs () {
	xmlTextReaderPtr reader;
	int ret;

	xmlChar *name, *value;
	int NodeType;
	int Depth;

	printd (LogDebug, "----------------------\n");
	printd (LogDebug, "Reading prefs file\n");
	printd (LogDebug, "----------------------\n");
	reader = xmlNewTextReaderFilename (	GetResourceDir(PREFSFILENAME, FileLocConfig));
	printd (LogDebug, "Reader %x\n", reader);
	if (reader != NULL) {
		ret = xmlTextReaderRead (reader);
		while (ret == 1) {
			processNode (reader, 1);
			while (xmlTextReaderMoveToNextAttribute (reader))
				processNode (reader, 2);

			ret = xmlTextReaderRead (reader);
		}
		xmlFreeTextReader (reader);
		if (ret != 0) {
			printd (LogTest, "%s : failed to parse\n", GetResourceDir(PREFSFILENAME, FileLocConfig));
			return (1);
		}
	} else {
		printd (LogTest, "Unable to open %s\n", GetResourceDir(PREFSFILENAME, FileLocConfig));
		return (2);
	}
	printd (LogDebug, "----------------------\n");
	printd (LogDebug, "Done Reading prefs file\n");
	printd (LogDebug, "----------------------\n");
	return (0);
}
