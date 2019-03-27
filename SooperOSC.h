/*---------------------------------------------------------------------
|
|	File: 	SooperOSC
|
|	Contains: 	
|
|
|	Written By: 	Elias Keshishoglou on Wed Sep 20 16:37:37 PDT 2017
|
|	Copyright ©: 	2017 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#ifndef SooperOSC_h
#define SooperOSC_h

/*
 * Place Includes here.
 */


/*
 * Place Global prototypes here.
 */
void MyOSCInit(void);

void MyOSCClose(void);
void MyOSCPoll(char	DownBeat);
int OSCCommand(int Command, char Option);
void MyOSCTap(char DownBeat);
void MyOSCSetSync(char Type);
void MyOSCLoadFile(char *FileName);
void MyOSCJackVol(int volume);

/*
 * Place Static variables here.
 */
enum {
	OSCSelectAll,
	OSCSelect,
	OSCRec,
	OSCPause,
	OSCTrig,
	OSCUndo,
	OSCAddLoop,
	OSCMute,

	MaxOSCCommands
};


#endif
