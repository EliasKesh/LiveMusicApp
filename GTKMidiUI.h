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

/*
 * Place Global prototypes here.
 */
#define OutPortAnalog 	0
#define OutPortMidi		1
#define MaxOutPorts		5
#define MaxStringPortName	32

// Timer countdown in Miliseconds
#define Timer1Ticks		250

typedef struct {
	char	Name[100];
	char	ID;
} AlsaPort;

typedef struct {
	char	Name[100];
	char	ID;
	char	NumPorts;
	AlsaPort	Ports[10];
} AlsaDevice;

typedef struct {
	char	NumDevices;
	AlsaDevice	Devices[20];
} PortsInfo;
	
typedef struct {
	char	*Button;
	char	Name[64];
	// Bank select -1 not to send Bank Info
	unsigned char	BankSelect;
	// The Patch number to send.
	unsigned	int		Patch;
	// Which virtual midi port to send the data
	char	OutPort;
	// The Midi channel.
	char	Channel;
	// Anything special we have to do 
	char	CustomCommand;
	// Chaing to Another Command 
	char	Chain;
} PatchInfo;

typedef struct {
	char *Name;
	char *location;

} SongInfo;

#define Max_Main_Buttons 30
#define Max_Patches	 90

typedef struct {
	char	Name[255];
	char	PortID;
} PortInformation;

typedef struct {
	char	thePreset1;
	char	thePreset2;
	char	theTempo;
} WebLoadPresets;

enum { AnalogApp = 0, MidiSoundApp, LooperApp, TransportApp, MidiControl, MaxApps };

typedef struct {
	// Preferences
	PatchInfo	MyPatchInfo[Max_Patches];
	char		BasePath[255];
	char		NumOutPorts;
	char		OutPortName[MaxOutPorts][MaxStringPortName];
	PortInformation		Apps[MaxApps];
	PortInformation		SoundGen;
	PortInformation		AnalogEffects;
	PortInformation		Looper;
	PortInformation		Transport;
	PortInformation		MidiController;

	// Generated
	snd_seq_t  *SeqPort[MaxOutPorts];
	WebLoadPresets	WebPresets;
	unsigned int	Timer1Count;
	unsigned char	Tempo;
	unsigned int	TempoReload;
	unsigned int	TempoTimerID;
	
} GTKMidiInfo;

enum {FluidPort = 0, RakarrakPort, LooperPort, TransportPort, GI20Port };
enum {NoCustom, ToNextDesktop, ToPrevDesktop, ToDesktop, Controller, RaiseApp, TransStart, TransCont,
  TransStop, TransPosition, TransTempo, MaxCommands };


#define SF32GM  0
#define SFDSF  1
#define SFFluidGM  2
#define SFA340Base  3
#define SFMusica 4
#define SFFlute 5
#define SFGI 6
#define SFBoogie 7
#define SFGoodFlue 8
#define SFFluidGMLow 10
#define SFA340Low 11
#define SFA340High 15

/*
 * Place Static variables here.
 */

#ifdef GTKMidiUI_c
//PatchInfo  *MyPatchInfo;
GTKMidiInfo	gMyInfo;

PortsInfo	theInPorts;
PortsInfo	theOutPorts;
char *CustomCommands[] = {
		"NoCustom", 
		"ToNextDesktop",
		"ToPrevDesktop",
		"ToDesktop",
		"Controller",
		"RaiseApp",
		"TransStart",
		"TransStop",
		"TransPosition",
		"TransTempo"
	};

#else
//extern PatchInfo  *MyPatchInfo;
extern GTKMidiInfo	gMyInfo;
extern char *CustomCommands[];
extern PortsInfo	theInPorts;
extern PortsInfo	theOutPorts;
#endif
void NextDesktop(void);
void PrevDesktop(void);
void GoToDesktop(char Number);
void SetTempo(unsigned char NewTempo);
void RaiseWindows( char *AppName );
void RaiseWindowsNum( char AppNumber );
#endif
