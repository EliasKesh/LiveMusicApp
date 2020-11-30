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
#if 1
	if (ReadPrefs ()) {
		memcpy (&gMyInfo, &GlobalInfo, sizeof (LiveMusicInfo));
		printd(LogDebug, "ReadFrefs Failed. \n");
		NewInstall = 1;
	}

#if 0
	sprintf(FileString, "%s/MySongs", homedir);
	printf("CheckForStartupDirs %s\n", FileString);
	printf("CheckForStartupDirs %s\n", homedir);
	printf("CheckForStartupDirs %s\n", gMyInfo.BasePath);
	err = stat(FileString, &s);
	if (-1 == err) {
		sprintf(CommandString, "mv %s/.config/LiveMusicApp/MySongs %s/", homedir, homedir);
		system(CommandString);
//		NewInstall = 1;
//		exit(0);
	}
#endif

#if 0
	/* Set up the not Found String.
	*/
	strcpy(
	    gMyInfo.MyPatchInfo[Patch_Not_Found].Name,
	    "NotFound");
#endif
//	PrintDataStructure(&gMyInfo, NULL);
//	exit(0);
#else
	memcpy (&gMyInfo, &GlobalInfo, sizeof (LiveMusicInfo));
#endif
	/* If it's a new install set the Charts directory.
	*/
	if (NewInstall) {
		strcpy(gMyInfo.BasePath, GetResourceDir("index.html", FileLocTunes ));
		printd(LogInfo, "New Install\n");
	}

	printd(LogInfo, "Prefs file %s\n",gMyInfo.BasePath);

	/* Clear some of the basic user variables.
	*/
	gMyInfo.TempoMax = 8;

	for (Count = 0; Count < MaxPresetButtons; Count++)
		gMyInfo.WebPresets.thePreset[Count] = -1;

#if 0
	gMyInfo.ControlRoute[0].OutPort = InternalPort;
	gMyInfo.ControlRoute[0].OutControl = 0;
	gMyInfo.ControlRoute[1].OutPort = GuitarixPort;
	gMyInfo.ControlRoute[1].OutControl = 7;
	gMyInfo.ControlRoute[2].OutPort = FluidPort;
	gMyInfo.ControlRoute[2].OutControl = 7;
	gMyInfo.ControlRoute[3].OutPort = LooperPort;
	gMyInfo.ControlRoute[3].OutControl = 9;
	gMyInfo.ControlRoute[4].OutPort = GuitarixPort;
	gMyInfo.ControlRoute[4].OutControl = 9;
	gMyInfo.ControlRoute[5].OutPort = GuitarixPort;
	gMyInfo.ControlRoute[5].OutControl = 10;
	gMyInfo.ControlRoute[7].OutPort = TransportPort;
	gMyInfo.ControlRoute[7].OutControl = 12;
#endif

	// WritePrefs();

	/* If we wish to generate an include file from the
	existing preferences.
	*/
	if (GenerateHFile)
		PrintDataStructure(&gMyInfo, DefinePrefsFile);

	/* Setup some parameters that require preferences values.
	*/
	PostProcessPrefs(&gMyInfo);
	gMyInfo.MidiThresholdLevel = 35;

//	exit(0);
}

/*--------------------------------------------------------------------
 * Function:		PostProcessPrefs
 *
 * Description:		Make any assignments not included in the prefs
 *
 *---------------------------------------------------------------------*/
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
 * Function:		PrintDataStructure
 *
 * Description:		Print out the data structure in an "#include" format
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
//			if (myInfo->MyPatchInfo[Loop].BankSelect <= MaxSoundFonts)
//				strcpy(SFName, SoundFontBankNames[myInfo->MyPatchInfo[Loop].BankSelect]);
//			else
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
		        gMyInfo.OutPortName[myInfo->MyPatchInfo[Loop].OutPort],
//		        CustomPorts[myInfo->MyPatchInfo[Loop].OutPort],
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
			         gMyInfo.OutPortName[myInfo->MyPatchInfo[Loop].OutPort],
//			         CustomPorts[myInfo->MyPatchInfo[Loop].OutPort],
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

		fprintf (PrefsFile, " %d, /* Number of Strings   */\n {", myInfo->NumberOfStrings);


//	printf (" %d, /* Number of Strings   */\n {", myInfo->NumberOfStrings);

		for (Loop = 0; Loop < TotalMaxStrings; Loop++) {
			fprintf (PrefsFile, "%d,\n", myInfo->BaseStringName[Loop]);
//		printf ("%d,\n", myInfo->BaseStringName[Loop]);
		}

		fprintf (PrefsFile, "    },\n");
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

	/* Controller prefs.
	*/
	if (PrefsFile)
		fprintf (PrefsFile, "{ // Controller mapping \n");

	for (Loop = 0; Loop < MaxControllers; Loop++) {
		printd (LogInfo, "Cont=%d %d\n", Loop,
		        gMyInfo.ControlRoute[Loop].OutPort);

		if (PrefsFile)
			fprintf (PrefsFile, "{ /* %3d */  %d, %d },\n", Loop,
			         gMyInfo.ControlRoute[Loop].OutPort,
			         gMyInfo.ControlRoute[Loop].OutControl);
	}

	if (PrefsFile)
		fprintf (PrefsFile, "}, \n");



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

//	LIBXML_TEST_VERSION;

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
//		printf("%d  %s\n", Loop, gMyInfo.MyPatchInfo[Loop].Name);
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

	/* Application names.
	*/
	node1 = xmlNewChild (root_node, NULL, BAD_CAST "AppNames", NULL);
	for (Loop = 0; Loop < MaxApps; Loop++) {
		sprintf (buff, "App%03d", Loop);
		node = xmlNewChild (node1, NULL, buff, NULL);
		xmlSetProp (node, "Name", gMyInfo.Apps[Loop].Name);
		sprintf (buff, "%03d", gMyInfo.Apps[Loop].PortID);
		xmlSetProp (node, "PortID", buff);
	}

	/* Controller mapping.
	*/
	node1 = xmlNewChild (root_node, NULL, BAD_CAST "ControllerMap", NULL);
	for (Loop = 0; Loop < MaxControllers; Loop++) {
		sprintf (buff, "App%03d", Loop);
		node = xmlNewChild (node1, NULL, buff, NULL);
		sprintf (buff, "%03d", gMyInfo.ControlRoute[Loop].OutPort);
		xmlSetProp (node, "Port", buff);
		sprintf (buff, "%03d", gMyInfo.ControlRoute[Loop].OutControl);
		xmlSetProp (node, "Midi", buff);
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

		for (PatchLoop = 0; PatchLoop < MaxLayoutPatches; PatchLoop++) {
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

int LayoutPatchIndex;
int MainPatchIndex;

/*--------------------------------------------------------------------
 * Function:		LoadXMLPair
 *
 * Description:		Take the data from the XML and put it into
 * 	the global data structure.
 *
 *---------------------------------------------------------------------*/
void LoadXMLPair(ParseData *theData) {
	char NodeType = 1;
	char *name;
	char *value;
	int ParseCountL2 = 0;
	int HoldIndex;

	/* Extract the basic information for the top level.
	*/
	name = theData[1].name;
	value = theData[2].value;

	/* Let's start by looking for the highest level qualifier.
	 */
	if (!strcmp ("MainButtons", name) && theData[2].name[0]) {
		sscanf (theData[2].name, "Preset%03d", &HoldIndex);

		if (!strcmp ("Name", theData[3].name)) {
#if DontIndexFromXML
			HoldIndex = MainPatchIndex;
			MainPatchIndex++;
#endif
			strcpy (gMyInfo.MyPatchInfo[HoldIndex].Name, theData[3].value);
		} else if (!strcmp ("Channel", theData[3].name))
			gMyInfo.MyPatchInfo[HoldIndex].Channel = atoi (theData[3].value);
		else if (!strcmp ("Controller", theData[3].name))
			gMyInfo.MyPatchInfo[HoldIndex].BankSelect = atoi (theData[3].value);
		else if (!strcmp ("OutPort", theData[3].name))
			gMyInfo.MyPatchInfo[HoldIndex].OutPort = atoi (theData[3].value);
		else if (!strcmp ("Patch", theData[3].name))
			gMyInfo.MyPatchInfo[HoldIndex].Patch = atoi (theData[3].value);
		else if (!strcmp ("Custom", theData[3].name))
			gMyInfo.MyPatchInfo[HoldIndex].CustomCommand = atoi (theData[3].value);
		else if (!strcmp ("Chain", theData[3].name)) {

			if (theData[3].value[0] == 0)
				strcpy (gMyInfo.MyPatchInfo[HoldIndex].Chain, "None");
			else
				strcpy (gMyInfo.MyPatchInfo[HoldIndex].Chain, theData[3].value);

			if (gMyInfo.MyPatchInfo[HoldIndex].Name[0] != 0)
				printd (LogDebug, "Patch %d %-25s %03d %03d %03d %03d %s\n",
				        HoldIndex,
				        gMyInfo.MyPatchInfo[HoldIndex].Name,
				        gMyInfo.MyPatchInfo[HoldIndex].Channel,
				        gMyInfo.MyPatchInfo[HoldIndex].BankSelect,
				        gMyInfo.MyPatchInfo[HoldIndex].Patch,
				        gMyInfo.MyPatchInfo[HoldIndex].CustomCommand,
				        gMyInfo.MyPatchInfo[HoldIndex].Chain);
		}
	}

	if (!strcmp ("NumOutPorts", name)) {
		gMyInfo.NumOutPorts = atoi (value);
		printd (LogDebug, "\n***Numout Ports %d\n", gMyInfo.NumOutPorts);
	}

	if (!strcmp ("NumberOfStrings", name)) {
		gMyInfo.NumberOfStrings = atoi (value);
		printd (LogDebug, "\n***Number of Strings %d\n", gMyInfo.NumberOfStrings);
	}

	if (!strcmp ("Strings", name)  && theData[2].name[0]) {
//			TopLevelParse = dTopStrings;
		sscanf (theData[2].name, "StrNum%03d", &HoldIndex);
		gMyInfo.BaseStringName[HoldIndex] = atoi(theData[3].value);
		printd (LogDebug, "dTopStrings %d %d\n",
		        HoldIndex,
		        gMyInfo.BaseStringName[HoldIndex]);
	}

	if (!strcmp ("OutPorts", name) && theData[2].name[0]) {
		sscanf (theData[2].name, "Port%03d", &HoldIndex);
		strcpy (gMyInfo.OutPortName[HoldIndex], theData[3].value);
		printd (LogDebug, "dTopLevelOutPorts %d %s\n", HoldIndex,
		        gMyInfo.OutPortName[HoldIndex]);
	}

	if (!strcmp ("SongPath", name)) {
		printd (LogDebug, "Song Path %s\n", value);
		strncpy (gMyInfo.BasePath, value, 255);
		printd (LogDebug, "Song Path1 %s\n", value);
	}

	if (!strcmp ("AppNames", name)) {
//			TopLevelParse = dTopLevelAppName;
		sscanf (theData[2].name, "App%03d", &HoldIndex);

		if (!strcmp ("Name", theData[3].name))
			strcpy (gMyInfo.Apps[HoldIndex].Name, theData[3].value);
		else if (!strcmp ("PortID", theData[3].name)) {
			gMyInfo.Apps[HoldIndex].PortID = atoi (theData[3].value);

			printd (LogDebug, "dTopLevelAppName %d %s %d\n",
			        HoldIndex,
			        gMyInfo.Apps[HoldIndex].Name,
			        gMyInfo.Apps[HoldIndex].PortID);
		}

	}

	if (!strcmp ("ControllerMap", name)  && theData[2].name[0]) {
//			TopLevelParse = dTopLevelControllerMap;
		sscanf (theData[2].name, "App%03d", &HoldIndex);

		if (!strcmp ("Name", theData[3].name))
			strcpy (gMyInfo.ControlRoute[HoldIndex].OutPort, theData[3].value);
		else if (!strcmp ("PortID", theData[3].name)) {
			gMyInfo.ControlRoute[HoldIndex].OutControl = atoi (theData[3].value);

			printd (LogDebug, "dTopLevelControllerMap %d %s %s\n",
			        HoldIndex,
			        gMyInfo.Apps[HoldIndex].Name,
			        gMyInfo.Apps[HoldIndex].PortID);

		}
	}

	if (!strcmp ("TempoMax", name)) {
		printd (LogDebug, "dTopLevelMetronome %s\n", value);
		gMyInfo.TempoMax = atoi (value);
	}

	if (!strcmp ("MetroOn", name)) {
		printd (LogDebug, "dTopLevelMetronomeOn %s\n", value);
		gMyInfo.MetronomeOn = atoi (value);
	}

	if (!strcmp ("MidiBaseNote", name)) {
		printd (LogDebug, "dTopLevelMidiBase %s\n", value);
		gMyInfo.MidiBaseNote = atoi (value);
	}

	if (!strcmp ("Click1", name)) {
		printd (LogDebug, "dTopLevelClick1 %s\n", value);
		gMyInfo.Drum1 = atoi (value);
	}

	if (!strcmp ("ClickRest", name)) {
		printd (LogDebug, "dTopLevelClickRest %s\n", value);
		gMyInfo.DrumRest = atoi (value);
	}

	if (!strcmp ("Layouts", name)  && theData[2].name[0]) {
		sscanf (theData[2].name, "Layout%03d", &HoldIndex);

		if (!strcmp ("LayoutName", theData[3].name)) {
			strcpy (gMyInfo.LayoutPresets[HoldIndex].Name, theData[3].value);
			theData[3].value[0] = 0;
			LayoutPatchIndex = 0;
		} else if (!strcmp ("PatchName", theData[4].name) && theData[2].name[0]) {
			sscanf (theData[3].name, "Patch%03d", &ParseCountL2);

#if DontIndexFromXML
// printf("LayoutIndex %d %d %d\n",ParseCountL2,LayoutPatchIndex/2,LayoutPatchIndex);
// printf("LayoutIndex2 %s %s\n",theData[2].name,theData[2].value);
// printf("LayoutIndex3 %s %s\n",theData[3].name,theData[3].value);
// printf("LayoutIndex4 %s %s\n",theData[4].name,theData[4].value);

			ParseCountL2 = LayoutPatchIndex / 2;
			LayoutPatchIndex++;
#endif

			if (theData[4].value[0] != 0) {
				/* Check for valid Range.
				*/
				if (ParseCountL2 >= MaxLayoutPatches)
					ParseCountL2 = MaxLayoutPatches - 1;

				strcpy (gMyInfo.
				        LayoutPresets[HoldIndex].Presets[ParseCountL2],
				        theData[4].value);

				theData[4].value[0] = 0;

				if (gMyInfo.LayoutPresets[HoldIndex].Presets[ParseCountL2][0] != 0)
					printd (LogInfo, "dTopLevelLayouts %d %d [%s] [%s]\n",
					        HoldIndex, ParseCountL2,
					        gMyInfo.LayoutPresets[HoldIndex].Name,
					        gMyInfo.LayoutPresets[HoldIndex].Presets[ParseCountL2]
					       );
			}
		}
	}

	if (!strcmp ("Tempo", name)) {
		gMyInfo.Tempo = atoi (theData[2].value);
		printd (LogDebug, "dTopLevelTempo %d\n", gMyInfo.Tempo);
	}

	if (!strcmp ("MidiPassThru", name)) {
		printd (LogDebug, "dTopMidiPassThru %s\n", value);
		gMyInfo.MidiPassThru = atoi (value);
	}

	if (!strcmp ("MidiPassLevel", name)) {
		printd (LogDebug, "dTopMidiPassLevel %s\n", value);
		gMyInfo.MidiPassLevel = atoi (value);
	}

	if (!strcmp ("CountInBeats", name)) {
		printd (LogDebug, "dTopLevelCountInBeats %s\n", value);
		gMyInfo.CountInBeats = atoi (value);
	}

	if (!strcmp ("LoopRecBeats", name)) {
		printd (LogDebug, "dTopLevelLoopRecBeats %s\n", value);
		gMyInfo.LoopRecBeats = atoi (value);
	}
	if (!strcmp ("BeatsPerMeasure", name)) {
		printd (LogDebug, "dTopLevelBeatsPerMeasure %s\n", value);
		gMyInfo.BeatsPerMeasure = atoi (value);
	}

	if (!strcmp ("AnalogVolume", name)) {
		printd (LogDebug, "dTopLevelAnalogVolume %s\n", value);
		gMyInfo.AnalogVolume = atoi (value);
	}

	if (!strcmp ("MidiVolume", name)) {
		printd (LogDebug, "dTopLevelMidiVolume %s\n", value);
		gMyInfo.MidiVolume = atoi (value);
	}

	if (!strcmp ("StatusTextColor", name)) {
		printd (LogDebug, "dTopLevelStatusTextColor %s\n", value);
		sscanf(value, "%x", &gMyInfo.StatusTextColor);
	}

	if (!strcmp ("ButtonTextColor", name)) {
		printd (LogDebug, "dTopLevelButtonTextColor %s\n", value);
		sscanf(value, "%x", &gMyInfo.ButtonTextColor);
	}

	if (!strcmp ("OSCIPAddress", name)) {
		printd (LogDebug, "dTopLevelOSCIPAddress %s\n", value);
		strncpy (gMyInfo.OSCIPAddress, value, sizeof (gMyInfo.OSCIPAddress));
	}

	if (!strcmp ("OSCPortNumLooper", name)) {
		printd (LogDebug, "dTopLevelOSCPortNumLooper %s\n", value);
		strncpy (gMyInfo.OSCPortNumLooper, value, sizeof (gMyInfo.OSCPortNumLooper));
	}

	if (!strcmp ("OSCPortNumJackVol", name)) {
		printd (LogDebug, "dTopLevelOSCPortNumJackVol %s\n", value);
		strncpy (gMyInfo.OSCPortNumJackVol, value, sizeof (gMyInfo.OSCPortNumJackVol));
	}

	if (!strcmp ("OSCPortNumHydrogen", name)) {
		printd (LogDebug, "dTopLevelOSCPortNumHydrogen %s\n", value);
		strncpy (gMyInfo.OSCPortNumHydrogen, value, sizeof (gMyInfo.OSCPortNumHydrogen));
	}

}

/*--------------------------------------------------------------------
 * Function:		LoadXMLData
 *
 * Description:		Debug to print values as they are parsed XML.
 *
 *---------------------------------------------------------------------*/
int LoadXMLData(ParseData *theData) {
	int Level = 0;

#if 0
	while (theData[++Level].Valid) {
		if (theData[Level].name[0] != '\n' &&
		        theData[Level].name[0] != '#')
			printf("%d[%s],", Level,
			       theData[Level].name);

		if (theData[Level].value[0] != '\n' &&
		        theData[Level].value[0] != '#')
			printf("(%s),",
			       theData[Level].value);
	}

	printf("\n");
#endif
	LoadXMLPair(theData);

}

/*--------------------------------------------------------------------
 * Function:		processNode
 *
 * Description:		Read the next node of XML and copy the info to the
 * 	ParseData structure.
 *
 *---------------------------------------------------------------------*/
static int
processNode(xmlTextReaderPtr reader, ParseData *theData) {
	const xmlChar *name, *value;
	int XMLName;
	int XMLType;

	name = xmlTextReaderConstName(reader);

//    if (xmlTextReaderNodeType(reader) >10)
//        return(0);

	if (name)
		strcpy(theData->name, name);

	theData->Level = xmlTextReaderDepth(reader);
	theData->Type = xmlTextReaderNodeType(reader);
	value = xmlTextReaderConstValue(reader);

	if (value)
		strcpy(theData->value, value);

	if (theData->Type < MaxLevels)
		theData->Valid = 1;

	return (0);
}

static int
streamFile(const char *filename) {
	xmlTextReaderPtr reader;
	int ret;
	ParseData theData1, theData2;
	ParseData myData[MaxLevels];

	/*
	 * Pass some special parsing options to activate DTD attribute defaulting,
	 * entities substitution and DTD validation
	 */
	reader = xmlReaderForFile(filename, NULL,
	                          XML_PARSE_DTDATTR |  /* default DTD attributes */
	                          XML_PARSE_NOENT |    /* substitute entities */
	                          XML_PARSE_NOWARNING |
	                          XML_PARSE_PEDANTIC |
	                          XML_PARSE_NOERROR
	                          // XML_PARSE_DTDVALID /* validate with the DTD */
	                         );

	if (reader != NULL) {
		ret = xmlTextReaderRead(reader);

		/* Walk thru the preference file.
		*/
		while (ret == 1) {

			processNode(reader, &theData1);
			if (theData1.Level == 1 && theData1.Type == 1) {
				memset(myData, 0, sizeof (myData));
//				myData[theData1.Level] = theData1;
			}

#if 1
			if (theData1.Level < MaxLevels ) {
				myData[theData1.Level] = theData1;
				// printd(LogDebug, "theData1.Level < MaxLevels\n");
				//printf("StreamFile 30\n");
				if (theData1.Type < MaxLevels)
					LoadXMLData(myData);

			}
#else
			if (theData1.Level == 2 && theData1.Type == 1) {
				myData[theData1.Level] = theData1;
			}

			if (theData1.Level == 2 && theData1.Type == 3) {
				myData[theData1.Level] = theData1;
				LoadXMLData(myData);
			}

			if (theData1.Level == 3 && theData1.Type == 1) {
				myData[theData1.Level] = theData1;
//				LoadXMLData(myData);
			}
#endif
			while (xmlTextReaderMoveToNextAttribute (reader)) {
				processNode(reader, &theData1);
#if 1
				if (theData1.Level < MaxLevels ) {
					myData[theData1.Level] = theData1;
					if (theData1.Type < MaxLevels)
						LoadXMLData(myData);
				}

#else
				if (theData1.Level == 3 && theData1.Type == 2) {
					myData[theData1.Level] = theData1;
					LoadXMLData(myData);
				}

				if (theData1.Level == 4 && theData1.Type == 2) {
					myData[theData1.Level] = theData1;
					LoadXMLData(myData);
				}
#endif
			}
			ret = xmlTextReaderRead(reader);
		}
		/*
		 * Once the document has been fully parsed check the validation results
		 */
		// if (xmlTextReaderIsValid(reader) != 1) {
		// 	fprintf(stderr, "Document %s does not validate\n", filename);
		// }
		xmlFreeTextReader(reader);
		if (ret != 0) {
			printd(LogError, "%s : failed to parse\n", filename);
		return (1);
	
		}
	} else {
		printd(LogError, "Unable to open %s\n", filename);
		return (1);
	}

	return (ret);
}

/*--------------------------------------------------------------------
 * Function:		ReadPrefs
 *
 * Description:		Abstraction interface to XML
 *
 *---------------------------------------------------------------------*/
int ReadPrefs () {

	printd (LogDebug, "----------------------\n");
	printd (LogDebug, "Reading prefs file\n");
	printd (LogDebug, "----------------------\n");
	return (streamFile(GetResourceDir(PREFSFILENAME, FileLocConfig)));
}

