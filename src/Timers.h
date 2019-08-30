/*---------------------------------------------------------------------
|
|	File: 	Timers
|
|	Contains:
|
|
|	Written By: 	Elias Keshishoglou on Sun Mar 8 11:03:40 PDT 2015
|
|	Copyright �: 	2015 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#ifndef Timers_h
#define Timers_h

/*
 * Place Includes here.
 */


/*
 * Place Global prototypes here.
 */

void MyTimerInit(void);
void SetTempo(unsigned int NewTempo);
void ToggleTempo(void);

extern int TempoState;
// #define AlsaTimer
enum { tmrAlsa = 0, tmrGTK = 1, tmrRT = 2 };
#define TimerToUse tmrRT

//#define AlsaTimer 1
//#define GTKTimer 1
#define RTTimer 1

/*
 * Place Static variables here.
 */


#endif
