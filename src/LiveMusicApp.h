/*---------------------------------------------------------------------
 |
 |	File: 	LiveMusicApp.h
 |
 |	Contains: Global information for the entire projects
 |            should be placed in this file.
 |
 |
 |	Written By: 	Elias Keshishoglou on Wed Dec 26 14:13:15 PST 2012
 |
 |	Copyright �: 	2012 Elias Keshishoglou all rights reserved.
 |
 |	This program is free software; you can redistribute it and/or
 |	modify it under the terms of the GNU General Public License
 |	as published by the Free Software Foundation; either version 2
 |	of the License, or (at your option) any later version.
 |
 |	This program is distributed in the hope that it will be useful,
 |	but WITHOUT ANY WARRANTY; without even the implied warranty of
 |	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 |	GNU General Public License for more details.
 |
 |	You should have received a copy of the GNU General Public License
 |	along with this program; if not, write to the Free Software
 |	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 |
 |
 |---------------------------------------------------------------------*/

#ifndef LiveMusicApp_h
#define LiveMusicApp_h

/*
 * Place Includes here.
 */

#include <alsa/asoundlib.h>
#include <gtk/gtk.h>
#include <gladeui/glade.h>
#include "MyWidgets.h"
#include <stdbool.h>

/*
 * Place Global prototypes here.
 */

#define LogTest 10

#define LogRealTime 7
#define LogDebug 6
#define LogError 5
#define LogAlert 4
#define LogWarn 3
#define LogInfo 2
#define LogNone 1
#define MyLogLevel	LogNone

#define PatchNameSize 60
#define MaxHardSliders 5
#define TotalMaxStrings 10

#define EliasPresets 1

char *printd(char LogLevel, const char *fmt, ...);
int WriteToHistory(char *str);
int	 ShowPatchListSelect(GtkWidget *Temp, int Current);

//#define ResourceDirectory	"/usr/share/LiveMusicApp/"
//#define ResourceDirectory	"~/.config/LiveMusicApp/"

#define MaxStringPortName	32
enum {
	FluidPort = 0, GuitarixPort, LooperPort, TransportPort,
	TempoPort, ClickPort, PedalPort, InternalPort, 
	User1, User2, User3, MaxOutPorts
};

// Timer countdown in Miliseconds
#define Timer1Ticks		250

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

#define Max_Main_Buttons 80
#define MaxPresetButtons 6
#define Max_Patches	 182
#define Preset1Patch (Max_Patches + 1)
#define Preset2Patch (Max_Patches + 2)
#define Preset3Patch (Max_Patches + 3)
#define Preset4Patch (Max_Patches + 4)
#define Preset5Patch (Max_Patches + 5)
#define Preset6Patch (Max_Patches + 6)
#define Max_Patches_W_Presets Preset6Patch
#define Max_Layouts	 10
#define FileNameMaxLength	250

typedef struct {
	char Name[PatchNameSize];
	char Presets[Max_Patches][PatchNameSize];
} LayoutType;

#define Slider1		0
#define Slider2		1
#define Slider3		2
#define Slider4		3

typedef struct {
	char Name[255];
	char PortID;
} PortInformation;

#define MaxPresetButtons 6
typedef struct {
	tPatchIndex	thePreset[MaxPresetButtons];
	int	theTempo;
} WebLoadPresets;

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
	char        NumberOfStrings;
	char 		BaseStringName[TotalMaxStrings];

	ControllerRoute ControlRoute[MaxControllers];
	LayoutType LayoutPresets[Max_Layouts];



	// Generated Beyond this point.
	//-------------------------------------

	snd_seq_t *SeqPort[MaxOutPorts];

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
	unsigned long AlsaResolution;
	unsigned int ExpreP1Out;
	unsigned int ExpreP1Port;
	unsigned int ExpreP1Slider;
	tPatchIndex HardSlider[MaxHardSliders];
	tPatchIndex SliderUpdate;
	float 	LoopPosition;
	int   	LoopTempo;
	float 	RecordStopLoop;
	int 	GuitarMidiCall;
	int 	GuitarMidiCallParam1;
	int 	LayoutCall;
	int 	LayoutCallParam1;
	int 	LayoutCallParam2;
//	int 	CurrentLoop;
	unsigned int 	TabSwitch;
	unsigned int 	RaiseSwitch;
	unsigned int 	NextDeskSwitch;
	unsigned int 	PrevDeskSwitch;
	unsigned int 	GoToDeskSwitch;
	unsigned int 	IncrementSwitch;

} LiveMusicInfo;

/* Semophore for UI in GTK not timers.
*/
#define NULLSwitch	250

enum {GuitarMidiCallNo, GuitarMidiCallStart, GuitarMidiCallComplete };

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
	cmdHardSlider,
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

#ifndef MY_BUILD_NUMBER
#define MY_BUILD_NUMBER 1
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


#define PedalLEDAllOn	0
#define PedalLEDAllOff	1
#define PedalLED1On		2
#define PedalLED1Off	3
#define PedalLED2On		4
#define PedalLED2Off	5

// Tempo Leds
#define PedalLED3On		6
#define PedalLED3Off	7
#define PedalLED4On		8
#define PedalLED4Off	9

//
#define PedalLED5On		10
#define PedalLED5Off	11
#define PedalLED6On		12
#define PedalLED6Off	13
#define PedalLED7On		14
#define PedalLED7Off 	15

// Mute Hold
#define PedalLED8Off	16
#define PedalLED8On		17

/* Note values for the fretboard view.
*/
enum { NValueA = 0, NValueBb, NValueB,
		NValueC, NValueDb, NValueD,
		NValueEb, NValueE, NValueF,
		NValueGb, NValueG, NValueAb, NValueNone };

#ifdef LiveMusicApp_c
#define EXTERN /* */
char *GlobalNoteNames[] = {
	"A","Bb","B","C","Db",
	"D","Eb","E","F","Gb",
	"G","Ab"};

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
	"cmdHardSlider",
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
EXTERN int      GenerateHFile;
EXTERN char		JackName[MaxStringPortName];
EXTERN int 		FishmanSwitch;
EXTERN int 		FishmanSelSwitch;
//EXTERN int 		LastPatch;
//EXTERN int 		LastAbsPatch;
EXTERN int 		CurrentTab;
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
EXTERN char NewInstall;




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
int GuitarMidiPreset(char Wait);
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
void SetScale4Label(char *String);
int GTKIdel_cb(gpointer data);
enum {FileLocConfig = 0, FileLocTunes };
char *GetResourceDir(char *FileName, char WhichLoc);
#endif
