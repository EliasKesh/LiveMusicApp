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

#define PREFSFILENAME "LiveMusicRes/LiveMusic.xml"

/*
 * Place Local prototypes here
 */

void WritePrefs(void);

/*
 * Place Static variables here
 */
/* @formatter:off */
GTKMidiInfo GlobalInfo =
	{
		{
			//Title			Bank	Patch 	Outport		Chan	Command Chain
			{ "Chorus", 0xff, 1, RakarrakPort, 1, NoCustom, "None" },
			{ "SatrianiComp", 0xff, 12, RakarrakPort, 1, NoCustom, "None" },
			{ "CleanBass", 0xff, 3, RakarrakPort, 1, NoCustom, "None" },
			{ "MedDist", 0xff, 4, RakarrakPort, 1, NoCustom, "None" },
			{ "OctBass", 0xff, 5, RakarrakPort, 1, NoCustom, "None" },
			{ "FunkBass", 0xff, 9, RakarrakPort, 1, NoCustom, "None" },
			{ "Satriani", 0xff, 16, RakarrakPort, 1, NoCustom, "None" },
			{ "FunkyWah", 0xff, 6, RakarrakPort, 1, NoCustom, "None" },
			{ "JazzRev", 0xff, 28, RakarrakPort, 1, NoCustom, "None" },
			{ "LayoutSwitch", 0xff, 0, FluidPort, 1, cmdBankSelect, "None" },
			//	{"Preset10",	"Change",		0xff,		0,		RakarrakPort,	2,	ToNextDesktop,0},
			// 10	Button		Title		Bank	Patch	Outport
			{ "Piano", SFDSF, 63, FluidPort, 1, NoCustom, "None" },
			{ "Honky", SFFluidGM, 3, FluidPort, 1, NoCustom, "None" },
			{ "Breath", SFFluidGM, 53, FluidPort, 1, NoCustom, "None" },
			{ "Organ", SFFluidGM, 16, FluidPort, 1, NoCustom, "None" },
			{ "Flute", SFFlute, 73, FluidPort, 1, NoCustom, "None" },
			{ "SynthLead", SFDSF, 43, FluidPort, 1, NoCustom, "None" },
			{ "Strings", SFDSF, 85, FluidPort, 1, NoCustom, "None" },
			{ "Trumpet", SFDSF, 66, FluidPort, 1, NoCustom, "None" },
			{ "Steel Drums", SFFluidGM, 114, FluidPort, 1, NoCustom, "None" },
			{ "Walky", SFDSF, 3, FluidPort, 1, NoCustom, "None" },
			// 20	Button		Title		Bank	Patch	Outport
//       { "Drums", 128, 26, FluidPort, 1, NoCustom, ""  },
			{ "Drums", SFDrumsElias, 1, FluidPort, 1, NoCustom, "None" },
			{ "Slap Bass", SFDSF, 24, FluidPort, 1, NoCustom, "None" },
			{ "Synth Hard", SFDSF, 60, FluidPort, 1, NoCustom, "None" },
			{ "Marimba", SFDSF, 12, FluidPort, 1, NoCustom, "None" },
			{ "Breath", SFDSF, 48, FluidPort, 1, NoCustom, "None" },
			{ "Steel Drums", SFDSF, 114, FluidPort, 1, NoCustom, "None" },
			{ "Low Synth", SFDSF, 45, FluidPort, 1, NoCustom, "None" },
			{ "Mid0", 0xff, 0, 0, 1, NoCustom, "None" },
			{ "Mid-24", 0xff, 2, 0, 1, NoCustom, "None" },
			{ "Mid Screen", SFDSF, 0, FluidPort, 1, NoCustom, "None" },
			// 30	Button		Title		Bank	Patch	Outport
			{ "Rak Eff6", 116, 11, FluidPort, 1, Controller, "None" },
			{ "Rak Eff7", 116, 13, FluidPort, 1, Controller, "None" },
			{ "Rak Eff8", 116, 15, FluidPort, 1, Controller, "None" },
			{ "Rak Eff9", 116, 17, FluidPort, 1, Controller, "None" },
			{ "Rak Eff10", 116, 18, FluidPort, 1, Controller, "None" },
			{ "Rak Eff1", 116, 0, FluidPort, 1, Controller, "None" },
			{ "Rak Eff2", 116, 2, FluidPort, 1, Controller, "None" },
			{ "Rak Eff3", 116, 4, FluidPort, 1, Controller, "None" },
			{ "Rak Eff4", 116, 6, FluidPort, 1, Controller, "None" },
			{ "Rak Eff5", 116, 8, FluidPort, 1, Controller, "None" },
			// 40	Button		Title		Bank	Patch	Outport
			{ "TransStart", 0xff, 0, TransportPort, 1, TransStart, "None" },
			{ "TransCont", 0xff, 0, TransportPort, 1, TransCont, "None" },
			{ "TransStop", 0xff, 0, TransportPort, 1, TransStop, "None" },
			{ "TransBack", 0xff, 0, TransportPort, 1, TransStart, "None" },
			{ "TransPos", 0xff, 0, TransportPort, 1, TransStart, "None" },
			{ "Tap", 116, 121, FluidPort, 1, Controller, "None" },
			{ "Midi Tog", 116, 122, FluidPort, 1, Controller, "None" },
			{ "Tuner", 116, 123, FluidPort, 1, Controller, "None" },
			{ "AnaOnOff", 116, 124, FluidPort, 1, Controller, "None" },
			{ "AnaOnOff", 116, 124, FluidPort, 1, Controller, "None" },
			// 50	Button		Title		Bank	Patch	Outport
			{ "LP Undo", 0xff, 69, LooperPort, 1, NoCustom, "None" },
			{ "LP 1", 0xff, 70, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP 2", 0xff, 71, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP 3", 0xff, 72, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP 4", 0xff, 73, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP Rec", 0xff, 74, LooperPort, 1, NoCustom, "LooperApp" },
			{ "LP Pause", 0xff, 75, LooperPort, 1, NoCustom, "None" },
			{ "LP Trig", 0xff, 76, LooperPort, 1, NoCustom, "None" },
			{ "LP Tap", 0xff, 77, LooperPort, 1, NoCustom, "None" },
			{ "LP SelAll", 0xff, 78, LooperPort, 1, NoCustom, "None" },
			// 60	Button		Title		Bank	Patch	Outport
			{ "AnalogApp", 0xff, AnalogApp, 0, 1, RaiseApp, "None" },
			{ "MidiSoundApp", 0xff, MidiSoundApp, 0, 1, RaiseApp, "None" },
			{ "LooperApp", 0xff, LooperApp, 0, 1, RaiseApp, "None" },
			{ "MidiApp", 0xff, LiveMusicApp, 0, 1, RaiseApp, "None" },
			{ "MP3Player", 0xff, MP3Player, 0, 1, RaiseApp, "None" },
			{ "TabPlayer", 0xff, TabPlayer, 0, 1, RaiseApp, "None" },
			{ "PreTab", 0xff, 0, 0, 1, SwitchTab, "None" },
			{ "ScoreTab", 0xff, 1, 0, 1, SwitchTab, "None" },
			{ "TransportApp", 0xff, TransportApp, 0, 1, RaiseApp, "None" },
			{ "NULL", 0xff, 0, 0, 1, RaiseApp, "None" },
			// 70	Button		Title		Bank	Patch	Outport
			{ "Switch0", 0xff, 0, 0, 1, ToDesktop, "None" },
			{ "Switch1", 0xff, 1, 0, 1, ToDesktop, "None" },
			{ "Switch2", 0xff, 2, 0, 1, ToDesktop, "None" },
			{ "Switch3", 0xff, 3, 0, 1, ToDesktop, "None" },
			{ "Switch4", 0xff, 4, 0, 1, ToDesktop, "None" },
			{ "Switch5", 0xff, 5, 0, 1, ToDesktop, "None" },
			{ "VolUp", 0xff, 5, 0, 1, cmdVolume, "None" },
			{ "VolDown", 0xff, -5, 0, 1, cmdVolume, "None" },
			{ "RepeatA", 0xff, cmdLnTSetA, 0, 1, cmdLnTransPort, "None" },
			{ "RepeatB", 0xff, cmdLnTSetB, 0, 1, cmdLnTransPort, "None" },
			// 80	Button		Title		Bank	Patch	Outport
			{ "EBuzz", SFElias, 0, FluidPort, 1, NoCustom, "None" },
			{ "SynthB11", SFElias, 1, FluidPort, 1, NoCustom, "None" },
			{ "KStatB", SFElias, 2, FluidPort, 1, NoCustom, "None" },
			{ "MS20Bass", SFElias, 3, FluidPort, 1, NoCustom, "None" },
			{ "Juno", SFElias, 4, FluidPort, 1, NoCustom, "None" },
			{ "Moog1", SFElias, 9, FluidPort, 1, NoCustom, "None" },
			{ "Moog2", SFElias, 10, FluidPort, 1, NoCustom, "None" },
			{ "BassSlap", SFElias, 13, FluidPort, 1, NoCustom, "None" },
			{ "Taurus2", SFElias, 26, FluidPort, 1, NoCustom, "None" },
			{ "Ultimate2a", SFElias, 33, FluidPort, 1, NoCustom, "None" },
			// 90	Button		Title		Bank	Patch	Outport
			{ "SynthB1", SFElias, 38, FluidPort, 1, NoCustom, "None" },
			{ "FilterBass", SFElias, 39, FluidPort, 1, NoCustom, "None" },
			{ "BasBiy", SFElias, 45, FluidPort, 1, NoCustom, "None" },
			{ "CZOrgan", SFElias, 51, FluidPort, 1, NoCustom, "None" },
			{ "MoogLead1", SFElias, 53, FluidPort, 1, NoCustom, "None" },
			{ "CZBrass4", SFElias, 64, FluidPort, 1, NoCustom, "None" },
			{ "SinSymph", SFElias, 90, FluidPort, 1, NoCustom, "None" },
			{ "Trig4Count", 0xff, 4, TransportPort, 1, cmdCountIn, "None" },
			{ "NextSong", 0xff, 2, FluidPort, 1, cmdSetList, "None" },
			{ "PrevSong", 0xff, 1, FluidPort, 1, cmdSetList, "None" },
			// 100	Button		Title		Bank	Patch	Outport
			{ "Scroll", 0xff, 0, 0, 1, cmdScroll, "None" },
			{ "NextSeq", 0xff, cmdLnTNext, 0, 1, cmdLnTransPort, "None" },
			{ "PrevSeg", 0xff, cmdLnTPrev, 0, 1, cmdLnTransPort, "None" },
			{ "SpeeedUp", 0xff, cmdLnTUp, 0, 1, cmdLnTransPort, "None" },
			{ "SlowDown", 0xff, cmdLnTDown, 0, 1, cmdLnTransPort, "None" },
			{ "PlPlay", 0xff, cmdLnTStart, 0, 1, cmdLnTransPort, "None" },
			{ "Loop Toggle", 0xff, cmdLnTLoop, 0, 1, cmdLnTransPort, "None" },
			{ "Seek Fw", 0xff, cmdLnTSeekFw, 0, 1, cmdLnTransPort, "None" },
			{ "Seek Bk", 0xff, cmdLnTSeekBk, 0, 1, cmdLnTransPort, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },

			// 110	Button		Title		Bank	Patch	Outport
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },

			// 120	Button		Title		Bank	Patch	Outport
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },
			{ "Null", 0xff, 0, 0, 1, MaxCommands, "None" },

			// 130	Button		Title		Bank	Patch	Outport
			{ "Preset1", 0xff, 1, 0, 1, cmdPreset, "None" },
			{ "Preset2", 0xff, 2, 0, 1, cmdPreset, "None" },
			{ "Preset3", 0xff, 3, 0, 1, cmdPreset, "None" },
			{ "Preset4", 0xff, 4, 0, 1, cmdPreset, "None" },
			{ "Preset5", 0xff, 5, 0, 1, cmdPreset, "None" },
			{ "Preset6", 0xff, 6, 0, 1, cmdPreset, "None" },
			{ "Midi Pre", 0xff, FluidPort, 0, 1, cmdMidiSelect, "None" },
			// FIX THIS
			{ "Slide1", 0xff, MIDI_CTL_MSB_MAIN_VOLUME, RakarrakPort, 1, NoCustom, "None" },
			{ "Slide2", 0xff, MIDI_CTL_MSB_MAIN_VOLUME, FluidPort, 1, NoCustom, "None" },
			{ "Slide3", 0xff, MIDI_CTL_MSB_MAIN_VOLUME, FluidPort, 2, NoCustom, "None" },

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
				"Chorus", 		"SatrianiComp", 	"MS20Bass", 	"Flute", 		"CleanBass",	"FunkyWah",
				"PreTab", 		 "ScoreTab", 	"OctBass",		"Strings", 		"Trumpet",		"Steel Drums",
				"Scroll", 		"Drums", 		"Scroll", 		"NextSong",	"PrevSong", 	"LayoutSwitch",

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
				"FunkBass", 	"FunkyWah", 	"Breath", 		"Flute",		"MS20Bass",  	"SatrianiComp",
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

		{ "Transcribe",
			{
	/*			1			2			3			4			5			6 		*/
				"Preset1", 		"Preset2", 		"Preset3",		"Chorus", 		"NextSeq", 		"PrevSeg",
				"RepeatA", 		"RepeatB", 		"Loop Toggle",	"PlPlay",		"SpeeedUp",  	"SlowDown",
				"Seek Fw", 		"Seek Bk", 		"LP 3", 		"LP Undo",		"LP Rec", 		"LP Pause",
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
// START-ECLIPSE-FORMATTING

/*--------------------------------------------------------------------
 * Function:		InitPref
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void InitPref(void) {
	int Count;

	memset(&gMyInfo, 0, sizeof(GTKMidiInfo));

#if 1
	memcpy(&gMyInfo, &GlobalInfo, sizeof(GTKMidiInfo));
	WritePrefs();
#else
	ReadPrefs();

	#endif
#if 1
	gMyInfo.TempoMax = 8;

	for (Count = 0; Count < MaxPresetButtons; Count++)
		gMyInfo.WebPresets.thePreset[Count] = -1;

	gMyInfo.MidiBaseNote = 40;
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

	/*
	 * Create the popup Menu's not what we have the presets.
	 */
	CreatePatchPopupMenu();
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

	printd(LogInfo, "Main Information\n");

	for (Loop = 0; Loop < Max_Patches; Loop++) {
		printd(LogInfo, "ID=%d %s %d %d %d %d %s\n", Loop,
			myInfo->MyPatchInfo[Loop].Name,
			myInfo->MyPatchInfo[Loop].Patch,
			myInfo->MyPatchInfo[Loop].OutPort,
			myInfo->MyPatchInfo[Loop].Channel,
			myInfo->MyPatchInfo[Loop].CustomCommand,
			myInfo->MyPatchInfo[Loop].Chain);
	}

	printd(LogInfo, "BaseName %s\n", myInfo->BasePath);

	printd(LogInfo, "Num Ports %d\n", myInfo->NumOutPorts);
	for (Loop = 0; Loop < myInfo->NumOutPorts; Loop++) {
		printd(LogInfo, "Ports %s\n", myInfo->OutPortName[Loop]);
	}

	for (Loop = 0; Loop < MaxApps; Loop++) {
		printd(LogInfo, "Apps %s %d\n", myInfo->Apps[Loop].Name,
			myInfo->Apps[Loop].PortID);
	}

	thePorts = &theInPorts;
	for (Loop = 0; Loop < thePorts->NumDevices; Loop++) {
		printd(LogInfo, "In Name [%s]\n", thePorts->Devices[Loop].Name);
		for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
			printd(LogInfo, "In SName %d, [%s]\n", Loop1,
				thePorts->Devices[Loop].Ports[Loop1].Name);
		}
	}

	thePorts = &theOutPorts;
	for (Loop = 0; Loop < thePorts->NumDevices; Loop++) {
		printd(LogInfo, "Out Name [%s]\n", thePorts->Devices[Loop].Name);
		for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
			printd(LogInfo, "Out SName %d, [%s]\n", Loop1,
				thePorts->Devices[Loop].Ports[Loop1].Name);
		}
	}

	for (Loop = 0; gMyInfo.LayoutPresets[Loop].Name[0] != 0; Loop++) {
		printd(LogInfo, "Lay=%d %s %d %d %d\n", Loop,
			gMyInfo.LayoutPresets[Loop].Name,
			gMyInfo.LayoutPresets[Loop].Presets[0],
			gMyInfo.LayoutPresets[Loop].Presets[1],
			gMyInfo.LayoutPresets[Loop].Presets[2]);
	}

	printd(LogInfo, "Metronome Base %d On %d %d %d\n", myInfo->TempoMax,
		myInfo->MetronomeOn, myInfo->Drum1, myInfo->DrumRest);
	printd(LogInfo, "Midi Base Note %d\n", myInfo->MidiBaseNote);

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

	printd(LogInfo, "In Write Prefs\n");
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
//	printd(LogInfo, "Starting %x %s\n",node, gMyInfo.LayoutPresets[Loop].Name);
	while(gMyInfo.LayoutPresets[Loop].Name[0]) {
		sprintf(buff, "Layout%03d", Loop);
		node1 = xmlNewChild(node, NULL, buff, NULL);
//		printd(LogInfo, "Loop %d %x %s\n",Loop,node1, LayoutPresets[Loop].Name);

		xmlSetProp(node1, "LayoutName", gMyInfo.LayoutPresets[Loop].Name);
//		printd(LogInfo, "xmlSetProp %d %x %s\n",Loop,node1, LayoutPresets[Loop].Name);

//		node1 = xmlNewChild(node, NULL, BAD_CAST LayoutPresets[Loop].Name, NULL);
		for (PatchLoop = 0; PatchLoop< Max_Patches ;PatchLoop++) {
			sprintf(buff, "Patch%03d", PatchLoop);
//			printd(LogInfo, "Patchnames %s\n",buff);
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
int	LayoutIndexXML;
int	PatchIndexXML;

static void processNode(xmlTextReaderPtr reader, char Location) {
	xmlChar *name, *value;
	int NodeType;
	int Depth;
	char Buffer[100];
	int HoldIndex;


	name = xmlTextReaderName(reader);
	if (name == NULL)
		name = xmlStrdup(BAD_CAST "");

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
			printd(LogInfo, "Found Layouts\n");
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
//printd(LogInfo, "IN the TopLevel Layouts Parse\n");
//		printd(LogInfo, "dTopLevelLayouts %d %d\n",LayoutIndexXML,  PatchIndexXML);

		if (Depth == 2 && NodeType == 1 ) {
			sscanf(name, "Layout%03d\n", &HoldIndex);
//			printd(LogInfo, "Layout Index %d \n",HoldIndex );

			/* This is the index for the Layouts. */
			LayoutIndexXML = HoldIndex;
		}

		if (Depth == 3 && NodeType == 2 ) {
			/* Assign Names here.	*/
			printd(LogInfo, "Layout Name Read %d %s\n",LayoutIndexXML, value );
#if 0
			if (!*value) {
				TopLevelParse = dTopLevelNone;
				printd(LogInfo, "Layout Names end\n");
				exit(0);
				return;
			}
#endif
			strcpy(gMyInfo.LayoutPresets[LayoutIndexXML].Name, value);
			}

		if (Depth == 3 && NodeType == 1 ) {
			sscanf(name, "Patch%03d\n", &HoldIndex);
			PatchIndexXML = HoldIndex;
		}

		if (Depth == 4 && NodeType == 2 ) {
			/* Assign Presets here.	*/
//		printd(LogInfo, "Preset Name %d %s\n",ParseCountL3, value );
		strcpy(gMyInfo.LayoutPresets[LayoutIndexXML].Presets[PatchIndexXML], value);
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
