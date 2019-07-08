LiveMusicInfo GlobalInfo = {
 	{
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*   0 */ {"BaseChorus",            NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "None" },
/*   1 */ {"SusDist",       NoBank         ,   1, GuitarixPort   ,   1, NoCustom  , "None" },
/*   2 */ {"ChorusUp",            NoBank         ,   7, GuitarixPort   ,   1, NoCustom  , "None" },
/*   3 */ {"HardDist",           NoBank         ,  2, GuitarixPort   ,   1, NoCustom  , "None" },
/*   5 */ {"Juicy",           NoBank         ,   6, GuitarixPort   ,   1, NoCustom  , "Mel Tog" },
/*   5 */ {"BaseMel",         NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "Mel Tog" },
/*   6 */ {"BaseWah",         NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "Wah Tog" },
/*   7 */ {"BaseDist",        NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "Dist Tog" },
/*   8 */ {"Wah Tog",         NoBank         ,  10, GuitarixPort   ,   1, cmdSendCC , "None" },
/*   9 */ {"Dist Tog",        NoBank         ,  11, GuitarixPort   ,   1, cmdSendCC , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  11 */ {"Mel Tog",         NoBank         ,  13, GuitarixPort   ,   1, cmdSendCC , "None" },
/*  12 */ {"Expr Vol",        NoBank         ,   7, GuitarixPort   ,   1, cmdSetExpr, "None" },
/*  13 */ {"Expr Wah",        NoBank         ,  11, GuitarixPort   ,   1, cmdSetExpr, "None" },
/*  14 */ {"Expr Dist",       NoBank         ,   2, GuitarixPort   ,   1, cmdSetExpr, "None" },
/*  15 */ {"Transport ****",  NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  16 */ {"TransStart",      NoBank         ,   0, TransportPort  ,   1, TransStart, "None" },
/*  17 */ {"TransCont",       NoBank         ,   0, TransportPort  ,   1, TransCont , "None" },
/*  18 */ {"TransStop",       NoBank         ,   0, TransportPort  ,   1, TransStop , "None" },
/*  19 */ {"TransBack",       NoBank         ,   0, TransportPort  ,   1, TransStart, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  20 */ {"TransPos",        NoBank         ,   0, TransportPort  ,   1, TransStart, "None" },
/*  21 */ {"Tap",             NoBank         , 121, InternalPort   ,   1, Controller, "None" },
/*  22 */ {"Midi Tog",        NoBank         , 122, FluidPort      ,   1, Controller, "None" },
/*  23 */ {"Tuner",           NoBank         , 123, FluidPort      ,   1, Controller, "None" },
/*  24 */ {"AnaOnOff",        NoBank         , 124, FluidPort      ,   1, Controller, "None" },
/*  25 */ {"Loop***",         NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  26 */ {"TrigCountIn",     NoBank         ,   4, InternalPort   ,   1, cmdCountIn, "LooperApp" },
/*  27 */ {"LP Undo",         NoBank         ,   5, InternalPort   ,   1, cmdOSC    , "None" },
/*  28 */ {"LP 1",            0x01           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  29 */ {"LP 2",            0x02           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  30 */ {"LP 3",            0x03           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  31 */ {"LP 4",            0x04           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  32 */ {"LP Rec",          NoBank         ,   2, InternalPort   ,   1, cmdOSC    , "None" },
/*  33 */ {"LP Pause",        NoBank         ,   3, InternalPort   ,   1, cmdOSC    , "None" },
/*  34 */ {"LP Trig",         NoBank         ,   4, InternalPort   ,   1, cmdOSC    , "None" },
/*  35 */ {"LP Tap",          NoBank         ,  77, InternalPort   ,   1, cmdOSC    , "None" },
/*  36 */ {"LP Add",          NoBank         ,   6, InternalPort   ,   1, cmdOSC    , "None" },
/*  37 */ {"App ***",         NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  38 */ {"Loop Toggle",     NoBank         ,   3, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  39 */ {"LayoutSwitch",    NoBank         ,   0, InternalPort   ,   1, cmdBankSelect, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  40 */ {"AnalogApp",       NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  41 */ {"MidiSoundApp",    NoBank         ,   1, InternalPort   ,   1, RaiseApp  , "None" },
/*  42 */ {"LiveApp",         NoBank         ,   4, InternalPort   ,   1, RaiseApp  , "None" },
/*  43 */ {"LooperApp",       NoBank         ,   2, InternalPort   ,   1, RaiseApp  , "None" },
/*  44 */ {"MP3Player",       NoBank         ,   5, InternalPort   ,   1, RaiseApp  , "None" },
/*  45 */ {"TabPlayer",       NoBank         ,   6, InternalPort   ,   1, RaiseApp  , "None" },
/*  46 */ {"PreTab",          NoBank         ,   0, InternalPort   ,   1, SwitchTab , "LiveApp" },
/*  47 */ {"ScoreTab",        NoBank         ,   1, InternalPort   ,   1, SwitchTab , "LiveApp" },
/*  48 */ {"TransportApp",    NoBank         ,   3, InternalPort   ,   1, RaiseApp  , "None" },
/*  49 */ {"RepeatA",         NoBank         ,   0, InternalPort   ,   1, cmdLnTransPort, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  50 */ {"RepeatB",         NoBank         ,   1, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  51 */ {"NextSeq",         NoBank         ,   4, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  52 */ {"PrevSeg",         NoBank         ,   5, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  53 */ {"SpeeedUp",        NoBank         ,   6, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  54 */ {"SlowDown",        NoBank         ,   7, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  55 */ {"NextSong",        NoBank         ,   2, InternalPort   ,   1, cmdSetList, "None" },
/*  56 */ {"PrevSong",        NoBank         ,   1, InternalPort   ,   1, cmdSetList, "None" },
/*  57 */ {"Scroll",          NoBank         ,   0, InternalPort   ,   1, cmdScroll , "None" },
/*  58 */ {"Global ****",     NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  59 */ {"Switch0",         NoBank         ,   0, InternalPort   ,   1, ToDesktop , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  60 */ {"Switch1",         NoBank         ,   1, InternalPort   ,   1, ToDesktop , "None" },
/*  61 */ {"Switch2",         NoBank         ,   2, InternalPort   ,   1, ToDesktop , "None" },
/*  62 */ {"Switch3",         NoBank         ,   3, InternalPort   ,   1, ToDesktop , "None" },
/*  63 */ {"Switch4",         NoBank         ,   4, InternalPort   ,   1, ToDesktop , "None" },
/*  64 */ {"Switch5",         NoBank         ,   5, InternalPort   ,   1, ToDesktop , "None" },
/*  65 */ {"VolUp",           NoBank         ,   5, InternalPort   ,   1, cmdVolume , "None" },
/*  66 */ {"VolDown",         NoBank         ,  -5, InternalPort   ,   1, cmdVolume , "None" },
/*  67 */ {"PlPlay",          NoBank         ,   2, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  68 */ {"Seek Fw",         NoBank         ,   8, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  69 */ {"Seek Bk",         NoBank         ,   9, InternalPort   ,   1, cmdLnTransPort, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  70 */ {"LP All",          NoBank         ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  71 */ {"LP Mute",         NoBank         ,   7, InternalPort   ,   1, cmdOSC    , "None" },
/*  72 */ {"Preset1",         NoBank         ,   1, InternalPort   ,   1, cmdPreset , "None" },
/*  73 */ {"Preset2",         NoBank         ,   2, InternalPort   ,   1, cmdPreset , "None" },
/*  74 */ {"Preset3",         NoBank         ,   3, InternalPort   ,   1, cmdPreset , "None" },
/*  75 */ {"Preset4",         NoBank         ,   4, InternalPort   ,   1, cmdPreset , "None" },
/*  76 */ {"Preset5",         NoBank         ,   5, InternalPort   ,   1, cmdPreset , "None" },
/*  77 */ {"Preset6",         NoBank         ,   6, InternalPort   ,   1, cmdPreset , "None" },
/*  78 */ {"MIDI***",         NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  79 */ {"Mid0",            NoBank         ,   0, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  80 */ {"Mid-24",          NoBank         ,   2, FluidPort      ,   1, NoCustom  , "None" },
/*  81 */ {"Inst M Sel",      NoBank         ,   0, InternalPort   ,   1, cmdMidiSelect, "None" },
/*  82 */ {"Piano",           SFDSF          ,  63, FluidPort      ,   1, NoCustom  , "None" },
/*  83 */ {"Honky",           SFFluidGM      ,   3, FluidPort      ,   1, NoCustom  , "None" },
/*  84 */ {"Breath",          SFFluidGM      ,  53, FluidPort      ,   1, NoCustom  , "None" },
/*  85 */ {"Organ",           SFFluidGM      ,  16, FluidPort      ,   1, NoCustom  , "None" },
/*  86 */ {"Flute",           SFFlute        ,  73, FluidPort      ,   1, NoCustom  , "None" },
/*  87 */ {"SynthLead",       SFDSF          ,  43, FluidPort      ,   1, NoCustom  , "None" },
/*  88 */ {"Strings",         SFDSF          ,  85, FluidPort      ,   1, NoCustom  , "None" },
/*  89 */ {"Steel Drums",     SFFluidGM      , 114, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  90 */ {"Walky",           SFDSF          ,   3, FluidPort      ,   1, NoCustom  , "None" },
/*  91 */ {"Drums",           NoBank         ,   1, FluidPort      ,   1, NoCustom  , "None" },
/*  92 */ {"Slap Bass",       SFDSF          ,  24, FluidPort      ,   1, NoCustom  , "None" },
/*  93 */ {"Synth Hard",      SFDSF          ,  60, FluidPort      ,   1, NoCustom  , "None" },
/*  94 */ {"Marimba",         SFDSF          ,  12, FluidPort      ,   1, NoCustom  , "None" },
/*  95 */ {"Breath1",         SFDSF          ,  48, FluidPort      ,   1, NoCustom  , "None" },
/*  96 */ {"Low Synth",       SFDSF          ,  45, FluidPort      ,   1, NoCustom  , "None" },
/*  97 */ {"NiceAhh",         SFDSF          ,  90, FluidPort      ,   1, NoCustom  , "None" },
/*  98 */ {"BetAhh",          SFVintage      ,  63, FluidPort      ,   1, NoCustom  , "None" },
/*  99 */ {"ToLate",          SFVintage      , 108, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 100 */ {"BellsComp",       SFDX7          ,  10, FluidPort      ,   1, NoCustom  , "None" },
/* 101 */ {"Sax",             SFSonidoGM     ,  66, FluidPort      ,   1, NoCustom  , "None" },
/* 102 */ {"Vibra",           SFVintage      ,  93, FluidPort      ,   1, NoCustom  , "None" },
/* 103 */ {"Mariba1",         SFDX7          ,  91, FluidPort      ,   1, NoCustom  , "None" },
/* 104 */ {"Tenor",           SFRolandTenor  ,   1, FluidPort      ,   1, NoCustom  , "None" },
/* 105 */ {"EBuzz",           SFElias        ,   0, FluidPort      ,   1, NoCustom  , "None" },
/* 106 */ {"SynthB11",        SFElias        ,   1, FluidPort      ,   1, NoCustom  , "None" },
/* 107 */ {"KStatB",          SFElias        ,   2, FluidPort      ,   1, NoCustom  , "None" },
/* 108 */ {"MS20Bass",        SFElias        ,   3, FluidPort      ,   1, NoCustom  , "None" },
/* 109 */ {"Juno",            SFElias        ,   4, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 110 */ {"Moog1",           SFElias        ,   9, FluidPort      ,   1, NoCustom  , "None" },
/* 111 */ {"Moog2",           SFElias        ,  10, FluidPort      ,   1, NoCustom  , "None" },
/* 112 */ {"BassSlap",        SFElias        ,  13, FluidPort      ,   1, NoCustom  , "None" },
/* 113 */ {"Taurus2",         SFElias        ,  26, FluidPort      ,   1, NoCustom  , "None" },
/* 114 */ {"Ultimate2a",      SFElias        ,  33, FluidPort      ,   1, NoCustom  , "None" },
/* 115 */ {"SynthB1",         SFElias        ,  38, FluidPort      ,   1, NoCustom  , "None" },
/* 116 */ {"FilterBass",      SFElias        ,  39, FluidPort      ,   1, NoCustom  , "None" },
/* 117 */ {"BasBiy",          SFElias        ,  45, FluidPort      ,   1, NoCustom  , "None" },
/* 118 */ {"CZOrgan",         SFElias        ,  51, FluidPort      ,   1, NoCustom  , "None" },
/* 119 */ {"MoogLead1",       SFElias        ,  53, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 120 */ {"CZBrass4",        SFElias        ,  64, FluidPort      ,   1, NoCustom  , "None" },
/* 121 */ {"SinSymph",        SFElias        ,  90, FluidPort      ,   1, NoCustom  , "None" },
/* 122 */ {"",                NoBank         ,   0, FluidPort      ,   1, NoCustom  , "None" },
/* 123 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 124 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 125 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 126 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 127 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 128 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 129 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 131 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 132 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 133 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 134 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 135 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 136 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 137 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 138 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 139 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 140 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 141 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 142 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 143 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 144 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 145 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 146 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 147 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 148 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 149 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 150 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 151 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 152 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 153 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 154 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 155 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 156 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 157 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 158 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 159 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 160 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 161 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 162 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 163 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 164 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 165 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 166 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 167 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 168 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 169 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 170 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 171 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 172 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 173 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 174 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 175 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 176 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 177 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 178 */ {"Guitar V",        NoBank         ,   7, GuitarixPort   ,   1, cmdSetExpr, "None" },
/* 179 */ {"Midi V",          NoBank         ,   7, FluidPort      ,   1, cmdSetExpr, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 180 */ {"Master V",        NoBank         ,   7, InternalPort   ,   1, cmdSetExpr, "None" },
/* 181 */ {"Wah P",           NoBank         ,   2, GuitarixPort   ,   1, cmdSetExpr, "None" },
},
"/home/Dropbox/FusionBlue/index.html",
6, 
 { 
"Fluid",
"Guitarix",
"Looper",
"Transport",
"Tempo",
"Click",
"Pedal"
},
{
{ "guitarix", 0 },
{ "qsynth", 0 },
{ "SooperLooper", 0 },
{ "audacity", 0 },
{ "LiveMusicApp", 0 },
{ "clementine", 0 },
{ "Tux", 0 }
},
 8, /* TempoMax  */
 0, /* Metronome On  */
 37, /* Drum down beat, midi code  */
 44, /* Drum Other beats  */
 45, /* Base Midi Note for switching  */
 120, /* Tempo   */
 8, /* CountInBeats   */
 16, /* LoopRecBeats   */
 1, /* MidiPassThru   */
 110, /* MidiPassLevel   */
 4, /* BeatsPerMeasure   */
 100, /* AnalogVolume   */
 80, /* MidiVolume   */
 0xffffe0, /* StatusTextColor   */
 0xffe0e0, /* ButtonTextColor   */
 "127.0.0.1", /* OSCIPAddress   */
 "9951", /* OSCPortNumLooper   */
 "9952", /* OSCPortNumJackVol   */
 "9000", /* OSCPortNumHydrogen   */
{ 
 	{
 "Guitar", 
{
/*   0 */  "Preset1", 
/*   1 */  "Preset2", 
/*   2 */  "Preset3", 
/*   3 */  "Base", 
/*   4 */  "BaseChorus", 
/*   5 */  "BaseMel", 
/*   6 */  "BaseWah", 
/*   7 */  "BaseDist", 
/*   8 */  "ScoreTab", 
/*   9 */  "PreTab", 
/*  10 */  "AnalogApp", 
/*  11 */  "PrevSong", 
/*  12 */  "NextSong", 
/*  13 */  "Wah Tog", 
/*  14 */  "Dist Tog", 
/*  15 */  "Mel Tog", 
/*  16 */  "AnalogApp", 
/*  17 */  "LiveApp", 
/*  18 */  "ToLate", 
/*  19 */  "MedDist", 
/*  20 */  "BaseMel", 
/*  21 */  "LayoutSwitch", 
/*  22 */  "Inst M Sel", 
/*  23 */  "Expr Vol", 
/*  24 */  "Expr Wah", 
/*  25 */  "Expr Dist", 
/*  26 */  "Guitar V", 
/*  27 */  "Midi V", 
/*  28 */  "Master V", 
/*  29 */  "Wah P", 
/*  30 */  "Piano", 
/*  31 */  "Honky", 
/*  32 */  "Breath", 
/*  33 */  "Organ", 
/*  34 */  "Flute", 
/*  35 */  "SynthLead", 
/*  36 */  "Trumpet", 
/*  37 */  "Steel Drums", 
/*  38 */  "Walky", 
/*  39 */  "Drums", 
/*  40 */  "Slap Bass", 
/*  41 */  "Synth Hard", 
/*  42 */  "Marimba", 
/*  43 */  "Breath1", 
/*  44 */  "Low Synth", 
/*  45 */  "EBuzz", 
/*  46 */  "SynthB11", 
/*  47 */  "KStatB", 
/*  48 */  "MS20Bass", 
/*  49 */  "Juno", 
/*  50 */  "Moog1", 
/*  51 */  "Moog2", 
},}, 
	{
 "Midi", 
{
/*   0 */  "Piano", 
/*   1 */  "Honky", 
/*   2 */  "Breath", 
/*   3 */  "Organ", 
/*   4 */  "Flute", 
/*   5 */  "SynthLead", 
/*   6 */  "Trumpet", 
/*   7 */  "Steel Drums", 
/*   8 */  "Walky", 
/*   9 */  "Drums", 
/*  10 */  "Slap Bass", 
/*  11 */  "Synth Hard", 
/*  12 */  "Marimba", 
/*  13 */  "Breath1", 
/*  14 */  "Low Synth", 
/*  15 */  "EBuzz", 
/*  16 */  "SynthB11", 
/*  17 */  "KStatB", 
/*  18 */  "MS20Bass", 
/*  19 */  "Juno", 
/*  20 */  "BaseChorus", 
/*  21 */  "HardDist", 
/*  22 */  "SusDist", 
/*  23 */  "MedDist", 
/*  24 */  "BaseMel", 
/*  25 */  "Moog1", 
/*  26 */  "Moog2", 
/*  27 */  "Preset1", 
/*  28 */  "Preset2", 
/*  29 */  "LayoutSwitch", 
/*  30 */  "Piano", 
/*  31 */  "Honky", 
/*  32 */  "BassSlap", 
/*  33 */  "Taurus2", 
/*  34 */  "Ultimate2a", 
/*  35 */  "SynthB1", 
/*  36 */  "FilterBass", 
/*  37 */  "BasBiy", 
/*  38 */  "CZOrgan", 
/*  39 */  "MoogLead1", 
/*  40 */  "CZBrass4", 
/*  41 */  "SinSymph", 
/*  42 */  "BetAhh", 
/*  43 */  "ToLate", 
/*  44 */  "BellsComp", 
/*  45 */  "Sax", 
/*  46 */  "Vibra", 
/*  47 */  "Mariba1", 
/*  48 */  "Tenor", 
/*  49 */  "Mid Screen", 
/*  50 */  "Rak Eff6", 
/*  51 */  "Rak Eff7", 
/*  52 */  "Rak Eff8", 
/*  53 */  "Rak Eff9", 
/*  54 */  "Rak Eff10", 
/*  55 */  "Rak Eff1", 
/*  56 */  "Rak Eff2", 
/*  57 */  "Rak Eff3", 
/*  58 */  "Rak Eff4", 
/*  59 */  "Rak Eff5", 
},}, 
	{
 "Looper", 
{
/*   0 */  "LP Rec", 
/*   1 */  "LP Trig", 
/*   2 */  "LP Pause", 
/*   3 */  "LP All", 
/*   4 */  "LP Pause All", 
/*   5 */  "LP Add", 
/*   6 */  "LooperApp", 
/*   7 */  "LiveApp", 
/*   8 */  "LP 1", 
/*   9 */  "LP 2", 
/*  10 */  "LP 3", 
/*  11 */  "LP 4", 
/*  12 */  "LP All", 
/*  13 */  "TrigCountIn", 
/*  14 */  "LP Undo", 
/*  15 */  "LP Tap", 
/*  16 */  "LP Mute", 
/*  17 */  "LP Undo", 
/*  18 */  "LP Rec", 
/*  19 */  "LP Pause", 
/*  20 */  "LP Mute", 
/*  21 */  "LayoutSwitch", 
},}, 
	{
 "Transcribe", 
{
/*   0 */  "Preset1", 
/*   1 */  "Preset2", 
/*   2 */  "Preset3", 
/*   3 */  "BaseChorus", 
/*   4 */  "NextSeq", 
/*   5 */  "PrevSeg", 
/*   6 */  "RepeatA", 
/*   7 */  "RepeatB", 
/*   8 */  "Loop Toggle", 
/*   9 */  "PlPlay", 
/*  10 */  "SpeeedUp", 
/*  11 */  "SlowDown", 
/*  12 */  "Seek Fw", 
/*  13 */  "Seek Bk", 
/*  14 */  "LP 3", 
/*  15 */  "LP Undo", 
/*  16 */  "LP Rec", 
/*  17 */  "LP Pause", 
/*  18 */  "Drums", 
/*  19 */  "Flute", 
/*  20 */  "BaseDist", 
/*  21 */  "MedDist", 
/*  22 */  "BaseMel", 
/*  23 */  "LayoutSwitch", 
/*  24 */  "Piano", 
/*  25 */  "Honky", 
/*  26 */  "Breath", 
/*  27 */  "Inst M Sel", 
/*  28 */  "Flute", 
/*  29 */  "Drums", 
/*  30 */  "Slap Bass", 
/*  31 */  "Marimba", 
/*  32 */  "Breath", 
/*  33 */  "Steel Drums", 
/*  34 */  "Low Synth", 
/*  35 */  "EBuzz", 
/*  36 */  "SynthB11", 
/*  37 */  "KStatB", 
/*  38 */  "MS20Bass", 
/*  39 */  "Juno", 
/*  40 */  "Moog1", 
/*  41 */  "Moog2", 
/*  42 */  "BassSlap", 
/*  43 */  "Taurus2", 
/*  44 */  "Ultimate2a", 
/*  45 */  "SynthB1", 
/*  46 */  "FilterBass", 
/*  47 */  "CZOrgan", 
/*  48 */  "MoogLead1", 
/*  49 */  "CZBrass4", 
/*  50 */  "SinSymph", 
/*  51 */  "Rak Eff1", 
/*  52 */  "Rak Eff2", 
/*  53 */  "Rak Eff3", 
},}, 
{
 "", { },	},
 },
}; 
