/*---------------------------------------------------------------------
 |
 |	File: 	LiveMusicApp
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

#ifndef LiveMusicApp_h
#define LiveMusicApp_h

/*
 * Place Includes here.
 */

#include <asoundlib.h>
#include <gtk/gtk.h>
#include <gladeui/glade.h>
#include "MyWidgets.h"
#include <stdbool.h>

/*
 * Place Global prototypes here.
 */

#define LogError 1
#define LogAlert 2
#define LogWarn 3
#define LogInfo 4
#define LogDebug 5
#define MyLogLevel	LogDebug

#define PatchNameSize 60

char *printd(char LogLevel, const char *fmt, ...);
int WriteToHistory(char *str);

#define ResourceDirectory	"./LiveMusicRes/"

#define MaxStringPortName	32
enum {
	FluidPort = 0, GuitarixPort, LooperPort, TransportPort,
	TempoPort, ClickPort, PedalPort, InternalPort, MaxOutPorts
};



// Timer countdown in Miliseconds

#define Timer1Ticks		250

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
	char Chain[PatchNameSize];
//    int Chain;
} PatchInfo;

typedef struct {
	char *Name;
	char *location;
} SongInfo;

#define Max_Main_Buttons 70
#define Max_Patches	 182
#define Max_Layouts	 10
#define FileNameMaxLength	250

typedef struct {
	char Name[PatchNameSize];
	char Presets[Max_Patches][PatchNameSize];
} LayoutType;

#define Slider1		Max_Patches -4
#define Slider2		Max_Patches -3
#define Slider3		Max_Patches -2
#define Slider4		Max_Patches -1

typedef struct {
	char Name[255];
	char PortID;
} PortInformation;

#define MaxPresetButtons 6
typedef struct {
	char	thePreset[MaxPresetButtons];
	char 	theTempo;
} WebLoadPresets;

enum {
	AnalogApp = 0,
	MidiSoundApp,
	LooperApp,
	TransportApp,
	LiveMusicApp,
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
	char	Drum1;
	char	DrumRest;
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
	unsigned long	StatusTextColor;
	unsigned long	ButtonTextColor;

	char 		OSCIPAddress[20];
	char 		OSCPortNumLooper[6];
	char 		OSCPortNumJackVol[6];
	char 		OSCPortNumHydrogen[6];

	LayoutType LayoutPresets[Max_Layouts];


	// Generated Beyond this point.
	//-------------------------------------

	snd_seq_t *SeqPort[MaxOutPorts];
	//	int			SeqQueue[MaxOutPorts];

	/* Web paged preset are load here when the
	HTML page is loaded. 
	*/
	WebLoadPresets WebPresets;
	char LoopFileName[100];
	//	unsigned int	Timer1Count;	/* Not used	*/
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
	unsigned int ExpreP1Out;
	unsigned int ExpreP1Port;
	unsigned int ExpreP1Slider;



} LiveMusicInfo;

#define MaxSoundFonts 12

enum {
	NoCustom = 0,
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
	cmdCountIn,
	cmdVolume,
	cmdLnTransPort,
	cmdSetList,
	cmdScroll,
	cmdOSC,
	cmdSendCC,
	cmdSetExpr,
	MaxCommands
};

enum { cmdLnTSetA, cmdLnTSetB, cmdLnTStart, cmdLnTLoop, cmdLnTNext, cmdLnTPrev, cmdLnTUp, cmdLnTDown, cmdLnTSeekFw, cmdLnTSeekBk };

#define fsPatchNames	1
#define fsPortNames		2
#define fsAppNames		3

/*
 * Sounds Banks in Fluid Synth.
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
#define SFDrums		128
#define SFDrumsElias 129
#define NoBank		0xff

#if 0
#define SF32GM  0
#define SFDSF  1
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
#endif

#if 0
#define ModeSwitchKey	29
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

enum {
	FishmanUp = 0,
	FishmanDown,
	FishmanLeft,
	FishmanRight,
	FishmanGuitar,
	FishmanMix,
	FishmanSynth
};

#define MAINPREFS_FILE ".GTKMidi"

#define DefaultMidiChannel		1
#define DrumMidiChannel		10
#define MidiProgramChange		32

#if 0
#define ModeDefault		0
#define ModeRehearsal		1
#define ModePractice		2
#define ModePerformance	3
#define ModeLooper		4
#define ModeLastItem		ModeLooper
#endif

/*
 * Place Static variables here.
 */

enum { cntStateWaitingIdle = 0, 
		cntStateRecording = 1, 
		cntStateWaitingforRecCount = 2, 
		cntStateWaitingforCountIn = 3,
		cntStatePostRecord = 4 };


#define PedalLEDAllOn		0
#define PedalLEDAllOff		1
#define PedalLED1On		2
#define PedalLED1Off	3
#define PedalLED2On		4
#define PedalLED2Off	5
#define PedalLED3On		6
#define PedalLED3Off	7
#define PedalLED4On		8
#define PedalLED4Off	9
#define PedalLED5On		10
#define PedalLED6Off	11
#define PedalLED6On		12
#define PedalLED7Off	13
#define PedalLED7On		14
#define PedalLED8Off	15
#define PedalLED8On		16


#ifdef LiveMusicApp_c
#define EXTERN /* */
char *CustomPorts[] = {
	"FluidPort",
	"GuitarixPort",
	"LooperPort",
	"TransportPort",
	"TempoPort",
	"ClickPort",
	"PedalPort",
	"InternalPort"
};

char *CustomCommands[] = {
	"NoCustom",
	"ToNextDesktop",
	"ToPrevDesktop",
	"ToDesktop",
	"Controller",
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
	"MaxCommands",
	"oops2"
};

char *SoundFontBankNames[] = {
	"SFFluidGM",
	"SFElias",
	"SFSonidoGM",
	"SFSynthGM",
	"SFVintage",
	"SFDX7",
	"SFDSF",
	"SFRolandTenor",
	"SFAltoSax",
	"SFFlute",
	"SFDrums",
	"SFDrumsElias"
};
#else
#define EXTERN extern
extern char *CustomPorts[];
extern char *CustomCommands[];
extern char *SoundFontBankNames[];
#endif

//PatchInfo  *MyPatchInfo;
EXTERN LiveMusicInfo gMyInfo;
//LayoutType LayoutPresets[Max_Layouts];
EXTERN int LastPortUsed;


EXTERN PortsInfo theInPorts;
EXTERN PortsInfo theOutPorts;

//int		ModeArray[ModeLastItem][Max_Patches];

//int		preModeSwitch[Max_Patches];
EXTERN int WaitingforMidi;
EXTERN int WaitingforMidiHold;
EXTERN GtkBuilder *gxml;
EXTERN int CountInCount;
EXTERN int LoopRecBeats;
EXTERN char CountInActiveState;
EXTERN char	SysCallString[200];
EXTERN GdkPixbuf *MainButtonOnImage;
EXTERN GdkPixbuf *MainButtonOffImage;
EXTERN GdkPixbuf *PatchButtonOnImage;
EXTERN GdkPixbuf *PatchButtonOffImage;
EXTERN char gUpdateTempo;
EXTERN int		CurrentSetListSong;
EXTERN unsigned int CurrentLayout;
EXTERN unsigned int CurrentPreset;
EXTERN GtkWidget *PatchPopupMenu;
EXTERN char		ScreenSize;
EXTERN int      verbose_flag;
EXTERN char		JackName[MaxStringPortName];
EXTERN int 		FishmanSwitch;
EXTERN int 		FishmanSelSwitch;
EXTERN int 		LastPatch;
EXTERN int 		LastAbsPatch;
EXTERN int 		PreviousTab;
EXTERN snd_seq_event_t AlsaEvent;
EXTERN theImageButtons TempoDraw;
// Standard font description we use across the program
EXTERN PangoFontDescription *Tempofont_desc;
// Hold the tempo string so we do not draw at inturrupt time.
EXTERN char TempoUpdateString[100];
EXTERN int 	BeatCount;
EXTERN int 	UIUpdateFromTimer;
EXTERN char TempStrBuf[100];
EXTERN GtkWidget *theMainWindow;
EXTERN char 	JackRunning;

void NextDesktop(void);
void PrevDesktop(void);
void GoToDesktop(int Number);
void SetTempo(unsigned int NewTempo);
void RaiseWindows(char *AppName);
void RaiseWindowsNum(int AppNumber);
void ClearMainButtons(void);
void CreatePatchPopupMenu(void);

tPatchIndex ModeSwitchPatch(tPatchIndex MidiIn, char DoAction);

void PrintDataStructure (LiveMusicInfo * myInfo, char *PrefsRef);

int GuitarMidiPresetComplete(tPatchIndex MidiNote);
int GuitarMidiPreset(void);
int SetVolume1(int Value);
int SetVolume2(int Value);
int SetVolume3(int Value);
int SetVolume4(int Value);
int FindString(int StringList, char *String);
int ChorderMain(GtkWidget *MainWindow, GtkWidget *window);
int InitConnections(void);
void SwitchToTab(char Tab);
void SetUpMainButtons(PatchInfo *MyPatchInfo);
void IncrementMode(void);
tPatchIndex LayoutSwitchPatch(tPatchIndex MidiIn, char DoAction);
tPatchIndex DoPatch(PatchInfo *thePatch);

#endif
