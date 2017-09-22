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
int OSCCommand(int Command);

/*
 * Place Static variables here.
 */
enum {
	OSCSelect1,
	OSCSelect2,
	OSCSelect3,
	OSCSelect4,
	OSCRec,
	OSCPause,
	OSCTrig,
	OSCUndo,
	OSCAddLoop,

	MaxOSCCommands
};


#endif
