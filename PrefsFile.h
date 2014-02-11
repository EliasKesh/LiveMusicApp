/*---------------------------------------------------------------------
|
|	File: 	PrefsFile
|
|	Contains: 	
|
|
|	Written By: 	Elias Keshishoglou on Tue Dec 25 17:29:01 PST 2012
|
|	Copyright �: 	2012 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#ifndef PrefsFile_h
#define PrefsFile_h

/*
 * Place Includes here.
 */


/*
 * Place Global prototypes here.
 */

void InitPref(void);
void ReadPrefs(void);

/*
 * Place Static variables here.
 */
#define XML_BASIC_SETTINGS "MidiControlBase"
#define XML_SONGS_BASE_PATH "BaseSongPath"

#endif
