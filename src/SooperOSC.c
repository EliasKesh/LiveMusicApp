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
|
|---------------------------------------------------------------------*/

#define SooperOSC_c

/*
 * Place Includes Here
 */

#include "LiveMusicApp.h"
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
		sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
		printd(LogDebug, "Sync On %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "sf", "sync", 1.0);
		break;

	case OSCSyncOff:
		sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
		printd(LogDebug, "Sync Off %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "sf", "sync", 0.0);
		break;

	case OSCSyncSource:
//		sprintf(NewCommand, "/sl/-1/set", CurrentLoop);
		printd(LogDebug, "OSCSyncSource %d\n", Option);
//		printd(LogDebug, "OSCStartRecord %s\n", NewCommand);
		lo_send(SLOSCaddr, "/set", "si", "sync_source", Option);
		break;

	case OSCRecThres:
		sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
		printd(LogDebug, "Sync Off %s\n", NewCommand);
		lo_send(SLOSCaddr, NewCommand, "sf", "rec_thresh", (float)Option/127 );
		break;

//oscsend localhost 9951 /set si "sync_source" -3

// lo_send(SLOSCaddr, "/sl/-2/set", "sf", "tap_tempo", 1);
	}

}

static int ctrl_handler(const char *path, const char *types, lo_arg **argv, int argc,
                        void *data, void *user_data) {
	// 1st arg is instance, 2nd ctrl string, 3nd is float value
	//int index = argv[0]->i;
	char *arge1 = (char *)&argv[1];
	float val  = atof((char *)argv[2]);

//	params_val_map[ctrl] = val;

//	updated = true;
//	printd(LogDebug, "ctrl_handler %s %s\n", argv[1], argv[2]);
	return 0;
}

static int pingack_handler(const char *path, const char *types, lo_arg **argv, int argc,
                           void *data, void *user_data) {
	// pingack expects: s:engine_url s:version i:loopcount
	// 1st arg is instance, 2nd ctrl string, 3nd is float value
	//int index = argv[0]->i;
	//string eurl(&argv[0]->s);
	//string vers (&argv[1]->s);
	//int loops = argv[2]->i;
	printd(LogDebug, "pingack_handler %s %s\n", argv[1], argv[2]);

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
	                     "/ctrl", "isf", ctrl_handler, NULL);

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

	lo_server_recv_noblock(osc_server, 2);
//    lo_send(SLOSCaddr, "/ping", "ss", our_url, "/pingack");
	lo_send(SLOSCaddr, "/sl/0/get", "sss", "loop_pos", our_url, "/ctrl");
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
	lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/master", "f", (float)Volume / 127);
}

/*--------------------------------------------------------------------
 * Function:		MyOSCJackMute.
 *
 * Description:		Set Jack Master Volume
 * 		Values 0 - 127
 *---------------------------------------------------------------------*/
void MyOSCJackMute(int Mute, int channel) {
	lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/master/mute", "i", Mute);
}

//	oscsend localhost 9951 /net/mhcloud/volume/jack-volume/master f  1.0

#if 0

load_session s

klick -o 9954  -T -L 120
oscsend localhost 9954 /klick/simple/set_tempo f 90


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