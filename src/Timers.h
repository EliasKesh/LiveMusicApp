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

// GTK timer not very accurate.
//#define GTKTimer 1

// Accurate but, locks in 
// __GI___poll (fds=0x7fff254dd658, nfds=1, timeout=-1)
//    at ../sysdeps/unix/sysv/linux/poll
#define RTTimer 1

/*
 * Place Static variables here.
 */


#endif
