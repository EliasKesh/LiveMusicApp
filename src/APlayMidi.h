/*---------------------------------------------
|
|   File:   APlayMidi
|
|   Contains:
|
|
|   Written By:     Elias Keshishoglou on Tue Nov 17 02:31:07 PM PST 2020
|
|   Copyright :     2020 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------*/

#ifndef APlayMidi_h
#define APlayMidi_h

#include "APlayMidi.h"
/*
 * Place Includes here.
 */


/*
 * Place Global prototypes here.
 */

int MyAlsaLoopClose ( void );
int alsa_loop_init ( void );
void SetLoopTempo ( int NewTempo );
void StopMidiLoop ( void );
void StartMidiLoop ( char *filename );
int InitMidiLooper ( void );
bool ToggleMidiLoop ( void );

/*
 * Place Static variables here.
 */


#endif /*APlayMidi.h_h */
