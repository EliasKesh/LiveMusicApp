/*---------------------------------------------------------------------
|
|	File: 	APlayMidi
|
|	Contains: 	
|
|
|	Written By: 	Elias Keshishoglou on Tue Nov 17 02:31:07 PM PST 2020
|
|	Copyright : 	2020 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#ifndef APlayMidi_h
#define APlayMidi_h

/*
 * Place Includes here.
 */


/*
 * Place Global prototypes here.
 */
int MyAlsaLoopClose(void);
int alsa_loop_init(void);
void *alsa_Loop_thread(void * context_ptr);
void SetLoopTempo(int NewTempo);
void StopMidiLoop(void);
void StartMidiLoop(char *filename);
static void errormsg(const char *msg, ...);
int InitMidiLooper(void);


/*
 * Place Static variables here.
 */


#endif /*APlayMidi.h_h */
