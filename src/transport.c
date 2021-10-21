/*
 *  transport.c -- JACK transport master example client.
 *
 *  Copyright (C) 2003 Jack O'Quin.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Modifications for LiveMusicApp: Elias Keshishoglou
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <jack/jack.h>
#include <jack/transport.h>
#include "LiveMusicApp.h"

char *package = "LiveMusicTrans";               /* program name */
int done = 0;
jack_client_t *client;

/* Time and tempo variables.  These are global to the entire,
 * transport timeline.  There is no attempt to keep a true tempo map.
 * The default time signature is: "march time", 4/4, 120bpm
 */
float time_beats_per_bar = 4.0;
float time_beat_type = 4.0;
double time_ticks_per_beat = 1920.0;
double time_beats_per_minute = 120.0;
volatile int time_reset = 1;        /* true when time values change */

/* JACK timebase callback.
 *
 * Runs in the process thread.  Realtime, must not wait.
 */
static void timebase(jack_transport_state_t state, jack_nframes_t nframes,
                     jack_position_t *pos, int new_pos) {
    double min;         /* minutes since frame 0 */
    long abs_tick;          /* ticks since frame 0 */
    long abs_beat;          /* beats since frame 0 */

    if (new_pos || time_reset) {

        pos->valid = JackPositionBBT;
        pos->beats_per_bar = time_beats_per_bar;
        pos->beat_type = time_beat_type;
        pos->ticks_per_beat = time_ticks_per_beat;
        pos->beats_per_minute = time_beats_per_minute;

        time_reset = 0;     /* time change complete */

        /* Compute BBT info from frame number.  This is relatively
         * simple here, but would become complex if we supported tempo
         * or time signature changes at specific locations in the
         * transport timeline. */

        min = pos->frame / ((double) pos->frame_rate * 60.0);
        abs_tick = min * pos->beats_per_minute * pos->ticks_per_beat;
        abs_beat = abs_tick / pos->ticks_per_beat;

        pos->bar = abs_beat / pos->beats_per_bar;
        pos->beat = abs_beat - (pos->bar * pos->beats_per_bar) + 1;
        pos->tick = abs_tick - (abs_beat * pos->ticks_per_beat);
        pos->bar_start_tick = pos->bar * pos->beats_per_bar *
                              pos->ticks_per_beat;
        pos->bar++;     /* adjust start to bar 1 */

#if 1
        /* some debug code... */
        printd(LogDebug,  "\nnew position: %" PRIu32 "\tBBT: %3"
               PRIi32 "|%" PRIi32 "|%04" PRIi32 "\n",
               pos->frame, pos->bar, pos->beat, pos->tick);
#endif

    }
    else {

        /* Compute BBT info based on previous period. */
        pos->tick +=
            nframes * pos->ticks_per_beat * pos->beats_per_minute
            / (pos->frame_rate * 60);

        while (pos->tick >= pos->ticks_per_beat) {
            pos->tick -= pos->ticks_per_beat;
            //          ToggleTempo();
            printd(LogDebug, "Beat %ld\n", pos->beat);
            if (++pos->beat > pos->beats_per_bar) {
                pos->beat = 1;
                ++pos->bar;
                pos->bar_start_tick +=
                    pos->beats_per_bar
                    * pos->ticks_per_beat;
            }
        }
        //      printd(LogDebug, "Old Jack Position %ld %ld %ld\n", pos->beat,
        //          pos->tick, pos->ticks_per_beat);
    }
}

void jack_poll(void) {
    jack_position_t current;
    jack_transport_state_t transport_state;

    transport_state = jack_transport_query(client, &current);

    printd(LogDebug, "JackPoll: %lf %d %d %d\n",
           current.beats_per_minute,
           current.bar,
           current.beat,
           current.tick);
}

static void jack_shutdown(void) {
#if defined(RL_READLINE_VERSION) && RL_READLINE_VERSION >= 0x0400
    rl_cleanup_after_signal();
#endif
    printd(LogError, "JACK shut down, exiting ...\n");
}

static void signal_handler(void) {
    jack_client_close(client);
    printd(LogError, "signal received, exiting ...\n");
}

/* Command functions: see commands[] table following. */

static void com_activate(void) {
    if (jack_activate(client)) {
        printd(LogError, "cannot activate client");
    }
}

static void com_deactivate(void) {
    if (jack_deactivate(client)) {
        printd(LogError, "cannot deactivate client");
    }
}

static void com_exit(void) {
    done = 1;
}

static void com_locate(int NewFrame) {
    jack_nframes_t frame = NewFrame;

    jack_transport_locate(client, frame);
}

static void com_master(void) {
    if (jack_set_timebase_callback(client, 0, timebase, NULL) != 0) {
        printd(LogDebug, "Unable to take over timebase.\n");
    }
}

void com_play(void) {
    JackRunning = TRUE;
    jack_transport_start(client);
}

static void com_release(void) {
    JackRunning = FALSE;
    jack_release_timebase(client);
}

void com_stop(void) {
    JackRunning = FALSE;
    jack_transport_stop(client);
}

/* Change the tempo for the entire timeline, not just from the current
 * location. */
void com_tempo(int NewTempo) {

    time_beats_per_minute = NewTempo;
    time_reset = 1;
}

void com_setBeats(int Beats, int Type) {
    float time_beats_per_bar = Beats;
    float time_beat_type = Type;
}

/* Set sync timeout in seconds. */
static void com_timeout(int TimeOut) {
    double timeout = TimeOut;

    jack_set_sync_timeout(client, (jack_time_t)(timeout * 1000000));
}

/* Toggle between play and stop state */
static void com_toggle(void) {
    jack_position_t current;
    jack_transport_state_t transport_state;

    transport_state = jack_transport_query(client, &current);

    switch (transport_state) {
    case JackTransportStopped:
        com_play();
        break;
    case JackTransportRolling:
        com_stop();
        break;
    case JackTransportStarting:
        printd(LogError, "state: Starting - no transport toggling");
        break;
    default:
        printd(LogError, "unexpected state: no transport toggling");
    }
}

int InitJackTransport(void) {
    jack_status_t status;

#ifdef ejk
    /* basename $0 */
    package = strrchr(argv[0], '/');
    if (package == 0) {
        package = argv[0];
    }
    else {
        package++;
    }
#endif
    /* open a connection to the JACK server */
    client = jack_client_open(package, JackNullOption, &status);
    if (client == NULL) {
        printd(LogError, "jack_client_open() failed, "
               "status = 0x%2.0x\n", status);
        return 1;
    }

    jack_on_shutdown(client, jack_shutdown, 0);

    if (jack_activate(client)) {
        printd(LogError, "cannot activate client");
        return 1;
    }

    com_master();
    // Using jack as tempo master.
    JackRunning = FALSE;
    //  com_play();
    com_activate();
    printd(LogDebug, "*********** Jack done\n");
}

int CloseJackTransport(void) {

    jack_client_close(client);
}
