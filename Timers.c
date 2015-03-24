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



/*
 * Place defines and Typedefs here
 */
//#define AlsaTimer
//#define TimerTicksPerQuater 	24
#define TimerTicksPerQuater 	2


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
extern theImageButtons TempoDraw;
extern PangoFontDescription *Tempofont_desc;
extern char TempStrBuf[10];



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

	gMyInfo.TempoReload = 24900/NewTempo;
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
	gMyInfo.TempoReload = (60000 / (NewTempo * TimerTicksPerQuater));
	printd(LogInfo, "New Tempo %d Val  %d\n", NewTempo, gMyInfo.TempoReload);

	/* Start the new timer 1000 is one second.
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

	printd(LogInfo," IN time_handler\n");
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
	char Count;
	int	Loop;

	/*
	 * Needs to be sent 24 time per quarter.
	 */
	if (++TempoState >= (gMyInfo.TempoMax * (TimerTicksPerQuater/2)))
		TempoState = 0;

	ClearMainButtons();

	if (!(TempoState % TimerTicksPerQuater)) {
		gUpdateTempo = 1;
		Count = (TempoState / TimerTicksPerQuater) + 1;

		if (CountInActive == 1 && Count == 1) {
			printf("*** Found Second Down beat, Sending Loop Record\n");
			CountInActive = 0;
			gMyInfo.MetronomeOn = FALSE;
			DoPatch(&gMyInfo.MyPatchInfo[FindString(fsPatchNames, "LP Rec")]);
		}

		if (CountInActive == 2 && Count == 1) {
printf("*** Found First Down beat, Senting TransStart\n");
			CountInActive = 1;
			gMyInfo.MetronomeOn = TRUE;
			DoPatch(
				&gMyInfo.MyPatchInfo[FindString(fsPatchNames, "TransStart")]);
		}

		/* On the first beat play a different sound.
		 */
		if (gMyInfo.MetronomeOn) {
			if (TempoState)
				SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort,
				DrumMidiChannel, 00, (int) gMyInfo.DrumRest);
			else
				SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort,
				DrumMidiChannel, 00, (int) gMyInfo.Drum1);

			sprintf(TempStrBuf, "On   %d", Count);
		}
		else
			sprintf(TempStrBuf, "Off  %d", Count);

		MyImageButtonSetText(&TempoDraw, TempStrBuf);

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
	PlayerPoll();

	/* HANDE Tempo Midi
	 */
	ToggleTempo();

	return TRUE;
}
