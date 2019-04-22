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
#include "GTKMidiUI.h"
#include "SooperOSC.h"


/*
 * Place defines and Typedefs here
 */
//#define AlsaTimer
#define TimerTicksPerQuater 	1


/*
 * Place Local prototypes here
 */

#ifndef AlsaTimer
static gboolean time_handler(GtkWidget *widget);
#endif
static gboolean tempo_handler(GtkWidget *widget);
int GTKIdel_cb(gpointer data);

/*
 * Place Static variables here
 */
/* Used to Toggle the Tempo GUI display.
 */
int TempoState;
int SubBeats;

/*--------------------------------------------------------------------
 * Function:		MyTimerInit
 * Description:		Setup the timers used to Tempo and MTC
 *
 *---------------------------------------------------------------------*/
void MyTimerInit(void) {

	/* Set up a timer for Tempo.
	*/
#ifdef AlsaTimer
	SetupAlsaTimer(90);
#else
	gMyInfo.TempoTimerID = 0;
#endif
	SetTempo(120);

	CountInActiveState = cntStateWaitingIdle;
	LoopRecBeats = 0;
	SubBeats = 0;

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
	
	com_tempo(NewTempo);

	Tempofont_desc = pango_font_description_from_string("Sans Bold 18");
	gMyInfo.Tempo = NewTempo;
//	gMyInfo.TempoReload = 40;
//	TempoChild = gtk_bin_get_child((GTK_BIN(TempoDraw)));
//	gtk_widget_override_font((TempoChild), Tempofont_desc);

#ifdef AlsaTimer
	/* Send out a message our tempo is changing.
	 */
	setTimerFreq(15000 / NewTempo);

#else
	/* Tell the timer to stop.
	 */
	if (gMyInfo.TempoTimerID)
		g_source_remove(gMyInfo.TempoTimerID);

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
	gMyInfo.TempoTimerID = g_timeout_add(gMyInfo.TempoReload,
	                                     (GSourceFunc) tempo_handler, (gpointer) gxml);

//      gMyInfo.Timer1Count = 0;
#endif
}


#ifndef AlsaTimer
/*--------------------------------------------------------------------
 * Function:		Timer Callback
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static gboolean time_handler(GtkWidget *widget) {

	printd(LogInfo, " IN time_handler\n");
	return TRUE;
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

//	gettimeofday(&Time0, NULL);
//	printd(LogDebug, ("%ld:%ld->",Time0.tv_sec, Time0.tv_usec);
//	printd(LogDebug, ("Tempo %d %d\n", TempoState,  TempoState);

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
	g_idle_add(GTKIdel_cb, theMainWindow);

//	jack_poll();
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
//	PlayerPoll(TRUE);
	return TRUE;
}
