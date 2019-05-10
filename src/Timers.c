/*---------------------------------------------------------------------
|
|	File: 	Timers
|
|	Contains:
|
|
|	Written By: 	Elias Keshishoglou on Sun Mar 8 11:03:38 PDT 2015
|
|	Copyright �: 	2015 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#define Timers_c

/*
 * Place Includes Here
 */

#include "Timers.h"
#include <gtk/gtk.h>
#include "LiveMusicApp.h"
#include "SooperOSC.h"
#include "AlsaUtils.h"
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>

/*
 * Place defines and Typedefs here
 */
#ifdef RTTimer 
#define TimerTicksPerQuater 	1
#else
#define TimerTicksPerQuater 	1
#endif

/*
 * Place Local prototypes here
 */
static gboolean time_handler(GtkWidget *widget);
static gboolean tempo_handler(GtkWidget *widget);
static void time_handlerRT (union sigval val);

void com_stop(void);
void com_play(void);
void com_tempo(int NewTempo);

int GTKIdel_cb(gpointer data);

/*
 * Place Static variables here
 */
/* Used to Toggle the Tempo GUI display.
 */
int TempoState;
int SubBeats;

#ifdef GTKTimer
/*--------------------------------------------------------------------
 * Function:		MyTimerInit
 * Description:		Setup the timers used to Tempo and MTC
 *
 *---------------------------------------------------------------------*/
void MyTimerInit(void) {

	CountInActiveState = cntStateWaitingIdle;
	LoopRecBeats = 0;
	SubBeats = 0;

	/* Set up a timer for Tempo.
	*/
	gMyInfo.TempoTimerID = 0;

	SetTempo(120);
}

/*--------------------------------------------------------------------
 * Function:		Set the tempo
 * Description:		Set the tempo to a new value. Re-setup the timer
 * 	interrupts to handle double the tempo.
 *
 *---------------------------------------------------------------------*/
void SetTempo(unsigned int NewTempo) {

	if (NewTempo <= 0)
		return;

	/* Set the jack transport for timers.
	*/
	com_tempo(NewTempo);

	Tempofont_desc = pango_font_description_from_string("Sans Bold 18");
	gMyInfo.Tempo = NewTempo;
//	gMyInfo.TempoReload = 40;
//	TempoChild = gtk_bin_get_child((GTK_BIN(TempoDraw)));
//	gtk_widget_override_font((TempoChild), Tempofont_desc);

	/* Tell the timer to stop.
	 */
	if (gMyInfo.TempoTimerID) {
		g_source_remove(gMyInfo.TempoTimerID);
		printf("********TIMER REMOVED*****\n");
	}

	/* Store the tempo information.
	 */
	gMyInfo.Tempo = NewTempo;

	/*
	 * This gives us TimerTicksPerQuater ticks per quarter.
	 */
	gMyInfo.TempoReload = (60000 / (NewTempo * TimerTicksPerQuater));
	printd(LogInfo, "New Tempo %d Val  %d\n", NewTempo, gMyInfo.TempoReload);

	/* Start the new timer.
	 */
	gMyInfo.TempoTimerID = g_timeout_add(gMyInfo.TempoReload,(GSourceFunc) tempo_handler, (gpointer) gxml);

//      gMyInfo.Timer1Count = 0;
}

/*--------------------------------------------------------------------
 * Function:		Timer Callback
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static gboolean time_handler(GtkWidget *widget) {

	printd(LogInfo, " IN time_handler\n");
	ToggleTempo();
	g_idle_add(GTKIdel_cb, theMainWindow);

	return TRUE;
}

#endif


#ifdef AlsaTimer
/*--------------------------------------------------------------------
 * Function:		MyTimerInit
 * Description:		Setup the timers used to Tempo and MTC
 *
 *---------------------------------------------------------------------*/
void MyTimerInit(void) {

	CountInActiveState = cntStateWaitingIdle;
	LoopRecBeats = 0;
	SubBeats = 0;

	/* Set up a timer for Tempo.
	*/
	SetupAlsaTimer(90);
	SetTempo(120);
}

/*--------------------------------------------------------------------
 * Function:		Set the tempo
 * Description:		Set the tempo to a new value. Re-setup the timer
 * 	interrupts to handle double the tempo.
 *
 *---------------------------------------------------------------------*/
void SetTempo(unsigned int NewTempo) {

	if (NewTempo <= 30)
		return;

	/* Set the jack transport for timers.
	*/
	com_tempo(NewTempo);

	Tempofont_desc = pango_font_description_from_string("Sans Bold 18");
	gMyInfo.Tempo = NewTempo;
//	gMyInfo.TempoReload = 40;
//	TempoChild = gtk_bin_get_child((GTK_BIN(TempoDraw)));
//	gtk_widget_override_font((TempoChild), Tempofont_desc);

	/* Send out a message our tempo is changing.
	 */
	setTimerFreq(15000 / NewTempo);
//printf("Set Alsa Timer\n");

}

/*--------------------------------------------------------------------
 * Function:		Tempo callback.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static gboolean tempo_handler(GtkWidget *widget) {

	if (!JackRunning)
		ToggleTempo();

	g_idle_add(GTKIdel_cb, theMainWindow);

//printf("Call Toggle from tempo\n");
//	PlayerPoll(TRUE);
	return TRUE;
}

#endif

#ifdef RTTimer
/*--------------------------------------------------------------------
 * Function:		MyTimerInit
 * Description:		Setup the timers used to Tempo and MTC
 *
 *---------------------------------------------------------------------*/
void MyTimerInit(void) {

	CountInActiveState = cntStateWaitingIdle;
	LoopRecBeats = 0;
	SubBeats = 0;

	/* Set up a timer for Tempo.
	*/
	gMyInfo.TempoTimerID = 0;

	SetTempo(120);
}

/*--------------------------------------------------------------------
 * Function:		Set the tempo
 * Description:		Set the tempo to a new value. Re-setup the timer
 * 	interrupts to handle double the tempo.
 *
 *---------------------------------------------------------------------*/
void SetTempo(unsigned int NewTempo) {
	int Ret;
	pthread_attr_t attr;
	struct sched_param parm;
	struct sigevent sig;
	struct itimerspec in, out;
	timer_t timerid;
	long T_High;
	long L_High;

	if (NewTempo <= 30)
		return;

	/* Set the jack transport for timers.
	*/
	com_tempo(NewTempo);
	gMyInfo.Tempo = NewTempo;

	printf("***** RT Timer init ****\n");

	if (gMyInfo.TempoTimerID) {
		printf("***** RT Timer timer_delete %d ****\n", gMyInfo.TempoTimerID);
//        memset((void*)&in, 0, sizeof(in));
//		timer_settime(gMyInfo.TempoTimerID, 0, &in, NULL);
		timer_delete(gMyInfo.TempoTimerID);
	}

	pthread_attr_init( &attr );
	parm.sched_priority = 255;
	pthread_attr_setschedparam(&attr, &parm);

	sig.sigev_notify = SIGEV_THREAD;
	sig.sigev_notify_function = time_handlerRT;
	// This get's passed to the handler.
	sig.sigev_value.sival_int = 20;
	sig.sigev_notify_attributes = &attr;

	Ret = timer_create(CLOCK_REALTIME, &sig, &timerid);
	printf("***** RT Timer Create **** %d %d\n", Ret, timerid);
	if (Ret == 0) {
		// Can't be zero.
		in.it_value.tv_sec = 1;
		in.it_value.tv_nsec = 0;
		in.it_interval.tv_sec = 0;

		// Double the timer interval.
		in.it_interval.tv_nsec = 30000000000 / NewTempo;

		//issue the periodic timer request here.
		Ret = timer_settime(timerid, 0, &in, &out);
		printf("***** RT Timer SetTime **** %d %ld\n", Ret, in.it_interval.tv_nsec);
		if (Ret != 0) {
			printf("timer_settime() failed with %d\n", errno);
			//delete the timer.
			timer_delete(timerid);
			timerid = 0;
		}
	} else
		printf("timer_create() failed with %d\n", errno);

	gMyInfo.TempoTimerID = timerid;
}

/*--------------------------------------------------------------------
 * Function:		Timer Callback
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static void time_handlerRT (union sigval val) {

//	printd(LogInfo, " IN time_handler\n");

	if (++SubBeats > 1) {
		ToggleTempo();
		SubBeats = 0;
	}

	g_idle_add(GTKIdel_cb, theMainWindow);

	return;
}
#endif


/*--------------------------------------------------------------------
 * Function:		ToggleTempo
 *
 * Description:		Toggle based on the common tempo.
 * 	Tempostate is the current index * 24
 *
 *---------------------------------------------------------------------*/
void ToggleTempo(void) {
//	char Count;
//	int	Loop;
	struct timeval Time0;

	// gettimeofday(&Time0, NULL);
	// printf("%ld:%ld->\n",Time0.tv_sec, Time0.tv_usec);
//	printd(LogDebug, "Tempo %d %d\n", TempoState,  TempoState);

	/* This is the tempo in BPM
		Currently we use 4 clocks per quarter.
	*/
	if (!(++TempoState % TimerTicksPerQuater)) {
		gUpdateTempo = 1;

		/* Check for a beat 1.
		*/
		if (++BeatCount > gMyInfo.BeatsPerMeasure) {
			BeatCount = 1;
			TempoState = 0;
			SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
			         DrumMidiChannel, 04, (int) PedalLED3On);
			SendMidi(SND_SEQ_EVENT_NOTEON, PedalPort,
			         DrumMidiChannel, 00, (int) gMyInfo.Drum1);
		} else {
			SendMidi(SND_SEQ_EVENT_NOTEON, PedalPort,
			         DrumMidiChannel, 00, (int) gMyInfo.DrumRest);
			SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
			         DrumMidiChannel, 04, (int) PedalLED4On );
		}

		/* Make sure the buttons are all up after being pressed.
		*/
		ClearMainButtons();
		/* Handle any recording for the looper.
		*/
		switch (CountInActiveState) {
		case cntStateWaitingforCountIn:
			printd(LogDebug, "cntStateWaitingforRecCount %d\n", BeatCount);
			/* Wait for the downbeat.
			*/
			if (BeatCount == 1)
				CountInActiveState = cntStateWaitingforRecCount;

			break;

		case cntStateWaitingforRecCount:
			printd(LogDebug, "cntStateWaitingforRecCount %d %d\n", CountInCount, gMyInfo.CountInBeats );
			if (CountInCount-- == gMyInfo.CountInBeats) {
				SendMidi(SND_SEQ_EVENT_START, TransportPort, 1,
				         0, 0);
				printd(LogDebug, "Start %d %d\n", CountInCount,  TempoState);
				/* Set sync source to Internal
				*/
				OSCCommand(OSCSyncSource, typeSyncjack);

				// In case there is no downbeat.
				OSCCommand(OSCRecThres, 0);

			}

			if ( CountInCount == 0) {
				CountInActiveState = cntStateRecording;

//							DoPatch(&gMyInfo.MyPatchInfo[FindString(fsPatchNames, "LP Rec")]);
				// Send a start record over OSC
				com_play();
				//		OSCCommand(OSCStartRecord, 0);
//				gMyInfo.MetronomeOn = FALSE;
				printd(LogDebug, "Loop Start 1\n\n");
			}

			break;

		/* If we are recording and still not done.
		*/
		case cntStateRecording:
			printd(LogDebug, "cntStateRecording %d\n", LoopRecBeats);
			if (LoopRecBeats == gMyInfo.LoopRecBeats)
				OSCCommand(OSCStartRecord, 0);

			if (--LoopRecBeats < 0) {
				OSCCommand(OSCStopRecord, 0);
				com_stop();
				CountInActiveState = cntStatePostRecord;

				/* Set Sync to Loop for the remaining tracks.
				*/
				SendMidi(SND_SEQ_EVENT_STOP, TransportPort, 1,
				         0, 0);

				printd(LogDebug, "Loop Off\n\n");
			}


			break;

		case cntStatePostRecord:
			CountInActiveState = cntStateWaitingIdle;
			OSCCommand(OSCSyncOn, 0);
			OSCCommand(OSCSyncSource, 1);

			break;

		/* If we are not recording, but the CountIn has
		been set let's start Drum machine.
		*/
		case cntStateWaitingIdle:
			/* You need to set the the midi pref in hydrogen to
			CC 99 Beatcounter not to tap.
			*/
			// should change to OSC for compatibility.
			SendMidi(SND_SEQ_EVENT_CONTROLLER, TempoPort,
			         1, 99, 0);

			// Send the tap tempo to sooperlooper.
			MyOSCTap(TempoState);

			break;
		}

		/* On the first beat play a different sound.
		 */
		if (gMyInfo.MetronomeOn) {
			if (BeatCount != 1) {
				SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort,
				         DrumMidiChannel, 00, (int) gMyInfo.DrumRest);
			} else {
				SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort,
				         DrumMidiChannel, 00, (int) gMyInfo.Drum1);
			}

			sprintf(TempoUpdateString, "On %d - %d", gMyInfo.Tempo, BeatCount);
		} else
			sprintf(TempoUpdateString, "Off %d - %d", gMyInfo.Tempo, BeatCount);

		UIUpdateFromTimer = TRUE;
	}
#if 0
	else {
//		if (TempoState == 2) {
		/*  Turn lights off
		 */
		SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
		         DrumMidiChannel, 04, (int) PedalLED3Off );

		SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
		         DrumMidiChannel, 04, (int) PedalLED4Off );
//		}
	}
#endif

//	jack_poll();
}

