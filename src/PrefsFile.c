/*	File: 	PrefsFile
 |
 |	Contains:
 |
 |
 |	Written By: 	Elias Keshishoglou on Tue Dec 25 17:28:58 PST 2012
 |
 |	Copyright �: 	2012 Elias Keshishoglou all rights reserved.
 |
 |
 |---------------------------------------------------------------------*/

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

#define PREFSFILENAME "LiveMusicRes/LiveMusic.xml"
#define DefinePrefsFile "./src/GenPrefs.h"

/*
 * Place Local prototypes here
 */

void WritePrefs (void);

/*
 * Place Static variables here
 */
/* @formatter:off */
//#include "DefPrefs.h"
#include "GenPrefs.h"

// START-ECLIPSE-FORMATTING

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
	memcpy (&gMyInfo, &GlobalInfo, sizeof (LiveMusicInfo));

#if 0
	WritePrefs ();
	ReadPrefs ();
	PrintDataStructure(&gMyInfo, DefinePrefsFile);
	exit(0);

#endif
#if 1
	gMyInfo.TempoMax = 8;

	for (Count = 0; Count < MaxPresetButtons; Count++)
		gMyInfo.WebPresets.thePreset[Count] = -1;

#endif

#if 0 // Loaded from file.
	gMyInfo.AnalogVolume = 100;
	gMyInfo.MidiVolume = 80;
	gMyInfo.StatusTextColor = 0xffffe0;
	gMyInfo.ButtonTextColor = 0xffe0e0;

	gMyInfo.MidiBaseNote = 45;
	gMyInfo.BeatsPerMeasure = 4;
	gMyInfo.LoopRecBeats = 16;
	gMyInfo.CountInBeats = 4;
	gMyInfo.ExpreP1Slider = Slider3;
	gMyInfo.MidiPassThru = 1;
	gMyInfo.MidiPassLevel = 110;

	strcpy (gMyInfo.OSCIPAddress, "127.0.0.1");
	strcpy (gMyInfo.OSCPortNumLooper, "9951");
	strcpy (gMyInfo.OSCPortNumJackVol, "9952");
	strcpy (gMyInfo.OSCPortNumHydrogen, "9000");
#endif
//    ReadPrefs();
//	PrintDataStructure(&gMyInfo, DefinePrefsFile);
//      printd(LogInfo, "Prefs %s %s\n", GlobalInfo.Apps[2].Name, &gMyInfo.Apps[2].Name);
//      WritePrefs();

	/*
	 * Create the popup Menu's now that we have the presets.
	 */
	CreatePatchPopupMenu ();
}

/*--------------------------------------------------------------------
 * Function:		PrintDataStructure
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void
PrintDataStructure (LiveMusicInfo * myInfo, char *PrefsRef) {
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


		/* Patch definiations.
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
void
WritePrefs (void) {
	xmlDocPtr doc = NULL;		/* document pointer */
	xmlNodePtr root_node = NULL, node = NULL, node0 = NULL, node1 = NULL, node2 = NULL;	/* node pointers */
	xmlDtdPtr dtd = NULL;		/* DTD pointer */
	char buff[256];
	int Loop;
	int PatchLoop;;

	LIBXML_TEST_VERSION;

	printd (LogInfo, "In Write Prefs\n");
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

	node1 = xmlNewChild (root_node, NULL, BAD_CAST "OutPorts", NULL);
	for (Loop = 0; Loop < MaxOutPorts; Loop++) {
		sprintf (buff, "Port%03d", Loop);
		node = xmlNewChild (node1, NULL, buff, NULL);
		xmlSetProp (node, "Name", gMyInfo.OutPortName[Loop]);
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
	Loop = 0;
	node = xmlNewChild (root_node, NULL, BAD_CAST "Layouts", NULL);
//      printd(LogInfo, "Starting %x %s\n",node, gMyInfo.LayoutPresets[Loop].Name);
	while (gMyInfo.LayoutPresets[Loop].Name[0]) {
		sprintf (buff, "Layout%03d", Loop);
		node1 = xmlNewChild (node, NULL, buff, NULL);
//              printd(LogInfo, "Loop %d %x %s\n",Loop,node1, LayoutPresets[Loop].Name);

		xmlSetProp (node1, "LayoutName", gMyInfo.LayoutPresets[Loop].Name);
//              printd(LogInfo, "xmlSetProp %d %x %s\n",Loop,node1, LayoutPresets[Loop].Name);

//              node1 = xmlNewChild(node, NULL, BAD_CAST LayoutPresets[Loop].Name, NULL);
		for (PatchLoop = 0; PatchLoop < Max_Patches; PatchLoop++) {
			sprintf (buff, "Patch%03d", PatchLoop);
//                      printd(LogInfo, "Patchnames %s\n",buff);
			node2 = xmlNewChild (node1, NULL, buff, NULL);
			xmlSetProp (node2, "PatchName",
			            gMyInfo.LayoutPresets[Loop].Presets[PatchLoop]);
//                      node2= xmlNewChild(node1, NULL, BAD_CAST LayoutPresets[Loop].Presets[PatchLoop], NULL);
		}
		Loop++;
	}

#if 0
	typedef struct {
		char Name[PatchNameSize];
		char Presets[Max_Patches][PatchNameSize];
	} LayoutType;

	LayoutType LayoutPresets
#endif
	/*
	 * Dumping document to stdio or file
	 */
	xmlSaveFormatFileEnc (PREFSFILENAME, doc, "UTF-8", 1);

	xmlFreeDoc (doc);

	xmlCleanupParser ();
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
enum { dTopLevelNone = 0,dTopLevelMainButtons,dTopLevelNumOutPorts,
dTopLevelOutPorts, dTopLevelSongPath, dTopLevelAppName,
dTopLevelMetronome, dTopLevelMetronomeOn, dTopLevelMidiBase,
dTopLevelClick1, dTopLevelClickRest, dTopLevelLayouts,
dTopLevelTempo, dTopLevelCountInBeats, dTopLevelLoopRecBeats, dTopLevelBeatsPerMeasure,
dTopLevelAnalogVolume,  dTopLevelMidiVolume, dTopLevelStatusTextColor, dTopLevelButtonTextColor,
dTopLevelOSCIPAddress, dTopLevelOSCPortNumLooper,
dTopLevelOSCPortNumJackVol, dTopLevelOSCPortNumHydrogen,
dTopMidiPassThru,dTopMidiPassLevel
 };

int TopLevelParse;
int ParseCountL4;
int ParseCountL2;
int ParseCountL3;
int LayoutIndexXML;
int PatchIndexXML;

static void
processNode (xmlTextReaderPtr reader, char Location) {
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
#if 0
	printd (LogInfo, "%d D=%d T=%d N=[%s] V=[%s] E=%d\n", Location,
	        Depth, NodeType, name, value, xmlTextReaderIsEmptyElement (reader));
#endif

	if (Depth == 2)
		ParseCountL2++;

	if (Depth == 3)
		ParseCountL3++;

	if (Depth == 4)
		ParseCountL4++;

	if (Depth == 1 && NodeType == 14) {
		printd (LogInfo, "\n*** Setting Top level to None\n");
		TopLevelParse = dTopLevelNone;
	}

	if (Depth == 1 && NodeType == 1) {
		printd (LogInfo, "\nTop Level Clear \n");
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
		}			/* Depth == 1 NodeType == 1   */

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
		if (Depth == 2 && NodeType == 1) {
			sscanf (name, "Port%03d", &HoldIndex);
//                                      printd(LogInfo, "\nButton Number %d\n",HoldIndex );
			ParseCountL2 = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {
//                                      printd(LogInfo, "But %d %s %s", ParseCountL2, name, value);
			if (!strcmp ("Name", name))
				strcpy (gMyInfo.OutPortName[ParseCountL2], value);

		}
	}

	if (TopLevelParse == dTopLevelAppName) {
		/* Get the second element.
		 */
		if (Depth == 2 && NodeType == 1) {
			sscanf (name, "App%03d", &HoldIndex);
//                                      printd(LogInfo, "\nButton Number %d\n",HoldIndex );
			ParseCountL2 = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {
//                                      printd(LogInfo, "But %d %s %s", ParseCountL2, name, value);
			if (!strcmp ("Name", name))
				strcpy (gMyInfo.Apps[ParseCountL2].Name, value);

			if (!strcmp ("PortID", name))
				gMyInfo.Apps[ParseCountL2].PortID = atoi (value);
		}
	}

	/* ********************************** */
	if (TopLevelParse == dTopLevelLayouts) {
//printd(LogInfo, "IN the TopLevel Layouts Parse\n");
//              printd(LogInfo, "dTopLevelLayouts %d %d\n",LayoutIndexXML,  PatchIndexXML);

		if (Depth == 2 && NodeType == 1) {
			sscanf (name, "Layout%03d\n", &HoldIndex);
//                      printd(LogInfo, "Layout Index %d \n",HoldIndex );

			/* This is the index for the Layouts. */
			LayoutIndexXML = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {
			/* Assign Names here.   */
			printd (LogInfo, "Layout Name Read %d %s\n", LayoutIndexXML, value);
#if 0
			if (!*value) {
				TopLevelParse = dTopLevelNone;
				printd (LogInfo, "Layout Names end\n");
				exit (0);
				return;
			}
#endif
			strcpy (gMyInfo.LayoutPresets[LayoutIndexXML].Name, value);
		}

		if (Depth == 3 && NodeType == 1) {
			sscanf (name, "Patch%03d\n", &HoldIndex);
			PatchIndexXML = HoldIndex;
		}

		if (Depth == 4 && NodeType == 2) {
			/* Assign Presets here. */
//              printd(LogInfo, "Preset Name %d %s\n",ParseCountL3, value );
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
//                                      printd(LogInfo, "\nButton Number %d\n",HoldIndex );
			ParseCountL2 = HoldIndex;
// Index                        gMyInfo.MyPatchInfo[ParseCountL2].Index = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {

//              printd(LogInfo, "But %d %s %s", ParseCountL2, name, value);

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

	}				/* TopLevelParse == dTopLevelMainButtons      */
	if (Depth == 2 && NodeType == 3) {
		if (TopLevelParse == dTopLevelNumOutPorts) {
			gMyInfo.NumOutPorts = atoi (value);
			printd (LogInfo, "\n***Numout Ports %d\n", gMyInfo.NumOutPorts);
		}

		if (TopLevelParse == dTopLevelSongPath) {
			printd (LogInfo, "Song Path %s\n", value);
			strncpy (gMyInfo.BasePath, value, 255);
		}

		if (TopLevelParse == dTopLevelMetronome) {
			printd (LogInfo, "dTopLevelMetronome %s\n", value);
			gMyInfo.TempoMax = atoi (value);
		}

		if (TopLevelParse == dTopLevelMetronomeOn) {
			printd (LogInfo, "dTopLevelMetronomeOn %s\n", value);
			gMyInfo.MetronomeOn = atoi (value);
		}

		if (TopLevelParse == dTopLevelMidiBase) {
			printd (LogInfo, "dTopLevelMidiBase %s\n", value);
			gMyInfo.MidiBaseNote = atoi (value);
		}

		if (TopLevelParse == dTopLevelClick1) {
			printd (LogInfo, "dTopLevelClick1 %s\n", value);
			gMyInfo.Drum1 = atoi (value);
		}

		if (TopLevelParse == dTopLevelClickRest) {
			printd (LogInfo, "dTopLevelClickRest %s\n", value);
			gMyInfo.DrumRest = atoi (value);
		}

		if (TopLevelParse == dTopLevelTempo) {
			printd (LogInfo, "dTopLevelTempo %s\n", value);
			gMyInfo.Tempo = atoi (value);
		}

		if (TopLevelParse == dTopLevelCountInBeats) {
			printd (LogInfo, "dTopLevelCountInBeats %s\n", value);
			gMyInfo.CountInBeats = atoi (value);
		}

		if (TopLevelParse == dTopLevelLoopRecBeats) {
			printd (LogInfo, "dTopLevelLoopRecBeats %s\n", value);
			gMyInfo.LoopRecBeats = atoi (value);
		}

		if (TopLevelParse == dTopLevelBeatsPerMeasure) {
			printd (LogInfo, "dTopLevelBeatsPerMeasure %s\n", value);
			gMyInfo.BeatsPerMeasure = atoi (value);
		}

		if (TopLevelParse == dTopLevelAnalogVolume) {
			printd (LogInfo, "dTopLevelAnalogVolume %s\n", value);
			gMyInfo.AnalogVolume = atoi (value);
		}

		if (TopLevelParse == dTopLevelMidiVolume) {
			printd (LogInfo, "dTopLevelMidiVolume %s\n", value);
			gMyInfo.MidiVolume = atoi (value);
		}

		if (TopLevelParse == dTopMidiPassLevel) {
			printd (LogInfo, "dTopMidiPassLevel %s\n", value);
			gMyInfo.MidiPassLevel = atoi (value);
		}

		if (TopLevelParse == dTopMidiPassThru) {
			printd (LogInfo, "dTopMidiPassThru %s\n", value);
			gMyInfo.MidiPassThru = atoi (value);
		}

		if (TopLevelParse == dTopLevelOSCIPAddress) {
			printd (LogInfo, "dTopLevelOSCIPAddress %s\n", value);
			strncpy (gMyInfo.OSCIPAddress, value, 255);
		}

		if (TopLevelParse == dTopLevelOSCPortNumJackVol) {
			printd (LogInfo, "dTopLevelOSCPortNumJackVol %s\n", value);
			strncpy (gMyInfo.OSCPortNumJackVol, value, 255);
		}

		if (TopLevelParse == dTopLevelOSCPortNumLooper) {
			printd (LogInfo, "dTopLevelOSCPortNumLooper %s\n", value);
			strncpy (gMyInfo.OSCPortNumLooper, value, 255);
		}

		if (TopLevelParse == dTopLevelOSCPortNumHydrogen) {
			printd (LogInfo, "dTopLevelOSCPortNumHydrogen %s\n", value);
			strncpy (gMyInfo.OSCPortNumHydrogen, value, 255);
		}

		if (TopLevelParse == dTopLevelStatusTextColor) {
			printd (LogInfo, "dTopLevelStatusTextColor %s\n", value);
			sscanf(value, "%x", &gMyInfo.StatusTextColor);
		}


		if (TopLevelParse == dTopLevelButtonTextColor) {
			printd (LogInfo, "dTopLevelButtonTextColor %s\n", value);
			sscanf(value, "%x", &gMyInfo.ButtonTextColor);
		}

	}				/* Depth == 2 && NodeType == 3        */

#if 0
	if (Location == 2) {

		if (TopLevelParse == dTopLevelSongPath) {
			printd (LogInfo, "Song Path %s\n", value);
			strncpy (gMyInfo.BasePath, value, 255);
		}

	}
#endif

	if (value != NULL)
		xmlFree (value);

	if (NodeType == 1)
		xmlFree (name);
}

void
ReadPrefs () {
	xmlTextReaderPtr reader;
	int ret;

	xmlChar *name, *value;
	int NodeType;
	int Depth;

	printd (LogInfo, "----------------------\n");
	printd (LogInfo, "Reading prefs file\n");
	printd (LogInfo, "----------------------\n");
	reader = xmlNewTextReaderFilename (PREFSFILENAME);
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
			printd (LogInfo, "%s : failed to parse\n", PREFSFILENAME);
		}
	} else {
		printd (LogInfo, "Unable to open %s\n", PREFSFILENAME);
	}
	printd (LogInfo, "----------------------\n");
	printd (LogInfo, "Done Reading prefs file\n");
	printd (LogInfo, "----------------------\n");
}
