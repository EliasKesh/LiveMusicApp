/*---------------------------------------------------------------------
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
|
|---------------------------------------------------------------------*/

#ifndef Player_h
#define Player_h

/*
 * Place Includes here.
 */


/*
 * Place Global prototypes here.
 */
void PlayerPoll(void);
void SetPlayerFile(char	*FileName);
int LivePlayerClose(void);
void plSetA(void) ;
void plSetB(void) ;
void plPausePlay(void);
void plLoopToggle(void);
void plNextSeg(void);
void plNextSeg(void) ;
void plSpeedUp(void) ;
void plSpeedDown(void) ;
void plSeekFw(void) ;
void plSeekBk(void) ;

/*
 * Place Static variables here.
 */


#endif Player.h_h
