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
#define TimerTicksPerQuater 	4


/*
 * Place Local prototypes here
 */

#ifndef AlsaTimer
static gboolean time_handler(GtkWidget *widget);
#endif
static gboolean tempo_handler(GtkWidget *widget);

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
	SetupAlsaTimer(100);
#else
//	g_timeout_add(Timer1Ticks, (GSourceFunc) time_handler, (gpointer) gxml);
#endif
	gMyInfo.TempoTimerID = 0;
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

	Tempofont_desc = pango_font_description_from_string("Sans Bold 18");

//	TempoChild = gtk_bin_get_child((GTK_BIN(TempoDraw)));
//	gtk_widget_override_font((TempoChild), Tempofont_desc);

#ifdef AlsaTimer
	/* Send out a message our tempo is changing.
	 */
	SendMidi(SND_SEQ_EVENT_TEMPO, TempoPort, DefaultMidiChannel, 0,
	         (int) NewTempo);
//      NewDivider = 100000;
//      gMyInfo.TempoReload = 25000/NewTempo;

	gMyInfo.TempoReload = 24900 / NewTempo;
	gMyInfo.TimerCount = 0;
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
	gMyInfo.TempoReload = (30000 / (NewTempo * TimerTicksPerQuater));
	printd(LogInfo, "New Tempo %d Val  %d\n", NewTempo, gMyInfo.TempoReload);
	printf( " ****   New Tempo %d Val  %d\n", NewTempo, gMyInfo.TempoReload);

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

static char		TempoCount = 0;
#define PedalLEDAllOn		0
#define PedalLEDAllOff		1
#define PedalLED1On		2
#define PedalLED1Off	3
#define PedalLED2On		4
#define PedalLED2Off	5
#define PedalLED3On		6
#define PedalLED3Off	7
#define PedalLED4On		8
#define PedalLED4Off	9
#define PedalLED5On		10
#define PedalLED6Off	11
#define PedalLED7On		12
#define PedalLED7Off	13
#define PedalLED7On		14
#define PedalLED7Off	15
#define PedalLED8On		17
#define PedalLED8Off	117

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

	/*
	 * Needs to be sent 24 time per quarter.
	 */
// EJK Remove gMyInfo.TempoMax
	if (++TempoState >= (gMyInfo.TempoMax * (TimerTicksPerQuater / 2))) {
		TempoState = 0;
	}


	/* This is the tempo in BPM		*/
	if (!(TempoState % TimerTicksPerQuater)) {
		gUpdateTempo = 1;

		if (++TempoCount > gMyInfo.BeatsPerMeasure)
			TempoCount = 1;

		/* Make sure the buttons are all up after being pressed.
		*/
		ClearMainButtons();

		/* Hitting record a second time stops the recording.
		*/
		if (CountInActiveState == cntStateRecording && LoopRecBeats > 0) {

			if (--LoopRecBeats == 0) {
				CountInActiveState = cntStateWaitingIdle;
				SendMidi(SND_SEQ_EVENT_STOP, TransportPort, 1,
				         0, 0);
				OSCCommand(OSCStartRecord, 0);
				OSCCommand(OSCSyncSource, 1);

//			DoPatch(&gMyInfo.MyPatchInfo[FindString(fsPatchNames, "LP Rec")]);
				printf("Loop Off\n\n");
//				OSCCommand(OSCSyncOn, 0);

			}
//			else {
//				OSCCommand(OSCSyncOff, 0);
//			}
		}

#if 0
		/* CountInActiveState is set from the patch cmdCountIn	*/
		if (CountInActiveState == 1) {
//			DoPatch(&gMyInfo.MyPatchInfo[FindString(fsPatchNames, "LP Rec")]);
			SendMidi(SND_SEQ_EVENT_START, TransportPort, 1,
			         0, 0);
			OSCCommand(OSCStartRecord, 0);
			printf("Loop CountInActiveState 1\n\n");
			CountInActiveState = 0;
			gMyInfo.MetronomeOn = FALSE;
		}
#endif
		/* If We just started turn on the metronome.
		*/
		if (CountInActiveState == cntStateWaitingforCount) {
			gMyInfo.MetronomeOn = TRUE;

			/* If count in is zero turn off metronome and start recording.
			*/
			if ( CountInCount-- == 0) {
				CountInActiveState = cntStateRecording;

//							DoPatch(&gMyInfo.MyPatchInfo[FindString(fsPatchNames, "LP Rec")]);
				// Send a start to the transport port.
				SendMidi(SND_SEQ_EVENT_START, TransportPort, 1,
				         0, 0);
				// Send a start record over OSC


				OSCCommand(OSCSyncSource, -3);
				OSCCommand(OSCStartRecord, 0);
				gMyInfo.MetronomeOn = FALSE;
				printf("Loop Start 1\n\n");

			}
		}

#if 1
		if (TempoCount != 1) {
			SendMidi(SND_SEQ_EVENT_NOTEON, PedalPort,
			         DrumMidiChannel, 00, (int) gMyInfo.DrumRest);
			SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
			         DrumMidiChannel, 04, (int) PedalLED4On );
		} else {
			SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
			         DrumMidiChannel, 04, (int) PedalLED3On);
			SendMidi(SND_SEQ_EVENT_NOTEON, PedalPort,
			         DrumMidiChannel, 00, (int) gMyInfo.Drum1);
		}
#endif
		// Send the tap tempo to sooperlooper.
		MyOSCTap(TempoState);
		// Send a PC of 99, hydrogen is configured for tap.
		// should change to OSC for compatibility.
		SendMidi(SND_SEQ_EVENT_CONTROLLER, TempoPort,
		         1, 99, 0);

		/* On the first beat play a different sound.
		 */
		if (gMyInfo.MetronomeOn) {
			if (TempoCount != 1) {
				SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort,
				         DrumMidiChannel, 00, (int) gMyInfo.DrumRest);
			} else {
				SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort,
				         DrumMidiChannel, 00, (int) gMyInfo.Drum1);
			}

			sprintf(TempStrBuf, "On %d - %d", gMyInfo.Tempo, TempoCount);
		} else
			sprintf(TempStrBuf, "Off %d - %d", gMyInfo.Tempo, TempoCount);

		MyImageButtonSetText(&TempoDraw, TempStrBuf);
		MyOSCPoll(FALSE);
	}
}


/*--------------------------------------------------------------------
 * Function:		Tempo callback.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static gboolean tempo_handler(GtkWidget *widget) {

//	printd(LogInfo," IN tempo_handler\n");
	/* Turn this off for now MTC	*/
#if 0
// #ifndef AlsaTimer
	snd_seq_event_t ev;
	int err;
	unsigned long adjbpm;
	snd_seq_queue_tempo_t *queue_tempo;
	snd_seq_ev_clear(&ev);
	snd_seq_ev_set_source(&ev, TempoPort);
	snd_seq_ev_set_subs(&ev);

	/* Channel, Controller, Value
	 */
	snd_seq_ev_set_controller(&ev, 0, 0, 0);

	/* Send with out queueing.
	 */
	snd_seq_ev_set_direct(&ev);

	ev.type = SND_SEQ_EVENT_CLOCK;
	err = snd_seq_event_output_direct(gMyInfo.SeqPort[TempoPort], &ev);
	snd_seq_drain_output(gMyInfo.SeqPort[TempoPort]);
#endif

	if (++SubBeats > 1) {
		SubBeats = 0;

		PlayerPoll(TRUE);

		/*  Tempo Midi
		 */
		ToggleTempo();
	} else {
		SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
		         DrumMidiChannel, 04, (int) PedalLED3Off );

		SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
		         DrumMidiChannel, 04, (int) PedalLED4Off );
	}


	return TRUE;
}
