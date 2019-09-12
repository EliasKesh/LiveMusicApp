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
