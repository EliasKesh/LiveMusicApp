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

#include "GTKMidiUI.h"
#include "SooperOSC.h"
// #include <netinet/in.h>
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
static lo_address OSCaddr;
static lo_server osc_server = 0;
static char our_url[100];
static char CurrentLoop;

int OSCCommand(int Command) {
char NewCommand[100];

	printd(LogDebug, "OSCCommand: %d\n",Command);

	switch(Command) {
		case OSCSelect1:
		CurrentLoop = 0;
		break;

		case OSCSelect2:
		CurrentLoop = 1;
		break;

		case  OSCSelect3:
		CurrentLoop = 2;
		break;

		case  OSCSelect4:
		CurrentLoop = 3;
		break;

		case  OSCRec:
			sprintf(NewCommand,"/sl/%d/hit", CurrentLoop);
			printd(LogDebug, "OSCRec %s\n",NewCommand);

			lo_send(OSCaddr, NewCommand, "s", "record");
		break;

		case  OSCPause:
		break;

		case  OSCTrig:
		break;

		case  OSCUndo:
		break;

		case OSCAddLoop:
			lo_send(OSCaddr, "/loop_add", "if", 1, DefaultLoopLength);
		break;

// lo_send(OSCaddr, "/sl/-2/set", "sf", "tap_tempo", 1);
	}

}

static int ctrl_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data)
{
	// 1st arg is instance, 2nd ctrl string, 3nd is float value
	//int index = argv[0]->i;
	char *arge1 = &argv[1];
	float val  = atof(&argv[2]);

//	params_val_map[ctrl] = val;

//	updated = true;
	printd(LogDebug, "ctrl_handler %s %s\n",argv[1], argv[2]);
	return 0;
}

static int pingack_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data)
{
	// pingack expects: s:engine_url s:version i:loopcount
	// 1st arg is instance, 2nd ctrl string, 3nd is float value
	//int index = argv[0]->i;
	//string eurl(&argv[0]->s);
	//string vers (&argv[1]->s);
	//int loops = argv[2]->i;
		printd(LogDebug, "pingack_handler %s %s\n",argv[1], argv[2]);

//	_acked = TRUE;
	return 0;
}


void MyOSCInit(void) {

	printd(LogDebug, "MyOSCInit: %s  %s\n",
			gMyInfo.OSCIPAddress, 
			gMyInfo.OSCPortNum );

	OSCaddr = lo_address_new(
			gMyInfo.OSCIPAddress, 
			gMyInfo.OSCPortNum );

    osc_server = lo_server_new(NULL, NULL);
    strcpy(our_url, lo_server_get_url (osc_server) );
	printd(LogDebug, "MyOSCInit Leave %s  %d\n",
			our_url, 
			osc_server );

    lo_server_add_method(osc_server, 
    	"/ctrl", "isf", ctrl_handler, NULL);

    lo_server_add_method(osc_server, 
    	"/pingack", "ssi", pingack_handler, NULL);

	CurrentLoop = 0;
}


void MyOSCPoll(char DownBeat) {
	

	if (DownBeat == 0) {
		printd(LogDebug, "Downbeat\n");
	}

	printd(LogDebug, "MyOSCPoll: %x %s Down %d\n", 
		osc_server, our_url, DownBeat);

	lo_server_recv_noblock (osc_server, 2);
//    lo_send(OSCaddr, "/ping", "ss", our_url, "/pingack");
//	lo_send(OSCaddr, "/sl/0/get", "sss", "loop_pos", our_url, "/ctrl");
 lo_send(OSCaddr, "/sl/-2/set", "sf", "tap_tempo", 1.0);

}


void MyOSCClose(void) {
	
	printd(LogDebug, "MyOSCClose: %x\n", osc_server);
	
	lo_address_free (OSCaddr);

	lo_server_free (osc_server);

}



#if 0

http://essej.net/sooperlooper/docs.html
http://essej.net/sooperlooper/doc_osc.html


oscsend localhost 9951 /sl/0/hit s "pause"  osc.udp://localhost:9952/ osc.udp://localhost:9952/
oscdump  9953

oscsend localhost 9951 /ping ss osc.udp://localhost:9952 osc.udp://localhost:9952/
oscsend localhost 9951 /set sf "tempo"  100 osc.udp://localhost:9952 osc.udp://localhost:9952/

oscsend localhost 9951 /loop_add  if  "1"  20 osc.udp://localhost:9952/ osc.udp://localhost:9952/

oscsend localhost 9951 /sl/1/load_loop s "/home/elias/Dropbox/LiveEffects/Guitarix/guitarix/pluginpresets/loops/tape1.wav"  osc.udp://localhost:9952/ osc.udp://localhost:9952/
oscsend localhost 9951 /sl/1/load_loop sss /home/elias/Dropbox/LiveEffects/Guitarix/guitarix/pluginpresets/loops/tape1.wav osc.udp://localhost:9952/ osc.udp://localhost:9952

oscsend localhost 9951 /get sss "sync_source"   osc.udp://localhost:9952/ osc.udp://localhost:9952/
oscsend localhost 9951 /set si "sync_source" -1
oscsend localhost 9951 /sl/1/get sss "cycle_len"   osc.udp://localhost:9952/ osc.udp://localhost:9952/
oscsend localhost 9951 /set sf "eighth_per_cycle" 10
oscsend localhost 9951 /sl/-1/set sf "overdub_quantized" 0
oscsend localhost 9951 /sl/-1/set sf "replace_quantized" 0
oscsend localhost 9951 /sl/-1/set sf "round" 1
oscsend localhost 9951 /sl/-1/set sf "relative_sync" 1
oscsend localhost 9951 /set sf "smart_eighths" 0

oscsend localhost 9951 /sl/-1/set sf sync 1
oscsend localhost 9951 /sl/-1/set sf playback_sync 1
oscsend localhost 9951 /sl/-1/set si quantize 1
oscsend localhost 9951 /sl/-1/set si mute_quantized 1
oscsend localhost 9951 /sl/-1/hit s mute
oscsend localhost 9951 /sl/-1/hit s trigger


#endif