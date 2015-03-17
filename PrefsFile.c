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
#include "GTKMidiUI.h"
#include "PrefsFile.h"
#include <stdio.h>
#include <libxml/parser.h>
#include "xmltok/xmlparse.h"

// /usr/include/libxml2/
/*
 * Place defines and Typedefs here
 */

#define PREFSFILENAME "LiveMusic.xml"
#define Test2

/*
 * Place Local prototypes here
 */

void parseDoc(char *docname);
void WritePrefs(void);

/*
 * Place Static variables here
 */
/* @formatter:off */
GTKMidiInfo GlobalInfo =
	{
		{
			//Title			Bank	Patch 	Outport		Chan	Command Chain
			{ "Chorus", 0xff, 1, RakarrakPort, 1, NoCustom, "" },
			{ "SatrianiComp", 0xff, 12, RakarrakPort, 1, NoCustom, "" },
			{ "CleanBass", 0xff, 3, RakarrakPort, 1, NoCustom, "" },
			{ "MedDist", 0xff, 4, RakarrakPort, 1, NoCustom, "" },
			{ "OctBass", 0xff, 5, RakarrakPort, 1, NoCustom, "" },
			{ "FunkBass", 0xff, 9, RakarrakPort, 1, NoCustom, "" },
			{ "Satriani", 0xff, 16, RakarrakPort, 1, NoCustom, "" },
			{ "OldPat", 0xff, 13, RakarrakPort, 1, NoCustom, "" },
			{ "JazzRev", 0xff, 28, RakarrakPort, 1, NoCustom, "" },
			{ "LayoutSwitch", 0xff, 0, FluidPort, 1, cmdBankSelect, "" },
			//	{"Preset10",	"Change",		0xff,		0,		RakarrakPort,	2,	ToNextDesktop,0},
			// 10	Button		Title		Bank	Patch	Outport
			{ "Piano", SFDSF, 63, FluidPort, 1, NoCustom, "" },
			{ "Honky", SFFluidGM, 3, FluidPort, 1, NoCustom, "" },
			{ "Breath", SFFluidGM, 53, FluidPort, 1, NoCustom, "" },
			{ "Organ", SFFluidGM, 16, FluidPort, 1, NoCustom, "" },
			{ "Flute", SFFlute, 73, FluidPort, 1, NoCustom, "" },
			{ "SynthLead", SFDSF, 43, FluidPort, 1, NoCustom, "" },
			{ "Strings", SFDSF, 85, FluidPort, 1, NoCustom, "" },
			{ "Trumpet", SFDSF, 66, FluidPort, 1, NoCustom, "" },
			{ "Steel Drums", SFFluidGM, 114, FluidPort, 1, NoCustom, "" },
			{ "Walky", SFDSF, 3, FluidPort, 1, NoCustom, "" },
			// 20	Button		Title		Bank	Patch	Outport
//       { "Drums", 128, 26, FluidPort, 1, NoCustom, ""  },
			{ "Drums", SFDrumsElias, 1, FluidPort, 1, NoCustom, "" },
			{ "Slap Bass", SFDSF, 24, FluidPort, 1, NoCustom, "" },
			{ "Synth Hard", SFDSF, 60, FluidPort, 1, NoCustom, "" },
			{ "Marimba", SFDSF, 12, FluidPort, 1, NoCustom, "" },
			{ "Breath", SFDSF, 48, FluidPort, 1, NoCustom, "" },
			{ "Steel Drums", SFDSF, 114, FluidPort, 1, NoCustom, "" },
			{ "Low Synth", SFDSF, 45, FluidPort, 1, NoCustom, "" },
			{ "Mid0", 0xff, 0, 0, 1, NoCustom, "" },
			{ "Mid-24", 0xff, 2, 0, 1, NoCustom, "" },
			{ "Mid Screen", SFDSF, 0, FluidPort, 1, NoCustom, "" },
			// 30	Button		Title		Bank	Patch	Outport
			{ "Rak Eff6", 116, 11, FluidPort, 1, Controller, "" },
			{ "Rak Eff7", 116, 13, FluidPort, 1, Controller, "" },
			{ "Rak Eff8", 116, 15, FluidPort, 1, Controller, "" },
			{ "Rak Eff9", 116, 17, FluidPort, 1, Controller, "" },
			{ "Rak Eff10", 116, 18, FluidPort, 1, Controller, "" },
			{ "Rak Eff1", 116, 0, FluidPort, 1, Controller, "" },
			{ "Rak Eff2", 116, 2, FluidPort, 1, Controller, "" },
			{ "Rak Eff3", 116, 4, FluidPort, 1, Controller, "" },
			{ "Rak Eff4", 116, 6, FluidPort, 1, Controller, "" },
			{ "Rak Eff5", 116, 8, FluidPort, 1, Controller, "" },
			// 40	Button		Title		Bank	Patch	Outport
			{ "TransStart", 0xff, 0, TransportPort, 1, TransStart, "" },
			{ "TransCont", 0xff, 0, TransportPort, 1, TransCont, "" },
			{ "TransStop", 0xff, 0, TransportPort, 1, TransStop, "" },
			{ "TransBack", 0xff, 0, TransportPort, 1, TransStart, "" },
			{ "TransPos", 0xff, 0, TransportPort, 1, TransStart, "" },
			{ "Tap", 116, 121, FluidPort, 1, Controller, "" },
			{ "Midi Tog", 116, 122, FluidPort, 1, Controller, "" },
			{ "Tuner", 116, 123, FluidPort, 1, Controller, "" },
			{ "AnaOnOff", 116, 124, FluidPort, 1, Controller, "" },
			{ "AnaOnOff", 116, 124, FluidPort, 1, Controller, "" },
			// 50	Button		Title		Bank	Patch	Outport
			{ "LP Undo", 0xff, 69, LooperPort, 1, NoCustom, "" },
			{ "LP 1", 0xff, 70, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP 2", 0xff, 71, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP 3", 0xff, 72, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP 4", 0xff, 73, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP Rec", 0xff, 74, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP Pause", 0xff, 75, LooperPort, 1, NoCustom, "" },
			{ "LP Trig", 0xff, 76, LooperPort, 1, NoCustom, "" },
			{ "LP Tap", 0xff, 77, LooperPort, 1, NoCustom, "" },
			{ "LP SelAll", 0xff, 78, LooperPort, 1, NoCustom, "" },
			// 60	Button		Title		Bank	Patch	Outport
			{ "AnalogApp", 0xff, AnalogApp, 0, 1, RaiseApp, "" },
			{ "MidiSoundApp", 0xff, MidiSoundApp, 0, 1, RaiseApp, "" },
			{ "LooperApp", 0xff, LooperApp, 0, 1, RaiseApp, "" },
			{ "MidiApp", 0xff, LiveMusicApp, 0, 1, RaiseApp, "" },
			{ "MP3Player", 0xff, MP3Player, 0, 1, RaiseApp, "" },
			{ "TabPlayer", 0xff, TabPlayer, 0, 1, RaiseApp, "" },
			{ "PreTab", 0xff, 0, 0, 1, SwitchTab, "" },
			{ "ScoreTab", 0xff, 1, 0, 1, SwitchTab, "" },
			{ "TransportApp", 0xff, TransportApp, 0, 1, RaiseApp, "" },
			{ "NULL", 0xff, 0, 0, 1, RaiseApp, "" },
			// 70	Button		Title		Bank	Patch	Outport
			{ "Switch0", 0xff, 0, 0, 1, ToDesktop, "" },
			{ "Switch1", 0xff, 1, 0, 1, ToDesktop, "" },
			{ "Switch2", 0xff, 2, 0, 1, ToDesktop, "" },
			{ "Switch3", 0xff, 3, 0, 1, ToDesktop, "" },
			{ "Switch4", 0xff, 4, 0, 1, ToDesktop, "" },
			{ "Switch5", 0xff, 5, 0, 1, ToDesktop, "" },
			{ "VolUp", 0xff, 5, 0, 1, cmdVolume, "" },
			{ "VolDown", 0xff, -5, 0, 1, cmdVolume, "" },
			{ "RepeatA", 0xff, cmdLnTSetA, 0, 1, cmdLnTransPort, "" },
			{ "RepeatB", 0xff, cmdLnTSetB, 0, 1, cmdLnTransPort, "" },
			// 80	Button		Title		Bank	Patch	Outport
			{ "EBuzz", SFElias, 0, FluidPort, 1, NoCustom, "" },
			{ "SynthB11", SFElias, 1, FluidPort, 1, NoCustom, "" },
			{ "KStatB", SFElias, 2, FluidPort, 1, NoCustom, "" },
			{ "MS20Bass", SFElias, 3, FluidPort, 1, NoCustom, "" },
			{ "Juno", SFElias, 4, FluidPort, 1, NoCustom, "" },
			{ "Moog1", SFElias, 9, FluidPort, 1, NoCustom, "" },
			{ "Moog2", SFElias, 10, FluidPort, 1, NoCustom, "" },
			{ "BassSlap", SFElias, 13, FluidPort, 1, NoCustom, "" },
			{ "Taurus2", SFElias, 26, FluidPort, 1, NoCustom, "" },
			{ "Ultimate2a", SFElias, 33, FluidPort, 1, NoCustom, "" },
			// 90	Button		Title		Bank	Patch	Outport
			{ "SynthB1", SFElias, 38, FluidPort, 1, NoCustom, "" },
			{ "FilterBass", SFElias, 39, FluidPort, 1, NoCustom, "" },
			{ "BasBiy", SFElias, 45, FluidPort, 1, NoCustom, "" },
			{ "CZOrgan", SFElias, 51, FluidPort, 1, NoCustom, "" },
			{ "MoogLead1", SFElias, 53, FluidPort, 1, NoCustom, "" },
			{ "CZBrass4", SFElias, 64, FluidPort, 1, NoCustom, "" },
			{ "SinSymph", SFElias, 90, FluidPort, 1, NoCustom, "" },
			{ "Trig4Count", 0xff, 4, TransportPort, 1, cmdCountIn, "" },
			{ "NextSong", 0xff, 2, FluidPort, 1, cmdSetList, "" },
			{ "PrevSong", 0xff, 1, FluidPort, 1, cmdSetList, "" },
			// 100	Button		Title		Bank	Patch	Outport
			{ "Scroll", 0xff, 0, 0, 1, cmdScroll, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },

			// 110	Button		Title		Bank	Patch	Outport
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },

			// 120	Button		Title		Bank	Patch	Outport
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "" },

			// 130	Button		Title		Bank	Patch	Outport
			{ "Preset1", 0xff, 1, 0, 1, cmdPreset, "" },
			{ "Preset2", 0xff, 2, 0, 1, cmdPreset, "" },
			{ "Preset3", 0xff, 3, 0, 1, cmdPreset, "" },
			{ "Preset4", 0xff, 4, 0, 1, cmdPreset, "" },
			{ "Preset5", 0xff, 5, 0, 1, cmdPreset, "" },
			{ "Preset6", 0xff, 6, 0, 1, cmdPreset, "" },
			{ "Midi Pre", 0xff, FluidPort, 0, 1, cmdMidiSelect, "" },
			{ "Slide1", 0xff, MIDI_CTL_MSB_MAIN_VOLUME, RakarrakPort, 1,
				NoCustom, "" },
			{ "Slide2", 0xff, MIDI_CTL_MSB_MAIN_VOLUME, FluidPort, 1, NoCustom,
				"" },
			{ "Slide3", 0xff, MIDI_CTL_MSB_MAIN_VOLUME, FluidPort, 2, NoCustom,
				"" },

		},
		// Path the Charts directory
		"/home/Dropbox/FusionBlue/index.html",
		// Number and name of the output ports
		5,
		{ "Fluid", "Rakarrak", "Looper", "Transport", "Tempo", "Click" },

		// Apps[MaxApps];
		// enum { AnalogApp = 0, MidiSoundApp, LooperApp, TransportApp, MidiApp, MP3Player, TabPlayer, MaxApps };
		{
			{ "rakarrack", 0 },
			{ "qsynth", 0 },
			{ "SooperLooper", 0 },
			{ "audacity", 0 },
			{ "LiveMusicApp", 0 },
			{ "clementine", 0 },
			{ "Tux", 0 }
		},
		8, /* TempoMax	*/
		1, /* Metronome On	*/
		37, /* Drum down beat, midi code.*/
		44, /* Drum Other beats */
		40, /* Base Midi Note for switching.	*/

		/* Begin Layouts Section.	*/
		{
		{ "Default",
			{
	/*			1			2			3			4			5			6 		*/
				"Preset1", 		"Preset2", 		"Preset3", 		"Preset4",	 	"VolUp",		"VolDown",
				"Chorus", 		"SatrianiComp", 	"MS20Bass", 	"Flute", 		"CleanBass",	"Breath",
				"PreTab", 		 "ScoreTab", 	"OctBass",		"Strings", 		"Trumpet",		"Steel Drums",
				"Scroll", 		"Drums", 		"Slap Bass", 	"NextSong",	"PrevSong", 	"LayoutSwitch",

				"Piano", 		"Honky", 		"Breath", 		"Midi Pre", 	"Flute",
				"Drums", 		"Slap Bass",  	"Marimba", 	"Breath",  		"Steel Drums",
				"Low Synth",	"EBuzz", 		"SynthB11", 	"KStatB", 		"MS20Bass",
				"Juno", 		"Moog1",		 "Moog2",		"BassSlap", 	 "Taurus2",
				"Ultimate2a", 	"SynthB1", 		"FilterBass",	"CZOrgan", 	"MoogLead1",
				"CZBrass4", 	"SinSymph",	"Rak Eff1", 		"Rak Eff2", 		"Rak Eff3",

			},
		},

		{ "Practice",
			{
	/*			1			2			3			4			5			6 		*/
				"Preset1", 		"Preset2", 		"Preset3",		"Chorus", 		"SatrianiComp", 	"CleanBass",
				"TransStart", 	"TransCont", 	"TransStop", 	"TransBack",	"MidiApp",  	"MP3Player",
				"LP 1", 		"LP 2", 		"LP 3", 		"LP Undo",		"LP Rec", 		"LP Pause",
				"Drums",		"RepeatA", 		"RepeatB", 		"MedDist", 		"OctBass", 		"LayoutSwitch",


				"Piano", 		"Honky", 		"Breath", 		"Midi Pre", 	"Flute",
				"Drums", 		"Slap Bass",  	"Marimba", 	"Breath",  		"Steel Drums",
				"Low Synth",	"EBuzz", 		"SynthB11", 	"KStatB", 		"MS20Bass",
				"Juno", 		"Moog1",		 "Moog2",		"BassSlap", 	 "Taurus2",
				"Ultimate2a", 	"SynthB1", 		"FilterBass",	"CZOrgan", 	"MoogLead1",
				"CZBrass4", 	"SinSymph",	"Rak Eff1", 	"Rak Eff2", 	"Rak Eff3",
			},
		},

		{ "Rehearsal",
			{
				"TransStart", "TransCont", "TransStop", "TransBack", "TransPos",
				"Tap", "Midi Tog", "Tuner", "AnaOnOff", "AnaOnOff",
				"LP Undo", "LP 1", "LP 2", "LP 3", "LP 4",
				"LP Rec", "LP Pause", "LP Trig", "LP Tap", "LP SelAll",
				"Chorus", "HardDist", "CleanBass", "MedDist", "OctBass",
				"LayoutSwitch", "Satriani", "Preset1", "Preset2", "LayoutSwitch",
				"Piano", "Honky", "Breath", "Organ", "Flute",
				"SynthLead", "Strings", "Trumpet", "Steel Drums", "Walky",
				"Drums", "Slap Bass", "Synth Hard", "Marimba", "Breath",
				"Steel Drums", "Low Synth", "Mid0", "Mid-24", "Mid Screen",
				"Rak Eff6", "Rak Eff7", "Rak Eff8", "Rak Eff9", "Rak Eff10",
				"Rak Eff1", "Rak Eff2", "Rak Eff3", "Rak Eff4", "Rak Eff5",
			},
		},

		{ "Looper",
			{
	/*			1			2			3			4			5			6 		*/
				"Preset1", 		"Preset2", 		"Preset3",		"Chorus", 		"SatrianiComp", 	"CleanBass",
				"TransStart", 	"TransCont", 	"TransStop", 	"TransBack",	"MidiApp",  	"MP3Player",
				"LP 1", 		"LP 2", 		"LP 3", 		"LP Undo",		"LP Rec", 		"LP Pause",
				"Drums",		"Flute", 		"Satriani", 		"MedDist", 		"OctBass", 		"LayoutSwitch",

				"Piano", 		"Honky", 		"Breath", 		"Midi Pre", 	"Flute",
				"Drums", 		"Slap Bass",  	"Marimba", 	"Breath",  		"Steel Drums",
				"Low Synth",	"EBuzz", 		"SynthB11", 	"KStatB", 		"MS20Bass",
				"Juno", 		"Moog1",		 "Moog2",		"BassSlap", 	 "Taurus2",
				"Ultimate2a", 	"SynthB1", 		"FilterBass",	"CZOrgan", 	"MoogLead1",
				"CZBrass4", 	"SinSymph",	"Rak Eff1", 		"Rak Eff2", 		"Rak Eff3",
	//			"Rak Eff6", "Rak Eff7", "Rak Eff8", "Rak Eff9", "Rak Eff10",
	//			"Rak Eff1", "Rak Eff2", "Rak Eff3", "Rak Eff4", "Rak Eff5",
			},
		},

		{ "Bass",
			{
	/*			1			2			3			4			5			6 		*/
				"Preset1", 		"Preset2", 		"Preset3",		"Chorus", 		"OctBass", 		"CleanBass",
				"FunkBass", 	"Slap Bass", 	"Breath", 		"Flute",		"MS20Bass",  	"SatrianiComp",
				"LP 1", 		"LP 2", 		"LP 3", 		"LP Undo",		"LP Rec", 		"LP Pause",
				"Drums",		"Flute", 		"Satriani", 		"MedDist", 		"OctBass", 		"LayoutSwitch",

				"Piano", 		"Honky", 		"Breath", 		"Midi Pre", 	"Flute",
				"Drums", 		"Slap Bass",  	"Marimba", 	"Breath",  		"Steel Drums",
				"Low Synth",	"EBuzz", 		"SynthB11", 	"KStatB", 		"MS20Bass",
				"Juno", 		"Moog1",		 "Moog2",		"BassSlap", 	 "Taurus2",
				"Ultimate2a", 	"SynthB1", 		"FilterBass",	"CZOrgan", 	"MoogLead1",
				"CZBrass4", 	"SinSymph",	"Rak Eff1", 		"Rak Eff2", 		"Rak Eff3",
			},
		},

		/*
		 * Terminator for Layouts.
		 */
		{ "",
			{
			},
		},

		},
	};
#if 0
// @formatter:off
LayoutType LayoutPresets[] =
	{

	{ "Default",
		{
/*			1			2			3			4			5			6 		*/
			"Preset1", 		"Preset2", 		"Preset3", 		"Preset4",	 	"VolUp",		"VolDown",
			"Chorus", 		"SatrianiComp", 	"MS20Bass", 	"Flute", 		"CleanBass",	"Breath",
			"PreTab", 		 "ScoreTab", 	"OctBass",		"Strings", 		"Trumpet",		"Steel Drums",
			"Scroll", 		"Drums", 		"Slap Bass", 	"NextSong",	"PrevSong", 	"LayoutSwitch",

			"Piano", 		"Honky", 		"Breath", 		"Midi Pre", 	"Flute",
			"Drums", 		"Slap Bass",  	"Marimba", 	"Breath",  		"Steel Drums",
			"Low Synth",	"EBuzz", 		"SynthB11", 	"KStatB", 		"MS20Bass",
			"Juno", 		"Moog1",		 "Moog2",		"BassSlap", 	 "Taurus2",
			"Ultimate2a", 	"SynthB1", 		"FilterBass",	"CZOrgan", 	"MoogLead1",
			"CZBrass4", 	"SinSymph",	"Rak Eff1", 		"Rak Eff2", 		"Rak Eff3",

		},
	},

	{ "Practice",
		{
/*			1			2			3			4			5			6 		*/
			"Preset1", 		"Preset2", 		"Preset3",		"Chorus", 		"SatrianiComp", 	"CleanBass",
			"TransStart", 	"TransCont", 	"TransStop", 	"TransBack",	"MidiApp",  	"MP3Player",
			"LP 1", 		"LP 2", 		"LP 3", 		"LP Undo",		"LP Rec", 		"LP Pause",
			"Drums",		"RepeatA", 		"RepeatB", 		"MedDist", 		"OctBass", 		"LayoutSwitch",


			"Piano", 		"Honky", 		"Breath", 		"Midi Pre", 	"Flute",
			"Drums", 		"Slap Bass",  	"Marimba", 	"Breath",  		"Steel Drums",
			"Low Synth",	"EBuzz", 		"SynthB11", 	"KStatB", 		"MS20Bass",
			"Juno", 		"Moog1",		 "Moog2",		"BassSlap", 	 "Taurus2",
			"Ultimate2a", 	"SynthB1", 		"FilterBass",	"CZOrgan", 	"MoogLead1",
			"CZBrass4", 	"SinSymph",	"Rak Eff1", 	"Rak Eff2", 	"Rak Eff3",
		},
	},

	{ "Rehearsal",
		{
			"TransStart", "TransCont", "TransStop", "TransBack", "TransPos",
			"Tap", "Midi Tog", "Tuner", "AnaOnOff", "AnaOnOff",
			"LP Undo", "LP 1", "LP 2", "LP 3", "LP 4",
			"LP Rec", "LP Pause", "LP Trig", "LP Tap", "LP SelAll",
			"Chorus", "HardDist", "CleanBass", "MedDist", "OctBass",
			"LayoutSwitch", "Satriani", "Preset1", "Preset2", "LayoutSwitch",
			"Piano", "Honky", "Breath", "Organ", "Flute",
			"SynthLead", "Strings", "Trumpet", "Steel Drums", "Walky",
			"Drums", "Slap Bass", "Synth Hard", "Marimba", "Breath",
			"Steel Drums", "Low Synth", "Mid0", "Mid-24", "Mid Screen",
			"Rak Eff6", "Rak Eff7", "Rak Eff8", "Rak Eff9", "Rak Eff10",
			"Rak Eff1", "Rak Eff2", "Rak Eff3", "Rak Eff4", "Rak Eff5",
		},
	},

	{ "Looper",
		{
/*			1			2			3			4			5			6 		*/
			"Preset1", 		"Preset2", 		"Preset3",		"Chorus", 		"SatrianiComp", 	"CleanBass",
			"TransStart", 	"TransCont", 	"TransStop", 	"TransBack",	"MidiApp",  	"MP3Player",
			"LP 1", 		"LP 2", 		"LP 3", 		"LP Undo",		"LP Rec", 		"LP Pause",
			"Drums",		"Flute", 		"Satriani", 		"MedDist", 		"OctBass", 		"LayoutSwitch",

			"Piano", 		"Honky", 		"Breath", 		"Midi Pre", 	"Flute",
			"Drums", 		"Slap Bass",  	"Marimba", 	"Breath",  		"Steel Drums",
			"Low Synth",	"EBuzz", 		"SynthB11", 	"KStatB", 		"MS20Bass",
			"Juno", 		"Moog1",		 "Moog2",		"BassSlap", 	 "Taurus2",
			"Ultimate2a", 	"SynthB1", 		"FilterBass",	"CZOrgan", 	"MoogLead1",
			"CZBrass4", 	"SinSymph",	"Rak Eff1", 		"Rak Eff2", 		"Rak Eff3",
//			"Rak Eff6", "Rak Eff7", "Rak Eff8", "Rak Eff9", "Rak Eff10",
//			"Rak Eff1", "Rak Eff2", "Rak Eff3", "Rak Eff4", "Rak Eff5",
		},
	},

	{ "Bass",
		{
/*			1			2			3			4			5			6 		*/
			"Preset1", 		"Preset2", 		"Preset3",		"Chorus", 		"OctBass", 		"CleanBass",
			"FunkBass", 	"Slap Bass", 	"Breath", 		"Flute",		"MS20Bass",  	"SatrianiComp",
			"LP 1", 		"LP 2", 		"LP 3", 		"LP Undo",		"LP Rec", 		"LP Pause",
			"Drums",		"Flute", 		"Satriani", 		"MedDist", 		"OctBass", 		"LayoutSwitch",

			"Piano", 		"Honky", 		"Breath", 		"Midi Pre", 	"Flute",
			"Drums", 		"Slap Bass",  	"Marimba", 	"Breath",  		"Steel Drums",
			"Low Synth",	"EBuzz", 		"SynthB11", 	"KStatB", 		"MS20Bass",
			"Juno", 		"Moog1",		 "Moog2",		"BassSlap", 	 "Taurus2",
			"Ultimate2a", 	"SynthB1", 		"FilterBass",	"CZOrgan", 	"MoogLead1",
			"CZBrass4", 	"SinSymph",	"Rak Eff1", 		"Rak Eff2", 		"Rak Eff3",
		},
	},

	/*
	 * Terminator for Layouts.
	 */
	{ "",
		{
		},
	},

	};
#endif
// START-ECLIPSE-FORMATTING

/*--------------------------------------------------------------------
 * Function:		InitPref
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void InitPref(void) {
	int Count;
#if 1
	gMyInfo.TempoMax = 8;

	memset(&gMyInfo, 0, sizeof(GTKMidiInfo));
//	memcpy(&gMyInfo, &GlobalInfo, sizeof(GTKMidiInfo));
	for (Count = 0; Count < MaxPresetButtons; Count++)
		gMyInfo.WebPresets.thePreset[Count] = -1;

	gMyInfo.MidiBaseNote = 40;
//	WritePrefs();
	ReadPrefs();
#endif
	gMyInfo.AnalogVolume = 80;
	gMyInfo.MidiVolume = 80;
	gMyInfo.StatusTextColor = 0xffffe0;
	gMyInfo.ButtonTextColor = 0xffe0e0;
//    ReadPrefs();
//    PrintDataStructure(&gMyInfo);
//exit(1);
//	printd(LogInfo, "Prefs %s %s\n", GlobalInfo.Apps[2].Name, &gMyInfo.Apps[2].Name);
//	WritePrefs();
}

/*--------------------------------------------------------------------
 * Function:		PrintDataStructure
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void PrintDataStructure(GTKMidiInfo *myInfo) {
	int Loop;
	int Loop1;
	PortsInfo *thePorts;

	printf("Main Information\n");

	for (Loop = 0; Loop < Max_Patches; Loop++) {
		printf("ID=%d %s %d %d %d %d %s\n", Loop,
			myInfo->MyPatchInfo[Loop].Name,
			myInfo->MyPatchInfo[Loop].Patch,
			myInfo->MyPatchInfo[Loop].OutPort,
			myInfo->MyPatchInfo[Loop].Channel,
			myInfo->MyPatchInfo[Loop].CustomCommand,
			myInfo->MyPatchInfo[Loop].Chain);
	}

	printf("BaseName %s\n", myInfo->BasePath);

	printf("Num Ports %d\n", myInfo->NumOutPorts);
	for (Loop = 0; Loop < myInfo->NumOutPorts; Loop++) {
		printf("Ports %s\n", myInfo->OutPortName[Loop]);
	}

	for (Loop = 0; Loop < MaxApps; Loop++) {
		printf("Apps %s %d\n", myInfo->Apps[Loop].Name,
			myInfo->Apps[Loop].PortID);
	}

	thePorts = &theInPorts;
	for (Loop = 0; Loop < thePorts->NumDevices; Loop++) {
		printf("In Name [%s]\n", thePorts->Devices[Loop].Name);
		for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
			printf("In SName %d, [%s]\n", Loop1,
				thePorts->Devices[Loop].Ports[Loop1].Name);
		}
	}

	thePorts = &theOutPorts;
	for (Loop = 0; Loop < thePorts->NumDevices; Loop++) {
		printf("Out Name [%s]\n", thePorts->Devices[Loop].Name);
		for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
			printf("Out SName %d, [%s]\n", Loop1,
				thePorts->Devices[Loop].Ports[Loop1].Name);
		}
	}

	for (Loop = 0; gMyInfo.LayoutPresets[Loop].Name[0] != 0; Loop++) {
		printf("Lay=%d %s %d %d %d\n", Loop,
			gMyInfo.LayoutPresets[Loop].Name,
			gMyInfo.LayoutPresets[Loop].Presets[0],
			gMyInfo.LayoutPresets[Loop].Presets[1],
			gMyInfo.LayoutPresets[Loop].Presets[2]);
	}

	printf("Metronome Base %d On %d %d %d\n", myInfo->TempoMax,
		myInfo->MetronomeOn, myInfo->Drum1, myInfo->DrumRest);
	printf("Midi Base Note %d\n", myInfo->MidiBaseNote);

}

/*--------------------------------------------------------------------
 * Function:		WritePref
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void WritePrefs(void) {
	xmlDocPtr doc = NULL; /* document pointer */
	xmlNodePtr root_node = NULL, node = NULL,
		node0 = NULL, node1 = NULL, node2 = NULL; /* node pointers */
	xmlDtdPtr dtd = NULL; /* DTD pointer */
	char buff[256];
	int Loop;
	int PatchLoop;;

	LIBXML_TEST_VERSION
	;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST XML_BASIC_SETTINGS);
	xmlDocSetRootElement(doc, root_node);

	xmlNewChild(root_node, NULL, BAD_CAST "SongPath",
	BAD_CAST gMyInfo.BasePath);

	/*
	 * Write out the Buttons
	 */
	node1 = xmlNewChild(root_node, NULL, BAD_CAST "MainButtons", NULL);
	for (Loop = 0; Loop < Max_Patches; Loop++) {
//		doc->children = xmlNewDocNode(doc, NULL, gMyInfo.MyPatchInfo[Loop].Button, NULL);
		sprintf(buff, "Preset%03d", Loop);
		node = xmlNewChild(node1, NULL, buff, NULL);
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
		sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].Chain);
		xmlSetProp(node, "Chain", buff);
	}

	sprintf(buff, "%03d", gMyInfo.NumOutPorts);
	xmlNewChild(root_node, NULL, BAD_CAST "NumOutPorts", BAD_CAST buff);

	/*
	 * Write out the Output Ports.
	 */
	node1 = xmlNewChild(root_node, NULL, BAD_CAST "OutPorts", NULL);
	for (Loop = 0; Loop < MaxOutPorts; Loop++) {
		sprintf(buff, "Port%03d", Loop);
		node = xmlNewChild(node1, NULL, buff, NULL);
		xmlSetProp(node, "Name", gMyInfo.OutPortName[Loop]);
	}

	node1 = xmlNewChild(root_node, NULL, BAD_CAST "AppNames", NULL);
	for (Loop = 0; Loop < MaxApps; Loop++) {
		sprintf(buff, "App%03d", Loop);
		node = xmlNewChild(node1, NULL, buff, NULL);
		xmlSetProp(node, "Name", gMyInfo.Apps[Loop].Name);
		sprintf(buff, "%03d", gMyInfo.Apps[Loop].PortID);
		xmlSetProp(node, "PortID", buff);
	}

	sprintf(buff, "%03d", gMyInfo.TempoMax);
	xmlNewChild(root_node, NULL, BAD_CAST "TempoMax", BAD_CAST buff);

	sprintf(buff, "%03d", gMyInfo.MetronomeOn);
	xmlNewChild(root_node, NULL, BAD_CAST "MetroOn", BAD_CAST buff);

	sprintf(buff, "%03d", gMyInfo.MidiBaseNote);
	xmlNewChild(root_node, NULL, BAD_CAST "MidiBaseNote", BAD_CAST buff);

	sprintf(buff, "%03d", gMyInfo.Drum1);
	xmlNewChild(root_node, NULL, BAD_CAST "Click1", BAD_CAST buff);

	sprintf(buff, "%03d", gMyInfo.DrumRest);
	xmlNewChild(root_node, NULL, BAD_CAST "ClickRest", BAD_CAST buff);

	/*
	 * Dump the layouts to the XML File.
	 */
	Loop = 0;
	node = xmlNewChild(root_node, NULL, BAD_CAST "Layouts", NULL);
	printf("Starting %x %s\n",node, gMyInfo.LayoutPresets[Loop].Name);
	while(gMyInfo.LayoutPresets[Loop].Name[0]) {
		sprintf(buff, "Layout%03d", Loop);
		node1 = xmlNewChild(node, NULL, buff, NULL);
//		printf("Loop %d %x %s\n",Loop,node1, LayoutPresets[Loop].Name);

		xmlSetProp(node1, "LayoutName", gMyInfo.LayoutPresets[Loop].Name);
//		printf("xmlSetProp %d %x %s\n",Loop,node1, LayoutPresets[Loop].Name);

//		node1 = xmlNewChild(node, NULL, BAD_CAST LayoutPresets[Loop].Name, NULL);
		for (PatchLoop = 0; PatchLoop< Max_Patches ;PatchLoop++) {
			sprintf(buff, "Patch%03d", PatchLoop);
//			printf("Patchnames %s\n",buff);
			node2 = xmlNewChild(node1, NULL, buff, NULL);
			xmlSetProp(node2, "PatchName", gMyInfo.LayoutPresets[Loop].Presets[PatchLoop]);
//			node2= xmlNewChild(node1, NULL, BAD_CAST LayoutPresets[Loop].Presets[PatchLoop], NULL);
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
	xmlSaveFormatFileEnc(PREFSFILENAME, doc, "UTF-8", 1);

	xmlFreeDoc(doc);

	xmlCleanupParser();
}

/*--------------------------------------------------------------------
 * Function:		getReference
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void getReference(xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *key;
	xmlNodePtr node = NULL;
	printd(LogInfo, "In Get Reference\n");
	int Loop = 0;

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		printd(LogInfo, "In Read Loop %s\n", cur->name);

		if ((!xmlStrcmp(cur->name, (const xmlChar *) XML_BASIC_SETTINGS))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printd(LogInfo, "keyword: %s\n", key);
			xmlFree(key);
		}

		if ((!xmlStrcmp(cur->name, (const xmlChar *) "SongPath"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printd(LogInfo, "Read Path: %s\n", key);
			strncpy(gMyInfo.BasePath, key, 255);
			xmlFree(key);
		}

		/*
		 * Read the Button information
		 */
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "MainButtons"))) {
			node = cur->xmlChildrenNode;
			while (node) {
				if (xmlStrcmp(node->name, (const xmlChar *) "text")) {
					printd(LogInfo, "Preset: %s ", node->name);

					key = xmlGetProp(node, "Name");
					printd(LogInfo, "Name: %s ", key);
					strcpy(gMyInfo.MyPatchInfo[Loop].Name, key);
					xmlFree(key);

					key = xmlGetProp(node, "Channel");
					printd(LogInfo, "Chan: %d ", atoi(key));
					gMyInfo.MyPatchInfo[Loop].Channel = atoi(key);
					xmlFree(key);

					key = xmlGetProp(node, "Controller");
					printd(LogInfo, "Controller: %d ", atoi(key));
					gMyInfo.MyPatchInfo[Loop].BankSelect = atoi(key);
					xmlFree(key);

					key = xmlGetProp(node, "OutPort");
					printd(LogInfo, "OutPort: %d ", atoi(key));
					gMyInfo.MyPatchInfo[Loop].OutPort = atoi(key);
					xmlFree(key);

					key = xmlGetProp(node, "Patch");
					printd(LogInfo, "Patch: %d \n", atoi(key));
					gMyInfo.MyPatchInfo[Loop].Patch = atoi(key);
					xmlFree(key);

					/* This is Bad, Compare with Button index. */
					Loop++;
				}
				node = node->next;
			}
		}

		/*
		 * Read the port information
		 */
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "NumOutPorts"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printd(LogInfo, "Read Path: %s\n", key);
			gMyInfo.NumOutPorts = atoi(key);
			xmlFree(key);
		}

		if ((!xmlStrcmp(cur->name, (const xmlChar *) "OutPorts"))) {
			gMyInfo.NumOutPorts = 0;
			node = cur->xmlChildrenNode;
			while (node) {
				if (xmlStrcmp(node->name, (const xmlChar *) "text")) {
					printd(LogInfo, "Preset: %s ", node->name);

					gMyInfo.NumOutPorts++;
					key = xmlGetProp(node, "Name");
					printd(LogInfo, "Name: %s ", key);
					strcpy(gMyInfo.OutPortName[Loop], key);
					xmlFree(key);

					/* This is Bad, Compare with Button index. */
					Loop++;
				}

				/*
				 * Read the Button information
				 */
				if ((!xmlStrcmp(cur->name, (const xmlChar *) "Layouts"))) {
					node = cur->xmlChildrenNode;
					while (node) {
printf("In XML Read found Layouts\n");
#if 0
						if (xmlStrcmp(node->name, (const xmlChar *) "text")) {
							printd(LogInfo, "Preset: %s ", node->name);

							key = xmlGetProp(node, "Name");
							printd(LogInfo, "Name: %s ", key);
							strcpy(gMyInfo.MyPatchInfo[Loop].Name, key);
							xmlFree(key);

							key = xmlGetProp(node, "Channel");
							printd(LogInfo, "Chan: %d ", atoi(key));
							gMyInfo.MyPatchInfo[Loop].Channel = atoi(key);
							xmlFree(key);

							key = xmlGetProp(node, "Controller");
							printd(LogInfo, "Controller: %d ", atoi(key));
							gMyInfo.MyPatchInfo[Loop].BankSelect = atoi(key);
							xmlFree(key);

							key = xmlGetProp(node, "OutPort");
							printd(LogInfo, "OutPort: %d ", atoi(key));
							gMyInfo.MyPatchInfo[Loop].OutPort = atoi(key);
							xmlFree(key);

							key = xmlGetProp(node, "Patch");
							printd(LogInfo, "Patch: %d \n", atoi(key));
							gMyInfo.MyPatchInfo[Loop].Patch = atoi(key);
							xmlFree(key);

							/* This is Bad, Compare with Button index. */
							Loop++;
						}
#endif
						node = node->next;
					}
				}








				node = node->next;
			}
		}
		cur = cur->next;
	}

	return;
}

/*--------------------------------------------------------------------
 * Function:		parseDoc
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void parseDoc(char *docname) {
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if (doc == NULL) {
		fprintf(stderr, "Document not parsed successfully. \n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) XML_BASIC_SETTINGS)) {
		fprintf(stderr, "document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return;
	}

	getReference(doc, cur);
	xmlFreeDoc(doc);
	return;
}

#ifdef Test2
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
#define dTopLevelNone 0
#define dTopLevelMainButtons 1
#define dTopLevelNumOutPorts 2
#define dTopLevelOutPorts 3
#define dTopLevelSongPath 4
#define dTopLevelAppName 5
#define dTopLevelMetronome 6
#define dTopLevelMetronomeOn 7
#define dTopLevelMidiBase 8
#define dTopLevelClick1	9
#define dTopLevelClickRest 10
#define dTopLevelLayouts 11

int TopLevelParse;
int ParseCountL4;
int ParseCountL2;
int ParseCountL3;
static void processNode1(xmlTextReaderPtr reader, xmlChar *name, xmlChar *value,
	int NodeType, int Depth) {

}

static void processNode(xmlTextReaderPtr reader, char Location) {
	xmlChar *name, *value;
	int NodeType;
	int Depth;
	char Buffer[100];
	int HoldIndex;

	name = xmlTextReaderName(reader);
	if (name == NULL)
		name = xmlStrdup(BAD_CAST "--");

	value = xmlTextReaderValue(reader);

	NodeType = xmlTextReaderNodeType(reader);
	Depth = xmlTextReaderDepth(reader);
	//	if (NodeType != 14 && NodeType != 3) {
#if 0
	printd(LogInfo, "%d D=%d T=%d N=[%s] V=[%s] E=%d\n",Location,
		Depth,
		NodeType,
		name,
		value,
		xmlTextReaderIsEmptyElement(reader));
#endif

	if (Depth == 2)
		ParseCountL2++;

	if (Depth == 3)
		ParseCountL3++;

	if (Depth == 4)
		ParseCountL4++;

	if (Depth == 1 && NodeType == 14) {
		printd(LogInfo, "\n*** Setting Top level to None\n");
		TopLevelParse = dTopLevelNone;
	}

	if (Depth == 1 && NodeType == 1) {
		printd(LogInfo, "\nTop Level Clear \n");
		TopLevelParse = dTopLevelNone;
		ParseCountL4 = 0;
		ParseCountL2 = 0;
		ParseCountL3 = 0;

		/* Let's start by looking for the highest level qualifier.
		 */
		if (!strcmp("MainButtons", name) && NodeType == 1) {
			TopLevelParse = dTopLevelMainButtons;
		}

		if (!strcmp("NumOutPorts", name) && NodeType == 1) {
			TopLevelParse = dTopLevelNumOutPorts;
		}

		if (!strcmp("OutPorts", name) && NodeType == 1) {
			TopLevelParse = dTopLevelOutPorts;
		}

		if (!strcmp("SongPath", name) && NodeType == 1) {
			TopLevelParse = dTopLevelSongPath;
		}

		if (!strcmp("AppNames", name) && NodeType == 1) {
			TopLevelParse = dTopLevelAppName;
		}

		if (!strcmp("TempoMax", name) && NodeType == 1) {
			TopLevelParse = dTopLevelMetronome;
		}

		if (!strcmp("MetroOn", name) && NodeType == 1) {
			TopLevelParse = dTopLevelMetronomeOn;
		}

		if (!strcmp("MidiBaseNote", name) && NodeType == 1) {
			TopLevelParse = dTopLevelMidiBase;
		}

		if (!strcmp("Click1", name) && NodeType == 1) {
			TopLevelParse = dTopLevelClick1;
		}

		if (!strcmp("ClickRest", name) && NodeType == 1) {
			TopLevelParse = dTopLevelClickRest;

		printd(LogInfo, "\nTop Level Set %d\n", TopLevelParse);
		} /* Depth == 1 NodeType == 1	*/

		if (!strcmp("Layouts", name) && NodeType == 1) {
			TopLevelParse = dTopLevelLayouts;
			printf("Found Layouts\n");
		}
	}
	if (TopLevelParse == dTopLevelOutPorts) {
		/* Get the second element.
		 */
		if (Depth == 2 && NodeType == 1) {
			sscanf(name, "Port%03d", &HoldIndex);
//					printd(LogInfo, "\nButton Number %d\n",HoldIndex );
			ParseCountL2 = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {
//					printd(LogInfo, "But %d %s %s", ParseCountL2, name, value);
			if (!strcmp("Name", name))
				strcpy(gMyInfo.OutPortName[ParseCountL2], value);

		}
	}

	if (TopLevelParse == dTopLevelAppName) {
		/* Get the second element.
		 */
		if (Depth == 2 && NodeType == 1) {
			sscanf(name, "App%03d", &HoldIndex);
//					printd(LogInfo, "\nButton Number %d\n",HoldIndex );
			ParseCountL2 = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {
//					printd(LogInfo, "But %d %s %s", ParseCountL2, name, value);
			if (!strcmp("Name", name))
				strcpy(gMyInfo.Apps[ParseCountL2].Name, value);

			if (!strcmp("PortID", name))
				gMyInfo.Apps[ParseCountL2].PortID = atoi(value);
		}
	}


/* ********************************** */
	if (TopLevelParse == dTopLevelLayouts) {
//printf("IN the TopLevel Layouts Parse\n");
		if (Depth == 2 && NodeType == 1 ) {
			sscanf(name, "Layout%03d\n", &HoldIndex);

			/* This is the index for the Layouts. */
			ParseCountL2 = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2 ) {
			/* Assign Names here.	*/
//			printd(LogInfo, "Layout Name %d %s\n",ParseCountL2, value );
			strcpy(gMyInfo.LayoutPresets[ParseCountL2].Name, value);

		}

		if (Depth == 3 && NodeType == 1 ) {
			sscanf(name, "Patch%03d\n", &HoldIndex);
			ParseCountL3 = HoldIndex;
		}

		if (Depth == 4 && NodeType == 2 ) {
			/* Assign Presets here.	*/
//		printd(LogInfo, "Preset Name %d %s\n",ParseCountL3, value );
		strcpy(gMyInfo.LayoutPresets[ParseCountL2].Presets[ParseCountL3], value);

		}
}

	if (TopLevelParse == dTopLevelMainButtons) {
		/* Get the second element.
		 */
		if (Depth == 2 && NodeType == 1) {
			sscanf(name, "Preset%03d", &HoldIndex);
//					printd(LogInfo, "\nButton Number %d\n",HoldIndex );
			ParseCountL2 = HoldIndex;
// Index			gMyInfo.MyPatchInfo[ParseCountL2].Index = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2) {

//        	printd(LogInfo, "But %d %s %s", ParseCountL2, name, value);

			if (!strcmp("Name", name))
				strcpy(gMyInfo.MyPatchInfo[ParseCountL2].Name, value);

			if (!strcmp("Channel", name))
				gMyInfo.MyPatchInfo[ParseCountL2].Channel = atoi(value);

			if (!strcmp("Controller", name))
				gMyInfo.MyPatchInfo[ParseCountL2].BankSelect = atoi(value);

			if (!strcmp("OutPort", name))
				gMyInfo.MyPatchInfo[ParseCountL2].OutPort = atoi(value);

			if (!strcmp("Patch", name))
				gMyInfo.MyPatchInfo[ParseCountL2].Patch = atoi(value);

			if (!strcmp("Custom", name))
				gMyInfo.MyPatchInfo[ParseCountL2].CustomCommand = atoi(value);

			if (!strcmp("Chain", name))
				strcpy(gMyInfo.MyPatchInfo[ParseCountL2].Chain, value);

		}

	} /* TopLevelParse == dTopLevelMainButtons	*/
	if (Depth == 2 && NodeType == 3) {
		if (TopLevelParse == dTopLevelNumOutPorts) {
			gMyInfo.NumOutPorts = atoi(value);
			printd(LogInfo, "\n***Numout Ports %d\n", gMyInfo.NumOutPorts);

		}

		if (TopLevelParse == dTopLevelSongPath) {
			printd(LogInfo, "Song Path %s\n", value);
			strncpy(gMyInfo.BasePath, value, 255);
		}
		if (TopLevelParse == dTopLevelMetronome) {
			printd(LogInfo, "dTopLevelMetronome %s\n", value);
			gMyInfo.TempoMax = atoi(value);
		}

		if (TopLevelParse == dTopLevelMetronomeOn) {
			printd(LogInfo, "dTopLevelMetronomeOn %s\n", value);
			gMyInfo.MetronomeOn = atoi(value);
		}

		if (TopLevelParse == dTopLevelMidiBase) {
			printd(LogInfo, "dTopLevelMidiBase %s\n", value);
			gMyInfo.MidiBaseNote = atoi(value);
		}

		if (TopLevelParse == dTopLevelClick1) {
			printd(LogInfo, "dTopLevelClick1 %s\n", value);
			gMyInfo.Drum1 = atoi(value);
		}

		if (TopLevelParse == dTopLevelClickRest) {
			printd(LogInfo, "dTopLevelClickRest %s\n", value);
			gMyInfo.DrumRest = atoi(value);
		}

	} /* Depth == 2 && NodeType == 3	*/

#if 0
	if (Location == 2) {

		if (TopLevelParse == dTopLevelSongPath) {
			printd(LogInfo, "Song Path %s\n", value);
			strncpy(gMyInfo.BasePath, value, 255);
		}

	}
#endif

	if (value != NULL)
		xmlFree(value);

	if (NodeType == 1)
		xmlFree(name);
}

void ReadPrefs() {
	xmlTextReaderPtr reader;
	int ret;

	xmlChar *name, *value;
	int NodeType;
	int Depth;

	printd(LogInfo, "----------------------\n");
	printd(LogInfo, "Reading prefs file\n");
	printd(LogInfo, "----------------------\n");
	reader = xmlNewTextReaderFilename(PREFSFILENAME);
	if (reader != NULL) {
		ret = xmlTextReaderRead(reader);
		while (ret == 1) {
			processNode(reader, 1);
			while (xmlTextReaderMoveToNextAttribute(reader))
				processNode(reader, 2);

			ret = xmlTextReaderRead(reader);
		}
		xmlFreeTextReader(reader);
		if (ret != 0) {
			printd(LogInfo, "%s : failed to parse\n", PREFSFILENAME);
		}
	} else {
		printd(LogInfo, "Unable to open %s\n", PREFSFILENAME);
	}
	printd(LogInfo, "----------------------\n");
	printd(LogInfo, "Reading prefs file\n");
	printd(LogInfo, "----------------------\n");
}

#endif
