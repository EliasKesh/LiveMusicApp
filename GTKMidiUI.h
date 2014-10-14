/*---------------------------------------------------------------------
 |
 |	File: 	GTKMidiUI
 |
 |	Contains:
 |
 |
 |	Written By: 	Elias Keshishoglou on Wed Dec 26 14:13:15 PST 2012
 |
 |	Copyright �: 	2012 Elias Keshishoglou all rights reserved.
 |
 |
 |---------------------------------------------------------------------*/

#ifndef GTKMidiUI_h
#define GTKMidiUI_h

/*
 * Place Includes here.
 */

#include <asoundlib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

/*
 * Place Global prototypes here.
 */

#define LogError 1
#define LogAlert 2
#define LogWarn 3
#define LogInfo 4
#define LogDebug 5
#define MyLogLevel	5

#define PatchNameSize 60

char *printd(char LogLevel, const char *fmt, ...);

#define ResourceDirectory	"./LiveMusicRes/"

#define OutPortAnalog 	0
#define OutPortMidi		1
#define MaxOutPorts		6
#define MaxStringPortName	32

// Timer countdown in Miliseconds

#define Timer1Ticks		250

typedef unsigned short tPatchIndex;

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

typedef struct {
//	char	Button[32];
//	int Index;
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
    int Chain;
} PatchInfo;

typedef struct {
    char *Name;
    char *location;
} SongInfo;

#define Max_Main_Buttons 55
#define Max_Patches	 90

#define Slider1		Max_Patches -3
#define Slider2		Max_Patches -2
#define Slider3		Max_Patches -1

typedef struct {
    char Name[255];
    char PortID;
} PortInformation;

typedef struct {
    char thePreset1;
    char thePreset2;
    char theTempo;
} WebLoadPresets;

enum {
    AnalogApp = 0,
    MidiSoundApp,
    LooperApp,
    TransportApp,
    MidiControl,
    MP3Player,
    TabPlayer,
    MaxApps
};

typedef struct {
    // Preferences
    PatchInfo MyPatchInfo[Max_Patches];
    char BasePath[255];
    char NumOutPorts;
    char OutPortName[MaxOutPorts][MaxStringPortName];
    PortInformation Apps[MaxApps];
    char TempoMax;
    char MetronomeOn;
    tPatchIndex	MidiBaseNote;

    // Generated Beyond this point.
    snd_seq_t *SeqPort[MaxOutPorts];
//	int			SeqQueue[MaxOutPorts];
    WebLoadPresets WebPresets;
//	unsigned int	Timer1Count;	/* Not used	*/
    unsigned char Tempo;
    unsigned int TempoReload;
    unsigned int TempoTimerID;


    unsigned int AnalogVolume;
    unsigned int MidiVolume;

} GTKMidiInfo;

enum {
    FluidPort = 0, RakarrakPort, LooperPort, TransportPort, GI20Port, TempoPort
};
enum {
    NoCustom,
    ToNextDesktop,
    ToPrevDesktop,
    ToDesktop,
    Controller,
    SwitchTab,
    RaiseApp,
    TransStart,
    TransCont,
    TransStop,
    TransPosition,
    TransTempo,
    cmdPreset,
    cmdBankSelect,
    cmdMidiSelect,
    MaxCommands
};

/*
 * Sounds Banks in Fluid Synth.
 */
#define SF32GM  0
#define SFDSF  1
#define SFFluidGM  2
#define SFA340Base  3
#define SFMusica 4
#define SFFlute 5
#define SFGI 6
#define SFBoogie 7
#define SFGoodFlue 8
#define SFHold8	 8
#define SFDrumKit	 9
#define SFFluidGMLow 10
#define SFA340Low 11
#define SFHold12	 12
#define SFHold13	 13
#define SFHold14	 14
#define SFA340High 15
#define SFDrums		128

#if 0
#define ModeSwitchKey		29
#define PresetMidiKey	28
#define Preset1FButton	27
#define Preset2FButton	26
#endif

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

#define MAINPREFS_FILE ".GTKMidi"

#define DefaultMidiChannel		1
#define DrumMidiChannel		10
#define MidiProgramChange		32

#define ModeDefault		0
#define ModeRehearsal		1
#define ModePractice		2
#define ModePerformance	3
#define ModeLooper		4
#define ModeLastItem		ModeLooper

/*
 * Place Static variables here.
 */

#ifdef GTKMidiUI_c
//PatchInfo  *MyPatchInfo;
GTKMidiInfo gMyInfo;

PortsInfo theInPorts;
PortsInfo theOutPorts;
char *CustomCommands[] = {
    "NoCustom",
    "ToNextDesktop",
    "ToPrevDesktop",
    "ToDesktop",
    "Controller",
    "SwitchTab",
    "RaiseApp",
    "TransStart",
    "TransStop",
    "TransPosition",
    "TransTempo"
};


char *theModes[] = {
    "Default",
    "Rehearsal",
    "Practice",
    "Performance",
    "Looper"
};
//int		ModeArray[ModeLastItem][Max_Patches];

//int		preModeSwitch[Max_Patches];
int preModePractice[Max_Patches] = {
    /* Front Row */
    0, 1, 2, 3, 4, 5, 6, 7, 66, 67,
    60, 61, 62, 63, 64, 65, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89
};

int preModeRehearsal[Max_Patches] = {
	/* Front Row */
    0, 1, 2, 3, 4, 5, 6, 7, 66, 67,
    60, 61, 62, 63, 64, 65, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89
};

int preModePerformance[Max_Patches] = {
	/* Front Row */
    0, 1, 2, 3, 4, 5, 6, 7, 66, 67,
    60, 61, 62, 63, 64, 65, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89
};

int preModeLooper[Max_Patches] = {
    /* Front Row */
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    06, 01, 9, 20, 14, 65, 40, 41, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89
};
int		WaitingforMidi;
GtkBuilder *gxml;

#else
//extern PatchInfo  *MyPatchInfo;
extern GTKMidiInfo gMyInfo;
extern char *CustomCommands[];
extern PortsInfo theInPorts;
extern PortsInfo theOutPorts;
extern char *theModes[];
extern int preModePractice[];
extern int preModeLooper[];
extern int preModePerformance[];
extern int preModeRehearsal[];
extern int WaitingforMidi;
extern GtkBuilder *gxml;

#endif

void NextDesktop(void);
void PrevDesktop(void);
void GoToDesktop(char Number);
void SetTempo(unsigned char NewTempo);
void RaiseWindows(char *AppName);
void RaiseWindowsNum(char AppNumber);

tPatchIndex ModeSwitchPatch(tPatchIndex MidiIn, char	DoAction);

void PrintDataStructure(GTKMidiInfo *myInfo);
int	GuitarMidiPresetComplete(tPatchIndex	MidiNote);
int	GuitarMidiPreset(void);
int	SetVolume1(int Value);
int	SetVolume2(int Value);

#endif
