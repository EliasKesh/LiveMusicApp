/*---------------------------------------------------------------------
|
|	File: 	SooperOSC
|
|	Contains:
|
|
|	Written By: 	Elias Keshishoglou on Wed Sep 20 16:37:34 PDT 2017
|
|	Copyright ©: 	2017 Elias Keshishoglou all rights reserved.
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

#define SooperOSC_c

/*
 * Place Includes Here
 */

#include "LiveMusicApp.h"
#include "AlsaUtils.h"
#include "SooperOSC.h"
#include <lo/lo.h>

/*
 * Place defines and Typedefs here
 */
#define DefaultLoopLength	20.0


/*
 * Place Local prototypes here
 */


/*
 * Place Static variables here
 */
static lo_address SLOSCaddr;
static lo_address JackVoladdr;
static lo_address Hydrogenaddr;
static lo_server osc_server = 0;
static lo_server osc_server1 = 0;
static char our_url[100];
static char CurrentLoop;
// http://essej.net/sooperlooper/doc_osc.html
/*--------------------------------------------------------------------
 * Function:		OSCCommand.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
int OSCCommand(int Command, char Option) {
	char NewCommand[100];

	printd(LogDebug, "OSCCommand: %d %d\n", Command, Option);
	switch (Command) {
	case  OSCSelect:
		CurrentLoop = Option;

		if (CurrentLoop >= 0)
			CurrentLoop = Option - 1;

		break;

	case  OSCRec:
		sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
		printd(LogDebug, "OSCRec %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "s", "record");
		gMyInfo.LoopTempo = gMyInfo.Tempo;
		break;

	case  OSCPause:
		sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
		printd(LogDebug, "OSCPause %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "s", "pause");
		break;

	case  OSCTrig:
		sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
		printd(LogDebug, "OSCTrig %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "s", "trigger");
		break;

	case  OSCUndo:
		sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
		printd(LogDebug, "OSCUndo %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "s", "undo");
		break;

	case OSCAddLoop:
		printd(LogDebug, "OSC Add\n");
		lo_send(SLOSCaddr, "/loop_add", "if", 1, DefaultLoopLength);
		break;

	case OSCMute:
		sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
		printd(LogDebug, "OSCMute %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "s", "mute");
		break;

	case OSCStartRecord:
		sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
		printd(LogDebug, "OSCStartRecord %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "s", "record");
		break;

	case OSCStopRecord:
		sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
		printd(LogDebug, "OSCStopRecord %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "s", "record");
		break;

	case OSCSyncOn:
		lo_send(SLOSCaddr, NewCommand, "sf", "sync", 1.0);
		sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
		printd(LogDebug, "Sync On %s\n", NewCommand);
		break;

	case OSCSyncOff:
		sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
		printd(LogDebug, "Sync Off %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "sf", "sync", 0.0);
		break;

	case OSCSyncSource:
//		sprintf(NewCommand, "/sl/-1/set", CurrentLoop);
		printd(LogDebug, "OSCSyncSource %d\n", Option);
		lo_send(SLOSCaddr, "/set", "si", "sync_source", Option);
		break;

	case OSCRecThres:
		sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
		printd(LogDebug, "Sync Off %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "sf", "rec_thresh", (float)Option / 127 );
		break;

	case OSCRecLoop:
		sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
		printd(LogDebug, "OSCRecLP %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "s", "record");
		gMyInfo.LoopTempo = gMyInfo.Tempo;
		gMyInfo.RecordStopLoop = gMyInfo.LoopPosition;
		break;

//oscsend localhost 9951 /set si "sync_source" -3

// lo_send(SLOSCaddr, "/sl/-2/set", "sf", "tap_tempo", 1);
	}
}

/*--------------------------------------------------------------------
 * Function:		ctrl_handler.
 *
 * Description:		Return command from the one sent in
 * 	the poll.
 *
 *---------------------------------------------------------------------*/
static int ctrl_handler(const char *path, const char *types, lo_arg **argv, int argc,
                        void *data, void *user_data) {
	int index = argv[0]->i;
	float val  = argv[2]->f;

	printd(LogDebug, "ctrl_handler %d %s f=%f\n", index, argv[1], val);
	gMyInfo.LoopPosition = val;

	/* Check to see if we should send the
	record off command.
	*/
	if (val > 0 && (gMyInfo.RecordStopLoop >= 0)) {
		printd(LogDebug, "ctrl_handler Stop Loop\n");
		gMyInfo.RecordStopLoop = -1;
		OSCCommand(OSCRec, 0);
	}

	return 0;
}

/*--------------------------------------------------------------------
 * Function:		pingack_handler.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static int pingack_handler(const char *path, const char *types, lo_arg **argv, int argc,
                           void *data, void *user_data) {
	// pingack expects: s:engine_url s:version i:loopcount
	// 1st arg is instance, 2nd ctrl string, 3nd is float value
	//int index = argv[0]->i;
	//string eurl(&argv[0]->s);
	//string vers (&argv[1]->s);
	//int loops = argv[2]->i;
	printd(LogDebug, "pingack_handler %d %s d=%f\n", argv[0], argv[1], argv[2]);
//	_acked = TRUE;
	return 0;
}


/*--------------------------------------------------------------------
 * Function:		MyOSCInit.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void MyOSCInit(void) {

	printd(LogDebug, "MyOSCInit: %s  L=%s V=%s H=%s\n",
	       gMyInfo.OSCIPAddress,
	       gMyInfo.OSCPortNumLooper,
	       gMyInfo.OSCPortNumJackVol,
	       gMyInfo.OSCPortNumHydrogen );

	/* Used as a trigger to stop recording.
	*/
	gMyInfo.RecordStopLoop = -1;

	SLOSCaddr = lo_address_new(
	                gMyInfo.OSCIPAddress,
	                gMyInfo.OSCPortNumLooper );

	JackVoladdr = lo_address_new(
	                  gMyInfo.OSCIPAddress,
	                  gMyInfo.OSCPortNumJackVol );

	Hydrogenaddr = lo_address_new(
	                   gMyInfo.OSCIPAddress,
	                   gMyInfo.OSCPortNumHydrogen );

	printd(LogDebug, "Init Second OSC \n");

	osc_server = lo_server_new(NULL, NULL);
	strcpy(our_url, lo_server_get_url (osc_server) );
	printd(LogDebug, "MyOSCInit Leave %s  %d\n",
	       our_url,
	       osc_server );
#if 0
	osc_server1 = lo_server_new(NULL, NULL);
	strcpy(our_url, lo_server_get_url (osc_server1) );
	printd(LogDebug, "MyOSCInit Leave %s  %d\n",
	       our_url,
	       osc_server );
#endif
	lo_server_add_method(osc_server,
	                     NULL, NULL, ctrl_handler, NULL);
//	                     "/ctrl", "isf", ctrl_handler, NULL);

	lo_server_add_method(osc_server,
	                     "/pingack", "ssi", pingack_handler, NULL);
	CurrentLoop = 0;
}


/*--------------------------------------------------------------------
 * Function:		MyOSCPoll.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void MyOSCPoll(char DownBeat) {
	char NewCommand[100];

	lo_server_recv_noblock(osc_server, 2);
//    lo_send(SLOSCaddr, "/ping", "ss", our_url, "/pingack");
	sprintf(NewCommand, "/sl/%d/get", CurrentLoop);
	lo_send(SLOSCaddr, NewCommand, "sss", "loop_pos", our_url, "/ctrl");
}

/*--------------------------------------------------------------------
 * Function:		MyOSCTap.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void MyOSCTap(char DownBeat) {
	lo_send(SLOSCaddr, "/sl/-2/set", "sf", "tap_tempo", 1.0);
}

/*--------------------------------------------------------------------
 * Function:		MyOSCClose.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void MyOSCClose(void) {

	printd(LogDebug, "MyOSCClose: %x\n", osc_server);
	lo_address_free (SLOSCaddr);
	lo_address_free (JackVoladdr);
	lo_address_free (Hydrogenaddr);

	lo_server_free (osc_server);
}

/*--------------------------------------------------------------------
 * Function:		MyOSCClose.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void MyOSCSetSync(char Type) {
	char NewCommand[100];

	// sprintf(NewCommand,"/sl/%d/hit", CurrentLoop);
	printd(LogDebug, "MyOSCSetSync %d\n", Type);

	/* Sync Internal	*/
	lo_send(SLOSCaddr, "/set", "si", "sync_source", "-3");
	lo_send(SLOSCaddr, "/sl/-1/set", "sf", "sync_source", "1.0");
}

/*--------------------------------------------------------------------
 * Function:		MyOSCClose.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void MyOSCLoadFile(char *FileName) {

	/* Load the file and send the results back to the SL GUI */
	lo_send(SLOSCaddr, "/load_session", "sss", FileName,
	        "osc.udp://localhost:9951/", "osc.udp://localhost:9951/");
}


/*--------------------------------------------------------------------
 * Function:		MyOSCJackVol.
 *
 * Description:		Set Jack Master Volume
 * 		Values 0 - 127
 *---------------------------------------------------------------------*/
void MyOSCJackVol(int Volume, int channel) {
	float VolumeFloat;

	VolumeFloat = ((float)Volume / 127);
//	printf("Vol Change %d %f\n", Volume, VolumeFloat);

	switch (channel) {
	case 0xff:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/master", "f", VolumeFloat);
		break;

	case 0:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/0", "f", VolumeFloat);
		break;

	case 1:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/1", "f", VolumeFloat);
		break;

	case 2:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/2", "f", VolumeFloat);
		break;

	case 3:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/3", "f", VolumeFloat);
		break;
	}
}

/*--------------------------------------------------------------------
 * Function:		MyOSCJackMute.
 *
 * Description:		Set Jack Master Volume
 * 		Values 0 - 127
 *---------------------------------------------------------------------*/
void MyOSCJackMute(int Mute, int channel) {

	if (Mute == 1) {
		printd(LogDebug, "MyOSCJackMute On\n");
		SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
		         DrumMidiChannel, 04, (int) PedalLED7On );
	} else {
		printd(LogDebug, "MyOSCJackMute Off\n");
		SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
		         DrumMidiChannel, 04, (int) PedalLED7Off );
	}


	switch (channel) {
	case 0xff:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/master/mute", "i", Mute);
		break;

	case 0:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/0/mute", "i", Mute);
		break;

	case 1:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/1/mute", "i", Mute);
		break;

	case 2:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/2/mute", "i", Mute);
		break;

	case 3:
		lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/3/mute", "i", Mute);
		break;
	}

}


#if 0
oscsend localhost 9952 / net / mhcloud / volume / jack - volume / 0  f 1.0

oscsend localhost 9952 / net / mhcloud / volume / jack - volume / master f  1.0

load_session s

klick - o 9954  - T - L 120
oscsend localhost 9954 / klick / simple / set_tempo f 90


http://essej.net/sooperlooper/docs.html
http://essej.net/sooperlooper/doc_osc.html


oscsend localhost 9951 / sl / 0 / hit s "pause"  osc.udp: //localhost:9952/ osc.udp://localhost:9952/
oscdump  9953
oscsend localhost 9951 / load_session sss "/home/elias/Dropbox/LiveEffects/Looper/LooperSync.slsess"   osc.udp: //localhost:9952/ osc.udp://localhost:9952/
oscsend localhost 9951 / ping ss osc.udp: //localhost:9952 osc.udp://localhost:9952/
oscsend localhost 9951 / set sf "tempo"  100 osc.udp: //localhost:9952 osc.udp://localhost:9952/

oscsend localhost 9951 / loop_add  if  "1"  20 osc.udp: //localhost:9952/ osc.udp://localhost:9952/

oscsend localhost 9951 / sl / 1 / load_loop s "/home/elias/Dropbox/LiveEffects/Guitarix/guitarix/pluginpresets/loops/tape1.wav"  osc.udp: //localhost:9952/ osc.udp://localhost:9952/
oscsend localhost 9951 / sl / 1 / load_loop sss / home / elias / Dropbox / LiveEffects / Guitarix / guitarix / pluginpresets / loops / tape1.wav osc.udp: //localhost:9952/ osc.udp://localhost:9952

oscsend localhost 9951 / get sss "sync_source"   osc.udp: //localhost:9952/ osc.udp://localhost:9952/
oscsend localhost 9951 / set si "sync_source" - 1
oscsend localhost 9951 / sl / 1 / get sss "cycle_len"   osc.udp: //localhost:9952/ osc.udp://localhost:9952/


The 8th / cycle parameter defines how many 8th notes as defined by the current tempo (there are 2 eighths per beat) are in a cycle of a current loop, or a potential loop. As described below, the cycle length is useful as a quantization boundary, as well as an eighth - note itself.
	oscsend localhost 9951 / set sf "eighth_per_cycle" 10
	oscsend localhost 9951 / sl / -1 / set sf "overdub_quantized" 0
	oscsend localhost 9951 / sl / -1 / set sf "replace_quantized" 0
	oscsend localhost 9951 / sl / -1 / set sf "round" 1
	oscsend localhost 9951 / sl / -1 / set sf "relative_sync" 1
	oscsend localhost 9951 / set sf "smart_eighths" 0

	If the sync option is checked for a particular loop, operations will be quantized to the selected boundary. This includes Record, Multiply, Replace, Substitute, Insert, Reverse, Trigger, and Once. Note that Overdub is never quantized. When a command is performed, the actual operation wont start / stop until the precise moment of the next sync boundary arrives. For instance, when the sync source is a loop, and the quantize parameter is Cycle, a Record operation will start and stop on an exact cycle boundary of the source loop. More interesting polyrhythms are possible when using 8ths as the quantize parameter, for example.
		oscsend localhost 9951 / sl / -1 / set sf sync 1
		oscsend localhost 9951 / sl / -1 / set sf playback_sync 1

		The quantize parameter defines when operations sync to in reference to the sync source. The available choices are Off, Cycle, 8th, and Loop. Note that no sync will ever occur if this value is set to Off. When the sync source is another loop, the choices are all valid.
			oscsend localhost 9951 / sl / -1 / set si quantize 1
			oscsend localhost 9951 / sl / -1 / set si mute_quantized 1
			oscsend localhost 9951 / sl / -1 / hit s mute
			oscsend localhost 9951 / sl / -1 / hit s trigger


#endif