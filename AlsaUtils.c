/*
 * AlsaUtils.c
 *
 *  Created on: Dec 25, 2012
 *	  Author: elias
 */
#include "stdlib.h"
#include <string.h>
#include "stdbool.h"
#include "GTKMidiUI.h"
#include "AlsaUtils.h"
#include <glib.h>
#include "alsa/timer.h"
#include "SooperOSC.h"

//snd_seq_t *SeqPort1;
//snd_seq_t *SeqPort2;
snd_seq_t *SeqPort1In;
// extern int SeqPort1Port;
snd_mixer_t *MixerHandle;
char *MixerHardwareName = "default";
const char *selem_name = "Master";
char TestProgram = 0;
// Midi thread for input
int g_midi_ignore;
pthread_t g_alsa_midi_tid; /* alsa_midi_thread id */
unsigned long MixerVolume;

snd_seq_t *CreatePort(snd_seq_t *Seq, char *Name);
void ProgramChange(unsigned int InputChange);
static void device_list(void);
static void pcm_list(void);
snd_seq_event_t MTCev;

static void async_callback(snd_async_handler_t *ahandler) {
int err;

	if (gMyInfo.TempoReload == ++gMyInfo.TimerCount) {
		gMyInfo.TimerCount = 0;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[TempoPort], &MTCev);
//		snd_seq_drain_output(gMyInfo.SeqPort[TempoPort]);
		ToggleTempo();
	}
}

void show_status(void *handle)
{
	int err;
	snd_timer_status_t *status;

	snd_timer_status_alloca(&status);
	if ((err = snd_timer_status(handle, status)) < 0) {
		fprintf(stderr, "timer status %i (%s)\n", err, snd_strerror(err));
		return;
	}
	printd(LogInfo, "STATUS:\n");
	printd(LogInfo, "  resolution = %li\n", snd_timer_status_get_resolution(status));
	printd(LogInfo, "  lost = %li\n", snd_timer_status_get_lost(status));
	printd(LogInfo, "  overrun = %li\n", snd_timer_status_get_overrun(status));
	printd(LogInfo, "  queue = %li\n", snd_timer_status_get_queue(status));
}

bool MyAlsaClose(void) {
	int ret;

	/* Cancel the thread. Don't know better way.
	 Poll or unblock mechanisms seem to not be
	 available for alsa sequencer */
	pthread_cancel(g_alsa_midi_tid);

	/* Wait midi thread to finish */
	ret = pthread_join(g_alsa_midi_tid, NULL);

	ret = snd_seq_close(SeqPort1In);
	if (ret < 0) {
		g_warning("Cannot close sequncer, %s\n", snd_strerror(ret));
	}
	snd_mixer_close(MixerHandle);
return(FALSE);
}

/*--------------------------------------------------------------------
 * Function:		MyAlsaInit
 *
 * Description:		Set up Alsa Midi and create the I/O Ports
 *
 *---------------------------------------------------------------------*/
bool MyAlsaInit() {
	int Loop;
	snd_seq_t *Seq;

	if (snd_seq_open(&Seq, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
		//   cerr<<"Could not open ALSA SeqPort1uencer: "<<snd_strerror(errno)<<endl;
		return false;
	}
	printd(LogInfo, "Init Ports  %d\n", gMyInfo.NumOutPorts);

	for (Loop = 0; Loop <= gMyInfo.NumOutPorts; Loop++) {
		gMyInfo.SeqPort[Loop] = CreatePort(Seq, gMyInfo.OutPortName[Loop]);
		printd(LogInfo, "Port %d %s\n", Loop, gMyInfo.OutPortName[Loop]);
//               gMyInfo.SeqQueue[Loop] = snd_seq_alloc_queue(gMyInfo.SeqPort[Loop]);
	}

#if 0
// Using the Same Seq causes it to add increment the port number.
	snd_seq_set_client_name(Seq, "LiveMusic");
	Loop = snd_seq_create_simple_port(Seq, "input",
		SND_SEQ_PORT_CAP_WRITE |
		SND_SEQ_PORT_CAP_SUBS_WRITE,
		SND_SEQ_PORT_TYPE_APPLICATION);
#endif

	alsa_input_init("LiveMusic");

	device_list();

	/*
	 * Open Mixer for manipulation
	 */
    snd_mixer_open(&MixerHandle, 0);
    snd_mixer_attach(MixerHandle, JackName);
    snd_mixer_selem_register(MixerHandle, NULL, NULL);
    snd_mixer_load(MixerHandle);

//      pcm_list();
//      queue_id = snd_seq_alloc_queue(seq_handle);
	return true;
}

void SetupAlsaTimer(int Count) {
	char timername[64];
	snd_async_handler_t *ahandler;
	int idx, err;
	int acount = 0;
	long NewDivider;
# if 0
	if (gMyInfo.AlsaTimerHandle) {
		printd(LogInfo, "Closing and freeing timers1\n");
		snd_timer_stop(gMyInfo.AlsaTimerHandle);
		printd(LogInfo, "Closing and freeing timers2\n");
		snd_timer_close(gMyInfo.AlsaTimerHandle);
		printd(LogInfo, "Closing and freeing timers3\n");
//				snd_timer_id_free(gMyInfo.AlsaTimerid);
		printd(LogInfo, "Closing and freeing timers4\n");
//  			snd_timer_info_free(&gMyInfo.AlsaTimerinfo);
		printd(LogInfo, "Closing and freeing timers5\n");
//  			snd_timer_params_free(&gMyInfo.AlsaTimerParams);
		printd(LogInfo, "Closing and freeing timers6\n");
		gMyInfo.AlsaTimerHandle = NULL;
//   			gMyInfo.AlsaTimerid = 0;
		//  			gMyInfo.AlsaTimerinfo = NULL;;
		// 			gMyInfo.AlsaTimerParams = NULL;
	} else {
#endif
	snd_timer_id_alloca(&gMyInfo.AlsaTimerid);
	snd_timer_info_alloca(&gMyInfo.AlsaTimerinfo);
	snd_timer_params_alloca(&gMyInfo.AlsaTimerParams);

	printd(LogInfo, "**************\n Setting up timers.\n *********************\n");

//	    	sprintf(timername, "hw:CLASS=%i,SCLASS=%i,CARD=%i,DEV=%i,SUBDEV=%i", class, sclass, card, device, subdevice);
	sprintf(timername, "hw:CLASS=%i,SCLASS=%i,CARD=%i,DEV=%i,SUBDEV=%i", 1, -1,
		0, 3, 0);
	printd(LogInfo, "Timer Name %s\n", timername);
	if ((err = snd_timer_open(&gMyInfo.AlsaTimerHandle, timername,
		SND_TIMER_OPEN_NONBLOCK)) < 0) {
		printd(LogInfo, "timer open %i (%s)\n", err, snd_strerror(err));
		exit(EXIT_FAILURE);
	}

	if ((err = snd_timer_info(gMyInfo.AlsaTimerHandle, gMyInfo.AlsaTimerinfo))
		< 0) {
		printd(LogInfo, "timer info %i (%s)\n", err, snd_strerror(err));
		exit(0);
	}
	printd(LogInfo, "Timer info:\n");
	printd(LogInfo, "  slave = %s\n",
		snd_timer_info_is_slave(gMyInfo.AlsaTimerinfo) ? "yes" : "no");
	printd(LogInfo, "  card = %i\n", snd_timer_info_get_card(gMyInfo.AlsaTimerinfo));
	printd(LogInfo, "  id = '%s'\n", snd_timer_info_get_id(gMyInfo.AlsaTimerinfo));
	printd(LogInfo, "  name = '%s'\n", snd_timer_info_get_name(gMyInfo.AlsaTimerinfo));
	printd(LogInfo, "  average resolution = %li\n",
		snd_timer_info_get_resolution(gMyInfo.AlsaTimerinfo));
	snd_timer_params_set_auto_start(gMyInfo.AlsaTimerParams, 1);
//	    	NewDivider = 2500000000/Count;
	NewDivider = 100000;

	/*
	 * Let's not do anything until we get a tempo change call.
	 */
	gMyInfo.TempoReload = 0x0fffffff;
	gMyInfo.TimerCount = 0;

	if (!snd_timer_info_is_slave(gMyInfo.AlsaTimerinfo)) {
//		snd_timer_params_set_ticks(gMyInfo.AlsaTimerParams,
//			(1000000000 / snd_timer_info_get_resolution(gMyInfo.AlsaTimerinfo))
//				/ 50); /* 50Hz */
		if (snd_timer_params_get_ticks(gMyInfo.AlsaTimerParams) < 1)
			snd_timer_params_set_ticks(gMyInfo.AlsaTimerParams, 1);
		snd_timer_params_set_ticks(gMyInfo.AlsaTimerParams, NewDivider);

		printd(LogInfo, "Using %li tick(s)\n",
			snd_timer_params_get_ticks(gMyInfo.AlsaTimerParams));
	} else {
		snd_timer_params_set_ticks(gMyInfo.AlsaTimerParams, 1);
	}

	if ((err = snd_timer_params(gMyInfo.AlsaTimerHandle,
		gMyInfo.AlsaTimerParams)) < 0) {
		printd(LogInfo, "timer params %i (%s)\n", err, snd_strerror(err));
		exit(0);
	}
	show_status(gMyInfo.AlsaTimerHandle);

	err = snd_async_add_timer_handler(&ahandler, gMyInfo.AlsaTimerHandle,
		async_callback, &acount);
	if (err < 0) {
		printd(LogInfo, "unable to add async handler %i (%s)\n", err, snd_strerror(err));
		exit(EXIT_FAILURE);
	}

	if ((err = snd_timer_start(gMyInfo.AlsaTimerHandle)) < 0) {
		printd(LogInfo, "timer start %i (%s)\n", err, snd_strerror(err));
		exit(EXIT_FAILURE);
	}

	/*
	 * Set up the structure for the interrupt event .
	 */
	snd_seq_ev_clear(&MTCev);
	snd_seq_ev_set_source(&MTCev, TempoPort);
	snd_seq_ev_set_subs(&MTCev);

	/* Channel, Controller, Value
	 */
	snd_seq_ev_set_controller(&MTCev, 0, 0, 0);

	/* Send with out queueing.
	 */
	snd_seq_ev_set_direct(&MTCev);
	MTCev.type = SND_SEQ_EVENT_CLOCK;


}
/*--------------------------------------------------------------------
 * Function:		CreatePort
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
snd_seq_t *CreatePort(snd_seq_t *Seq, char *Name) {
	int SeqStatus;

	snd_seq_set_client_name(Seq, "LiveMusic");
	SeqStatus = snd_seq_create_simple_port(Seq, Name,
	SND_SEQ_PORT_CAP_READ |
	SND_SEQ_PORT_CAP_SUBS_READ,
	SND_SEQ_PORT_TYPE_APPLICATION);
// SND_SEQ_PORT_TYPE_PORT, SND_SEQ_PORT_TYPE_APPLICATION

	if (SeqStatus < 0) {
//	cerr<<"Error creating SeqPort1uencer port: "<<snd_strerror(errno)<<endl;
		return false;
	}

	return Seq;
}

#define TICKS_PER_QUARTER 128
#define MAX_SEQ_LEN        64
/*--------------------------------------------------------------------
 * Function:             SendMidi
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
int SendMidi(char Type, char Port1, char Channel, char Controller, int Value) {
	snd_seq_event_t ev;
	int err;
	int	Port;
	unsigned long adjbpm;
	snd_seq_queue_tempo_t *queue_tempo;

	Port = Port1;
	snd_seq_ev_clear(&ev);
	snd_seq_ev_set_source(&ev, Port);
	snd_seq_ev_set_subs(&ev);

	/* Channel, Controller, Value
	 */
	snd_seq_ev_set_controller(&ev, Channel - 1, Controller, (int )Value);

	/* Send with out queueing.
	 */
	snd_seq_ev_set_direct(&ev);

	//      snd_seq_ev_set_dest(&ev, snd_seq_client_id(gMyInfo.SeqPort[Port]), Port);

	if (Type == SND_SEQ_EVENT_PGMCHANGE) {
		ev.type = SND_SEQ_EVENT_PGMCHANGE;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);
	}

	if (Type == SND_SEQ_EVENT_CONTROLLER) {
		ev.type = SND_SEQ_EVENT_CONTROLLER;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);
	}
//	SND_SEQ_EVENT_SAMPLE_VOLUME
	if (Type == MIDI_CTL_MSB_MAIN_VOLUME) {
		ev.type = SND_SEQ_EVENT_CONTROLLER;
		ev.data.control.param = MIDI_CTL_MSB_MAIN_VOLUME;
//		ev.data.sample.param.volume = Value;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);
		printd(LogInfo, "SendMidi Volume %d %d\n", Port, err);

	}

	if (Type == SND_SEQ_EVENT_NOTEON) {
		ev.type = SND_SEQ_EVENT_NOTEON;
		ev.data.note.note = Value;
		ev.data.note.velocity = 100;
		ev.data.note.duration = 300;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);

	}

	if (Type == SND_SEQ_EVENT_NOTEOFF) {
		ev.type = SND_SEQ_EVENT_NOTEOFF;
		ev.data.note.note = Value;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);

	}

	if (Type == SND_SEQ_EVENT_START) {
		ev.type = SND_SEQ_EVENT_START;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);

	}

	if (Type == SND_SEQ_EVENT_STOP) {
		ev.type = SND_SEQ_EVENT_STOP;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);

	}

	if (Type == SND_SEQ_EVENT_CONTINUE) {
		ev.type = SND_SEQ_EVENT_CONTINUE;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);

	}

	if (Type == SND_SEQ_EVENT_SETPOS_TICK) {
		ev.type = SND_SEQ_EVENT_SETPOS_TICK;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);

	}

	if (Type == SND_SEQ_EVENT_SETPOS_TIME) {
		ev.type = SND_SEQ_EVENT_SETPOS_TIME;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);

	}
#if 0
	Since there are 24 MIDI Clocks in every quarter note, the length of a MIDI Clock
	(ie, time inbetween each MIDI Clock message) is the microsecond tempo divided
	by 24. In the above example, that would be 500,000/24, or 20,833.3
	microseconds in every MIDI Clock. Alternately, you can relate this to your
	timebase (ie, PPQN clock). If you have 96 PPQN, then that means that a
	MIDI Clock byte must occur every 96 / 24 (ie, 4) PPQN clocks.
#endif
	if (Type == SND_SEQ_EVENT_CLOCK) {
		ev.type = SND_SEQ_EVENT_CLOCK;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);
		snd_seq_drain_output(gMyInfo.SeqPort[Port]);
	}

	if (Type == SND_SEQ_EVENT_QFRAME) {
		ev.type = SND_SEQ_EVENT_CLOCK;
		ev.data.queue.param.d32[1] = Value;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);
		snd_seq_drain_output(gMyInfo.SeqPort[Port]);
	}

	/* Every 10ms
	 */
	if (Type == SND_SEQ_EVENT_TICK) {
		ev.type = SND_SEQ_EVENT_TICK;
		ev.data.queue.param.d32[1] = Value;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);
		snd_seq_drain_output(gMyInfo.SeqPort[Port]);

#if 0
		snd_seq_queue_sync_t sync_info;
		snd_seq_addr_t dest;

		memset(&sync_info, 0, sizeof(sync_info));
		sync_info.format = SND_SEQ_SYNC_PRIVATE_CLOCK;
		sync_info.ppq = 0;
		sync_info.ticks = 4;
		dest.client = my_client;
		dest.port = my_port;
		ev.flags = SND_SEQ_TIME_STAMP_REAL | SND_SEQ_TIME_MODE_ABS;

		snd_seq_add_sync_master(handle, queue, &dest, &sync_info);

		snd_seq_add_sync_master_mtc(handle, queue, &dest, time_format);

#endif
	}

#if 0
	The tempo is given in micro seconds per quarter beat. To convert this to BPM we needs to use the following equation:
	BPM = 60,000,000/[tt tt tt]
#endif
	if (Type == SND_SEQ_EVENT_TEMPO) {
		ev.type = SND_SEQ_EVENT_TEMPO;
//        ev.dest.client = SND_SEQ_CLIENT_SYSTEM;
//        ev.dest.port = SND_SEQ_PORT_SYSTEM_TIMER;

		adjbpm = (unsigned long) ((unsigned long) (60.0 * 1000000.0)
			/ (unsigned long) Value);
		printd(LogInfo, "Tempo Change %d %ld port %d\n", Value, adjbpm, Port);
		ev.data.queue.param.value = adjbpm;
		err = snd_seq_event_output_direct(gMyInfo.SeqPort[Port], &ev);
		snd_seq_drain_output(gMyInfo.SeqPort[Port]);
	}

//printd(LogInfo, "SendMidi err = %d\n", err);
	return (err);
}
#if 0
int change_tempo(snd_seq_t *handle, int q, unsigned int tempo) {
	snd_seq_event_t ev;
	snd_seq_ev_clear(&ev);
	ev.dest.client = SND_SEQ_CLIENT_SYSTEM;
	ev.dest.port = SND_SEQ_PORT_SYSTEM_TIMER;
	ev.source.client = my_client_id;
	ev.source.port = my_port_id;
	ev.queue = SND_SEQ_QUEUE_DIRECT; // no scheduling
	ev.data.queue.queue = q;// affected queue id
	ev.data.queue.value = tempo;// new tempo in microsec.
	return snd_seq_event_output(handle, &ev);
}
#endif
/*--------------------------------------------------------------------
 * Function:		SendMidiPatch
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
int SendMidiPatch(PatchInfo *thePatch) {
	int err = 0;

	switch (thePatch->CustomCommand) {
		case NoCustom:
			/* If there is a bank number then send it first.
			 */
			if (thePatch->BankSelect != 0xff) {
				SendMidi(SND_SEQ_EVENT_CONTROLLER, thePatch->OutPort,
					thePatch->Channel,
					MIDI_CTL_MSB_BANK, (thePatch->BankSelect >> 7) & 0x7f);
				SendMidi(SND_SEQ_EVENT_CONTROLLER, thePatch->OutPort,
					thePatch->Channel,
					MIDI_CTL_LSB_BANK, thePatch->BankSelect & 0x7f);
				printd(LogInfo, "MidiPatch Con %x %x \n",
					(thePatch->BankSelect >> 7) & 0x7f,
					thePatch->BankSelect & 0x7f);
			}
			/* Now send the standard program change.
			 */
			TestProgram++;
			SendMidi(SND_SEQ_EVENT_PGMCHANGE, thePatch->OutPort,
				thePatch->Channel,
				TestProgram, thePatch->Patch);
			printd(LogInfo, "MidiPatch %x %x %x %x %x\n", thePatch->Channel,
				thePatch->BankSelect, thePatch->Patch, thePatch->OutPort);
			LastPortUsed = thePatch->OutPort;

			break;

		case ToNextDesktop:
			NextDesktop();
			break;

		case ToPrevDesktop:
			PrevDesktop();
			break;

		case ToDesktop:
			GoToDesktop(thePatch->Patch);
			break;

#if 0
			case cmdPreset:
			if (thePatch->Patch == 1)
			SendMidiPatch(&gMyInfo.MyPatchInfo[gMyInfo.WebPresets.thePreset1]);

			if (thePatch->Patch == 2)
			SendMidiPatch(&gMyInfo.MyPatchInfo[gMyInfo.WebPresets.thePreset2]);

			break;
#endif
		case cmdMidiSelect:
			GuitarMidiPreset();
			break;

		case cmdBankSelect:
			printd(LogInfo, "Bank Select COmmand\n");
			IncrementMode();
			break;

		case cmdSetList:
			printd(LogInfo, "cmdSetList Command %d \n", thePatch->Patch);
			if (thePatch->Patch == 1)
				OpenSetListSong(CurrentSetListSong -1);

			if (thePatch->Patch == 2)
				OpenSetListSong(CurrentSetListSong +1);

			break;

		case cmdScroll:
			ScrollDown(1);
			break;

		case Controller:
#if 1
			SendMidi(SND_SEQ_EVENT_CONTROLLER, thePatch->OutPort,
				thePatch->Channel,
				thePatch->BankSelect, thePatch->Patch);
#endif

			break;

			//   case ToLooperApp:

//       break;

		case SwitchTab:
			SwitchToTab(thePatch->Patch);
			break;

		case RaiseApp:
			RaiseWindowsNum(thePatch->Patch);
			break;

		case TransStart:
			printd(LogInfo, "Trans Start\n");
			SendMidi(SND_SEQ_EVENT_START, thePatch->OutPort, thePatch->Channel,
				TestProgram, thePatch->Patch);
			break;

		case TransStop:
			SendMidi(SND_SEQ_EVENT_STOP, thePatch->OutPort, thePatch->Channel,
				TestProgram, thePatch->Patch);
			break;

		case TransCont:
			SendMidi(SND_SEQ_EVENT_CONTINUE, thePatch->OutPort,
				thePatch->Channel,
				TestProgram, thePatch->Patch);
			break;

		case TransPosition:
			SendMidi(SND_SEQ_EVENT_SETPOS_TICK, thePatch->OutPort,
				thePatch->Channel, TestProgram, thePatch->Patch);
			break;

		case TransTempo:
			SendMidi(SND_SEQ_EVENT_TEMPO, thePatch->OutPort, thePatch->Channel,
				TestProgram, thePatch->Patch);
			break;

		case cmdCountIn:
			printd(LogInfo, "Setting Count Active\n");
			CountInCount = thePatch->Patch;
			CountInActive = 2;
			break;

		case cmdVolume:
			SetVolume1(thePatch->Patch + gMyInfo.AnalogVolume);
			break;

		/* OSC to sooperlooper
		*/
		case cmdOSC:
			OSCCommand(thePatch->Patch);
			break;

		case cmdLnTransPort:

			switch(thePatch->Patch) {
				case cmdLnTSetA:
					 plSetA() ;
//					system("/usr/bin/smplayer -send-action set_a_marker & ");
//					Mysystem("set_a_marker");
					break;

				case cmdLnTSetB:
					 plSetB() ;
//					Mysystem("set_b_marker");
//					system("/usr/bin/smplayer -send-action set_b_marker & ");
					break;

				case cmdLnTStart:
					plPausePlay();
					break;

				case cmdLnTLoop:
				plLoopToggle();
				break;

				case cmdLnTNext:
					plNextSeg();
					break;

				case	cmdLnTPrev:
					plPrevSeg();
					break;

				case cmdLnTUp:
					plSpeedUp();
					break;

				case cmdLnTDown:
					plSpeedDown();
					break;

				case cmdLnTSeekFw:
					plSeekFw();
					break;

				case cmdLnTSeekBk:
					plSeekBk();
					break;

				default:
					printd(LogError, "cmdLnTransPort No case %d",thePatch->Patch);

					break;

			}
			break;


// SND_SEQ_EVENT_SETPOS_TIME
// SND_SEQ_EVENT_TEMPO

		default:
			return (1);
	}

	return (err);
}

int Mysystem(char *cmd) {
    pid_t pid = fork();
    char *argv[4];
    extern char **environ;

    if (pid == 0) { /* child */
        argv[0] = "/usr/bin/smplayer";
        argv[1] = "-send-action";
        argv[2] = cmd;
        argv[3] = NULL;
        execve("/usr/bin/smplayer", argv, environ);
        _exit(127);
    }
    if (pid < 0)
     return 0; /* as provided by sh -c, or from _exit(127) above */

    return 1;
}
/*--------------------------------------------------------------------
 * Function:		alsa_midi_thread
 *
 * Description:		This is a seperate thread that handles midi messages
 * 		received.
 *
 *---------------------------------------------------------------------*/
void *alsa_midi_thread(void * context_ptr) {
//  GtkTreeIter iter;
	snd_seq_event_t * event_ptr;
//  GtkListStore * list_store_ptr;
//  GtkWidget * child_ptr;
	char time_str_ptr[255];
	char msg_str_ptr[255];
	char channel_str_ptr[255];
	const char * note_name;
	int octave;
	const char * drum_name;
	const char * cc_name;
	tPatchIndex PatchIndex;

//  child_ptr = GTK_WIDGET (gtk_builder_get_object (g_builder, "list"));

//  list_store_ptr = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(child_ptr)));

	while (snd_seq_event_input(SeqPort1In, &event_ptr) >= 0) {
		printf("Event Type %d %d %d %d\n", event_ptr->type, 
			event_ptr->data.control.channel, event_ptr->data.control.param,
			event_ptr->data.control.value);
		// 10 7 63 1,2,3
//		g_print("Midi Event:\n");

//	if (g_midi_ignore)
//	  continue;

//	time_str_ptr = g_string_new("");
		sprintf(time_str_ptr, "%u:%u",
			(unsigned int) event_ptr->time.time.tv_sec,
			(unsigned int) event_ptr->time.time.tv_nsec);

//   channel_str_ptr = g_string_new("");

		/* Workaround for compiler warnings... */
		drum_name = NULL;
		note_name = NULL;
		octave = 0;

		if (event_ptr->type == SND_SEQ_EVENT_NOTE
			|| event_ptr->type == SND_SEQ_EVENT_NOTEON
			|| event_ptr->type == SND_SEQ_EVENT_NOTEOFF
			|| event_ptr->type == SND_SEQ_EVENT_KEYPRESS) {
			sprintf(channel_str_ptr, "%u",
				(unsigned int) event_ptr->data.note.channel + 1);
			printd(LogInfo, "%s %s\n", time_str_ptr, channel_str_ptr);

#if 0
			if (event_ptr->data.note.channel + 1 == 10) {
				drum_name = gm_get_drum_name(event_ptr->data.note.note);
			} else {
				drum_name = NULL;
			}
#endif

//	  note_name = g_note_names[event_ptr->data.note.note % 12];
//	  octave = event_ptr->data.note.note / 12 - 1;
		}

		if (event_ptr->type == SND_SEQ_EVENT_CONTROLLER
			|| event_ptr->type == SND_SEQ_EVENT_PGMCHANGE
			|| event_ptr->type == SND_SEQ_EVENT_PITCHBEND) {
			sprintf(channel_str_ptr, "%u",
				(unsigned int) event_ptr->data.control.channel + 1);
		}

//	msg_str_ptr = g_string_new("unknown event");
		switch (event_ptr->type) {
			case SND_SEQ_EVENT_SYSTEM:
				sprintf(msg_str_ptr, "System event");
				break;
			case SND_SEQ_EVENT_RESULT:
				sprintf(msg_str_ptr, "Result status event");
				break;
			case SND_SEQ_EVENT_NOTE:
				sprintf(msg_str_ptr, "Note");
				if (WaitingforMidi) {
					GuitarMidiPresetComplete(event_ptr->data.control.value);
				}
				break;
			case SND_SEQ_EVENT_NOTEON:
				if (WaitingforMidi) {
					GuitarMidiPresetComplete(event_ptr->data.note.note);
				}
#if 0
				else {
					if (event_ptr->data.note.velocity != 0) {
						if (drum_name != NULL) {
							sprintf(msg_str_ptr,
								"Drum: %s (%s, octave %d, velocity %u)", drum_name,
								note_name, octave, event_ptr->data.note.velocity);
						} else {
							sprintf(msg_str_ptr, "Note on, %s, octave %d, velocity %u",
								note_name, octave, event_ptr->data.note.velocity);
						}
					}
				}
#endif
				break;
			case SND_SEQ_EVENT_NOTEOFF:
				if (drum_name != NULL) /* ignore note off for drums */
					continue;
				if (!WaitingforMidi) {
					sprintf(msg_str_ptr, "Note off, %s, octave %d", note_name,
						octave);
				} else
					continue;

				break;
			case SND_SEQ_EVENT_KEYPRESS:
				sprintf(msg_str_ptr, "Key pressure change (aftertouch)");
				break;
			case SND_SEQ_EVENT_CONTROLLER:
				cc_name = NULL;
				switch (event_ptr->data.control.param) {
					case MIDI_CTL_MSB_BANK:
						cc_name = "Bank selection";
						break;
					case MIDI_CTL_MSB_MODWHEEL:
						cc_name = "Modulation";
						break;
					case MIDI_CTL_MSB_BREATH:
						cc_name = "Breath";
						break;
					case MIDI_CTL_MSB_FOOT:
						cc_name = "Foot";
						break;
					case MIDI_CTL_MSB_PORTAMENTO_TIME:
						cc_name = "Portamento time";
						break;
					case MIDI_CTL_MSB_DATA_ENTRY:
						cc_name = "Data entry";
						break;

						/* Here is the main Volume Pedal.
						 */
					case MIDI_CTL_MSB_MAIN_VOLUME:
						// ejk SEND
						cc_name = "Main volume";
						printd(LogInfo, "Send Midi Volume %d\n",
							event_ptr->data.control.value);
						
						/* If we are in Guitar mode.
						*/
						if ( (FishmanSwitch != 2) && (FishmanSwitch != 3) ) {
							SetVolume1(event_ptr->data.control.value);
						} else {
							SendMidi(MIDI_CTL_MSB_MAIN_VOLUME, 0, 1, TestProgram,
							event_ptr->data.control.value);
						}
						break;
					case MIDI_CTL_MSB_BALANCE:
						cc_name = "Balance";
						break;
					case MIDI_CTL_MSB_PAN:
						cc_name = "Panpot";
						break;
					case MIDI_CTL_MSB_EXPRESSION:
						cc_name = "Expression";
						break;
					case MIDI_CTL_MSB_EFFECT1:
						cc_name = "Effect1";
						break;
					case MIDI_CTL_MSB_EFFECT2:
						cc_name = "Effect2";
						break;
					case MIDI_CTL_MSB_GENERAL_PURPOSE1:
						cc_name = "General purpose 1";
						break;
					case MIDI_CTL_MSB_GENERAL_PURPOSE2:
						cc_name = "General purpose 2";
						break;
					case MIDI_CTL_MSB_GENERAL_PURPOSE3:
						cc_name = "General purpose 3";
						break;
					case MIDI_CTL_MSB_GENERAL_PURPOSE4:
						cc_name = "General purpose 4";
						break;
					case MIDI_CTL_LSB_BANK:
						cc_name = "Bank selection";
						break;
					case MIDI_CTL_LSB_MODWHEEL:
						cc_name = "Modulation";
						break;
					case MIDI_CTL_LSB_BREATH:
						cc_name = "Breath";
						break;
						/* Possible use of Custom Pedal messages here.
						 */
					case MIDI_CTL_LSB_FOOT:
						cc_name = "Foot";
						break;
					case MIDI_CTL_LSB_PORTAMENTO_TIME:
						cc_name = "Portamento time";
						break;
					case MIDI_CTL_LSB_DATA_ENTRY:
						cc_name = "Data entry";
						break;
					case MIDI_CTL_LSB_MAIN_VOLUME:
						cc_name = "LSB Main volume";
						SetVolume2(event_ptr->data.control.value);
						break;
					case MIDI_CTL_LSB_BALANCE:
						cc_name = "Balance";
						break;
					case MIDI_CTL_LSB_PAN:
						cc_name = "Panpot";
						break;
					case MIDI_CTL_LSB_EXPRESSION:
						cc_name = "Expression";
						break;
					case MIDI_CTL_LSB_EFFECT1:
						cc_name = "Effect1";
						break;
					case MIDI_CTL_LSB_EFFECT2:
						cc_name = "Effect2";
						break;
					case MIDI_CTL_LSB_GENERAL_PURPOSE1:
						cc_name = "General purpose 1";
						break;
					case MIDI_CTL_LSB_GENERAL_PURPOSE2:
						cc_name = "General purpose 2";
						break;
					case MIDI_CTL_LSB_GENERAL_PURPOSE3:
						cc_name = "General purpose 3";
						break;
					case MIDI_CTL_LSB_GENERAL_PURPOSE4:
						cc_name = "General purpose 4";
						break;
					case MIDI_CTL_SUSTAIN:
						cc_name = "Sustain pedal";
						break;
					case MIDI_CTL_PORTAMENTO:
						cc_name = "Portamento";
						break;
					case MIDI_CTL_SOSTENUTO:
						cc_name = "Sostenuto";
						break;
					case MIDI_CTL_SOFT_PEDAL:
						cc_name = "Soft pedal";
						break;
					case MIDI_CTL_LEGATO_FOOTSWITCH:
						cc_name = "Legato foot switch";
						break;
					case MIDI_CTL_HOLD2:
						cc_name = "Hold2";
						break;
					case MIDI_CTL_SC1_SOUND_VARIATION:
						cc_name = "SC1 Sound Variation";
						break;
					case MIDI_CTL_SC2_TIMBRE:
						cc_name = "SC2 Timbre";
						break;
					case MIDI_CTL_SC3_RELEASE_TIME:
						cc_name = "SC3 Release Time";
						break;
					case MIDI_CTL_SC4_ATTACK_TIME:
						cc_name = "SC4 Attack Time";
						break;
					case MIDI_CTL_SC5_BRIGHTNESS:
						cc_name = "SC5 Brightness";
						break;
					case MIDI_CTL_SC6:
						cc_name = "SC6";
						break;
					case MIDI_CTL_SC7:
						cc_name = "SC7";
						break;
					case MIDI_CTL_SC8:
						cc_name = "SC8";
						break;
					case MIDI_CTL_SC9:
						cc_name = "SC9";
						break;
					case MIDI_CTL_SC10:
						cc_name = "SC10";
						break;
					case MIDI_CTL_GENERAL_PURPOSE5:
						cc_name = "General purpose 5";
						break;
					case MIDI_CTL_GENERAL_PURPOSE6:
						cc_name = "General purpose 6";
						break;
					case MIDI_CTL_GENERAL_PURPOSE7:
						cc_name = "General purpose 7";
						break;
					case MIDI_CTL_GENERAL_PURPOSE8:
						cc_name = "General purpose 8";
						break;
					case MIDI_CTL_PORTAMENTO_CONTROL:
						cc_name = "Portamento control";
						break;
					case MIDI_CTL_E1_REVERB_DEPTH:
						cc_name = "E1 Reverb Depth";
						break;
					case MIDI_CTL_E2_TREMOLO_DEPTH:
						cc_name = "E2 Tremolo Depth";
						break;
					case MIDI_CTL_E3_CHORUS_DEPTH:
						cc_name = "E3 Chorus Depth";
						break;
					case MIDI_CTL_E4_DETUNE_DEPTH:
						cc_name = "E4 Detune Depth";
						break;
					case MIDI_CTL_E5_PHASER_DEPTH:
						cc_name = "E5 Phaser Depth";
						break;
					case MIDI_CTL_DATA_INCREMENT:
						cc_name = "Data Increment";
						break;
					case MIDI_CTL_DATA_DECREMENT:
						cc_name = "Data Decrement";
						break;
					case MIDI_CTL_NONREG_PARM_NUM_LSB:
						cc_name = "Non-registered parameter number";
						break;
					case MIDI_CTL_NONREG_PARM_NUM_MSB:
						cc_name = "Non-registered parameter number";
						break;
					case MIDI_CTL_REGIST_PARM_NUM_LSB:
						cc_name = "Registered parameter number";
						break;
					case MIDI_CTL_REGIST_PARM_NUM_MSB:
						cc_name = "Registered parameter number";
						break;
					case MIDI_CTL_ALL_SOUNDS_OFF:
						cc_name = "All sounds off";
						break;
					case MIDI_CTL_RESET_CONTROLLERS:
						cc_name = "Reset Controllers";
						break;
					case MIDI_CTL_LOCAL_CONTROL_SWITCH:
						cc_name = "Local control switch";
						break;
					case MIDI_CTL_ALL_NOTES_OFF:
						cc_name = "All notes off";
						break;
					case MIDI_CTL_OMNI_OFF:
						cc_name = "Omni off";
						break;
					case MIDI_CTL_OMNI_ON:
						cc_name = "Omni on";
						break;
					case MIDI_CTL_MONO1:
						cc_name = "Mono1";
						break;
					case MIDI_CTL_MONO2:
						cc_name = "Mono2";
						break;

					case 63:
						FishmanSwitch = event_ptr->data.control.value;
						printf("Fishman Switch %d\n", event_ptr->data.control.value);
					break;

				}

#if 0
				if (cc_name != NULL) {
					sprintf(msg_str_ptr, "CC %s (%u), value %u", cc_name,
						(unsigned int) event_ptr->data.control.param,
						(unsigned int) event_ptr->data.control.value);
				} else {
					sprintf(msg_str_ptr, "CC %u, value %u",
						(unsigned int) event_ptr->data.control.param,
						(unsigned int) event_ptr->data.control.value);
				}
#endif
				break;

				/*
				 * ------------------------------------------------------------
				 * 	FLASHING NEON, this is where the main input from the pedal
				 * gets recevied and the processing starts.
				 * ------------------------------------------------------------
				 */
			case SND_SEQ_EVENT_PGMCHANGE:
				sprintf(msg_str_ptr, "Program change, %d\n",
					(unsigned int) event_ptr->data.control.value);

				/* Here is where Program changes happen from Program change inputs.
				 */
				PatchIndex = LayoutSwitchPatch(event_ptr->data.control.value,
				true);

				break;
			case SND_SEQ_EVENT_CHANPRESS:
				sprintf(msg_str_ptr, "Channel pressure");
				break;
			case SND_SEQ_EVENT_PITCHBEND:
				sprintf(msg_str_ptr, "Pitchwheel, %d",
					(signed int) event_ptr->data.control.value);
				break;
			case SND_SEQ_EVENT_CONTROL14:
				sprintf(msg_str_ptr, "14 bit controller value");
				break;
			case SND_SEQ_EVENT_NONREGPARAM:
				sprintf(msg_str_ptr, "NRPN");
				break;
			case SND_SEQ_EVENT_REGPARAM:
				sprintf(msg_str_ptr, "RPN");
				break;
			case SND_SEQ_EVENT_SONGPOS:
				sprintf(msg_str_ptr, "Song position");
				break;
			case SND_SEQ_EVENT_SONGSEL:
				sprintf(msg_str_ptr, "Song select");
				break;
			case SND_SEQ_EVENT_QFRAME:
				//            sprintf(msg_str_ptr, "midi time code quarter frame");
				sprintf(msg_str_ptr,
					"midi time code quarter frame Q%d V%d P%d D%d D%d",
					event_ptr->data.queue.queue,
					event_ptr->data.queue.param.value,
					event_ptr->data.queue.param.position,
					event_ptr->data.queue.param.d32[0],
					event_ptr->data.queue.param.d32[1]);
				break;
			case SND_SEQ_EVENT_TIMESIGN:
				sprintf(msg_str_ptr, "SMF Time Signature event");
				break;
			case SND_SEQ_EVENT_KEYSIGN:
				sprintf(msg_str_ptr, "SMF Key Signature event");
				break;
			case SND_SEQ_EVENT_START:
				sprintf(msg_str_ptr, "MIDI Real Time Start message");
				break;
			case SND_SEQ_EVENT_CONTINUE:
				sprintf(msg_str_ptr, "MIDI Real Time Continue message");
				break;
			case SND_SEQ_EVENT_STOP:
				sprintf(msg_str_ptr, "MIDI Real Time Stop message");
				break;
			case SND_SEQ_EVENT_SETPOS_TICK:
				sprintf(msg_str_ptr, "Set tick queue position");
				break;
			case SND_SEQ_EVENT_SETPOS_TIME:
				sprintf(msg_str_ptr, "Set real-time queue position");
				break;
			case SND_SEQ_EVENT_TEMPO:
				//        	BPM = 60,000,000/[value]
				//           sprintf(msg_str_ptr, "(SMF) Tempo event");
				sprintf(msg_str_ptr, "(SMF) Tempo event Q%d V%d P%d D%d D%d",
					event_ptr->data.queue.queue,
					event_ptr->data.queue.param.value,
					event_ptr->data.queue.param.position,
					event_ptr->data.queue.param.d32[0],
					event_ptr->data.queue.param.d32[1]);
				break;
			case SND_SEQ_EVENT_CLOCK:
				//        	60000 / (BPM * PPQ) (milliseconds).
				//       	Pulse Length = 60/(BPM * PPQN)
//            sprintf(msg_str_ptr, "MIDI Real Time Clock message");
				//       	OK, 24 beats per quarter.
//        	60/(Value * 24)
				sprintf(msg_str_ptr,
					"MIDI Real Time Clock message Q%d V%d P%d D%d D%d",
					event_ptr->data.queue.queue,
					event_ptr->data.queue.param.value,
					event_ptr->data.queue.param.position,
					event_ptr->data.queue.param.d32[0],
					event_ptr->data.queue.param.d32[1]);
				break;
			case SND_SEQ_EVENT_TICK:
				//        	snd_seq_event_t * event_ptr
				sprintf(msg_str_ptr,
					"MIDI Real Time Tick message Q%d V%d P%d D%d D%d",
					event_ptr->data.queue.queue,
					event_ptr->data.queue.param.value,
					event_ptr->data.queue.param.position,
					event_ptr->data.queue.param.d32[0],
					event_ptr->data.queue.param.d32[1]);
				break;
			case SND_SEQ_EVENT_QUEUE_SKEW:
				sprintf(msg_str_ptr, "Queue timer skew");
				break;
			case SND_SEQ_EVENT_SYNC_POS:
				sprintf(msg_str_ptr, "Sync position changed");
				break;
			case SND_SEQ_EVENT_TUNE_REQUEST:
				sprintf(msg_str_ptr, "Tune request");
				break;
			case SND_SEQ_EVENT_RESET:
				sprintf(msg_str_ptr, "Reset");
				break;
			case SND_SEQ_EVENT_SENSING:
				continue; /* disable */
				sprintf(msg_str_ptr, "Active sensing");
				break;
			case SND_SEQ_EVENT_ECHO:
				sprintf(msg_str_ptr, "Echo-back event");
				break;
			case SND_SEQ_EVENT_OSS:
				sprintf(msg_str_ptr, "OSS emulation raw event");
				break;
			case SND_SEQ_EVENT_CLIENT_START:
				sprintf(msg_str_ptr, "New client has connected");
				break;
			case SND_SEQ_EVENT_CLIENT_EXIT:
				sprintf(msg_str_ptr, "Client has left the system");
				break;
			case SND_SEQ_EVENT_CLIENT_CHANGE:
				sprintf(msg_str_ptr, "Client status/info has changed");
				break;
			case SND_SEQ_EVENT_PORT_START:
				sprintf(msg_str_ptr, "New port was created");
				break;
			case SND_SEQ_EVENT_PORT_EXIT:
				sprintf(msg_str_ptr, "Port was deleted from system");
				break;
			case SND_SEQ_EVENT_PORT_CHANGE:
				sprintf(msg_str_ptr, "Port status/info has changed");
				break;
			case SND_SEQ_EVENT_PORT_SUBSCRIBED:
				sprintf(msg_str_ptr, "Port connected");
				break;
			case SND_SEQ_EVENT_PORT_UNSUBSCRIBED:
				sprintf(msg_str_ptr, "Port disconnected");
				break;
#if OLD_ALSA
				case SND_SEQ_EVENT_SAMPLE:
				sprintf(msg_str_ptr, "Sample select");
				break;
				case SND_SEQ_EVENT_SAMPLE_CLUSTER:
				sprintf(msg_str_ptr, "Sample cluster select");
				break;
				case SND_SEQ_EVENT_SAMPLE_START:
				sprintf(msg_str_ptr, "voice start");
				break;
				case SND_SEQ_EVENT_SAMPLE_STOP:
				sprintf(msg_str_ptr, "voice stop");
				break;
				case SND_SEQ_EVENT_SAMPLE_FREQ:
				sprintf(msg_str_ptr, "playback frequency");
				break;
				case SND_SEQ_EVENT_SAMPLE_VOLUME:
				sprintf(msg_str_ptr, "volume and balance");
				break;
				case SND_SEQ_EVENT_SAMPLE_LOOP:
				sprintf(msg_str_ptr, "sample loop");
				break;
				case SND_SEQ_EVENT_SAMPLE_POSITION:
				sprintf(msg_str_ptr, "sample position");
				break;
				case SND_SEQ_EVENT_SAMPLE_PRIVATE1:
				sprintf(msg_str_ptr, "private (hardware dependent) event");
				break;
#endif
			case SND_SEQ_EVENT_USR0:
				sprintf(msg_str_ptr, "user-defined event");
				break;
			case SND_SEQ_EVENT_USR1:
				sprintf(msg_str_ptr, "user-defined event");
				break;
			case SND_SEQ_EVENT_USR2:
				sprintf(msg_str_ptr, "user-defined event");
				break;
			case SND_SEQ_EVENT_USR3:
				sprintf(msg_str_ptr, "user-defined event");
				break;
			case SND_SEQ_EVENT_USR4:
				sprintf(msg_str_ptr, "user-defined event");
				break;
			case SND_SEQ_EVENT_USR5:
				sprintf(msg_str_ptr, "user-defined event");
				break;
			case SND_SEQ_EVENT_USR6:
				sprintf(msg_str_ptr, "user-defined event");
				break;
			case SND_SEQ_EVENT_USR7:
				sprintf(msg_str_ptr, "user-defined event");
				break;
			case SND_SEQ_EVENT_USR8:
				sprintf(msg_str_ptr, "user-defined event");
				break;
			case SND_SEQ_EVENT_USR9:
				sprintf(msg_str_ptr, "user-defined event");
				break;
#if OLD_ALSA
				case SND_SEQ_EVENT_INSTR_BEGIN:
				sprintf(msg_str_ptr, "begin of instrument management");
				break;
				case SND_SEQ_EVENT_INSTR_END:
				sprintf(msg_str_ptr, "end of instrument management");
				break;
				case SND_SEQ_EVENT_INSTR_INFO:
				sprintf(msg_str_ptr, "query instrument interface info");
				break;
				case SND_SEQ_EVENT_INSTR_INFO_RESULT:
				sprintf(msg_str_ptr, "result of instrument interface info");
				break;
				case SND_SEQ_EVENT_INSTR_FINFO:
				sprintf(msg_str_ptr, "query instrument format info");
				break;
				case SND_SEQ_EVENT_INSTR_FINFO_RESULT:
				sprintf(msg_str_ptr, "result of instrument format info");
				break;
				case SND_SEQ_EVENT_INSTR_RESET:
				sprintf(msg_str_ptr, "reset instrument instrument memory");
				break;
				case SND_SEQ_EVENT_INSTR_STATUS:
				sprintf(msg_str_ptr, "get instrument interface status");
				break;
				case SND_SEQ_EVENT_INSTR_STATUS_RESULT:
				sprintf(msg_str_ptr, "result of instrument interface status");
				break;
				case SND_SEQ_EVENT_INSTR_PUT:
				sprintf(msg_str_ptr, "put an instrument to port");
				break;
				case SND_SEQ_EVENT_INSTR_GET:
				sprintf(msg_str_ptr, "get an instrument from port");
				break;
				case SND_SEQ_EVENT_INSTR_GET_RESULT:
				sprintf(msg_str_ptr, "result of instrument query");
				break;
				case SND_SEQ_EVENT_INSTR_FREE:
				sprintf(msg_str_ptr, "free instrument(s)");
				break;
				case SND_SEQ_EVENT_INSTR_LIST:
				sprintf(msg_str_ptr, "get instrument list");
				break;
				case SND_SEQ_EVENT_INSTR_LIST_RESULT:
				sprintf(msg_str_ptr, "result of instrument list");
				break;
				case SND_SEQ_EVENT_INSTR_CLUSTER:
				sprintf(msg_str_ptr, "set cluster parameters");
				break;
				case SND_SEQ_EVENT_INSTR_CLUSTER_GET:
				sprintf(msg_str_ptr, "get cluster parameters");
				break;
				case SND_SEQ_EVENT_INSTR_CLUSTER_RESULT:
				sprintf(msg_str_ptr, "result of cluster parameters");
				break;
				case SND_SEQ_EVENT_INSTR_CHANGE:
				sprintf(msg_str_ptr, "instrument change");
				break;
#endif
			case SND_SEQ_EVENT_SYSEX:
				#if 0
				decode_sysex(
					(guint8 *)event_ptr->data.ext.ptr,
					event_ptr->data.ext.len,
					msg_str_ptr);
#endif
				break;
			case SND_SEQ_EVENT_BOUNCE:
				sprintf(msg_str_ptr, "error event");
				break;
			case SND_SEQ_EVENT_USR_VAR0:
				sprintf(msg_str_ptr, "reserved for user apps");
				break;
			case SND_SEQ_EVENT_USR_VAR1:
				sprintf(msg_str_ptr, "reserved for user apps");
				break;
			case SND_SEQ_EVENT_USR_VAR2:
				sprintf(msg_str_ptr, "reserved for user apps");
				break;
			case SND_SEQ_EVENT_USR_VAR3:
				break;
			case SND_SEQ_EVENT_USR_VAR4:
				sprintf(msg_str_ptr, "reserved for user apps");
				break;
		}

		printd(LogInfo, "%s\n", msg_str_ptr);
		printf( "%s\n", msg_str_ptr);
//		g_print("%s", msg_str_ptr);
		/* get GTK thread lock */
#if 0 //ejk
		gdk_threads_enter();
		if (g_row_count >= MAX_LIST_SIZE) {
			gtk_tree_model_get_iter_first(
				GTK_TREE_MODEL(list_store_ptr),
				&iter);

			gtk_list_store_remove(
				list_store_ptr,
				&iter);
		}

		/* Append an empty row to the list store. Iter will point to the new row */
		gtk_list_store_append(list_store_ptr, &iter);

		gtk_list_store_set(
			list_store_ptr,
			&iter,
			COL_TIME, time_str_ptr->str,
			COL_CHANNEL, channel_str_ptr->str,
			COL_MESSAGE, msg_str_ptr->str,
			-1);

		gtk_tree_view_scroll_to_cell(
			GTK_TREE_VIEW(child_ptr),
			gtk_tree_model_get_path(
				gtk_tree_view_get_model(GTK_TREE_VIEW(child_ptr)),
				&iter),
			NULL,
			true,
			0.0,
			1.0);

		/* Force update of scroll position. */
		/* Is it a bug that it does not update automagically ? */
		gtk_container_check_resize(GTK_CONTAINER(child_ptr));

		g_row_count++;
		/* release GTK thread lock */
		gdk_threads_leave();
#endif

//	g_string_free(channel_str_ptr, true);
//	g_string_free(msg_str_ptr, true);
//	g_string_free(time_str_ptr, true);
	}

	return NULL;
}

/*--------------------------------------------------------------------
 * Function:		alsa_input_init
 *
 * Description:		Setup the Alsa input port.
 *
 *---------------------------------------------------------------------*/
gboolean alsa_input_init(const char * name) {
	int ret;
	snd_seq_port_info_t * port_info = NULL;

	/* Open the port
	 */
	ret = snd_seq_open(&SeqPort1In, "default",
	SND_SEQ_OPEN_INPUT, 0);

	if (ret < 0) {
		g_warning("Cannot open sequncer, %s\n", snd_strerror(ret));
		goto fail;
	}

	/* Name the port.
	 */
	snd_seq_set_client_name(SeqPort1In, name);

#ifdef HAVE_LASH_1_0
	lash_alsa_client_id(g_lashc, snd_seq_client_id(SeqPort1In));
#endif

	snd_seq_port_info_alloca(&port_info);

	snd_seq_port_info_set_capability(port_info,
	SND_SEQ_PORT_CAP_WRITE |
	SND_SEQ_PORT_CAP_SUBS_WRITE);
	snd_seq_port_info_set_type(port_info,
	SND_SEQ_PORT_TYPE_APPLICATION);
	snd_seq_port_info_set_midi_channels(port_info, 16);
	snd_seq_port_info_set_port_specified(port_info, 1);

	snd_seq_port_info_set_name(port_info, "input");
	snd_seq_port_info_set_port(port_info, 0);

	ret = snd_seq_create_port(SeqPort1In, port_info);
	if (ret < 0) {
		g_warning("Error creating ALSA sequencer port, %s\n",
			snd_strerror(ret));
		goto fail_close_seq;
	}

	/* Start midi thread */
	ret = pthread_create(&g_alsa_midi_tid, NULL, alsa_midi_thread, NULL);

	return true;

	fail_close_seq:
	ret = snd_seq_close(SeqPort1In);
	if (ret < 0) {
		g_warning("Cannot close sequncer, %s\n", snd_strerror(ret));
	}

	fail:
	return false;
}

// /usr/include/alsa/seq_event.h
/*--------------------------------------------------------------------
 * Function:		alsa_input_init
 *
 * Description:		close the Alsa input port.
 *
 *---------------------------------------------------------------------*/
void ProgramChange(unsigned int InputChange) {

	switch (InputChange) {

		/* RakArrack
		 */
		case 0:

			break;

		case 1:
			SendMidi(SND_SEQ_EVENT_PGMCHANGE, 0, 1, SND_SEQ_EVENT_PGMCHANGE,
				10);
			break;

		case 2:
			SendMidi(SND_SEQ_EVENT_PGMCHANGE, 0, 1, MIDI_CTL_LSB_BANK, 2);
			SendMidi(SND_SEQ_EVENT_PGMCHANGE, 0, 1, MIDI_CTL_MSB_BANK, 10);
			SendMidi(SND_SEQ_EVENT_PGMCHANGE, 0, 1, SND_SEQ_EVENT_PGMCHANGE,
				10);
			break;

		case 3:

			break;

		case 4:

			break;

		case 5:

			break;

		case 6:

			break;

		case 7:

			break;

		case 8:

			break;

		case 9:

			break;

	}
}

#if 0
void sendMessageNow (const MidiMessage& message) {
	snd_seq_event_t ev;

	snd_seq_ev_clear(&ev);
	snd_seq_ev_set_source(&ev, outputPort);
	snd_seq_ev_set_subs(&ev);
	snd_seq_ev_set_direct(&ev);

	if (message.isNoteOnOrOff()) {
		if (message.isNoteOn ())
		ev.type = SND_SEQ_EVENT_NOTEON;
		else if (message.isNoteOff ())
		ev.type = SND_SEQ_EVENT_NOTEOFF;

		ev.data.note.channel = message.getChannel ();
		ev.data.note.note = message.getNoteNumber ();
		ev.data.note.velocity = message.getVelocity ();

		printd(LogInfo, "Sending note on/off to port %d, ch=%d, note=%d, vel=%d, pid=%d\n",
			outputPort, ev.data.note.channel, ev.data.note.note, ev.data.note.velocity, getpid());
	} else if (message.isAllNotesOff()) {
		// @XXX how to handle this ?
	} else if (message.isAllSoundOff()) {
		// @XXX how to handle this ?
	} else if (message.isPitchWheel ()) {
		ev.type = SND_SEQ_EVENT_PITCHBEND;
		ev.data.control.channel = message.getChannel ();
		ev.data.control.value = message.getPitchWheelValue ();

		printd(LogInfo, "Sending pitchbend to port %d, ch=%d, value=%d\n",
			outputPort, ev.data.control.channel, ev.data.control.value);
	} else if (message.isChannelPressure()) {
		ev.type = SND_SEQ_EVENT_CHANPRESS;
		ev.data.control.channel = message.getChannel ();
		ev.data.control.param = 0;
		ev.data.control.value = message.getChannelPressureValue ();

		printd(LogInfo, "Sending channel pressure to port %d, ch=%d, value=%d\n",
			outputPort, ev.data.control.channel, ev.data.control.value);
	} else if (message.isAftertouch()) {
		ev.type = SND_SEQ_EVENT_KEYPRESS
		ev.data.note.channel = message.getChannel ();
		ev.data.note.note = message.getNoteNumber ();
		ev.data.note.velocity = message.getAfterTouchValue ();
		ev.data.note.off_velocity = 0;
		ev.data.note.duration = 0;

		printd(LogInfo, "Sending aftertouch to port %d, ch=%d, value=%d\n",
			outputPort, ev.data.note.channel, ev.data.note.value);
	} else if (message.isController ()) {
		ev.type = SND_SEQ_EVENT_CONTROLLER;
		ev.data.control.channel = message.getChannel ();
		ev.data.control.param = message.getControllerNumber ();
		ev.data.control.value = message.getControllerValue ();

		printd(LogInfo, "Sending cc to port %d, ch=%d, cc=%d, value=%d\n",
			outputPort, ev.data.control.channel, ev.data.control.param, ev.data.control.value);
	} else if (message.isProgramChange()) {
		ev.type = SND_SEQ_EVENT_PGMCHANGE;
		ev.data.control.channel = message.getChannel ();
		ev.data.control.value = message.getProgramChangeNumber ();

		printd(LogInfo, "Sending program change to port %d, ch=%d, value=%d\n",
			outputPort, ev.data.note.channel, ev.data.note.value);
	}

	snd_seq_event_output_direct(seqHandle, &ev);
#endif

#if 0
00024 // MMC Command Codes.
00025
#define MMC_CMD_STOP					0x01
00026
#define MMC_CMD_PLAY					0x02
00027
#define MMC_CMD_DEFERRED_PLAY		   0x03
00028
#define MMC_CMD_FAST_FORWARD			0x04
00029
#define MMC_CMD_REWIND				  0x05
00030
#define MMC_CMD_RECORD_STROBE		   0x06
00031
#define MMC_CMD_RECORD_EXIT			 0x07
00032
#define MMC_CMD_RECORD_PAUSE			0x08
00033
#define MMC_CMD_PAUSE				   0x09
00034
#define MMC_CMD_EJECT				   0x0a
00035
#define MMC_CMD_CHASE				   0x0b
00036
#define MMC_CMD_COMMAND_ERROR_RESET	 0x0c
00037
#define MMC_CMD_MMC_RESET			   0x0d
00038
#define MMC_CMD_JOG_START			   0x20
00039
#define MMC_CMD_JOG_STOP				0x21
00040
#define MMC_CMD_WRITE				   0x40
00041
#define MMC_CMD_MASKED_WRITE			0x41
00042
#define MMC_CMD_READ					0x42
00043
#define MMC_CMD_UPDATE				  0x43
00044
#define MMC_CMD_LOCATE				  0x44
00045
#define MMC_CMD_VARIABLE_PLAY		   0x45
00046
#define MMC_CMD_SEARCH				  0x46
00047
#define MMC_CMD_SHUTTLE				 0x47
00048
#define MMC_CMD_STEP					0x48
00049
#define MMC_CMD_ASSIGN_SYSTEM_MASTER	0x49
00050
#define MMC_CMD_GENERATOR_COMMAND	   0x4a
00051
#define MMC_CMD_MTC_COMMAND			 0x4b
00052
#define MMC_CMD_MOVE					0x4c
00053
#define MMC_CMD_ADD					 0x4d
00054
#define MMC_CMD_SUBTRACT				0x4e
00055
#define MMC_CMD_DROP_FRAME_ADJUST	   0x4f
00056
#define MMC_CMD_PROCEDURE			   0x50
00057
#define MMC_CMD_EVENT				   0x51
00058
#define MMC_CMD_GROUP				   0x52
00059
#define MMC_CMD_COMMAND_SEGMENT		 0x53
00060
#define MMC_CMD_DEFERRED_VARIABLE_PLAY  0x54
00061
#define MMC_CMD_RECORD_STROBE_VARIABLE  0x55
00062
#define MMC_CMD_WAIT					0x7c
00063
#define MMC_CMD_RESUME				  0x7f
00064
00065 // Available MMC Masked Write sub-commands (information fields).
00066
#define MMC_CIF_TRACK_RECORD			0x4f
00067
#define MMC_CIF_TRACK_MUTE			  0x62
00068
#define MMC_CIF_TRACK_SOLO			  0x66 // Custom-implementation ;)

{
	unsigned char SysexSize;
	unsigned char SysexLen;
	unsigned char *SysexData;

	SysexSize = 6;
	if (MmcLen > 0)
	SysexSize += 1 + MmcLen;
	SysexData = (unsigned char *) alloca(SysexSize);
	SysexLen = 0;

	SysexData[SysexLen++] = 0xf0;	  // Sysex header.
	SysexData[SysexLen++] = 0x7f;// Realtime sysex.
	SysexData[SysexLen++] = 0x7f;// All-caller-id.
	SysexData[SysexLen++] = 0x06;// MMC command mode.
	SysexData[SysexLen++] = MmcCmd;// MMC command code.
	if (MmcData && MmcLen > 0) {
		SysexData[SysexLen++] = MmcLen;
		memcpy(&SysexData[SysexLen], MmcData, MmcLen);
		SysexLen += MmcLen;
	}
	SysexData[SysexLen++] = 0xf7;	  // Sysex trailer.

	snd_seq_ev_set_sysex(&Ev, SysexLen, SysexData);

	SubMitEvent();
}

#endif

// static snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
static snd_pcm_stream_t stream = SND_PCM_STREAM_CAPTURE;

/*--------------------------------------------------------------------
 * Function:		device_list
 *
 * Description:		Look for all devices.
 *
 *---------------------------------------------------------------------*/
static void device_list(void) {
	snd_ctl_t *handle;
	int card, err, dev, idx;
	snd_ctl_card_info_t *info;
	snd_pcm_info_t *pcminfo;
	snd_ctl_card_info_alloca(&info);
	snd_pcm_info_alloca(&pcminfo);

	card = -1;
	if (snd_card_next(&card) < 0 || card < 0) {
		error(("no soundcards found..."));
		return;
	}
	printd(LogInfo, ("**** List of %s Hardware Devices ****\n"),
		snd_pcm_stream_name(stream));
	while (card >= 0) {
		char name[32];
		sprintf(name, "hw:%d", card);
		if ((err = snd_ctl_open(&handle, name, 0)) < 0) {
			error("control open (%i): %s", card, snd_strerror(err));
			goto next_card;
		}
		if ((err = snd_ctl_card_info(handle, info)) < 0) {
			error("control hardware info (%i): %s", card, snd_strerror(err));
			snd_ctl_close(handle);
			goto next_card;
		}
		dev = -1;
		while (1) {
			unsigned int count;
			if (snd_ctl_pcm_next_device(handle, &dev) < 0)
				error("snd_ctl_pcm_next_device");
			if (dev < 0)
				break;
			snd_pcm_info_set_device(pcminfo, dev);
			snd_pcm_info_set_subdevice(pcminfo, 0);
			snd_pcm_info_set_stream(pcminfo, stream);
			if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0) {
				if (err != -ENOENT)
					error("control digital audio info (%i): %s", card,
						snd_strerror(err));
				continue;
			}
			printd(LogInfo, ("card %i: %s [%s], device %i: %s [%s]\n"), card,
				snd_ctl_card_info_get_id(info),
				snd_ctl_card_info_get_name(info), dev,
				snd_pcm_info_get_id(pcminfo),
				snd_pcm_info_get_name(pcminfo));
			count = snd_pcm_info_get_subdevices_count(pcminfo);
			printd(LogInfo, ("  Subdevices: %i/%i\n"),
				snd_pcm_info_get_subdevices_avail(pcminfo), count);
			for (idx = 0; idx < (int) count; idx++) {
				snd_pcm_info_set_subdevice(pcminfo, idx);
				if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0) {
					error("control digital audio playback info (%i): %s", card,
						snd_strerror(err));
				} else {
					printd(LogInfo, ("  Subdevice #%i: %s\n"), idx,
						snd_pcm_info_get_subdevice_name(pcminfo));
				}
			}
		}
		snd_ctl_close(handle);
		next_card:
		if (snd_card_next(&card) < 0) {
			error("snd_card_next");
			break;
		}
	}
}

/*--------------------------------------------------------------------
 * Function:		pcm_list
 *
 * Description:		Look for Capture Cards.
 *
 *---------------------------------------------------------------------*/
static void pcm_list(void) {
	void **hints, **n;
	char *name, *descr, *descr1, *io;
	const char *filter;

	if (snd_device_name_hint(-1, "pcm", &hints) < 0)
		return;
	n = hints;
//	filter = stream == SND_PCM_STREAM_CAPTURE ? "Input" : "Output";
	filter = "Output";
	while (*n != NULL) {
		name = snd_device_name_get_hint(*n, "NAME");
		descr = snd_device_name_get_hint(*n, "DESC");
		io = snd_device_name_get_hint(*n, "IOID");
		if (io != NULL && strcmp(io, filter) != 0)
			goto __end;
		printd(LogInfo, "%s\n", name);
		if ((descr1 = descr) != NULL) {
			printd(LogInfo, "	");
			while (*descr1) {
				if (*descr1 == '\n')
					printd(LogInfo, "\n	");
				else
					putchar(*descr1);
				descr1++;
			}
			putchar('\n');
		}
		__end:
		if (name != NULL)
			free(name);
		if (descr != NULL)
			free(descr);
		if (io != NULL)
			free(io);
		n++;
	}
	snd_device_name_free_hint(hints);
}

/*--------------------------------------------------------------------
 * Function:		SetAlsaMasterVolume
 *
 * Description:		Control the Alsa Mixer
 *
 *---------------------------------------------------------------------*/
void SetAlsaMasterVolume(long volume) {
    long min, max;
    snd_mixer_selem_id_t *sid;

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid,  "Master");
  snd_mixer_elem_t* elem = snd_mixer_find_selem(MixerHandle, sid);
printd(LogInfo, "SetAlsaMasterVolume MixerHandle %x elem %x\n", MixerHandle, elem);

	/* This is the Mute button.		*/
	snd_mixer_selem_set_playback_switch_all	(elem, 1 );
  snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &MixerVolume);
  snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);
}


/*--------------------------------------------------------------------
 * Function:		SetAlsaCaptureVolume
 *
 * Description:		Control the Alsa Mixer
 *
 *---------------------------------------------------------------------*/
void SetAlsaCaptureVolume(long volume) {
    long min, max;
    snd_mixer_selem_id_t *sid;

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid,  "Capture");
  snd_mixer_elem_t* elem = snd_mixer_find_selem(MixerHandle, sid);
  printd(LogInfo, "SetAlsaCaptureVolume MixerHandle %x elem %x %ld\n", MixerHandle, elem, volume);

  snd_mixer_selem_get_capture_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &MixerVolume);
//	  snd_mixer_selem_channel_id_t 	channel,long * 	value  )

  snd_mixer_selem_set_capture_switch_all	(elem, 1 );

  snd_mixer_selem_get_capture_volume_range(elem, &min, &max);
    snd_mixer_selem_set_capture_volume_all(elem, volume * max / 100);
}

#if 0
Rosegarden:
jumpTo->processEventsOut->processMidiOut->insertMTCQFrames->snd_seq_ev_schedule_real
processNotesOff->snd_seq_ev_schedule_real
insertMTCFullFrame->snd_seq_ev_schedule_real

RealTime scheduleTime = t + m_alsaPlayStartTime - m_playStartPosition;
 snd_seq_real_time_t atime =
     { (unsigned int)scheduleTime.sec,
       (unsigned int)scheduleTime.nsec };

 event.type = SND_SEQ_EVENT_QFRAME;
 event.data.control.value = c;

 snd_seq_ev_schedule_real(&event, m_queue, 0, &atime);


#endif

#if 0
Midi Clock = 24 per quarter.


#endif
