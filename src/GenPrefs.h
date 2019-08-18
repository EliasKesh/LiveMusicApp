LiveMusicInfo GlobalInfo = {
 	{
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*   0 */ {"BaseChorus",      NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "None" },
/*   1 */ {"SusDist",         NoBank         ,   1, GuitarixPort   ,   1, NoCustom  , "None" },
/*   2 */ {"Jazz",            NoBank         ,   2, GuitarixPort   ,   1, NoCustom  , "None" },
/*   3 */ {"Bell",            NoBank         ,   3, GuitarixPort   ,   1, NoCustom  , "None" },
/*   4 */ {"EchoLead",        NoBank         ,   4, GuitarixPort   ,   1, NoCustom  , "None" },
/*   5 */ {"CompLead",        NoBank         ,   5, GuitarixPort   ,   1, NoCustom  , "None" },
/*   6 */ {"Hard",            NoBank         ,   6, GuitarixPort   ,   1, NoCustom  , "None" },
/*   7 */ {"FlangeDist",      NoBank         ,   7, GuitarixPort   ,   1, NoCustom  , "None" },
/*   8 */ {"FunkyGuitar",     NoBank         ,   8, GuitarixPort   ,   1, NoCustom  , "None" },
/*   9 */ {"BaseMel",         NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "Mel Tog" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  10 */ {"BaseWah",         NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "Wah Tog" },
/*  11 */ {"BaseDist",        NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "Dist Tog" },
/*  12 */ {"Wah Tog",         NoBank         ,  10, GuitarixPort   ,   1, cmdSendCC , "None" },
/*  13 */ {"Dist Tog",        NoBank         ,  11, GuitarixPort   ,   1, cmdSendCC , "None" },
/*  14 */ {"Mel Tog",         NoBank         ,  13, GuitarixPort   ,   1, cmdSendCC , "None" },
/*  15 */ {"Expr Vol",        NoBank         ,   7, GuitarixPort   ,   1, cmdSetExpr, "None" },
/*  16 */ {"Expr Wah",        NoBank         ,  11, GuitarixPort   ,   1, cmdSetExpr, "None" },
/*  17 */ {"Expr Dist",       NoBank         ,   2, GuitarixPort   ,   1, cmdSetExpr, "None" },
/*  18 */ {"Transport ****",  NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  19 */ {"TransStart",      NoBank         ,   0, TransportPort  ,   1, TransStart, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  20 */ {"TransCont",       NoBank         ,   0, TransportPort  ,   1, TransCont , "None" },
/*  21 */ {"TransStop",       NoBank         ,   0, TransportPort  ,   1, TransStop , "None" },
/*  22 */ {"TransBack",       NoBank         ,   0, TransportPort  ,   1, TransStart, "None" },
/*  23 */ {"Tap",             NoBank         , 121, InternalPort   ,   1, Controller, "None" },
/*  24 */ {"TransPos",        NoBank         ,   0, TransportPort  ,   1, TransStart, "None" },
/*  25 */ {"Tap",             NoBank         , 121, InternalPort   ,   1, Controller, "None" },
/*  26 */ {"Midi Tog",        NoBank         , 122, FluidPort      ,   1, Controller, "None" },
/*  27 */ {"Tuner",           NoBank         , 123, FluidPort      ,   1, Controller, "None" },
/*  28 */ {"AnaOnOff",        NoBank         , 124, FluidPort      ,   1, Controller, "None" },
/*  29 */ {"Loop***",         NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  30 */ {"TrigCountIn",     NoBank         ,   4, InternalPort   ,   1, cmdCountIn, "LooperApp" },
/*  31 */ {"LP Undo",         NoBank         ,   5, InternalPort   ,   1, cmdOSC    , "None" },
/*  32 */ {"LP 1",            0x01           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  33 */ {"LP 2",            0x02           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  34 */ {"LP 3",            0x03           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  35 */ {"LP 4",            0x04           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  36 */ {"LP Rec",          NoBank         ,   2, InternalPort   ,   1, cmdOSC    , "None" },
/*  37 */ {"LP Pause",        NoBank         ,   3, InternalPort   ,   1, cmdOSC    , "None" },
/*  38 */ {"LP Trig",         NoBank         ,   4, InternalPort   ,   1, cmdOSC    , "None" },
/*  39 */ {"LP Tap",          NoBank         ,  77, InternalPort   ,   1, cmdOSC    , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  40 */ {"LP Add",          NoBank         ,   6, InternalPort   ,   1, cmdOSC    , "None" },
/*  41 */ {"LP All",          NoBank         ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  42 */ {"LP Mute",         NoBank         ,   7, InternalPort   ,   1, cmdOSC    , "None" },
/*  43 */ {"Loop Toggle",     NoBank         ,   3, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  44 */ {"LayoutSwitch",    NoBank         ,   0, InternalPort   ,   1, cmdBankSelect, "None" },
/*  45 */ {"AnalogApp",       NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  46 */ {"MidiSoundApp",    NoBank         ,   1, InternalPort   ,   1, RaiseApp  , "None" },
/*  47 */ {"LiveApp",         NoBank         ,   4, InternalPort   ,   1, RaiseApp  , "None" },
/*  48 */ {"LooperApp",       NoBank         ,   2, InternalPort   ,   1, RaiseApp  , "None" },
/*  49 */ {"MP3Player",       NoBank         ,   5, InternalPort   ,   1, RaiseApp  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  50 */ {"TabPlayer",       NoBank         ,   6, InternalPort   ,   1, RaiseApp  , "None" },
/*  51 */ {"PreTab",          NoBank         ,   0, InternalPort   ,   1, SwitchTab , "LiveApp" },
/*  52 */ {"ScoreTab",        NoBank         ,   1, InternalPort   ,   1, SwitchTab , "LiveApp" },
/*  53 */ {"TransportApp",    NoBank         ,   3, InternalPort   ,   1, RaiseApp  , "None" },
/*  54 */ {"RepeatA",         NoBank         ,   0, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  55 */ {"RepeatB",         NoBank         ,   1, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  56 */ {"NextSeq",         NoBank         ,   4, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  57 */ {"PrevSeg",         NoBank         ,   5, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  58 */ {"SpeeedUp",        NoBank         ,   6, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  59 */ {"SlowDown",        NoBank         ,   7, InternalPort   ,   1, cmdLnTransPort, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  60 */ {"NextSong",        NoBank         ,   2, InternalPort   ,   1, cmdSetList, "None" },
/*  61 */ {"PrevSong",        NoBank         ,   1, InternalPort   ,   1, cmdSetList, "None" },
/*  62 */ {"Scroll",          NoBank         ,   0, InternalPort   ,   1, cmdScroll , "None" },
/*  63 */ {"Global ****",     NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  64 */ {"Switch0",         NoBank         ,   0, InternalPort   ,   1, ToDesktop , "None" },
/*  65 */ {"Switch1",         NoBank         ,   1, InternalPort   ,   1, ToDesktop , "None" },
/*  66 */ {"Switch2",         NoBank         ,   2, InternalPort   ,   1, ToDesktop , "None" },
/*  67 */ {"Switch3",         NoBank         ,   3, InternalPort   ,   1, ToDesktop , "None" },
/*  68 */ {"Switch4",         NoBank         ,   4, InternalPort   ,   1, ToDesktop , "None" },
/*  69 */ {"Switch5",         NoBank         ,   5, InternalPort   ,   1, ToDesktop , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  70 */ {"VolUp",           NoBank         ,   5, InternalPort   ,   1, cmdVolume , "None" },
/*  71 */ {"VolDown",         NoBank         ,  -5, InternalPort   ,   1, cmdVolume , "None" },
/*  72 */ {"PlPlay",          NoBank         ,   2, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  73 */ {"Seek Fw",         NoBank         ,   8, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  74 */ {"Seek Bk",         NoBank         ,   9, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  75 */ {"Preset1",         NoBank         ,   1, InternalPort   ,   1, cmdPreset , "None" },
/*  76 */ {"Preset2",         NoBank         ,   2, InternalPort   ,   1, cmdPreset , "None" },
/*  77 */ {"Preset3",         NoBank         ,   3, InternalPort   ,   1, cmdPreset , "None" },
/*  78 */ {"Preset4",         NoBank         ,   4, InternalPort   ,   1, cmdPreset , "None" },
/*  79 */ {"Preset5",         NoBank         ,   5, InternalPort   ,   1, cmdPreset , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  80 */ {"Preset6",         NoBank         ,   6, InternalPort   ,   1, cmdPreset , "None" },
/*  81 */ {"MIDI***",         NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  82 */ {"Mid0",            NoBank         ,   0, FluidPort      ,   1, NoCustom  , "None" },
/*  83 */ {"Mid-24",          NoBank         ,   2, FluidPort      ,   1, NoCustom  , "None" },
/*  84 */ {"Inst M Sel",      NoBank         ,   0, InternalPort   ,   1, cmdMidiSelect, "None" },
/*  85 */ {"Piano",           SFDSF          ,  63, FluidPort      ,   1, NoCustom  , "None" },
/*  86 */ {"Honky",           SFFluidGM      ,   3, FluidPort      ,   1, NoCustom  , "None" },
/*  87 */ {"Breath",          SFFluidGM      ,  53, FluidPort      ,   1, NoCustom  , "None" },
/*  88 */ {"Organ",           SFFluidGM      ,  16, FluidPort      ,   1, NoCustom  , "None" },
/*  89 */ {"Flute",           SFFlute        ,  73, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  90 */ {"SynthLead",       SFDSF          ,  43, FluidPort      ,   1, NoCustom  , "None" },
/*  91 */ {"Strings",         SFDSF          ,  85, FluidPort      ,   1, NoCustom  , "None" },
/*  92 */ {"Steel Drums",     SFFluidGM      , 114, FluidPort      ,   1, NoCustom  , "None" },
/*  93 */ {"Walky",           SFDSF          ,   3, FluidPort      ,   1, NoCustom  , "None" },
/*  94 */ {"Drums",           NoBank         ,   1, FluidPort      ,   1, NoCustom  , "None" },
/*  95 */ {"Slap Bass",       SFDSF          ,  24, FluidPort      ,   1, NoCustom  , "None" },
/*  96 */ {"Synth Hard",      SFDSF          ,  60, FluidPort      ,   1, NoCustom  , "None" },
/*  97 */ {"Marimba",         SFDSF          ,  12, FluidPort      ,   1, NoCustom  , "None" },
/*  98 */ {"Breath1",         SFDSF          ,  48, FluidPort      ,   1, NoCustom  , "None" },
/*  99 */ {"Low Synth",       SFDSF          ,  45, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 100 */ {"NiceAhh",         SFDSF          ,  90, FluidPort      ,   1, NoCustom  , "None" },
/* 101 */ {"BetAhh",          SFVintage      ,  63, FluidPort      ,   1, NoCustom  , "None" },
/* 102 */ {"ToLate",          SFVintage      , 108, FluidPort      ,   1, NoCustom  , "None" },
/* 103 */ {"BellsComp",       SFDX7          ,  10, FluidPort      ,   1, NoCustom  , "None" },
/* 104 */ {"Sax",             SFSonidoGM     ,  66, FluidPort      ,   1, NoCustom  , "None" },
/* 105 */ {"Vibra",           SFVintage      ,  93, FluidPort      ,   1, NoCustom  , "None" },
/* 106 */ {"Mariba1",         SFDX7          ,  91, FluidPort      ,   1, NoCustom  , "None" },
/* 107 */ {"Tenor",           SFRolandTenor  ,   1, FluidPort      ,   1, NoCustom  , "None" },
/* 108 */ {"EBuzz",           SFElias        ,   0, FluidPort      ,   1, NoCustom  , "None" },
/* 109 */ {"SynthB11",        SFElias        ,   1, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 110 */ {"KStatB",          SFElias        ,   2, FluidPort      ,   1, NoCustom  , "None" },
/* 111 */ {"MS20Bass",        SFElias        ,   3, FluidPort      ,   1, NoCustom  , "None" },
/* 112 */ {"Juno",            SFElias        ,   4, FluidPort      ,   1, NoCustom  , "None" },
/* 113 */ {"Moog1",           SFElias        ,   9, FluidPort      ,   1, NoCustom  , "None" },
/* 114 */ {"Moog2",           SFElias        ,  10, FluidPort      ,   1, NoCustom  , "None" },
/* 115 */ {"BassSlap",        SFElias        ,  13, FluidPort      ,   1, NoCustom  , "None" },
/* 116 */ {"Taurus2",         SFElias        ,  26, FluidPort      ,   1, NoCustom  , "None" },
/* 117 */ {"Ultimate2a",      SFElias        ,  33, FluidPort      ,   1, NoCustom  , "None" },
/* 118 */ {"SynthB1",         SFElias        ,  38, FluidPort      ,   1, NoCustom  , "None" },
/* 119 */ {"FilterBass",      SFElias        ,  39, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 120 */ {"BasBiy",          SFElias        ,  45, FluidPort      ,   1, NoCustom  , "None" },
/* 121 */ {"CZOrgan",         SFElias        ,  51, FluidPort      ,   1, NoCustom  , "None" },
/* 122 */ {"MoogLead1",       SFElias        ,  53, FluidPort      ,   1, NoCustom  , "None" },
/* 123 */ {"CZBrass4",        SFElias        ,  64, FluidPort      ,   1, NoCustom  , "None" },
/* 124 */ {"SinSymph",        SFElias        ,  90, FluidPort      ,   1, NoCustom  , "None" },
/* 125 */ {"Guitar V",        NoBank         ,   7, GuitarixPort   ,   0, cmdHardSlider, "None" },
/* 126 */ {"Midi V",          NoBank         ,   7, FluidPort      ,   1, cmdHardSlider, "None" },
/* 127 */ {"Master V",        NoBank         ,   7, InternalPort   ,   2, cmdHardSlider, "None" },
/* 128 */ {"Expr P",          NoBank         ,   2, GuitarixPort   ,   3, cmdHardSlider, "None" },
/* 129 */ {"",                NoBank         ,   0, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 130 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
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
/* 178 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 179 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 180 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
/* 181 */ {"",                NoBank         ,   0, FluidPort      ,   0, NoCustom  , "" },
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
 50, /* Base Midi Note for switching  */
 120, /* Tempo   */
 8, /* CountInBeats   */
 16, /* LoopRecBeats   */
 1, /* MidiPassThru   */
 110, /* MidiPassLevel   */
 4, /* BeatsPerMeasure   */
 100, /* AnalogVolume   */
 100, /* MidiVolume   */
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
/*   3 */  "BaseChorus", 
/*   4 */  "SusDist", 
/*   5 */  "Jazz", 
/*   6 */  "Bell", 
/*   7 */  "EchoLead", 
/*   8 */  "CompLead", 
/*   9 */  "Hard", 
/*  10 */  "FlangeDist", 
/*  11 */  "FunkyGuitar", 
/*  12 */  "BaseMel", 
/*  13 */  "BaseWah", 
/*  14 */  "BaseDist", 
/*  15 */  "Wah Tog", 
/*  16 */  "Dist Tog", 
/*  17 */  "Mel Tog", 
/*  18 */  "Expr Vol", 
/*  19 */  "Expr Wah", 
/*  20 */  "Expr Dist", 
/*  21 */  "Guitar V", 
/*  22 */  "Midi V", 
/*  23 */  "Master V", 
/*  24 */  "TransCont", 
/*  25 */  "TransStop", 
/*  26 */  "TransBack", 
/*  27 */  "Tap", 
/*  28 */  "TransPos", 
/*  29 */  "Tap", 
/*  30 */  "Midi Tog", 
/*  31 */  "Tuner", 
/*  32 */  "AnaOnOff", 
/*  33 */  "Loop***", 
/*  34 */  "TrigCountIn", 
/*  35 */  "LP Undo", 
/*  36 */  "LP 1", 
/*  37 */  "LP 2", 
/*  38 */  "LP 3", 
/*  39 */  "LP 4", 
/*  40 */  "LP Rec", 
/*  41 */  "LP Pause", 
/*  42 */  "LP Trig", 
/*  43 */  "LP Tap", 
/*  44 */  "LP Add", 
/*  45 */  "LP All", 
/*  46 */  "LP Mute", 
/*  47 */  "Loop Toggle", 
/*  48 */  "LayoutSwitch", 
/*  49 */  "AnalogApp", 
/*  50 */  "MidiSoundApp", 
/*  51 */  "LiveApp", 
/*  52 */  "LooperApp", 
/*  53 */  "MP3Player", 
/*  54 */  "TabPlayer", 
/*  55 */  "PreTab", 
/*  56 */  "ScoreTab", 
/*  57 */  "TransportApp", 
/*  58 */  "RepeatA", 
/*  59 */  "RepeatB", 
/*  60 */  "NextSeq", 
/*  61 */  "PrevSeg", 
/*  62 */  "SpeeedUp", 
/*  63 */  "SlowDown", 
/*  64 */  "NextSong", 
/*  65 */  "PrevSong", 
/*  66 */  "Scroll", 
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
/*   6 */  "Strings", 
/*   7 */  "Steel Drums", 
/*   8 */  "Walky", 
/*   9 */  "Drums", 
/*  10 */  "Slap Bass", 
/*  11 */  "Synth Hard", 
/*  12 */  "Marimba", 
/*  13 */  "Breath1", 
/*  14 */  "Low Synth", 
/*  15 */  "NiceAhh", 
/*  16 */  "BetAhh", 
/*  17 */  "ToLate", 
/*  18 */  "BellsComp", 
/*  19 */  "Sax", 
/*  20 */  "Vibra", 
/*  21 */  "Mariba1", 
/*  22 */  "Tenor", 
/*  23 */  "EBuzz", 
/*  24 */  "SynthB11", 
/*  25 */  "KStatB", 
/*  26 */  "MS20Bass", 
/*  27 */  "Juno", 
/*  28 */  "Moog1", 
/*  29 */  "Moog2", 
/*  30 */  "BassSlap", 
/*  31 */  "Taurus2", 
/*  32 */  "Ultimate2a", 
/*  33 */  "SynthB1", 
/*  34 */  "FilterBass", 
/*  35 */  "BasBiy", 
/*  36 */  "CZOrgan", 
/*  37 */  "MoogLead1", 
/*  38 */  "CZBrass4", 
/*  39 */  "SinSymph", 
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
