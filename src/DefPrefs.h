LiveMusicInfo GlobalInfo = {
	{

#if 0
		//Title                 Bank    Patch   Outport         Chan    Command Chain
		{"Chorus", 0xff, 1, GuitarixPort, 1, NoCustom, "None"},
		{"SatrianiComp", 0xff, 2, GuitarixPort, 1, NoCustom, "None"},
		{"CleanBass", 0xff, 3, GuitarixPort, 1, NoCustom, "None"},
		{"MedDist", 0xff, 4, GuitarixPort, 1, NoCustom, "None"},
		{"OctBass", 0xff, 5, GuitarixPort, 1, NoCustom, "None"},
		{"FunkBass", 0xff, 7, GuitarixPort, 1, NoCustom, "None"},
		{"EBell", 0xff, 19, GuitarixPort, 1, NoCustom, "None"},
		{"FunkyGuitar", 0xff, 6, GuitarixPort, 1, NoCustom, "None"},
		{"WahToggle", 0xff, 28, GuitarixPort, 1, NoCustom, "None"},
		{"LayoutSwitch", 0xff, 0, FluidPort, 1, cmdBankSelect, "None"},
#else
		//Title                 Bank    Patch   Outport         Chan    Command Chain
		{"Chorus", 0xff, 0, GuitarixPort, 1, NoCustom, "None"},
		{"SatrianiComp", 0xff, 1, GuitarixPort, 1, NoCustom, "None"},
		{"CleanBass", 0xff, 2, GuitarixPort, 1, NoCustom, "None"},
		{"FunkyGuitar", 0xff, 3, GuitarixPort, 1, NoCustom, "None"},
		{"MedDist", 0xff, 4, GuitarixPort, 1, NoCustom, "None"},
		{"OctBass", 0xff, 5, GuitarixPort, 1, NoCustom, "None"},
		{"FunkBass", 0xff, 6, GuitarixPort, 1, NoCustom, "None"},
		{"EBell", 0xff, 7, GuitarixPort, 1, NoCustom, "None"},
		{"WahToggle", 0xff, 14, GuitarixPort, 1, cmdSendCC, "None"},
		{"LayoutSwitch", 0xff, 0, FluidPort, 1, cmdBankSelect, "None"},
#endif

		//      {"Preset10",    "Change",               0xff,           0,              GuitarixPort,   2,      ToNextDesktop,0},
		// 10   Button          Title           Bank    Patch   Outport
		{"Piano", SFDSF, 63, FluidPort, 1, NoCustom, "None"},
		{"Honky", SFFluidGM, 3, FluidPort, 1, NoCustom, "None"},
		{"Breath", SFFluidGM, 53, FluidPort, 1, NoCustom, "None"},
		{"Organ", SFFluidGM, 16, FluidPort, 1, NoCustom, "None"},
		{"Flute", SFFlute, 73, FluidPort, 1, NoCustom, "None"},
		{"SynthLead", SFDSF, 43, FluidPort, 1, NoCustom, "None"},
		{"Strings", SFDSF, 85, FluidPort, 1, NoCustom, "None"},
		{"Trumpet", SFDSF, 66, FluidPort, 1, NoCustom, "None"},
		{"Steel Drums", SFFluidGM, 114, FluidPort, 1, NoCustom, "None"},
		{"Walky", SFDSF, 3, FluidPort, 1, NoCustom, "None"},
		// 20   Button          Title           Bank    Patch   Outport
//       { "Drums", 128, 26, FluidPort, 1, NoCustom, ""  },
		{"Drums", SFDrumsElias, 1, FluidPort, 1, NoCustom, "None"},
		{"Slap Bass", SFDSF, 24, FluidPort, 1, NoCustom, "None"},
		{"Synth Hard", SFDSF, 60, FluidPort, 1, NoCustom, "None"},
		{"Marimba", SFDSF, 12, FluidPort, 1, NoCustom, "None"},
		{"Breath1", SFDSF, 48, FluidPort, 1, NoCustom, "None"},
		{"Steel DrumsDUP", SFDSF, 114, FluidPort, 1, NoCustom, "None"},
		{"Low Synth", SFDSF, 45, FluidPort, 1, NoCustom, "None"},
		{"Mid0", 0xff, 0, 0, 1, NoCustom, "None"},
		{"Mid-24", 0xff, 2, 0, 1, NoCustom, "None"},
		{"Mid Screen", SFDSF, 0, FluidPort, 1, NoCustom, "None"},
		// 30   Button          Title           Bank    Patch   Outport
		{"Rak Eff6", 116, 11, GuitarixPort, 1, Controller, "None"},
		{"Rak Eff7", 116, 13, GuitarixPort, 1, Controller, "None"},
		{"Rak Eff8", 116, 15, GuitarixPort, 1, Controller, "None"},
		{"Rak Eff9", 116, 17, GuitarixPort, 1, Controller, "None"},
		{"Rak Eff10", 116, 18, GuitarixPort, 1, Controller, "None"},
		{"Rak Eff1", 116, 0, GuitarixPort, 1, Controller, "None"},
		{"Rak Eff2", 116, 2, GuitarixPort, 1, Controller, "None"},
		{"Rak Eff3", 116, 4, GuitarixPort, 1, Controller, "None"},
		{"Rak Eff4", 116, 6, GuitarixPort, 1, Controller, "None"},
		{"Rak Eff5", 116, 8, GuitarixPort, 1, Controller, "None"},
		// 40   Button          Title           Bank    Patch   Outport
		{"TransStart", 0xff, 0, TransportPort, 1, TransStart, "None"},
		{"TransCont", 0xff, 0, TransportPort, 1, TransCont, "None"},
		{"TransStop", 0xff, 0, TransportPort, 1, TransStop, "None"},
		{"TransBack", 0xff, 0, TransportPort, 1, TransStart, "None"},
		{"TransPos", 0xff, 0, TransportPort, 1, TransStart, "None"},
		{"Tap", 116, 121, FluidPort, 1, Controller, "None"},
		{"Midi Tog", 116, 122, FluidPort, 1, Controller, "None"},
		{"Tuner", 116, 123, FluidPort, 1, Controller, "None"},
		{"AnaOnOff", 116, 124, FluidPort, 1, Controller, "None"},
#if 0
		// 50   Button          Title           Bank    Patch   Outport
		{"LP Undo", 0xff, 69, LooperPort, 1, NoCustom, "None"},
		{"LP 1", 0xff, 70, LooperPort, 1, NoCustom, "LooperApp"},
		{"LP 2", 0xff, 71, LooperPort, 1, NoCustom, "LooperApp"},
		{"LP 3", 0xff, 72, LooperPort, 1, NoCustom, "LooperApp"},
		{"LP 4", 0xff, 73, LooperPort, 1, NoCustom, "LooperApp"},
		{"LP Rec", 0xff, 74, LooperPort, 1, NoCustom, "LooperApp"},
		{"LP Pause", 0xff, 75, LooperPort, 1, NoCustom, "None"},
		{"LP Trig", 0xff, 76, LooperPort, 1, NoCustom, "None"},
		{"LP Tap", 0xff, 77, LooperPort, 1, NoCustom, "None"},
		{"LP SelAll", 0xff, 78, LooperPort, 1, NoCustom, "None"},
#else
		// 50   Title   Bank    Patch   Outport
		{"LP Undo", 0xff, OSCUndo, 0, 1, cmdOSC, "None"},
		{"LP 1", 1, OSCSelect, 0, 1, cmdOSC, "None"},
		{"LP 2", 2, OSCSelect, 0, 1, cmdOSC, "None"},
		{"LP 3", 3, OSCSelect, 0, 1, cmdOSC, "None"},
		{"LP 4", 4, OSCSelect, 0, 1, cmdOSC, "None"},
		{"LP Rec", 0xff, OSCRec, 0, 1, cmdOSC, "None"},
		{"LP Pause", 0xff, OSCPause, 0, 1, cmdOSC, "None"},
		{"LP Trig", 0xff, OSCTrig, 0, 1, cmdOSC, "None"},
		{"LP Tap", 0xff, 77, 0, 1, cmdOSC, "None"},
		{"LP Add", 0xff, OSCAddLoop, 0, 1, cmdOSC, "None"},
#endif
		// 60   Button          Title           Bank    Patch   Outport
		{"AnalogApp", 0xff, AnalogApp, 0, 1, RaiseApp, "None"},
		{"MidiSoundApp", 0xff, MidiSoundApp, 0, 1, RaiseApp, "None"},
		{"LooperApp", 0xff, LooperApp, 0, 1, RaiseApp, "None"},
		{"LiveApp", 0xff, LiveMusicApp, 0, 1, RaiseApp, "None"},
		{"MP3Player", 0xff, MP3Player, 0, 1, RaiseApp, "None"},
		{"TabPlayer", 0xff, TabPlayer, 0, 1, RaiseApp, "None"},
		{"PreTab", 0xff, 0, 0, 1, SwitchTab, "None"},
		{"ScoreTab", 0xff, 1, 0, 1, SwitchTab, "None"},
		{"TransportApp", 0xff, TransportApp, 0, 1, RaiseApp, "None"},
		{"NULL", 0xff, 0, 0, 1, RaiseApp, "None"},
		// 70   Button          Title           Bank    Patch   Outport
		{"Switch0", 0xff, 0, 0, 1, ToDesktop, "None"},
		{"Switch1", 0xff, 1, 0, 1, ToDesktop, "None"},
		{"Switch2", 0xff, 2, 0, 1, ToDesktop, "None"},
		{"Switch3", 0xff, 3, 0, 1, ToDesktop, "None"},
		{"Switch4", 0xff, 4, 0, 1, ToDesktop, "None"},
		{"Switch5", 0xff, 5, 0, 1, ToDesktop, "None"},
		{"VolUp", 0xff, 5, 0, 1, cmdVolume, "None"},
		{"VolDown", 0xff, -5, 0, 1, cmdVolume, "None"},
		{"RepeatA", 0xff, cmdLnTSetA, 0, 1, cmdLnTransPort, "None"},
		{"RepeatB", 0xff, cmdLnTSetB, 0, 1, cmdLnTransPort, "None"},
		// 80   Button          Title           Bank    Patch   Outport
		{"EBuzz", SFElias, 0, FluidPort, 1, NoCustom, "None"},
		{"SynthB11", SFElias, 1, FluidPort, 1, NoCustom, "None"},
		{"KStatB", SFElias, 2, FluidPort, 1, NoCustom, "None"},
		{"MS20Bass", SFElias, 3, FluidPort, 1, NoCustom, "None"},
		{"Juno", SFElias, 4, FluidPort, 1, NoCustom, "None"},
		{"Moog1", SFElias, 9, FluidPort, 1, NoCustom, "None"},
		{"Moog2", SFElias, 10, FluidPort, 1, NoCustom, "None"},
		{"BassSlap", SFElias, 13, FluidPort, 1, NoCustom, "None"},
		{"Taurus2", SFElias, 26, FluidPort, 1, NoCustom, "None"},
		{"Ultimate2a", SFElias, 33, FluidPort, 1, NoCustom, "None"},
		// 90   Button          Title           Bank    Patch   Outport
		{"SynthB1", SFElias, 38, FluidPort, 1, NoCustom, "None"},
		{"FilterBass", SFElias, 39, FluidPort, 1, NoCustom, "None"},
		{"BasBiy", SFElias, 45, FluidPort, 1, NoCustom, "None"},
		{"CZOrgan", SFElias, 51, FluidPort, 1, NoCustom, "None"},
		{"MoogLead1", SFElias, 53, FluidPort, 1, NoCustom, "None"},
		{"CZBrass4", SFElias, 64, FluidPort, 1, NoCustom, "None"},
		{"SinSymph", SFElias, 90, FluidPort, 1, NoCustom, "None"},
		{"TrigCountIn", 0xff, 4, TransportPort, 1, cmdCountIn, "LooperApp"},
		{"NextSong", 0xff, 2, FluidPort, 1, cmdSetList, "None"},
		{"PrevSong", 0xff, 1, FluidPort, 1, cmdSetList, "None"},
		// 100  Button          Title           Bank    Patch   Outport
		{"Scroll", 0xff, 0, 0, 1, cmdScroll, "None"},
		{"NextSeq", 0xff, cmdLnTNext, 0, 1, cmdLnTransPort, "None"},
		{"PrevSeg", 0xff, cmdLnTPrev, 0, 1, cmdLnTransPort, "None"},
		{"SpeeedUp", 0xff, cmdLnTUp, 0, 1, cmdLnTransPort, "None"},
		{"SlowDown", 0xff, cmdLnTDown, 0, 1, cmdLnTransPort, "None"},
		{"PlPlay", 0xff, cmdLnTStart, 0, 1, cmdLnTransPort, "None"},
		{"Loop Toggle", 0xff, cmdLnTLoop, 0, 1, cmdLnTransPort, "None"},
		{"Seek Fw", 0xff, cmdLnTSeekFw, 0, 1, cmdLnTransPort, "None"},
		{"Seek Bk", 0xff, cmdLnTSeekBk, 0, 1, cmdLnTransPort, "None"},
		{"Null", 0xff, 0, 0, 1, MaxCommands, "None"},
		{"Null", 0xff, 0, 0, 1, MaxCommands, "None"},
		{"Null", 0xff, 0, 0, 1, MaxCommands, "None"},

		// 110  Button          Title           Bank    Patch   Outport
		{"Holds-Mel", 0xff, 21, GuitarixPort, 1, NoCustom, "NiceAhh"},
		{"NiceAhh", SFDSF, 90, FluidPort, 1, NoCustom, "None"},
		{"Holds-Hard", 0xff, 22, GuitarixPort, 1, NoCustom, "NiceAhh"},
		{"BetAhh", SFVintage, 63, FluidPort, 1, NoCustom, "None"},
		{"ToLate", SFVintage, 108, FluidPort, 1, NoCustom, "None"},
		{"LP All", 0xff, OSCSelect, 0, 1, cmdOSC, "None"},
		{"LP Mute", 0xff, OSCMute, 0, 1, cmdOSC, "None"},
		{"Null", 0xff, 0, 0, 1, MaxCommands, "None"},
		{"Null", 0xff, 0, 0, 1, MaxCommands, "None"},
		{"Null", 0xff, 0, 0, 1, MaxCommands, "None"},

		// 120  Button          Title           Bank    Patch   Outport
		{"BellsComp", SFDX7, 10, FluidPort, 1, NoCustom, "None"},
		{"Sax", SFSonidoGM, 66, FluidPort, 1, NoCustom, "None"},
		{"Vibra", SFVintage, 93, FluidPort, 1, NoCustom, "None"},
		{"Mariba1", SFDX7, 91, FluidPort, 1, NoCustom, "None"},
		{"Tenor", SFRolandTenor, 1, FluidPort, 1, NoCustom, "None"},
		{"CleanDist", 0xff, 15, GuitarixPort, 1, NoCustom, "None"},
		{"OctUp", 0xff, 6, GuitarixPort, 1, NoCustom, "None"},
		{"AutoWah", 0xff, 7, GuitarixPort, 1, NoCustom, "None"},
		{"Null", 0xff, 0, 0, 1, MaxCommands, "None"},
		{"Null", 0xff, 0, 0, 1, MaxCommands, "None"},
		{"Null", 0xff, 0, 0, 1, MaxCommands, "None"},

		// 130  Button          Title           Bank    Patch   Outport
		{"Preset1", 0xff, 1, 0, 1, cmdPreset, "None"},
		{"Preset2", 0xff, 2, 0, 1, cmdPreset, "None"},
		{"Preset3", 0xff, 3, 0, 1, cmdPreset, "None"},
		{"Preset4", 0xff, 4, 0, 1, cmdPreset, "None"},
		{"Preset5", 0xff, 5, 0, 1, cmdPreset, "None"},
		{"Preset6", 0xff, 6, 0, 1, cmdPreset, "None"},
		{"Midi Pre", 0xff, FluidPort, 0, 1, cmdMidiSelect, "None"},
		// FIX THIS
		{
			"Slide1", 0xff, MIDI_CTL_MSB_MAIN_VOLUME, GuitarixPort, 1, NoCustom,
			"None"
		},
		{"Slide2", 0xff, MIDI_CTL_MSB_MAIN_VOLUME, FluidPort, 1, NoCustom, "None"},
		{"Slide3", 0xff, MIDI_CTL_MSB_MAIN_VOLUME, FluidPort, 2, NoCustom, "None"},

		/*
		   6 60
		   6 62
		   6 91 Stereo Marimba
		 */

	},
	// Path the Charts directory
	"/home/Dropbox/FusionBlue/index.html",
	// Number and name of the output ports
	6,
	{"Fluid", "Guitarix", "Looper", "Transport", "Tempo", "Click", "Pedal"},

	// Apps[MaxApps];
	// enum { AnalogApp = 0, MidiSoundApp, LooperApp, TransportApp, MidiApp, MP3Player, TabPlayer, MaxApps };
	{
		{"guitarix", 0},
		{"qsynth", 0},
		{"SooperLooper", 0},
		{"audacity", 0},
		{"LiveMusicApp", 0},
		{"clementine", 0},
		{"Tux", 0}
	},
	8,				/* TempoMax  */
	0,				/* Metronome On      */
	37,				/* Drum down beat, midi code. */
	44,				/* Drum Other beats */
	40,				/* Base Midi Note for switching.    */


	/* Begin Layouts Section.       */
	{
		{
			"Default",
			{
				/*			1				2				3				4				5					6 		*/
				"Preset1", "Preset2", "Preset3",
				"Preset4", "Chorus", "SatrianiComp",
// -------------------------------------------------------------------------------
				"CleanDist", "FunkyGuitar","PreTab", "ScoreTab","MedDist",
// -------------------------------------------------------------------------------
				"BetAhh", "AutoWah", "OctUp", 
				"Trumpet", "PrevSong", "NextSong",
// -------------------------------------------------------------------------------
				"Steel Drums", "Scroll", "Drums",
				"SinSymph", "Moog2", "MoogLead1",
// -------------------------------------------------------------------------------
				"LayoutSwitch", "Chorus", "SatrianiComp",
				"CleanBass", "MedDist", "OctBass",
// -------------------------------------------------------------------------------


				"BassSlap", "FunkBass", "EBell", "FunkyGuitar", "WahToggle",
				"Low Synth", "EBuzz", "SynthB11", "KStatB", "MS20Bass",
				"Juno", "Moog1", "Moog2", "BassSlap", "Taurus2",
				"Ultimate2a", "SynthB1", "FilterBass", "CZOrgan", "MoogLead1",
				"CZBrass4", "SinSymph", "Rak Eff1", "Rak Eff2", "Rak Eff3",
			},
		},

		{
			"Practice",
			{
				/*                      1                               2                               3                               4                               5                                       6               */
				"Preset1", "Preset2", "Preset3", "Chorus", "SatrianiComp", "FunkyGuitar",
				"TransStart", "TransCont", "TransStop", "TransBack", "MidiApp",
				"MP3Player",
				"LP 1", "LP 2", "LP 3", "LP Undo", "LP Rec", "LP Pause",
				"Drums", "RepeatA", "RepeatB", "MedDist", "OctBass", "LayoutSwitch",


				"Piano", "Honky", "Breath", "Midi Pre", "Flute",
				"Drums", "Slap Bass", "Marimba", "Breath", "Steel Drums",
				"Low Synth", "EBuzz", "SynthB11", "KStatB", "MS20Bass",
				"Juno", "Moog1", "Moog2", "BassSlap", "Taurus2",
				"Ultimate2a", "SynthB1", "FilterBass", "CZOrgan", "MoogLead1",
				"CZBrass4", "SinSymph", "Rak Eff1", "Rak Eff2", "Rak Eff3",
			},
		},

		{
			"Midi",
			{
				/*                      1                               2                               3                               4                               5                                       6               */
				"Piano", "Honky", "Breath", "Organ", "Flute",
				"SynthLead", "Trumpet", "Steel Drums", "Walky", "Drums",
				"Slap Bass", "Synth Hard", "Marimba", "Breath1", "Low Synth",
				"EBuzz", "SynthB11", "KStatB", "MS20Bass", "Juno",
				"Chorus", "HardDist", "CleanBass", "MedDist", "OctBass",
				"Moog1", "Moog2", "Preset1", "Preset2", "LayoutSwitch",
				"Piano", "Honky", "BassSlap", "Taurus2", "Ultimate2a",
				"SynthB1", "FilterBass", "BasBiy", "CZOrgan", "MoogLead1",
				"CZBrass4", "SinSymph", "BetAhh", "ToLate", "BellsComp",
				"Sax", "Vibra", "Mariba1", "Tenor", "Mid Screen",
				"Rak Eff6", "Rak Eff7", "Rak Eff8", "Rak Eff9", "Rak Eff10",
				"Rak Eff1", "Rak Eff2", "Rak Eff3", "Rak Eff4", "Rak Eff5",
			},
		},

		{
			"Rehearsal",
			{
				/*                      1                               2                               3                               4                               5                                       6               */
				"TransStart", "TransCont", "TransStop", "TransBack", "TransPos",
				"Tap", "Midi Tog", "Tuner", "AnaOnOff", "AnaOnOff",
				"LP Undo", "LP 1", "LP 2", "LP 3", "LP 4",
				"LP Rec", "LP Pause", "LP Trig", "LP Tap", "LP Add",
				"Chorus", "HardDist", "CleanBass", "MedDist", "OctBass",
				"LayoutSwitch", "NextSong", "Preset1", "Preset2", "PrevSong",
				"Piano", "Honky", "Breath", "Organ", "Flute",
				"SynthLead", "Strings", "Trumpet", "Steel Drums", "Walky",
				"Drums", "Slap Bass", "Synth Hard", "Marimba", "Breath",
				"Steel Drums", "Low Synth", "Mid0", "Mid-24", "Mid Screen",
				"NextSong", "PrevSong", "Rak Eff8", "Rak Eff9", "Rak Eff10",
				"Rak Eff1", "Rak Eff2", "Rak Eff3", "Rak Eff4", "Rak Eff5",
			},
		},

		{
			"Looper",
			{
				/*                      1                               2                               3                               4                               5                               6               */
				"LP Rec", "LP Trig", "LP Pause", "LP All", "LP Pause All", "LP Add",
				"LooperApp", "LiveApp",
				"LP 1", "LP 2", "LP 3", "LP 4", "LP All", "TrigCountIn",
				"LP Undo", "LP Tap", "LP Mute", "LP Undo", "LP Rec", "LP Pause",
				"EBuzz", "LP Mute", "OctBass", "LayoutSwitch",

				"Piano", "Honky", "Breath", "Midi Pre", "Flute",
				"Drums", "Slap Bass", "Marimba", "Breath", "Steel Drums",
				"Low Synth", "EBuzz", "SynthB11", "KStatB", "MS20Bass",
				"Juno", "Moog1", "Moog2", "BassSlap", "Taurus2",
				"Ultimate2a", "SynthB1", "FilterBass", "CZOrgan", "MoogLead1",
				"CZBrass4", "SinSymph", "Rak Eff1", "Rak Eff2", "Rak Eff3",
				//                      "Rak Eff6", "Rak Eff7", "Rak Eff8", "Rak Eff9", "Rak Eff10",
				//                      "Rak Eff1", "Rak Eff2", "Rak Eff3", "Rak Eff4", "Rak Eff5",
			},
		},

		{
			"Bass",
			{
				/*                      1                               2                               3                               4                               5                                       6               */
				"Preset1", "Preset2", "Preset3", "Chorus", "OctBass", "CleanBass",
				"FunkBass", "FunkyGuitar", "Breath", "Flute", "MS20Bass", "SatrianiComp",
				"LP 1", "LP 2", "LP 3", "LP Undo", "LP Rec", "LP Pause",
				"Drums", "Flute", "SatrianiComp", "MedDist", "OctBass", "LayoutSwitch",

				"Piano", "Honky", "Breath", "Midi Pre", "Flute",
				"Drums", "Slap Bass", "Marimba", "Breath", "Steel Drums",
				"Low Synth", "EBuzz", "SynthB11", "KStatB", "MS20Bass",
				"Juno", "Moog1", "Moog2", "BassSlap", "Taurus2",
				"Ultimate2a", "SynthB1", "FilterBass", "CZOrgan", "MoogLead1",
				"CZBrass4", "SinSymph", "Rak Eff1", "Rak Eff2", "Rak Eff3",
			},
		},

		{
			"Guitar",
			{
				/*                      1                               2                               3                               4                               5                                       6               */
				"Preset1", "Preset2", "Preset3", "Chorus", "SatrianiComp", "FunkyGuitar",
				"CleanDist", "MedDist", "Breath", "Flute", "SynthLead", "EBell",
				"Tenor", "Strings", "Vibra", "Sax", "AnalogApp", "LiveApp",
				"ToLate", "Flute", "Walky", "MedDist", "OctBass", "LayoutSwitch",

				"Piano", "Holds-Mel", "Holds-Hard", "Midi Pre", "Flute",
				"Drums", "Slap Bass", "Marimba", "Breath", "Steel Drums",
				"BellsComp", "EBuzz", "SynthB11", "KStatB", "MS20Bass",
				"Sax", "Vibra", "Moog2", "BassSlap", "Mariba1",
				"Ultimate2a", "SynthB1", "FilterBass", "CZOrgan", "MoogLead1",
				"CZBrass4", "SinSymph", "Rak Eff1", "Rak Eff2", "Rak Eff3",
			},
		},

		{
			"Transcribe",
			{
				/*                      1                               2                               3                               4                               5                                       6               */
				"Preset1", "Preset2", "Preset3", "Chorus", "NextSeq", "PrevSeg",
				"RepeatA", "RepeatB", "Loop Toggle", "PlPlay", "SpeeedUp", "SlowDown",
				"Seek Fw", "Seek Bk", "LP 3", "LP Undo", "LP Rec", "LP Pause",
				"Drums", "Flute", "SatrianiComp", "MedDist", "OctBass", "LayoutSwitch",

				"Piano", "Honky", "Breath", "Midi Pre", "Flute",
				"Drums", "Slap Bass", "Marimba", "Breath", "Steel Drums",
				"Low Synth", "EBuzz", "SynthB11", "KStatB", "MS20Bass",
				"Juno", "Moog1", "Moog2", "BassSlap", "Taurus2",
				"Ultimate2a", "SynthB1", "FilterBass", "CZOrgan", "MoogLead1",
				"CZBrass4", "SinSymph", "Rak Eff1", "Rak Eff2", "Rak Eff3",
			},
		},
		/*
		 * Terminator for Layouts.
		 */
		{
			"",
			{
			},
		},

	},
};
