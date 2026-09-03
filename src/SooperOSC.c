/*-------------------------------------------------
|
|   File:   SooperOSC
|
|   Contains:
|
|
|   Written By:     Elias Keshishoglou on Wed Sep 20 16:37:34 PDT 2017
|
|   Copyright ©:   2017 Elias Keshishoglou all rights reserved.
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
|-------------------------------------------------*/

#define SooperOSC_c

/*
 * Place Includes Here
 */

#include "LiveMusicApp.h"
#include "AlsaUtils.h"
#include "SooperOSC.h"
#include <lo/lo.h>
#include "math.h"

/*
 * Place defines and Typedefs here
 */
#define DefaultLoopLength 20.0

/*
 * Place Local prototypes here
 */

/*
 * Place Static variables here
 */
static lo_address SLOSCaddr = NULL;
static lo_address JackVoladdr = NULL;
static lo_address Carlaaddr = NULL;
static lo_address Hydrogenaddr = NULL;
static lo_server osc_server = 0;
static lo_server osc_server1 = 0;
static char our_url[100];
static char CurrentLoop;
// http://essej.net/sooperlooper/doc_osc.html
/*------------------------------------------------
 * Function:        OSCCommand.
 *
 * Description:     Translate an app-level command (one of the
 *                   OSCSelect/OSCRec/OSCPause/... constants) into the
 *                   corresponding SooperLooper OSC message and send it
 *                   via liblo (lo_send). SooperLooper's own OSC API is
 *                   documented at essej.net/sooperlooper/doc_osc.html
 *                   (see the URL comment above this function) - most
 *                   commands are a `/sl/<loop>/hit` message with a
 *                   string naming the action ("record", "mute",
 *                   "undo", etc.), addressed to whichever loop
 *                   `CurrentLoop` currently selects.
 *
 *                   OSCSelect is special: it sets CurrentLoop from
 *                   `Option`, then immediately decrements it by 1 if
 *                   it's >= 0 (i.e. converts a 1-based selection into
 *                   SooperLooper's 0-based loop indexing), while
 *                   leaving negative values - SooperLooper's "-1 =
 *                   all loops" sentinel among others - untouched.
 *
 *                   Returns 1 immediately without sending anything if
 *                   SLOSCaddr hasn't been set up yet (i.e.
 *                   MyOSCInit() hasn't successfully resolved
 *                   SooperLooper's OSC address); otherwise always
 *                   returns 0, even for a Command value that matches
 *                   none of the switch cases (silently does nothing).
 *
 *-------------------------------------------------*/
int OSCCommand(int Command, char Option)
{
    char NewCommand[100];

    printd(LogOSC, "OSCCommand: %d %d\n", Command, Option);
    if (SLOSCaddr == NULL)
    {
        return (1);
    }

    switch (Command)
    {
    case OSCSelect:
        CurrentLoop = Option;

        if (CurrentLoop >= 0)
        {
            CurrentLoop = Option - 1;
        }

        break;

    case OSCRec:
        sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
        printd(LogOSC, "OSCRec %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "s", "record");
        gMyInfo.LoopTempo = gMyInfo.Tempo;
        break;

    case OSCPause:
        sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
        printd(LogOSC, "OSCPause %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "s", "pause");
        break;

    case OSCTrig:

        sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
        printd(LogOSC, "OSCTrig %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "s", "trigger");
        break;

    case OSCUndo:
        sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
        printd(LogOSC, "OSCUndo %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "s", "undo");
        break;

    case OSCAddLoop:
        printd(LogOSC, "OSC Add\n");
        lo_send(SLOSCaddr, "/loop_add", "if", 1, DefaultLoopLength);
        break;

    case OSCMute:
        sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
        printd(LogOSC, "OSCMute %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "s", "mute");
        break;

    case OSCStartRecord:
        sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
        printd(LogOSC, "OSCStartRecord %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "s", "record");
        break;

    case OSCStopRecord:
        sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
        printd(LogOSC, "OSCStopRecord %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "s", "record");
        break;

    case OSCSyncOn:
        sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
        lo_send(SLOSCaddr, NewCommand, "sf", "sync", 1.0);
        printd(LogOSC, "Sync On %s\n", NewCommand);
        break;

    case OSCSyncOff:
        sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
        printd(LogOSC, "Sync Off %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "sf", "sync", 0.0);
        break;

    case OSCSyncSource:
        //      sprintf(NewCommand, "/sl/-1/set", CurrentLoop);

        printd(LogOSC, "OSCSyncSource %d\n", Option);
        lo_send(SLOSCaddr, "/set", "si", "sync_source", Option);
        break;

    case OSCRecThres:
        sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
        printd(LogOSC, "Sync Off %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "sf", "rec_thresh", (float)Option / 127);
        break;

    case OSCRecLoop:
        sprintf(NewCommand, "/sl/%d/set", CurrentLoop);
        lo_send(SLOSCaddr, NewCommand, "si", "sync", 1);
        printd(LogOSC, "Sync On %s\n", NewCommand);

        sprintf(NewCommand, "/sl/%d/hit", CurrentLoop);
        printd(LogOSC, "OSCRecLP %s\n", NewCommand);
        lo_send(SLOSCaddr, NewCommand, "s", "record");

        gMyInfo.LoopTempo = gMyInfo.Tempo;
        gMyInfo.RecordStopLoop = gMyInfo.LoopPosition;
        break;

        // oscsend localhost 9951 /set si "sync_source" -3

        // lo_send(SLOSCaddr, "/sl/-2/set", "sf", "tap_tempo", 1);
    }
    return (0);
}

/*------------------------------------------------
 * Function:        ctrl_handler.
 *
 * Description:     Registered in MyOSCInit() as a catch-all liblo
 *                   method (NULL path/types - see the commented-out,
 *                   more specific `"/ctrl", "isf"` registration just
 *                   below it, which this replaced), so this receives
 *                   any incoming OSC message not claimed by a more
 *                   specific handler - in practice, SooperLooper's
 *                   `loop_pos` reply to the periodic `/sl/N/get`
 *                   request MyOSCPoll() sends below lands here.
 *                   `argv[2]` (a float) is the loop's current
 *                   playback position, stored into
 *                   gMyInfo.LoopPosition every time this fires.
 *
 *                   Also implements "stop recording once playback
 *                   actually starts": if a record-stop was requested
 *                   (gMyInfo.RecordStopLoop >= 0, set elsewhere) and
 *                   the loop position has now moved past 0, sends the
 *                   OSCRec command (toggles record off) and clears
 *                   the pending flag.
 *-------------------------------------------------*/
static int ctrl_handler(const char *path, const char *types, lo_arg **argv, int argc,
                        void *data, void *user_data)
{
    int index = argv[0]->i;
    float val = argv[2]->f;

//    printd(LogOSC, "ctrl_handler %s %s %s %s\n", path, types, data, user_data);
//    printd(LogOSC, "ctrl_handler %d %s f=%f\n", index, argv[1], val);
    gMyInfo.LoopPosition = val;

    /* Check to see if we should send the
    record off command.
    */
    if (val > 0 && (gMyInfo.RecordStopLoop >= 0))
    {
        printd(LogOSC, "ctrl_handler Stop Loop\n");
        gMyInfo.RecordStopLoop = -1;
        OSCCommand(OSCRec, 0);
    }

    return 0;
}

/*------------------------------------------------
 * Function:        midi_handler.
 *
 * Description:     Registered in MyOSCInit() for the specific "/midi"
 *                   OSC path - a custom (non-SooperLooper) message
 *                   format this app defines for some external
 *                   controller/bridge to drive it over OSC instead of
 *                   real MIDI. `argv[0]` (a string) selects "button"
 *                   vs "slider" behavior:
 *                     - "button": queues a layout/patch call
 *                       (gMyInfo.LayoutCall + params), handled later
 *                       on the main thread rather than acted on here.
 *                     - "slider": applies directly via
 *                       SetExpressionControl(Controller, Value).
 *                   `argv[1]`/`argv[2]` are the controller number and
 *                   value respectively.
 *-------------------------------------------------*/
static int midi_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
    // pingack expects: s:engine_url s:version i:loopcount
    // 1st arg is instance, 2nd ctrl string, 3nd is float value
    int Value = argv[2]->i;
    int Controller = argv[1]->i;

    printd(LogOSC, "midi_handler1 %s %s\n", path, types);
    printd(LogOSC, "midi_handler %s %d %d\n", argv[0], Controller, Value);

    if (!strcmp(argv[0], "button"))
    {
        printd(LogOSC, "buttons %d %d\n", Controller, Value);
        gMyInfo.LayoutCall = TRUE;
        gMyInfo.LayoutCallParam1 = Controller;
        gMyInfo.LayoutCallParam2 = TRUE;
    }

    if (!strcmp(argv[0], "slider"))
    {
        printd(LogOSC, "Slider %d %d\n", Controller, Value);
        SetExpressionControl(Controller, Value);
    }

    //  _acked = TRUE;
    return 0;
}

/* liblo error callback, registered with lo_server_new() in
 * MyOSCInit() below. Just logs whatever liblo reports (bad OSC
 * message, network error, etc.) - takes no other action.
 * (Previously declared to return `lo_err_handler` - a function
 * pointer type - instead of `void`, which caused a real type
 * mismatch at its lo_server_new() call site, hidden by the
 * project-wide -Wno-incompatible-pointer-types suppression. Fixed;
 * see CLEANUP_PROGRESS.md / patch 0002.) */
void OSCErrHandler(int num, const char *msg, const char *where)
{

    printf("OSCErrHandler %d, %s, %s \n", num, msg, where);
}

// netstat -np | grep -i Live

/*------------------------------------------------
 * Function:        MyOSCInit.
 *
 * Description:     One-time OSC setup, called once at startup with the
 *                   local OSC server's listen port. Two halves:
 *                   1. Client side: resolves lo_address handles for
 *                      four remote OSC targets this app talks to -
 *                      SooperLooper (SLOSCaddr), a separate JACK
 *                      volume control endpoint (JackVoladdr), Carla
 *                      (Carlaaddr), and Hydrogen (Hydrogenaddr) - all
 *                      sharing the same configured IP but different
 *                      ports.
 *                   2. Server side: opens this app's own OSC server
 *                      (osc_server) on `PortNumber` and registers two
 *                      incoming-message handlers: midi_handler() for
 *                      the "/midi" path, and ctrl_handler() as a
 *                      catch-all for everything else (see both
 *                      functions' docs above).
 *                   Logs and leaves osc_server NULL (silently - no
 *                   further error propagation) if the server can't be
 *                   opened.
 *-------------------------------------------------*/
void MyOSCInit(char *PortNumber)
{
    lo_method LoMidi;

    printd(LogOSC, "MyOSCInit: %s  L=%s V=%s H=%s\n",
           gMyInfo.OSCIPAddress,
           gMyInfo.OSCPortNumLooper,
           gMyInfo.OSCPortNumJackVol,
           gMyInfo.OSCPortNumHydrogen);

    /* Used as a trigger to stop recording.
     */
    gMyInfo.RecordStopLoop = -1;

    SLOSCaddr = lo_address_new(
        gMyInfo.OSCIPAddress,
        gMyInfo.OSCPortNumLooper);

    JackVoladdr = lo_address_new(
        gMyInfo.OSCIPAddress,
        gMyInfo.OSCPortNumJackVol);

    Carlaaddr = lo_address_new(
        gMyInfo.OSCIPAddress,
        gMyInfo.OSCPortNumCarla);

    Hydrogenaddr = lo_address_new(
        gMyInfo.OSCIPAddress,
        gMyInfo.OSCPortNumHydrogen);

    printd(LogOSC, "Init Second OSC \n");
    int osc_port;

    //   osc_server = lo_server_new("15200", NULL);
    osc_server = lo_server_new(PortNumber, OSCErrHandler);
    if (osc_server)
    {
        strcpy(our_url, lo_server_get_url(osc_server));
        osc_port = lo_server_get_port(osc_server);
        printd(LogOSC, "MyOSCInit Leave %s %d %d\n",
               our_url,
               osc_port,
               osc_server);

        LoMidi = lo_server_add_method(osc_server,
                                      "/midi", NULL, midi_handler, NULL);

        lo_server_add_method(osc_server,
                             NULL, NULL, ctrl_handler, NULL);

        //                       "/ctrl", "isf", ctrl_handler, NULL);
    }
    else
    {
        printf("*** Can't Open osc Server port 60000\n");
    }

    CurrentLoop = 0;
}

/*------------------------------------------------
 * Function:        MyOSCPoll.
 *
 * Description:     Called periodically (from the app's main timer
 *                   loop) to service OSC I/O: drains any pending
 *                   incoming messages non-blocking (up to 2ms wait -
 *                   dispatches to midi_handler()/ctrl_handler() as
 *                   registered), then actively requests the current
 *                   loop's playback position from SooperLooper via
 *                   `/sl/N/get loop_pos`, asking for the reply to be
 *                   sent back to this app's own "/ctrl" endpoint -
 *                   which ctrl_handler() (registered as the catch-all
 *                   above) then receives. `DownBeat` is accepted but
 *                   unused.
 *-------------------------------------------------*/
void MyOSCPoll(char DownBeat)
{
    char NewCommand[100];

    if (SLOSCaddr == NULL || osc_server == 0)
    {
        printd(LogError, "Can't open osc_server %x \n",osc_server);

        return;
    }

    lo_server_recv_noblock(osc_server, 2);
    //    lo_send(SLOSCaddr, "/ping", "ss", our_url, "/pingack");
    sprintf(NewCommand, "/sl/%d/get", CurrentLoop);
    lo_send(SLOSCaddr, NewCommand, "sss", "loop_pos", our_url, "/ctrl");
}

/*------------------------------------------------
 * Function:        MyOSCTap.
 *
 * Description:     Send SooperLooper a "tap tempo" event
 *                   (`/sl/-2/set tap_tempo 1.0` - loop index -2 is
 *                   SooperLooper's "apply to the global/master engine
 *                   settings, not a specific loop" convention).
 *                   `DownBeat` is accepted but unused - this fires
 *                   once per call regardless of beat position.
 *-------------------------------------------------*/
void MyOSCTap(char DownBeat)
{

    if (SLOSCaddr == NULL)
    {
        return;
    }

    lo_send(SLOSCaddr, "/sl/-2/set", "sf", "tap_tempo", 1.0);
}

/*------------------------------------------------
 * Function:        MyOSCClose.
 *
 * Description:     Shutdown/cleanup: frees all four remote
 *                   lo_address handles (SLOSCaddr/JackVoladdr/
 *                   Carlaaddr/Hydrogenaddr) and this app's own OSC
 *                   server (osc_server). Note: doesn't free
 *                   LoMidi/the registered method handles explicitly,
 *                   but lo_server_free() takes care of any methods
 *                   still registered on that server.
 *-------------------------------------------------*/
void MyOSCClose(void)
{

    printd(LogOSC, "MyOSCClose: %x\n", osc_server);
    lo_address_free(SLOSCaddr);
    lo_address_free(JackVoladdr);
    lo_address_free(Carlaaddr);
    lo_address_free(Hydrogenaddr);

    lo_server_free(osc_server);
}

/*------------------------------------------------
 * Function:        MyOSCSetSync.
 *
 * Description:     Switch SooperLooper's sync configuration between
 *                   two modes, keyed off `Type`:
 *                     - Type == 1 ("before recording"): selects loop
 *                       1, sets internal sync source (-2 = MIDI clock
 *                       per the sync_source convention documented
 *                       inline below), 8th-note quantization, and
 *                       turns on relative_sync - without which,
 *                       per the inline comment, recordings won't
 *                       start correctly.
 *                     - Otherwise ("after recording"): switches sync
 *                       source to loop 1 itself (sync_source = 1),
 *                       different quantization, and turns
 *                       relative_sync back off (also per the inline
 *                       comment, otherwise the recording start point
 *                       drifts).
 *                   Every setting here is a direct SooperLooper OSC
 *                   `/set`/`/sl/N/set` call - see the inline comments
 *                   at each lo_send() for what each parameter means;
 *                   they're already well-annotated in the source.
 *-------------------------------------------------*/
void MyOSCSetSync(char Type)
{
    char NewCommand[100];

    // sprintf(NewCommand,"/sl/%d/hit", CurrentLoop);
    printd(LogOSC, "MyOSCSetSync %d\n", Type);

    // Start recording.
    if (Type == 1)
    {

        // Select Loop 1
        OSCCommand(OSCSelect, 1);

        // Set the sync to 8th notes.
        lo_send(SLOSCaddr, "/set", "si", "eighth_per_cycle", 16);

        // Quantize loop end on 8th note
        lo_send(SLOSCaddr, "/sl/-1/set", "si", "quantize", 2);

        // Loop 0 sync on
        lo_send(SLOSCaddr, "/sl/0/set", "si", "sync", 1);

        /* Sync Internal    */
        //  sync_source  :: -3 = internal,  -2 = midi, -1 = jack, 0 = none, # > 0 = loop number (1 indexed)
        lo_send(SLOSCaddr, "/set", "si", "sync_source", -2);

        // Recordings will not start without this.
        lo_send(SLOSCaddr, "/sl/-1/set", "si", "relative_sync", 1);
    }
    else
    {
        // After recording.
        lo_send(SLOSCaddr, "/set", "si", "sync_source", 1);

        lo_send(SLOSCaddr, "/sl/0/set", "si", "sync", 1);

        // Quantize to (3)loop No (1)cycle
        lo_send(SLOSCaddr, "/sl/-1/set", "si", "quantize", 1);

        // Set the sync to 8th notes.
        lo_send(SLOSCaddr, "/set", "si", "eighth_per_cycle", 16);

        // Turn this off or the recording start
        // will be anywhere..
        lo_send(SLOSCaddr, "/sl/-1/set", "si", "relative_sync", 0);
        // oscsend localhost 9951 /sl/-1/set sf "relative_sync" 0
        //      OSCCommand(OSCSyncSource, 1);
        //      OSCCommand(OSCSyncOn, 0);
    }
}

/*------------------------------------------------
 * Function:        MyOSCLoadFile.
 *
 * Description:     Tell SooperLooper to load a saved session file
 *                   (`FileName`, a .slsess path) via `/load_session`.
 *
 *                   *** WORTH VERIFYING ***
 *                   The two reply-address arguments are hardcoded to
 *                   `"osc.udp://localhost:9951/"` rather than using
 *                   `our_url` (this app's own actual OSC server URL,
 *                   computed dynamically in MyOSCInit() and used
 *                   correctly elsewhere - e.g. MyOSCPoll()'s `/ctrl`
 *                   reply-to). If this app's OSC server isn't
 *                   actually running on port 9951, SooperLooper's
 *                   response to the load would be sent nowhere this
 *                   app is listening. Flagging rather than changing -
 *                   see the manual oscsend example in the comment
 *                   just below, which shows the same hardcoded port.
 *-------------------------------------------------*/
void MyOSCLoadFile(char *FileName)
{

    if (SLOSCaddr == NULL)
    {
        return;
    }

    //   printf("MyOSCLoadFile %x %s\n",SLOSCaddr, FileName);
    /* Load the file and send the results back to the SL GUI */
    lo_send(SLOSCaddr, "/load_session", "sss", FileName,
            "osc.udp://localhost:9951/", "osc.udp://localhost:9951/");
}

// oscsend localhost 9951 /load_session sss "/home/Music/EliasOriginals/Looper/Looper.slsess" osc.udp: //localhost:9952/ osc.udp://localhost:9952/

/*------------------------------------------------
 * Function:        MyOSCJackVol.
 *
 * Description:     Set a volume level (0-127 in) on a specific
 *                   channel, `channel` being one of the
 *                   typeOSCVolMaster/GuitarL/Midi/MP3/Looper/GuitarR
 *                   constants. Converts to a log-scaled 0.0-1.0 float
 *                   (ConvertToLogF(), clamped both ends) and sends it
 *                   out to up to two separate OSC targets in parallel,
 *                   each with its own channel->address/parameter
 *                   mapping:
 *                     - JackVoladdr: a dedicated external JACK-volume
 *                       OSC service (`/net/mhcloud/volume/jack-volume/N`),
 *                       if configured.
 *                     - Carlaaddr: Carla's own OSC control surface
 *                       (`/Carla/<rack>/set_parameter_value`, scaled
 *                       by MaxLevel=4), if configured. Note: the
 *                       typeOSCVolGuitarR case here is empty (no
 *                       lo_send) - unlike the JackVoladdr switch just
 *                       above, which does have a GuitarR case. Could
 *                       be intentional (GuitarR not exposed as a
 *                       Carla parameter) or an oversight - worth
 *                       checking against your actual Carla rack setup.
 *
 *                   *** Known workaround, already flagged in-source ***
 *                   The "Bandaid fix" early-return means a volume
 *                   that computes to exactly 0.0 after log conversion
 *                   sends NOTHING at all rather than a 0 volume - so
 *                   this function can't be used to actually mute a
 *                   channel to silence (see MyOSCJackMute() below for
 *                   the real mute path). Not something introduced by
 *                   this comment pass - already marked as a bandaid
 *                   by whoever added it.
 *-------------------------------------------------*/
void MyOSCJackVol(int Volume, int channel)
{
    float VolumeFloat;
    char OSCString[255];

    VolumeFloat = ConvertToLogF((float)(Volume)/128);
    printd(LogOSC, "MyOSCJackVol: %x %d %f\n", channel, Volume, VolumeFloat);

    
    // Bandaid fix
    if (VolumeFloat == 0) {
        printf(" *****   MyOSCJackVol BANDAID\n");
        return;
    }
    
//    VolumeFloat = (float)Volume/128;
    if (VolumeFloat > 1)
    {
        VolumeFloat = 1.0;
    }
    if (VolumeFloat < 0.0)
    {
        VolumeFloat = 0;
    }

    if (JackVoladdr != NULL)
    {
        printd(LogOSC, "MyOSCJackVol: %x %d %f\n", channel, Volume, VolumeFloat);

        switch (channel)
        {
        case typeOSCVolMaster:
            lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/master", "f", VolumeFloat);
            break;

        case typeOSCVolGuitarL:
            lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/0", "f", VolumeFloat);
            break;

        case typeOSCVolMidi:
            lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/1", "f", VolumeFloat);
            break;

        case typeOSCVolMP3:
            lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/2", "f", VolumeFloat);
            break;

        case typeOSCVolLooper:
            lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/3", "f", VolumeFloat);
            break;

        case typeOSCVolGuitarR:
            lo_send(JackVoladdr, "/net/mhcloud/volume/jack-volume/4", "f", VolumeFloat);
            break;
        }
    }
#define MaxLevel 4

    if (Carlaaddr != NULL)
    {
        sprintf(OSCString,"/Carla/%d/set_parameter_value",CarlaRackNum);
        printd(LogOSC, "MyOSCCarla: %s %f\n", OSCString, VolumeFloat);

        switch (channel)
        {
        case typeOSCVolMaster:
            lo_send(Carlaaddr, OSCString, "if", 5, VolumeFloat*MaxLevel);
            break;

        case typeOSCVolGuitarL:
            lo_send(Carlaaddr, OSCString, "if", 16, VolumeFloat*MaxLevel);
            break;

        case typeOSCVolMidi:
            lo_send(Carlaaddr, OSCString, "if", 25, VolumeFloat*MaxLevel);
            break;

        case typeOSCVolMP3:
            lo_send(Carlaaddr, OSCString, "if", 34, VolumeFloat*MaxLevel);
            break;

        case typeOSCVolLooper:
            lo_send(Carlaaddr, OSCString, "if", 43, VolumeFloat*MaxLevel);
            break;

        case typeOSCVolGuitarR:
            break;
        }
    }
}

/*------------------------------------------------
 * Function:        MyOSCJackMute.
 *
 * Description:     Mute/unmute a channel (`Mute` 1/0), `channel`
 *                   being the same typeOSCVolMaster(0xff)/GuitarL(0)/
 *                   Midi(1)/MP3(2)/Looper(3)/GuitarR(4) values as
 *                   MyOSCJackVol() above, though this function uses
 *                   raw numeric case labels instead of the named
 *                   constants. Always sends a MIDI CC to the pedal's
 *                   mute LED (PedalPort/DrumMidiChannel, controller 4)
 *                   first, regardless of which channel - so the LED
 *                   only tracks the last mute/unmute action, not any
 *                   particular channel. Then mirrors the same
 *                   channel->address/parameter mapping as
 *                   MyOSCJackVol() out to JackVoladdr's own `.../mute`
 *                   sub-path and Carla's set_parameter_value, if each
 *                   is configured.
 *
 *                   *** LIKELY BUG ***
 *                   Neither switch here has a `case 4:` (
 *                   typeOSCVolGuitarR) - unlike MyOSCJackVol()'s
 *                   JackVoladdr switch, which does cover it. Muting
 *                   channel 4 silently sends nothing via OSC to
 *                   either target (only the MIDI LED update above
 *                   still happens). Flagging rather than adding the
 *                   missing cases in this comments-only pass - would
 *                   need the right Carla parameter index for GuitarR
 *                   mute to fill it in correctly (compare the pattern
 *                   of indices 0/11/20/29/38 for the other channels).
 *-------------------------------------------------*/
void MyOSCJackMute(int Mute, int channel)
{
char OSCString[255];

    if (Mute == 1)
    {
        printd(LogOSC, "MyOSCJackMute On\n");
        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
                 DrumMidiChannel, 04, (int)PedalLED7On);
    }
    else
    {
        printd(LogOSC, "MyOSCJackMute Off\n");
        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
                 DrumMidiChannel, 04, (int)PedalLED7Off);
    }

    if (JackVoladdr != NULL)
    {

        switch (channel)
        {
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


    if (Carlaaddr != NULL)
    {
        sprintf(OSCString,"/Carla/%d/set_parameter_value",CarlaRackNum);
        printd(LogOSC, "MyOSCJackMute OSC %s %d\n",OSCString, Mute);

        switch (channel)
        {
        case 0xff:
            lo_send(Carlaaddr, OSCString, "if", 0, (float)Mute);
            break;

        case 0:
            lo_send(Carlaaddr, OSCString, "if", 11, (float)Mute);
            break;

        case 1:
            lo_send(Carlaaddr, OSCString, "if", 20, (float)Mute);
            break;

        case 2:
            lo_send(Carlaaddr, OSCString, "if", 29, (float)Mute);
            break;

        case 3:
            lo_send(Carlaaddr, OSCString, "if", 38, (float)Mute);
            break;
        }
    }
}

#if 0
https://sonosaurus.com/sooperlooper/doc_osc.html

"oscsend localhost 9951 /save_session sss "/home/MySongs/GuitarSound/GuitarSound.slsess" //localhost:9952/ osc.udp://localhost:9952/"

"oscsend localhost 9951 /sl/0/save_loop sssss  "/home/MySongs/GuitarSound/GuitarSound0.wav" IEEE float WAV //localhost:9952/ osc.udp://localhost:9952/"

"oscsend localhost 9951 /sl/1/save_loop sssss  "/home/MySongs/GuitarSound/GuitarSound1.wav" IEEE float WAV //localhost:9952/ osc.udp://localhost:9952/"


oscsend localhost 9951 /sl/0/save_loop sssss  "/home/MySongs/GuitarSound/GuitarSound0.wav" IEEE float WAV //localhost:9952/ osc.udp://localhost:9952/

oscsend localhost 22752 /Carla/2/set_volume f 1.5
oscsend localhost 22752 /Carla/2/set_active i 1
oscsend localhost 22752 /Carla/2/set_drywet f 0.9
oscsend localhost 22752 /Carla/2/set_parameter_value if 2 10.0
oscsend localhost 22752 /Carla/3/set_program i 2

#carla Mixer
#channel 1 0 - 3.9
#Gain
oscsend localhost 22752 /Carla/1/set_parameter_value if 5 3.5

#Wet Out
oscsend localhost 22752 /Carla/1/set_parameter_value if 4  9.2

#Mixer out
oscsend localhost 22752 /Carla/1/set_parameter_value if 16 3.5
oscsend localhost 22752 /Carla/1/set_parameter_value if 25 3.5
oscsend localhost 22752 /Carla/1/set_parameter_value if 34 3.5
oscsend localhost 22752 /Carla/1/set_parameter_value if 43 3.5



"oscsend localhost 9951 /sl/-2/set sf tap_tempo"
"oscsend localhost 9951 /sl/0/hit s "pause""
"oscsend localhost 9951 /set si "sync_source" - 3"
"oscsend localhost 9951 /sl/-1 set si "quantize" 2"
"oscsend localhost 9951 /set si "eighth_per_cycle" 16"
"oscsend localhost 9951 /sl/2/set si sync 1"

oscsend localhost 9951 / load_session sss "/home/elias/MySongs/Looper/LooperSync.slsess" osc.udp: //localhost:9952/ osc.udp://localhost:9952/

oscsend localhost 9951 / set si "sync_source" - 2
sync_source  :: -3 = internal,  -2 = midi, -1 = jack, 0 = none, # > 0 = loop number (1 indexed)

oscsend localhost 9951 / sl / -1 / set si "quantize"

oscsend localhost 9951 / sl / 0 / set si "sync" 1


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

sc.udp://KeshG3.2:22752/Carla

#endif
