/*------------------------------------------------
|
|	File: 	Player
|
|	Contains:
|
|
|	Written By: 	Elias Keshishoglou on Sun Mar 22 14:18:32 PDT 2015
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
|------------------------------------------------*/

#ifndef Player_h
#define Player_h

/*
 * Place Includes here.
 */
#define MaxSavedLoops 100
typedef struct {
	char		LoopName[40];
	float		Start;
	float		Length;
    float       Position;
} SavedLoopType;

/*
 * Place Global prototypes here.
 */
void PlayerPoll(char How);
void SetPlayerFile(char	*FileName);
int LivePlayerClose(void);
void plSetA(void) ;
void plSetB(void) ;
bool plPausePlay(void);
bool plLoopToggle(void);
void plNextSeg(void);
void plPrevSeg(void) ;
void plSpeedUp(void) ;
void plSpeedDown(void) ;
void plSeekFw(void) ;
void plSeekBk(void) ;
int LivePlayerInit(GtkWidget *MainWindow, GtkWidget *window);
int LivePlayerClose(void);
int SetMP3PlayVol(int Value);
void plPlay(void);
void plStop(void);
void plSetPosition(float position);
void plScrub(float Amount);
int ResetPlayer(void);
int StartPlayer(void);


/*
 * Place Static variables here.
 */


#endif
