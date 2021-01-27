/*------------------------------------------------
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
|------------------------------------------------*/

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
int OSCCommand(int Command, char Option);
void MyOSCTap(char DownBeat);
void MyOSCSetSync(char Type);
void MyOSCLoadFile(char *FileName);
void MyOSCJackVol(int volume, int channel);
void MyOSCJackMute(int Mute, int channel);

/*
 * Place Static variables here.
 */
enum {
	OSCSelectAll,
	OSCSelect,
	OSCRec,
	OSCPause,
	OSCTrig,
	OSCUndo,
	OSCAddLoop,
	OSCMute,
	OSCStartRecord,
	OSCStopRecord,
	OSCSyncOn,
	OSCSyncOff,
	OSCSyncSource,
	OSCRecThres,
	OSCRecLoop,
	MaxOSCCommands
};

// http://essej.net/sooperlooper/doc_osc.html
enum { typeSyncInternal = -3, typeSyncmidi = -2, typeSyncjack = -1, typeSyncnone = 0 };


#endif
