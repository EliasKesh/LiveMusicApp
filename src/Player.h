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
#define MaxSavedLoops 40
typedef struct {
	char		LoopName[40];
	float		Start;
	float		Length;
} SavedLoopType;


/*
 * Place Global prototypes here.
 */
void PlayerPoll(char How);
void SetPlayerFile(char	*FileName);
int LivePlayerClose(void);
void plSetA(void) ;
void plSetB(void) ;
void plPausePlay(void);
void plLoopToggle(void);
void plNextSeg(void);
void plPrevSeg(void) ;
void plSpeedUp(void) ;
void plSpeedDown(void) ;
void plSeekFw(void) ;
void plSeekBk(void) ;
int LivePlayerInit(GtkWidget *MainWindow, GtkWidget *window);
int LivePlayerClose(void);

/*
 * Place Static variables here.
 */


#endif
