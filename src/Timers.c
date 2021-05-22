/*------------------------------------------------
|
|   File:   Timers
|
|   Contains:
|
|
|   Written By:     Elias Keshishoglou on Sun Mar 8 11:03:38 PDT 2015
|
|   Copyright �:  2015 Elias Keshishoglou all rights reserved.
|
|   This program is free software; you can redistribute it and/or
|   modify it under the terms of the GNU General Public License
|   as published by the Free Software Foundation; either version 2
|   of the License, or (at your option) any later version.
|
|   This program is distributed in the hope that it will be useful,
|   but WITHOUT ANY WARRANTY; without even the implied warranty of
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|   GNU General Public License for more details.
|
|   You should have received a copy of the GNU General Public License
|   along with this program; if not, write to the Free Software
|   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
|
|
|------------------------------------------------*/

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
#define TimerTicksPerQuater     8
#define TimerTicksPerQuaterSub     4
#else
#define TimerTicksPerQuater     1
#define TimerTicksPerQuaterSub     1
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
unsigned int OldTempo = 0;

#ifdef GTKTimer
/*-----------------------------------------------
 * Function:        MyTimerInit
 * Description:     Setup the timers used to Tempo and MTC
 *
 *------------------------------------------------*/
void MyTimerInit(void) {

    CountInActiveState = cntStateWaitingIdle;
    LoopRecBeats = 0;
    SubBeats = 0;

    /* Set up a timer for Tempo.
    */
    gMyInfo.TempoTimerID = 0;

    SetTempo(120);
}

/*-----------------------------------------------
 * Function:        Set the tempo
 * Description:     Set the tempo to a new value. Re-setup the timer
 *  interrupts to handle double the tempo.
 *
 *------------------------------------------------*/
void SetTempo(unsigned int NewTempo) {

    if (NewTempo <= MinTempoValue) {
        return;
    }

    /* Set the jack transport for timers.
    */
    com_tempo(NewTempo);

    Tempofont_desc = pango_font_description_from_string("Sans Bold 18");
    gMyInfo.Tempo = NewTempo;
    //  TempoChild = gtk_bin_get_child((GTK_BIN(TempoDraw)));
    //  gtk_widget_override_font((TempoChild), Tempofont_desc);

    /* Tell the timer to stop.
     */
    if (gMyInfo.TempoTimerID) {
        g_source_remove(gMyInfo.TempoTimerID);
        printd(LogInfo, "********TIMER REMOVED*****\n");
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
    gMyInfo.TempoTimerID = g_timeout_add(gMyInfo.TempoReload, (GSourceFunc) tempo_handler, (gpointer) gxml);

    //      gMyInfo.Timer1Count = 0;
}

/*-----------------------------------------------
 * Function:        Timer Callback
 *
 * Description:     <Description/Comments>
 *
 *------------------------------------------------*/
static gboolean tempo_handler(GtkWidget *widget) {

    printd(LogRealTime, " IN time_handler GTK\n");
    ToggleTempo();
    g_idle_add(GTKIdel_cb, theMainWindow);

    return TRUE;
}

#endif


#ifdef AlsaTimer
/*-----------------------------------------------
 * Function:        MyTimerInit
 * Description:     Setup the timers used to Tempo and MTC
 *
 *------------------------------------------------*/
void MyTimerInit(void) {

    CountInActiveState = cntStateWaitingIdle;
    LoopRecBeats = 0;
    SubBeats = 0;

    /* Set up a timer for Tempo.
    */
    SetTempo(100);
}

/*-----------------------------------------------
 * Function:        Set the tempo
 * Description:     Set the tempo to a new value. Re-setup the timer
 *  interrupts to handle double the tempo.
 *
 *------------------------------------------------*/
void SetTempo(unsigned int NewTempo) {

    if (NewTempo <= MinTempoValue) {
        return;
    }

    SetLoopTempo(NewTempo);
    /* Set the jack transport for timers.
    */
    com_tempo(NewTempo);

    gMyInfo.Tempo = NewTempo;

    /* Send out a message our tempo is changing.
     */
    SetupAlsaTimer(NewTempo);
}

/*-----------------------------------------------
 * Function:        Tempo callback.
 *
 * Description:     <Description/Comments>
 *
 *------------------------------------------------*/
static gboolean tempo_handler(GtkWidget *widget) {

    printd(LogRealTime, " IN time_handler Alsa\n");
    if (!JackRunning) {
        ToggleTempo();
    }

    g_idle_add(GTKIdel_cb, theMainWindow);

    //printd(LogTimer, "Call Toggle from tempo\n");
    //  PlayerPoll(TRUE);
    return TRUE;
}

#endif

#ifdef RTTimer
/*-----------------------------------------------
 * Function:        MyTimerInit
 * Description:     Setup the timers used to Tempo and MTC
 *
 *------------------------------------------------*/
void MyTimerInit(void) {

    CountInActiveState = cntStateWaitingIdle;
    LoopRecBeats = 0;
    SubBeats = 0;

    /* Set up a timer for Tempo.
    */
    gMyInfo.TempoTimerID = 0;
    gMyInfo.TimerCount = 0;
    gMyInfo.AlsaTimerHandle = 0;
    printd(LogTimer, "MyTimerInit\n");

    int Ret;
    timer_t timerid;
    struct sigevent sig;
    sig.sigev_notify = SIGEV_THREAD;
    sig.sigev_notify_function = time_handlerRT;
    // This get's passed to the handler.
    sig.sigev_value.sival_int = 20;
    sig.sigev_notify_attributes = NULL;
    sig.sigev_value.sival_ptr = &timerid;
    /* Let create a timer.
    */
    Ret = timer_create(CLOCK_REALTIME, &sig, &timerid);
    printd(LogTimer, "***** RT Timer Create **** %d %x\n", Ret, timerid);

    if (Ret != 0) {
        printd(LogTimer, "timer_settime() failed with %d\n", errno);
        //delete the timer.
        timer_delete(timerid);
        timerid = 0;
    }

    gMyInfo.TempoTimerID = timerid;

    SetTempo(111);
}

/*-----------------------------------------------
 * Function:        Set the tempo
 * Description:     Set the tempo to a new value. Re-setup the timer
 *  interrupts to handle double the tempo.
 *
 *------------------------------------------------*/
void SetTempo(unsigned int NewTempo) {
    int Ret;
    struct itimerspec in;


    printd(LogDebug, "SetTempo  %d ****\n", NewTempo);

    /* If the tempo is not reasonable.
    */
    if (NewTempo <= MinTempoValue) {
        return;
    }

    /* If the tempo's are the same don't bother.
    */
    if (gMyInfo.Tempo == OldTempo) {
        return;
    }

    SetLoopTempo(NewTempo);

    /* Set the jack transport for timers.
    */
    com_tempo(NewTempo);
    OldTempo = gMyInfo.Tempo = NewTempo;

#if 0
    pthread_attr_init( &attr );
    parm.sched_priority = 255;
    pthread_attr_setschedparam(&attr, &parm);
    sig.sigev_notify_attributes = &attr;
#endif

#if 0
    /* Delete current timer before creating new one.
    */
    if (gMyInfo.TempoTimerID) {
        printd(LogTimer, "***** RT Timer timer_delete %d ****\n", gMyInfo.TempoTimerID);
        timer_delete(gMyInfo.TempoTimerID);
        gMyInfo.TempoTimerID = 0;
    }


    sig.sigev_notify = SIGEV_THREAD;
    sig.sigev_notify_function = time_handlerRT;
    // This get's passed to the handler.
    sig.sigev_value.sival_int = 20;
    sig.sigev_value.sival_ptr = &timerid;
    sig.sigev_notify_attributes = NULL;

    /* Let create a timer.
    */
    Ret = timer_create(CLOCK_REALTIME, &sig, &timerid);
    printd(LogTimer, "***** RT Timer Create **** %d %d\n", Ret, timerid);
    gMyInfo.TempoTimerID = timerid;
#endif

    /* Was == 0 but this works much better.
    */
    memset(&in, 0, sizeof(in));
   // its.it_value.tv_sec = freq_nanosecs / 1000000000;
   // its.it_value.tv_nsec = freq_nanosecs % 1000000000;
   // its.it_interval.tv_sec = its.it_value.tv_sec;
   // its.it_interval.tv_nsec = its.it_value.tv_nsec;

    // Can't be zero.
    in.it_value.tv_sec = 1;
    in.it_value.tv_nsec = 0;
    in.it_interval.tv_sec = 0;

    //      in.it_interval.tv_nsec = 30000000000 / NewTempo;
    // Double the timer interval.
    in.it_interval.tv_nsec = 7500000000 / NewTempo;


    // in.it_value.tv_sec = 100000 / 1000000000;
    // in.it_value.tv_nsec = 100000 % 1000000000;
    // in.it_interval.tv_sec = in.it_value.tv_sec;
    // in.it_interval.tv_nsec = in.it_value.tv_nsec;



    //issue the periodic timer request here.
    Ret = timer_settime(gMyInfo.TempoTimerID, 0, &in, NULL);
    printd(LogTimer, "***** RT Timer SetTime **** %d %ld %x\n", Ret, in.it_interval.tv_nsec,gMyInfo.TempoTimerID);

    if (Ret == -1) {
      printd(LogError, "***** RT Timer SetTime Error **** %d %s\n", errno,strerror(errno));
    }

}

/*-----------------------------------------------
 * Function:        Timer Callback
 *
 * Description:     <Description/Comments>
 *
 *------------------------------------------------*/
static void time_handlerRT (union sigval val) {
    ToggleTempo();

    //    printd(LogTimer, "IN time_handler %d\n", SubBeats);

    if (++SubBeats > 7) {
        SubBeats = 0;
    }

    /* Queue the GUI idle task to draw.
    */
    g_idle_add(GTKIdel_cb, theMainWindow);

    return;
}
#endif


/*-----------------------------------------------
 * Function:        ToggleTempo
 *
 * Description:     Toggle based on the common tempo.
 *  Tempostate is the current index * 24
 *
 *------------------------------------------------*/
void ToggleTempo(void) {
    //  char Count;
    //  int Loop;
    struct timeval Time0;

    // gettimeofday(&Time0, NULL);
    // printd(LogTest, "%ld:%ld->\n", Time0.tv_sec, Time0.tv_usec);

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

            if (CountInActiveState == cntStateWaitingforCountIn) {
                CountInActiveState = cntStateWaitingforRecCount;
            }

        }

        /* Handle any recording for the looper. Make
        sure this is first.
        */
        switch (CountInActiveState) {
#if 0
        case cntStateWaitingforCountIn:
            printd(LogTimer, "cntStateWaitingforRecCount %d\n", BeatCount);
            /* Wait for the downbeat.
            */
            if (BeatCount == 1) {
                CountInActiveState = cntStateWaitingforRecCount;
            }

            break;
#endif
        case cntStateWaitingforRecCount:
            printd(LogTimer, "cntStateWaitingforRecCount %d %d\n", CountInCount, gMyInfo.CountInBeats );

            if (CountInCount-- == gMyInfo.CountInBeats) {

                MyOSCSetSync(1);

                // In case there is no downbeat.
                OSCCommand(OSCRecThres, 0);

                // Send a start record over OSC
                com_play();

                /* Tell Drums to start.
                */
                SendMidi(SND_SEQ_EVENT_START, TransportPort, 1,
                         0, 0);

                printd(LogTimer, "Start %d %d\n", CountInCount,  TempoState);

            }

            if ( CountInCount == 0) {
                CountInActiveState = cntStateRecording;

                //                          DoPatch(&gMyInfo.MyPatchInfo[FindString(fsPatchNames, "LP Rec")]);
                //      OSCCommand(OSCStartRecord, 0);
                gMyInfo.MetronomeOn = FALSE;
                printd(LogTimer, "Loop Start 1\n\n");
            }

            break;

        /* If we are recording and still not done.
        */
        case cntStateRecording:
            printd(LogTimer, "cntStateRecording %d\n", LoopRecBeats);

            if (LoopRecBeats == gMyInfo.LoopRecBeats) {

                /* Start the looper for recording.
                */
// ejk1                OSCCommand(OSCSyncSource, 0);

                OSCCommand(OSCStartRecord, 0);
                printd(LogTimer, "Start Recording %d\n", LoopRecBeats);
            }

            LoopRecBeats--;
#if 0
            if (LoopRecBeats == 0) {

                OSCCommand(OSCStopRecord, 0);
            }
#endif

            if (LoopRecBeats == 0) {

                /* Send stop to the looper.
                */
                OSCCommand(OSCStopRecord, 0);


                printd(LogTimer, "Stop Recording %d\n", LoopRecBeats);

                /* Stop the Jack timer.
                */
                com_stop();

                /* Set Sync to Loop for the remaining tracks.
                */
                SendMidi(SND_SEQ_EVENT_STOP, TransportPort, 1,
                         0, 0);

                printd(LogTimer, "Loop Off\n\n");

                CountInActiveState = cntStatePostRecord;
            }
            break;

        case cntStatePostRecord:
            printd(LogTimer, "Record Post\n\n");
            CountInActiveState = cntStateWaitingIdle;

            MyOSCSetSync(0);
            break;

        /* If we are not recording, but the CountIn has been set let's start Drum machine.
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
            }
            else {
                gLooperWaitForSync = 0;
                SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort,
                         DrumMidiChannel, 00, (int) gMyInfo.Drum1);
            }

 //           sprintf(TempoUpdateString, "%d-%d\nLP %2.2f", gMyInfo.Tempo, BeatCount, gMyInfo.LoopPosition);
        }
        else {
            gLooperWaitForSync = 0;
        }

        LEDControl(BeatCount, 1);
        UIUpdateFromTimer = TRUE;
        sprintf(TempoUpdateString, "%d-%d", gMyInfo.Tempo, BeatCount);

    }

#if 0
    if (!(TempoState % TimerTicksPerQuaterSub)) {

        /* On the first beat play a different sound.
         */
        sprintf(TempoUpdateString, "%d-%d\nLP %2.2f", gMyInfo.Tempo, BeatCount, gMyInfo.LoopPosition);
//        UIUpdateFromTimer = TRUE;

    }
#endif

    if (gMyInfo.Tempo != OldTempo) {
        SetTempo(gMyInfo.Tempo);
        // Must return or SegFault.
        return;
    }

#if 0
    else {
        //      if (TempoState == 2) {
        /*  Turn lights off
         */
        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
                 DrumMidiChannel, 04, (int) PedalLED3Off );

        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
                 DrumMidiChannel, 04, (int) PedalLED4Off );
        //      }
    }
#endif
    /* This is BAD FIXME
    */
    //  SetTempo(gMyInfo.Tempo);

}

