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
/*   9 */ {"Blues",           NoBank         ,   9, GuitarixPort   ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  10 */ {"BaseMel",         NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "Mel Tog" },
/*  11 */ {"BaseWah",         NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "Wah Tog" },
/*  12 */ {"BaseDist",        NoBank         ,   0, GuitarixPort   ,   1, NoCustom  , "Dist Tog" },
/*  13 */ {"Expr Vol",        NoBank         ,   7, GuitarixPort   ,   0, cmdSetExpr, "None" },
/*  14 */ {"Expr Wah",        NoBank         ,  11, GuitarixPort   ,   3, cmdSetExpr, "None" },
/*  15 */ {"Expr Dist",       NoBank         ,   2, GuitarixPort   ,   3, cmdSetExpr, "None" },
/*  16 */ {"Wah Tog",         NoBank         ,  10, GuitarixPort   ,   1, cmdSendCC , "Expr Wah" },
/*  17 */ {"Dist Tog",        NoBank         ,  11, GuitarixPort   ,   1, cmdSendCC , "None" },
/*  18 */ {"Mel Tog",         NoBank         ,  13, GuitarixPort   ,   1, cmdSendCC , "None" },
/*  19 */ {"Transport ****",  NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  20 */ {"TransStart",      NoBank         ,   0, TransportPort  ,   1, TransStart, "None" },
/*  21 */ {"TransCont",       NoBank         ,   0, TransportPort  ,   1, TransCont , "None" },
/*  22 */ {"TransStop",       NoBank         ,   0, TransportPort  ,   1, TransStop , "None" },
/*  23 */ {"TransBack",       NoBank         ,   0, TransportPort  ,   1, TransStart, "None" },
/*  24 */ {"Tap",             NoBank         , 121, InternalPort   ,   1, Controller, "None" },
/*  25 */ {"TransPos",        NoBank         ,   0, TransportPort  ,   1, TransStart, "None" },
/*  26 */ {"Tap",             NoBank         , 121, InternalPort   ,   1, Controller, "None" },
/*  27 */ {"Midi Tog",        NoBank         , 122, FluidPort      ,   1, Controller, "None" },
/*  28 */ {"Tuner",           NoBank         , 123, FluidPort      ,   1, Controller, "None" },
/*  29 */ {"AnaOnOff",        NoBank         , 124, FluidPort      ,   1, Controller, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  30 */ {"Loop***",         NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  31 */ {"TrigCountIn",     NoBank         ,   4, InternalPort   ,   1, cmdCountIn, "LooperApp" },
/*  32 */ {"LP Undo",         NoBank         ,   5, InternalPort   ,   1, cmdOSC    , "None" },
/*  33 */ {"LP 1",            0x01           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  34 */ {"LP 2",            0x02           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  35 */ {"LP 3",            0x03           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  36 */ {"LP 4",            0x04           ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  37 */ {"LP Rec",          NoBank         ,   2, InternalPort   ,   1, cmdOSC    , "LooperApp" },
/*  38 */ {"LP Pause",        NoBank         ,   3, InternalPort   ,   1, cmdOSC    , "None" },
/*  39 */ {"LP Trig",         NoBank         ,   4, InternalPort   ,   1, cmdOSC    , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  40 */ {"LP Tap",          NoBank         ,  77, InternalPort   ,   1, cmdOSC    , "None" },
/*  41 */ {"LP Add",          NoBank         ,   6, InternalPort   ,   1, cmdOSC    , "None" },
/*  42 */ {"LP All",          NoBank         ,   1, InternalPort   ,   1, cmdOSC    , "None" },
/*  43 */ {"LP Mute",         NoBank         ,   7, InternalPort   ,   1, cmdOSC    , "None" },
/*  44 */ {"LP RecLP",        NoBank         ,  14, InternalPort   ,   1, cmdOSC    , "LooperApp" },
/*  45 */ {"Loop Toggle",     NoBank         ,   3, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  46 */ {"LayoutSwitch",    NoBank         ,   0, InternalPort   ,   1, cmdBankSelect, "None" },
/*  47 */ {"AnalogApp",       NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  48 */ {"MidiSoundApp",    NoBank         ,   1, InternalPort   ,   1, RaiseApp  , "None" },
/*  49 */ {"LiveApp",         NoBank         ,   4, InternalPort   ,   1, RaiseApp  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  50 */ {"LooperApp",       NoBank         ,   2, InternalPort   ,   1, RaiseApp  , "None" },
/*  51 */ {"DrumApp",         NoBank         ,   6, InternalPort   ,   1, RaiseApp  , "None" },
/*  52 */ {"MP3Player",       NoBank         ,   5, InternalPort   ,   1, RaiseApp  , "None" },
/*  53 */ {"TabPlayer",       NoBank         ,   7, InternalPort   ,   1, RaiseApp  , "None" },
/*  54 */ {"PreTab",          NoBank         ,   0, InternalPort   ,   1, SwitchTab , "LiveApp" },
/*  55 */ {"ScoreTab",        NoBank         ,   1, InternalPort   ,   1, SwitchTab , "LiveApp" },
/*  56 */ {"PlayerTab",       NoBank         ,   2, InternalPort   ,   1, SwitchTab , "LiveApp" },
/*  57 */ {"ChordsTab",       NoBank         ,   3, InternalPort   ,   1, SwitchTab , "LiveApp" },
/*  58 */ {"TransportApp",    NoBank         ,   3, InternalPort   ,   1, RaiseApp  , "None" },
/*  59 */ {"RepeatA",         NoBank         ,   0, InternalPort   ,   1, cmdLnTransPort, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  60 */ {"RepeatB",         NoBank         ,   1, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  61 */ {"NextSeq",         NoBank         ,   4, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  62 */ {"PrevSeg",         NoBank         ,   5, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  63 */ {"SpeeedUp",        NoBank         ,   6, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  64 */ {"SlowDown",        NoBank         ,   7, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  65 */ {"NextSong",        NoBank         ,   2, InternalPort   ,   1, cmdSetList, "None" },
/*  66 */ {"PrevSong",        NoBank         ,   1, InternalPort   ,   1, cmdSetList, "None" },
/*  67 */ {"Scroll",          NoBank         ,   0, InternalPort   ,   1, cmdScroll , "None" },
/*  68 */ {"Global ****",     NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  69 */ {"Switch0",         NoBank         ,   0, InternalPort   ,   1, ToDesktop , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  70 */ {"Switch1",         NoBank         ,   1, InternalPort   ,   1, ToDesktop , "None" },
/*  71 */ {"Switch2",         NoBank         ,   2, InternalPort   ,   1, ToDesktop , "None" },
/*  72 */ {"Switch3",         NoBank         ,   3, InternalPort   ,   1, ToDesktop , "None" },
/*  73 */ {"Switch4",         NoBank         ,   4, InternalPort   ,   1, ToDesktop , "None" },
/*  74 */ {"Switch5",         NoBank         ,   5, InternalPort   ,   1, ToDesktop , "None" },
/*  75 */ {"VolUp",           NoBank         ,   5, InternalPort   ,   1, cmdVolume , "None" },
/*  76 */ {"VolDown",         NoBank         ,  -5, InternalPort   ,   1, cmdVolume , "None" },
/*  77 */ {"PlPlay",          NoBank         ,   2, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  78 */ {"Seek Fw",         NoBank         ,   8, InternalPort   ,   1, cmdLnTransPort, "None" },
/*  79 */ {"Seek Bk",         NoBank         ,   9, InternalPort   ,   1, cmdLnTransPort, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  80 */ {"Preset1",         NoBank         ,   1, InternalPort   ,   1, cmdPreset , "None" },
/*  81 */ {"Preset2",         NoBank         ,   2, InternalPort   ,   1, cmdPreset , "None" },
/*  82 */ {"Preset3",         NoBank         ,   3, InternalPort   ,   1, cmdPreset , "None" },
/*  83 */ {"Preset4",         NoBank         ,   4, InternalPort   ,   1, cmdPreset , "None" },
/*  84 */ {"Preset5",         NoBank         ,   5, InternalPort   ,   1, cmdPreset , "None" },
/*  85 */ {"Preset6",         NoBank         ,   6, InternalPort   ,   1, cmdPreset , "None" },
/*  86 */ {"MIDI***",         NoBank         ,   0, InternalPort   ,   1, RaiseApp  , "None" },
/*  87 */ {"Mid0",            NoBank         ,   0, FluidPort      ,   1, NoCustom  , "None" },
/*  88 */ {"Mid-24",          NoBank         ,   2, FluidPort      ,   1, NoCustom  , "None" },
/*  89 */ {"Inst M Sel",      NoBank         ,   0, InternalPort   ,   1, cmdMidiSelect, "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/*  90 */ {"Piano",           SFDSF          ,  63, FluidPort      ,   1, NoCustom  , "None" },
/*  91 */ {"Honky",           SFFluidGM      ,   3, FluidPort      ,   1, NoCustom  , "None" },
/*  92 */ {"Breath",          SFFluidGM      ,  53, FluidPort      ,   1, NoCustom  , "None" },
/*  93 */ {"Organ",           SFFluidGM      ,  16, FluidPort      ,   1, NoCustom  , "None" },
/*  94 */ {"Flute",           SFFlute        ,  73, FluidPort      ,   1, NoCustom  , "None" },
/*  95 */ {"SynthLead",       SFDSF          ,  43, FluidPort      ,   1, NoCustom  , "None" },
/*  96 */ {"Strings",         SFDSF          ,  85, FluidPort      ,   1, NoCustom  , "None" },
/*  97 */ {"Steel Drums",     SFFluidGM      , 114, FluidPort      ,   1, NoCustom  , "None" },
/*  98 */ {"Walky",           SFDSF          ,   3, FluidPort      ,   1, NoCustom  , "None" },
/*  99 */ {"Drums",           NoBank         ,   1, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 100 */ {"Slap Bass",       SFDSF          ,  24, FluidPort      ,   1, NoCustom  , "None" },
/* 101 */ {"Synth Hard",      SFDSF          ,  60, FluidPort      ,   1, NoCustom  , "None" },
/* 102 */ {"Marimba",         SFDSF          ,  12, FluidPort      ,   1, NoCustom  , "None" },
/* 103 */ {"Breath1",         SFDSF          ,  48, FluidPort      ,   1, NoCustom  , "None" },
/* 104 */ {"Low Synth",       SFDSF          ,  45, FluidPort      ,   1, NoCustom  , "None" },
/* 105 */ {"NiceAhh",         SFDSF          ,  90, FluidPort      ,   1, NoCustom  , "None" },
/* 106 */ {"BetAhh",          SFVintage      ,  63, FluidPort      ,   1, NoCustom  , "None" },
/* 107 */ {"ToLate",          SFVintage      , 108, FluidPort      ,   1, NoCustom  , "None" },
/* 108 */ {"BellsComp",       SFDX7          ,  10, FluidPort      ,   1, NoCustom  , "None" },
/* 109 */ {"Sax",             SFSonidoGM     ,  66, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 110 */ {"Vibra",           SFVintage      ,  93, FluidPort      ,   1, NoCustom  , "None" },
/* 111 */ {"Mariba1",         SFDX7          ,  91, FluidPort      ,   1, NoCustom  , "None" },
/* 112 */ {"Tenor",           SFRolandTenor  ,   1, FluidPort      ,   1, NoCustom  , "None" },
/* 113 */ {"EBuzz",           SFElias        ,   0, FluidPort      ,   1, NoCustom  , "None" },
/* 114 */ {"SynthB11",        SFElias        ,   1, FluidPort      ,   1, NoCustom  , "None" },
/* 115 */ {"KStatB",          SFElias        ,   2, FluidPort      ,   1, NoCustom  , "None" },
/* 116 */ {"MS20Bass",        SFElias        ,   3, FluidPort      ,   1, NoCustom  , "None" },
/* 117 */ {"Juno",            SFElias        ,   4, FluidPort      ,   1, NoCustom  , "None" },
/* 118 */ {"Moog1",           SFElias        ,   9, FluidPort      ,   1, NoCustom  , "None" },
/* 119 */ {"Moog2",           SFElias        ,  10, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 120 */ {"BassSlap",        SFElias        ,  13, FluidPort      ,   1, NoCustom  , "None" },
/* 121 */ {"Taurus2",         SFElias        ,  26, FluidPort      ,   1, NoCustom  , "None" },
/* 122 */ {"Ultimate2a",      SFElias        ,  33, FluidPort      ,   1, NoCustom  , "None" },
/* 123 */ {"SynthB1",         SFElias        ,  38, FluidPort      ,   1, NoCustom  , "None" },
/* 124 */ {"FilterBass",      SFElias        ,  39, FluidPort      ,   1, NoCustom  , "None" },
/* 125 */ {"BasBiy",          SFElias        ,  45, FluidPort      ,   1, NoCustom  , "None" },
/* 126 */ {"CZOrgan",         SFElias        ,  51, FluidPort      ,   1, NoCustom  , "None" },
/* 127 */ {"MoogLead1",       SFElias        ,  53, FluidPort      ,   1, NoCustom  , "None" },
/* 128 */ {"CZBrass4",        SFElias        ,  64, FluidPort      ,   1, NoCustom  , "None" },
/* 129 */ {"SinSymph",        SFElias        ,  90, FluidPort      ,   1, NoCustom  , "None" },
/*   Num   Name,              Bank         , Patch, Outport    ,  Channel, Custom,    Chain */ 
/* 130 */ {"Guitar V",        NoBank         ,   7, GuitarixPort   ,   0, cmdHardSlider, "None" },
/* 131 */ {"Midi V",          NoBank         ,   7, FluidPort      ,   1, cmdHardSlider, "None" },
/* 132 */ {"Master V",        NoBank         ,   7, InternalPort   ,   2, cmdHardSlider, "None" },
/* 133 */ {"Expr P",          NoBank         ,   2, GuitarixPort   ,   3, cmdHardSlider, "None" },
/* 134 */ {"FunkyFord",       NoBank         ,  10, GuitarixPort   ,   1, NoCustom  , "None" },
/* 135 */ {"Straights",       NoBank         ,  11, GuitarixPort   ,   1, NoCustom  , "None" },
/* 136 */ {"HoldM",           NoBank         ,  12, GuitarixPort   ,   1, NoCustom  , "None" },
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
9, 
 { 
"Fluid",
"Guitarix",
"Looper",
"Transport",
"Tempo",
"Click",
"Pedal",
"User1",
"User2",
"User3"
},
{
{ "guitarix", 0 },
{ "qsynth", 0 },
{ "sooperlooper", 0 },
{ "audacity", 0 },
{ "LiveMusicApp_1", 0 },
{ "clementine", 0 },
{ "tuxguitar", 0 },
{ "hydrogen", 0 },
{ "muse", 0 },
{ "gedit", 0 }
},
 8, /* TempoMax  */
 0, /* Metronome On  */
 37, /* Drum down beat, midi code  */
 44, /* Drum Other beats  */
 50, /* Base Midi Note for switching  */
 130, /* Tempo   */
 4, /* CountInBeats   */
 8, /* LoopRecBeats   */
 1, /* MidiPassThru   */
 110, /* MidiPassLevel   */
 4, /* BeatsPerMeasure   */
 0, /* AnalogVolume   */
 100, /* MidiVolume   */
 0xffffe0, /* StatusTextColor   */
 0xffe0e0, /* ButtonTextColor   */
 "127.0.0.1", /* OSCIPAddress   */
 "9951", /* OSCPortNumLooper   */
 "9952", /* OSCPortNumJackVol   */
 "9000", /* OSCPortNumHydrogen   */
 8, /* Number of Strings   */
 {9,
2,
7,
0,
5,
10,
3,
8,
12,
12,
    },
{ // Controller mapping 
{ /*   0 */  7, 0 },
{ /*   1 */  1, 7 },
{ /*   2 */  0, 7 },
{ /*   3 */  2, 9 },
{ /*   4 */  1, 9 },
{ /*   5 */  1, 10 },
{ /*   6 */  0, 0 },
{ /*   7 */  3, 12 },
}, 
{ 
 	{
 "Main", 
{
/*   0 */  "Preset1", 
/*   1 */  "Preset2", 
/*   2 */  "Preset3", 
/*   3 */  "BaseChorus", 
/*   4 */  "SusDist", 
/*   5 */  "Jazz", 
/*   6 */  "EchoLead", 
/*   7 */  "CompLead", 
/*   8 */  "Hard", 
/*   9 */  "FlangeDist", 
/*  10 */  "FunkyGuitar", 
/*  11 */  "Blues", 
/*  12 */  "MS20Bass", 
/*  13 */  "Breath", 
/*  14 */  "Expr Vol", 
/*  15 */  "Expr Wah", 
/*  16 */  "Expr Dist", 
/*  17 */  "Guitar V", 
/*  18 */  "Midi V", 
/*  19 */  "Master V", 
/*  20 */  "Wah Tog", 
/*  21 */  "Dist Tog", 
/*  22 */  "LiveApp", 
/*  23 */  "AnalogApp", 
/*  24 */  "DrumApp", 
/*  25 */  "LooperApp", 
/*  26 */  "TransCont", 
/*  27 */  "TransStop", 
/*  28 */  "TransBack", 
/*  29 */  "TrigCountIn", 
/*  30 */  "LP Undo", 
/*  31 */  "LP 1", 
/*  32 */  "LP 2", 
/*  33 */  "LP 3", 
/*  34 */  "LP 4", 
/*  35 */  "LP Rec", 
/*  36 */  "LP RecLP", 
/*  37 */  "LP Pause", 
/*  38 */  "Switch0", 
/*  39 */  "Switch1", 
/*  40 */  "Switch2", 
/*  41 */  "LP Trig", 
/*  42 */  "LP Tap", 
/*  43 */  "LP Add", 
/*  44 */  "LP All", 
/*  45 */  "LP Mute", 
/*  46 */  "Loop Toggle", 
/*  47 */  "Midi Tog", 
/*  48 */  "LayoutSwitch", 
/*  49 */  "AnalogApp", 
/*  50 */  "MidiSoundApp", 
/*  51 */  "LiveApp", 
/*  52 */  "LooperApp", 
/*  53 */  "MP3Player", 
/*  54 */  "TabPlayer", 
/*  55 */  "PreTab", 
/*  56 */  "ScoreTab", 
/*  57 */  "PlayerTab", 
/*  58 */  "ChordsTab", 
/*  59 */  "TransportApp", 
/*  60 */  "RepeatA", 
/*  61 */  "RepeatB", 
/*  62 */  "NextSeq", 
/*  63 */  "PrevSeg", 
/*  64 */  "SpeeedUp", 
/*  65 */  "SlowDown", 
/*  66 */  "NextSong", 
/*  67 */  "PrevSong", 
/*  68 */  "Scroll", 
/*  69 */  "SynthB11", 
/*  70 */  "KStatB", 
/*  71 */  "SynthLead", 
/*  72 */  "Juno", 
/*  73 */  "Moog1", 
/*  74 */  "Moog2", 
/*  75 */  "BassSlap", 
/*  76 */  "Taurus2", 
/*  77 */  "Honky", 
/*  78 */  "Organ", 
/*  79 */  "FunkyFord", 
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
/*  19 */  "LP RecLP", 
/*  20 */  "LP Pause", 
/*  21 */  "LP Mute", 
/*  22 */  "LayoutSwitch", 
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
