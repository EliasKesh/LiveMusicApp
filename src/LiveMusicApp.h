/*------------------------------------------------
 |
 |  File:   LiveMusicApp.h
 |
 |  Contains: Global information for the entire projects
 |            should be placed in this file.
 |
 |
 |  Written By:     Elias Keshishoglou on Wed Dec 26 14:13:15 PST 2012
 |
 |  Copyright :     2012-2021 Elias Keshishoglou all rights reserved.
 |
 |  This program is free software; you can redistribute it and/or
 |  modify it under the terms of the GNU General Public License
 |  as published by the Free Software Foundation; either version 2
 |  of the License, or (at your option) any later version.
 |
 |  This program is distributed in the hope that it will be useful,
 |  but WITHOUT ANY WARRANTY; without even the implied warranty of
 |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 |  GNU General Public License for more details.
 |
 |  You should have received a copy of the GNU General Public License
 |  along with this program; if not, write to the Free Software
 |  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 |
 |
 |------------------------------------------------*/

#ifndef LiveMusicApp_h
#define LiveMusicApp_h

/*
 * Place Includes here.
 */

#include <alsa/asoundlib.h>
#include <gtk/gtk.h>
// #include <gladeui/glade.h>
#include "MyWidgets.h"
#include <stdbool.h>

/*
 * Place Global prototypes here.
 */

/* Debugging definitions.
*/
#define LogHold1    0x0800
#define LogPlayer   0x400
#define LogMidi     0x200
#define LogRealTime 0x100
#define LogTimer    0x40
#define LogDebug    0x20
#define LogError    0x10
#define LogAlert    0x08
#define LogWarn     0x04
#define LogInfo     0x02
#define LogTest     0x01
#define MyLogLevel  LogNone

#define PatchNameSize 60
#define MaxHardSliders 5
#define TotalMaxStrings 10

#define MyOSCPortNumber "61000"

#define GuiUpdateCount 4

char *printd(int LogLevel, const char *fmt, ...);
int WriteToHistory(char *str);
int  ShowPatchListSelect(GtkWidget *Temp, int Current);

//#define ResourceDirectory "/usr/share/LiveMusicApp/"
//#define ResourceDirectory "~/.config/LiveMusicApp/"
#define NoteNameLength 12

#define MaxStringPortName   32
#if 1
enum {
    FluidPort = 0,
    GuitarixPort,
    LooperPort,
    TransportPort,
    TempoPort,
    ClickPort,
    PedalPort,
    DAWPort,
    DRLoop,
    User2,
    User3,
    InternalPort,
    MaxOutPorts
};


#endif

// Notebook tabs pages Part of Glade File.
enum {
    tabpagePatch = 0,
    tabpageChart,
    tabpagePlayer,
    tabpageChords,
    tabpagePrefs,
    tabpageMAX
};

// Timer countdown in Miliseconds
#define Timer1Ticks     250

/* Type used for patches.
*/
typedef unsigned short tPatchIndex;

/* These are used to hold connection information
currently not implemented.
*/
typedef struct {
    char Name[PatchNameSize];
    char ID;
} AlsaPort;

typedef struct {
    char Name[PatchNameSize];
    char ID;
    char NumPorts;
    AlsaPort Ports[10];
} AlsaDevice;

typedef struct {
    char NumDevices;
    AlsaDevice Devices[20];
} PortsInfo;

#define MaxControllers 8
typedef struct {
    int OutPort;
    int OutControl;
} ControllerRoute;


typedef struct {
    char Name[PatchNameSize];
    // Bank select -1 not to send Bank Info
    unsigned char BankSelect;
    // The Patch number to send.
    unsigned int Patch;
    // Which virtual midi port to send the data
    char OutPort;
    // The Midi channel.
    char Channel;
    // Anything special we have to do
    int CustomCommand;
    // Chaing to Another Command
    char Chain[PatchNameSize];
} PatchInfo;

typedef struct {
    char *Name;
    char *location;
} SongInfo;

#define Max_Main_Buttons 80

// Presets that are set by html.
#define MaxPresetButtons 6
#define PresetInvalid 65535

// Total usable patches.
#define Max_Patches  182
#define Patch_Not_Found (Max_Patches + 1)
#define Preset1Patch (Max_Patches + 2)
#define Preset2Patch (Max_Patches + 3)
#define Preset3Patch (Max_Patches + 4)
#define Preset4Patch (Max_Patches + 5)
#define Preset5Patch (Max_Patches + 6)
#define Preset6Patch (Max_Patches + 7)
#define Max_Patches_W_Presets Preset6Patch

// Total number of Layouts
#define Max_Layouts  10
#define FileNameMaxLength   400

// Total number of patches per layout
#define MaxLayoutPatches 81
#define MinTempoValue 50
#define MaxTempoValue 200

typedef struct {
    char Name[PatchNameSize];
    char Presets[Max_Patches_W_Presets][PatchNameSize];
} LayoutType;

// Fixed Flider controllers
enum {
    Slider1 = 1,
    Slider2,
    Slider3,
    Slider4
};

// SetExpress function control values.
enum {
    ecGuitarVolume = 1,
    ecMidiVolume,
    ecMasterVolume,
    ecTempChange,
    ecMP3Volume,
    ecExpress6,
    ecPedalControl,
    ecMidiThreshold,
    ecLooperVolume,
    ecMidiAnaMix,
    ecDistorion,
    ecChorus,
    ecWAH,
    exMaxItems
};

typedef struct {
    char Name[255];
    char PortID;
} PortInformation;

#define MaxPresetButtons 6
typedef struct {
    tPatchIndex thePreset[MaxPresetButtons];
    int theTempo;
} WebLoadPresets;

// Application names index,
// Names are part of the configuration xml.
enum {
    AnalogApp = 0,
    MidiSoundApp,
    LooperApp,
    TransportApp,
    LiveMusicApp,
    MP3Player,
    TabPlayer,
    DrumPlayer,
    MidiPlayer,
    HTMLEditor,
    ConnectApp,
    MaxApps
};

typedef struct {
    // Preferences
    PatchInfo MyPatchInfo[Max_Patches_W_Presets];
    char BasePath[FileNameMaxLength];
    char NumOutPorts;
    char OutPortName[MaxOutPorts][MaxStringPortName];
    // Mostly moved to bash script.
    PortInformation Apps[MaxApps];
    char TempoMax;
    char MetronomeOn;
    char    Drum1;
    char    DrumRest;
    tPatchIndex MidiBaseNote;
    unsigned char Tempo;
    unsigned char CountInBeats;
    unsigned char LoopRecBeats;
    unsigned int MidiPassThru;
    unsigned int MidiPassLevel;
    unsigned int BeatsPerMeasure;
    unsigned int AnalogVolume;
    unsigned int MidiVolume;

    /*
     * Colors
    */
    unsigned long   StatusTextColor;
    unsigned long   ButtonTextColor;

    char        OSCIPAddress[20];
    char        OSCPortNumLooper[6];
    char        OSCPortNumJackVol[6];
    char        OSCPortNumHydrogen[6];
    char        NumberOfStrings;
    char        BaseStringName[TotalMaxStrings];

    ControllerRoute ControlRoute[MaxControllers];
    LayoutType LayoutPresets[Max_Layouts];



    // Generated Beyond this point.
    //-------------------------------------

    snd_seq_t *SeqPort[MaxOutPorts];

    /* Web paged preset are load here when the
    HTML page is loaded.
    */
    WebLoadPresets WebPresets;
    char LoopFileName[FileNameMaxLength];
    //  unsigned int    Timer1Count;    /* Not used */
    unsigned long TempoReload;
    unsigned long TimerCount;
    unsigned long KeyTimer;
    timer_t TempoTimerID;

    int   KeyPatchValue;
    char  KeyChar;
    char  KeyIsValue;

    snd_timer_t *AlsaTimerHandle;
    snd_timer_params_t *AlsaTimerParams;
    snd_timer_id_t *AlsaTimerid;
    snd_timer_info_t *AlsaTimerinfo;
    unsigned long AlsaResolution;
    unsigned int ExpreP1Out;
    unsigned int ExpreP1Port;
    unsigned int ExpreP1Slider;
    tPatchIndex HardSlider[MaxHardSliders];
    tPatchIndex SliderUpdate;
    float   LoopPosition;
    int     LoopTempo;
    float   RecordStopLoop;
    int     GuitarMidiCall;
    int     GuitarMidiCallParam1;
    int     LayoutCall;
    int     LayoutCallParam1;
    int     LayoutCallParam2;
    //  int     CurrentLoop;
    unsigned int    TabSwitch;
    unsigned int    RaiseSwitch;
    unsigned int    NextDeskSwitch;
    unsigned int    PrevDeskSwitch;
    unsigned int    GoToDeskSwitch;
    unsigned int    IncrementSwitch;
    unsigned char   MidiThresholdLevel;
    unsigned char   MidiAnalMixLevel;
    unsigned int    SetMP3PlayVolBool;
    unsigned int    ScrollUpdate;
    unsigned int    V3Volume;
    unsigned int    V4Volume;
    unsigned int    V6Volume;
    char    PatchUpdate;
    unsigned int    SliderGUIUpdate;
    unsigned int    SliderGUIValue;
    unsigned int    SliderGUINumber;
    unsigned int    MarkerUpdate;

} LiveMusicInfo;

/* Semophore for UI in GTK not timers.
*/
#define NULLSwitch  250

/* State for using midi control to change patches.
*/
enum {
    GuitarMidiCallNo,
    GuitarMidiCallStart,
    GuitarMidiCallComplete
};

#define MaxSoundFonts 12

// Custom commands for each preset.
enum {
    NoCustom = 0,
    ToNextDesktop,
    ToPrevDesktop,
    ToDesktop,
    cmdController,
    SwitchTab,
    RaiseApp,
    TransStart,
    TransCont,
    TransStop,
    TransPosition, // 10
    TransTempo,
    cmdPreset,
    cmdBankSelect,
    cmdMidiSelect,
    cmdCountIn, // 15
    cmdVolume,
    cmdLnTransPort,
    cmdSetList,
    cmdScroll,
    cmdOSC, // 20
    cmdSendCC,
    cmdSetExpr,
    cmdHardSlider,
    cmdMidiOnOff,
    cmdDrumToggle,
    MaxCommands
};

enum { cmdLnTSetA, cmdLnTSetB, cmdLnTStart, cmdLnTLoop, cmdLnTNext, cmdLnTPrev, cmdLnTUp, cmdLnTDown, cmdLnTSeekFw, cmdLnTSeekBk };

#define fsPatchNames    1
#define fsPortNames     2
#define fsAppNames      3

#if 0
/*
 * Sounds Banks in Fluid Synth.
 * These are dynamic now.
 */
#define SFFluidGM  0
#define SFElias  1
#define SFSonidoGM  2
#define SFSynthGM  3
#define SFVintage  4
#define SFDX7  5
#define SFDSF  6
#define SFRolandTenor  7
#define SFAltoSax  8
#define SFFlute  9
#define SFDrums     128
#define SFDrumsElias 129
#endif
#define NoBank      0xff

enum {
    ToDesktop0 = 60,
    ToDesktop1,
    ToDesktop2,
    ToDesktop3,
    ToDesktop4,
    ToDesktop5,
    ToAnalogApp,
    ToMidiSoundApp,
    ToLooperApp,
    ToTransportApp,
    ToMidiControl
};

enum {
    FishmanUp = 0,
    FishmanDown,
    FishmanLeft,
    FishmanRight,
    FishmanGuitar,
    FishmanMix,
    FishmanSynth
};

#define DefaultMidiChannel      1
#define DrumMidiChannel     10
#define MidiProgramChange       32

#ifndef MY_BUILD_NUMBER
#define MY_BUILD_NUMBER 1
#endif

#ifndef MY_VERSION_NUMBER
#define MY_VERSION_NUMBER 1
#endif

/*
 * Place Static variables here.
 */

enum { cntStateWaitingIdle = 0,
       cntStateRecording = 1,
       cntStateWaitingforRecCount = 2,
       cntStateWaitingforCountIn = 3,
       cntStatePostRecord = 4
     };


#if 0
// Default Settings for Akai LPD8
enum {
    //  dLEDBeat1 = 36,
    dLEDBeat1 = 64,
    dLEDBeat2,
    dLEDBeat3,
    dLEDBeat4,
    dLEDBeat5,
    dLEDBeat6,
    dLEDBeat7,
    dLEDBeat8
};
#define dLEDMIDI    SND_SEQ_EVENT_NOTEOFF
#define dLEDMIDIO   SND_SEQ_EVENT_NOTEON
#define dLEDChan 1
#else
// Default Settings for kayfadr
enum {
    //  dLEDBeat1 = 40,
    dLEDBeat1 = 64,
    dLEDBeat2,
    dLEDBeat3,
    dLEDBeat4,
    dLEDBeat5,
    dLEDBeat6,
    dLEDBeat7,
    dLEDBeat8
};

enum {
    dLEDCnt_NoOp = 0,
    dLEDCnt_Beat1,
    dLEDCnt_Beat2,
    dLEDCnt_Beat3,
    dLEDCnt_Beat4,
    dLEDCnt_AllOff = 7,
    dLEDCnt_CountD1,
    dLEDCnt_CountD2,
    dLEDCnt_CountD3,


};

#define dLEDMIDI    SND_SEQ_EVENT_CONTROLLER
#define dLEDMIDIO   SND_SEQ_EVENT_CONTROLLER
#define dLEDChan 1
//#define dLEDChan 2
#endif

enum {
    PedalLEDAllOn = 0,
    PedalLEDAllOff,
    PedalLED1On,
    PedalLED1Off,
    PedalLED2On,
    PedalLED2Off,
    PedalLED3On,
    PedalLED3Off,
    PedalLED4On,
    PedalLED4Off,
    PedalLED5On,
    PedalLED5Off,
    PedalLED6On,
    PedalLED6Off,
    PedalLED7On,
    PedalLED7Off,
    PedalLED8Off,
    PedalLED8On,
    PedalLEDAlert = 50
};


/* Note values for the fretboard view.
*/
enum { NValueA = 0, NValueBb, NValueB,
       NValueC, NValueDb, NValueD,
       NValueEb, NValueE, NValueF,
       NValueGb, NValueG, NValueAb, NValueNone
     };

#ifdef LiveMusicApp_c
#define EXTERN /* */
char *GlobalNoteNames[] = {
    "A", "Bb", "B", "C", "Db",
    "D", "Eb", "E", "F", "Gb",
    "G", "Ab", "NA"
};

char *CustomCommands[] = {
    "NoCustom",
    "ToNextDesktop",
    "ToPrevDesktop",
    "ToDesktop",
    "cmdController",
    "SwitchTab",
    "RaiseApp",
    "TransStart",
    "TransCont",
    "TransStop",
    "TransPosition",
    "TransTempo",
    "cmdPreset",
    "cmdBankSelect",
    "cmdMidiSelect",
    "cmdCountIn",
    "cmdVolume",
    "cmdLnTransPort",
    "cmdSetList",
    "cmdScroll",
    "cmdOSC",
    "cmdSendCC",
    "cmdSetExpr",
    "cmdHardSlider",
    "cmdMidiOnOff",
    "MaxCommands",
    "oops2"
};

#else
#define EXTERN extern
extern char *CustomPorts[];
extern char *CustomCommands[];
extern char *SoundFontBankNames[];
extern char *GlobalNoteNames[];
#endif

//PatchInfo  *MyPatchInfo;
EXTERN LiveMusicInfo gMyInfo;
//LayoutType LayoutPresets[Max_Layouts];
EXTERN int LastPortUsed;

EXTERN PortsInfo theInPorts;
EXTERN PortsInfo theOutPorts;

EXTERN int      WaitingforMidi;
EXTERN int      WaitingforMidiHold;
EXTERN GtkBuilder *gxml;
EXTERN int      CountInCount;
EXTERN int      LoopRecBeats;
EXTERN char     CountInActiveState;
EXTERN char     SysCallString[200];
EXTERN GdkPixbuf *MainButtonOnImage;
EXTERN GdkPixbuf *MainButtonOffImage;
EXTERN GdkPixbuf *PatchButtonOnImage;
EXTERN GdkPixbuf *PatchButtonOffImage;
EXTERN char gUpdateTempo;
EXTERN int      CurrentSetListSong;
EXTERN unsigned int CurrentLayout;
EXTERN unsigned int CurrentPreset;
EXTERN GtkWidget *PatchPopupMenu;
EXTERN char     ScreenSize;
EXTERN int      GenerateHFile;
EXTERN char     JackName[MaxStringPortName];
EXTERN int      FishmanSwitch;
EXTERN int      FishmanSelSwitch;
EXTERN int      CurrentTab;
EXTERN int      PreviousTab;
EXTERN snd_seq_event_t AlsaEvent;
EXTERN theImageButtons TempoDraw;
// Standard font description we use across the program
EXTERN PangoFontDescription *Tempofont_desc;
// Hold the tempo string so we do not draw at inturrupt time.
EXTERN char     TempoUpdateString[100];
EXTERN int      BeatCount;
EXTERN int      UIUpdateFromTimer;
EXTERN char     TempStrBuf[100];
EXTERN GtkWidget *theMainWindow;
EXTERN char     JackRunning;
EXTERN char     NewInstall;
EXTERN float    PlayerDisTime;
EXTERN char     PlayerDisSection[30];
EXTERN char     DisableTextInput;
EXTERN char     WeAreRunning;
EXTERN volatile char    gLooperWaitForSync;
// The button size my change based on the screen size.
EXTERN int ButtonSize;

#define MaxSongMarks    200
#define SongSectionMax  30
EXTERN int      NumberSongMarks;
typedef struct {
    float SongMark;
    char SongSection[SongSectionMax];
} SongMarkType;

EXTERN SongMarkType SongMarkers[MaxSongMarks];


void NextDesktop(void);
void PrevDesktop(void);
void GoToDesktop(int Number);
void SetTempo(unsigned int NewTempo);
void RaiseWindows(char *AppName);
void RaiseWindowsNum(int AppNumber);
void ClearMainButtons(void);
void CreatePatchPopupMenu(void);

tPatchIndex ModeSwitchPatch(tPatchIndex MidiIn, char DoAction);

void PrintDataStructure(LiveMusicInfo * myInfo, char *PrefsRef);

int GuitarMidiPresetComplete(tPatchIndex MidiNote);
int GuitarMidiPreset(char Wait);
int SetVolume1(int Value);
int SetVolume2(int Value);
int SetVolume3(int Value);
int SetVolume4(int Value);
int FindString(int StringList, char *String);
int InitChorder(GtkWidget *MainWindow, GtkWidget *window);
int CloseChorder(void);
int InitConnections(void);
void SwitchToTab(char Tab);
void SetUpMainButtons(PatchInfo *MyPatchInfo);
void IncrementLayoutMode(void);
tPatchIndex LayoutSwitchPatch(tPatchIndex MidiIn, char DoAction);
tPatchIndex DoPatch(PatchInfo *thePatch);
void SetScale4Label(char *String);
int GTKIdel_cb(gpointer data);
enum {FileLocConfig = 0, FileLocTunes, FileLocUser };
char *GetResourceDir(char *FileName, char WhichLoc);
int SetExpressionControl(int Controller, int Value);
int GetExpressionControl(int Controller);
void LEDControl(char Beat, char State);
void UpdateStatus(char *String);
void jack_poll(void);
void SetMetronomeStatus(char State);

#endif
