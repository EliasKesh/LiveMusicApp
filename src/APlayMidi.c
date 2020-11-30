/*---------------------------------------------
|
|	File: 	APlayMidi
|
|	Contains:
|
|
|	Written By: 	Elias Keshishoglou on Tue Nov 17 02:31:07 PM PST 2020
|
|	Copyright : 	2020 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------*/

/* TODO: sequencer queue timer selection */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <alsa/asoundlib.h>
#include "aconfig.h"
#include "LiveMusicApp.h"

#define false 0
#define true 1

#include <pthread.h>

void *alsa_Loop_thread ( void * context_ptr );
static void errormsg ( const char *msg, ... );

/*
 * 31.25 kbaud, one start bit, eight data bits, two stop bits.
 * (The MIDI spec says one stop bit, but every transmitter uses two, just to be
 * sure, so we better not exceed that to avoid overflowing the output buffer.)
 */
#define MIDI_BYTES_PER_SEC (31250 / (1 + 8 + 2))

/*
 * A MIDI event after being parsed/loaded from the file.
 * There could be made a case for using snd_seq_event_t instead.
 */
struct event {
	struct event *next;		/* linked list */

	unsigned char type;		/* SND_SEQ_EVENT_xxx */
	unsigned char port;		/* port index */
	unsigned int tick;
	union {
		unsigned char d[3];	/* channel and data bytes */
		int tempo;
		unsigned int length;	/* length of sysex data */
	} data;
	unsigned char sysex[0];
};

struct track {
	struct event *first_event;	/* list of all events in this track */
	int end_tick;			/* length of this track */
	struct event *current_event;	/* used while loading and playing */
};

static snd_seq_t *seq;
//static int client;
// static int port_count;
static snd_seq_addr_t LoopPort;
static int queue;
static char file_name[FileNameMaxLength];
static FILE *file;
static int file_offset;		/* current offset in input file */
static int num_tracks;
static struct track *tracks;
static int smpte_timing;

//int g_midi_ignore;
pthread_t g_alsa_midi_Loop_tid; /* alsa_midi_thread id */
int 	MyTempo;
volatile int MidiLooping;

/*--------------------------------------------
* Function:  InitMidiLooper
*
* Description:           Initialize the Midifile looper
*---------------------------------------------*/
int InitMidiLooper ( void )
{
	snd_seq_port_info_t *pinfo;
	int err;

	seq = gMyInfo.SeqPort[DRLoop];

	printd ( LogDebug, "open sequencer %x\n", seq );

	LoopPort.client = snd_seq_client_id ( seq );
	LoopPort.port = DRLoop;
//	snd_seq_parse_address(seq, &LoopPort, "129:0");
	printd ( LogDebug, "LoopPort %d %d\n", LoopPort.client, LoopPort.port );

	queue = snd_seq_alloc_queue ( seq );

	MidiLooping = false;
	alsa_loop_init();

	return 0;
}

/*-----------------------------------------------
* Function: ToggleMidiLoop
*
* Description:  Description
*
*----------------------------------------------*/
void ToggleMidiLoop ( void )
{

	if ( MidiLooping ) {
		StopMidiLoop();

	} else {
		StartMidiLoop ( NULL );
	}
}

/*-------------------------------------------------
* Function:  StopMidiLoop
*
* Description:           Stop and clear buffer
*------------------------------------------------*/
void StopMidiLoop ( void )
{
	MidiLooping = false;
	snd_seq_drain_output ( seq );
	snd_seq_reset_pool_output ( seq );
	snd_seq_drop_output_buffer ( seq );
	// Fix this, need a context switch
//	poll();
	sleep ( 2 );
//	usleep(500);
}

/*--------------------------------------------
* Function:    StartMidiLoop
*
* Description: Pass midi file and open.
*---------------------------------------------*/
void StartMidiLoop ( char *filename )
{

	if ( MidiLooping )
		StopMidiLoop();

	if ( filename != NULL )
		strcpy ( file_name, filename );

	printd ( LogDebug, "Start File [%s]\n", file_name );
	gLooperWaitForSync = true;
	MidiLooping = true;
}

/*-----------------------------------------------
* Function: SetLoopStartSync
*
* Description:  Sync start with MetroNome.
*
*----------------------------------------------*/
void SetLoopStartSync ( void )
{
	snd_seq_event_t ev;

	snd_seq_start_queue ( seq, queue, &ev );
//	snd_seq_ev_set_queue_pos_tick(ev, queue, 0);

}

/*--------------------------------------------
* Function: SetLoopTempo
*
* Description: Adjust the playback speed
*---------------------------------------------*/
void SetLoopTempo ( int NewTempo )
{

	if ( !MidiLooping )
		return;

	MyTempo = NewTempo;
	snd_seq_event_t ev;
	int err;
	printd ( LogDebug, "SetLoopTempo %d\n", NewTempo );
	printd ( LogTest, "SetLoopTempo %d\n", NewTempo );

	ev.type = SND_SEQ_EVENT_TEMPO;
	ev.dest.port = SND_SEQ_PORT_SYSTEM_TIMER;
	ev.data.queue.queue = queue;
	ev.data.queue.param.value = 60000000 / NewTempo;
	/* this blocks when the output pool has been filled */
	err = snd_seq_event_output_direct ( seq, &ev );

}

/*-----------------------------------------------
* Function: MyAlsaLoopClose
*
* Description: Close the midi queue
*
*-------------------------------------------*/
int MyAlsaLoopClose ( void )
{
	int ret;

	snd_seq_close ( seq );
	sleep ( 1 );

	/* Cancel the thread. Don't know better way.
	Poll or unblock mechanisms seem to not be
	available for alsa sequencer */
	pthread_cancel ( g_alsa_midi_Loop_tid );

	/* Wait midi thread to finish */
	ret = pthread_join ( g_alsa_midi_Loop_tid, NULL );

	return ( false );
}

/*-----------------------------------------------
* Function: init_seq
*
* Description:  Description
*
*----------------------------------------------*/
static void init_seq ( void )
{
	int err;

	/* open sequencer */
	seq = gMyInfo.SeqPort[DRLoop];

	printd ( LogDebug, "open sequencer %d\n", err );

	/* find out who we actually are */
//	client = snd_seq_client_id(seq);
//	printd(LogDebug, "get client id %d\n", client);
}

static int read_byte ( void )
{
	++file_offset;
	return getc ( file );
}

/* reads a little-endian 32-bit integer */
static int read_32_le ( void )
{
	int value;
	value = read_byte();
	value |= read_byte() << 8;
	value |= read_byte() << 16;
	value |= read_byte() << 24;
	return !feof ( file ) ? value : -1;
}

/* reads a 4-character identifier */
static int read_id ( void )
{
	return read_32_le();
}
#define MAKE_ID(c1, c2, c3, c4) ((c1) | ((c2) << 8) | ((c3) << 16) | ((c4) << 24))

/* reads a fixed-size big-endian number */
static int read_int ( int bytes )
{
	int c, value = 0;

	do {
		c = read_byte();

		if ( c == EOF )
			return -1;

		value = ( value << 8 ) | c;
	} while ( --bytes );

	return value;
}

/* reads a variable-length number */
static int read_var ( void )
{
	int value, c;

	c = read_byte();
	value = c & 0x7f;

	if ( c & 0x80 ) {
		c = read_byte();
		value = ( value << 7 ) | ( c & 0x7f );

		if ( c & 0x80 ) {
			c = read_byte();
			value = ( value << 7 ) | ( c & 0x7f );

			if ( c & 0x80 ) {
				c = read_byte();
				value = ( value << 7 ) | c;

				if ( c & 0x80 )
					return -1;
			}
		}
	}

	return !feof ( file ) ? value : -1;
}

/* allocates a new event */
static struct event *new_event ( struct track *track, int sysex_length )
{
	struct event *event;

	event = malloc ( sizeof ( struct event ) + sysex_length );
//	check_mem(event);

	event->next = NULL;

	/* append at the end of the track's linked list */
	if ( track->current_event )
		track->current_event->next = event;
	else
		track->first_event = event;

	track->current_event = event;

	return event;
}

static void skip ( int bytes )
{
	while ( bytes > 0 )
		read_byte(), --bytes;
}

/* reads one complete track from the file */
static int read_track ( struct track *track, int track_end )
{
	int tick = 0;
	unsigned char last_cmd = 0;
	unsigned char port = 0;

	/* the current file position is after the track ID and length */
	while ( file_offset < track_end ) {
		unsigned char cmd;
		struct event *event;
		int delta_ticks, len, c;

		delta_ticks = read_var();

		if ( delta_ticks < 0 )
			break;

		tick += delta_ticks;

		c = read_byte();

		if ( c < 0 )
			break;

		if ( c & 0x80 ) {
			/* have command */
			cmd = c;

			if ( cmd < 0xf0 )
				last_cmd = cmd;

		} else {
			/* running status */
			ungetc ( c, file );
			file_offset--;
			cmd = last_cmd;

			if ( !cmd )
				goto _error;
		}

		switch ( cmd >> 4 ) {
			/* maps SMF events to ALSA sequencer events */
			static const unsigned char cmd_type[] = {
				[0x8] = SND_SEQ_EVENT_NOTEOFF,
				[0x9] = SND_SEQ_EVENT_NOTEON,
				[0xa] = SND_SEQ_EVENT_KEYPRESS,
				[0xb] = SND_SEQ_EVENT_CONTROLLER,
				[0xc] = SND_SEQ_EVENT_PGMCHANGE,
				[0xd] = SND_SEQ_EVENT_CHANPRESS,
				[0xe] = SND_SEQ_EVENT_PITCHBEND
			};

		case 0x8: /* channel msg with 2 parameter bytes */
		case 0x9:
		case 0xa:
		case 0xb:
		case 0xe:
			event = new_event ( track, 0 );
			event->type = cmd_type[cmd >> 4];
			event->port = port;
			event->tick = tick;
			event->data.d[0] = cmd & 0x0f;
			event->data.d[1] = read_byte() & 0x7f;
			event->data.d[2] = read_byte() & 0x7f;
			break;

		case 0xc: /* channel msg with 1 parameter byte */
		case 0xd:
			event = new_event ( track, 0 );
			event->type = cmd_type[cmd >> 4];
			event->port = port;
			event->tick = tick;
			event->data.d[0] = cmd & 0x0f;
			event->data.d[1] = read_byte() & 0x7f;
			break;

		case 0xf:
			switch ( cmd ) {
			case 0xf0: /* sysex */
			case 0xf7: /* continued sysex, or escaped commands */
				len = read_var();

				if ( len < 0 )
					goto _error;

				if ( cmd == 0xf0 )
					++len;

				event = new_event ( track, len );
				event->type = SND_SEQ_EVENT_SYSEX;
				event->port = port;
				event->tick = tick;
				event->data.length = len;

				if ( cmd == 0xf0 ) {
					event->sysex[0] = 0xf0;
					c = 1;

				} else {
					c = 0;
				}

				for ( ; c < len; ++c )
					event->sysex[c] = read_byte();

				break;

			case 0xff: /* meta event */
				c = read_byte();
				len = read_var();

				if ( len < 0 )
					goto _error;

				switch ( c ) {
				case 0x21: /* port number */
					if ( len < 1 )
						goto _error;

//					port = read_byte() % port_count;
					printf ( "Port %d\n",port );
					skip ( len - 1 );
					break;

				case 0x2f: /* end of track */
					track->end_tick = tick;
					skip ( track_end - file_offset );
					return 1;

				case 0x51: /* tempo */
					if ( len < 3 )
						goto _error;

					if ( smpte_timing ) {
						/* SMPTE timing doesn't change */
						skip ( len );

					} else {
						event = new_event ( track, 0 );
						event->type = SND_SEQ_EVENT_TEMPO;
						event->port = port;
						event->tick = tick;
						event->data.tempo = read_byte() << 16;
						event->data.tempo |= read_byte() << 8;
						event->data.tempo |= read_byte();
						printd ( LogDebug, "ejk Tempo %d\n", event->data.tempo );
						skip ( len - 3 );
					}

					break;

				default: /* ignore all other meta events */
					skip ( len );
					break;
				}

				break;

			default: /* invalid Fx command */
				goto _error;
			}

			break;

		default: /* cannot happen */
			goto _error;
		}
	}

_error:
	errormsg ( "%s: invalid MIDI data (offset %#x)", file_name, file_offset );
	return 0;
}

/* reads an entire MIDI file */
static int read_smf ( void )
{
	int header_len, type, time_division, i, err;
	snd_seq_queue_tempo_t *queue_tempo;

	/* the curren position is immediately after the "MThd" id */
	header_len = read_int ( 4 );

	if ( header_len < 6 ) {
invalid_format:
		errormsg ( "%s: invalid file format", file_name );
		return 0;
	}

	type = read_int ( 2 );

	if ( type != 0 && type != 1 ) {
		errormsg ( "%s: type %d format is not supported", file_name, type );
		return 0;
	}

	num_tracks = read_int ( 2 );

	if ( num_tracks < 1 || num_tracks > 1000 ) {
		errormsg ( "%s: invalid number of tracks (%d)", file_name, num_tracks );
		num_tracks = 0;
		printf ( "num_tracks %d\n",num_tracks );
		return 0;
	}

	tracks = calloc ( num_tracks, sizeof ( struct track ) );

	if ( !tracks ) {
		errormsg ( "out of memory" );
		num_tracks = 0;
		return 0;
	}

	time_division = read_int ( 2 );

	if ( time_division < 0 )
		goto invalid_format;

	printd ( LogDebug, "ejk read_smf %d\n",
	         time_division );
	/* interpret and set tempo */
	snd_seq_queue_tempo_alloca ( &queue_tempo );
	smpte_timing = !! ( time_division & 0x8000 );

	if ( !smpte_timing ) {
		printd ( LogDebug, "Loop !smpte_timing %d\n" );

		/* time_division is ticks per quarter */
//		snd_seq_queue_tempo_set_tempo(queue_tempo, 500000); /* default: 120 bpm */

		snd_seq_queue_tempo_set_tempo ( queue_tempo, 60000000 / gMyInfo.Tempo ); /* default: 120 bpm */

		snd_seq_queue_tempo_set_ppq ( queue_tempo, time_division );

	} else {
		printd ( LogDebug, "Loop smpte_timing %d\n" );

		/* upper byte is negative frames per second */
		i = 0x80 - ( ( time_division >> 8 ) & 0x7f );
		/* lower byte is ticks per frame */
		time_division &= 0xff;
		/* now pretend that we have quarter-note based timing */
		time_division = gMyInfo.Tempo;
		printd ( LogDebug, "ejk Tempo Case %d %d\n", i, time_division );

		switch ( i ) {
		case 24:
			snd_seq_queue_tempo_set_tempo ( queue_tempo, 500000 );
			snd_seq_queue_tempo_set_ppq ( queue_tempo, 12 * time_division );
			break;

		case 25:
			snd_seq_queue_tempo_set_tempo ( queue_tempo, 400000 );
			snd_seq_queue_tempo_set_ppq ( queue_tempo, 10 * time_division );
			break;

		case 29: /* 30 drop-frame */
			snd_seq_queue_tempo_set_tempo ( queue_tempo, 100000000 );
			snd_seq_queue_tempo_set_ppq ( queue_tempo, 2997 * time_division );
			break;

		case 30:
			snd_seq_queue_tempo_set_tempo ( queue_tempo, 500000 );
			snd_seq_queue_tempo_set_ppq ( queue_tempo, 15 * time_division );
			break;

		default:
			errormsg ( "%s: invalid number of SMPTE frames per second (%d)",
			           file_name, i );
			return 0;
		}
	}

#if 1
	printd ( LogDebug, "snd_seq_set_queue_tempo\n" );
	err = snd_seq_set_queue_tempo ( seq, queue, queue_tempo );

//	printd(LogDebug, "ejk read_smf %d\n", queue_tempo);
	if ( err < 0 ) {
		errormsg ( "Cannot set queue tempo (%u/%i)",
		           snd_seq_queue_tempo_get_tempo ( queue_tempo ),
		           snd_seq_queue_tempo_get_ppq ( queue_tempo ) );
//		return 0;
	}

#endif

	/* read tracks */
	for ( i = 0; i < num_tracks; ++i ) {
		int len;

		/* search for MTrk chunk */
		for ( ;; ) {
			int id = read_id();
			len = read_int ( 4 );

			if ( feof ( file ) ) {
				errormsg ( "%s: unexpected end of file", file_name );
				return 0;
			}

			if ( len < 0 || len >= 0x10000000 ) {
				errormsg ( "%s: invalid chunk length %d", file_name, len );
				return 0;
			}

			if ( id == MAKE_ID ( 'M', 'T', 'r', 'k' ) )
				break;

			skip ( len );
		}

		if ( !read_track ( &tracks[i], file_offset + len ) )
			return 0;
	}

	return 1;
}

static int read_riff ( void )
{
	/* skip file length */
	read_byte();
	read_byte();
	read_byte();
	read_byte();

	/* check file type ("RMID" = RIFF MIDI) */
	if ( read_id() != MAKE_ID ( 'R', 'M', 'I', 'D' ) ) {
invalid_format:
		errormsg ( "%s: invalid file format", file_name );
		return 0;
	}

	/* search for "data" chunk */
	for ( ;; ) {
		int id = read_id();
		int len = read_32_le();

		if ( feof ( file ) ) {
data_not_found:
			errormsg ( "%s: data chunk not found", file_name );
			return 0;
		}

		if ( id == MAKE_ID ( 'd', 'a', 't', 'a' ) )
			break;

		if ( len < 0 )
			goto data_not_found;

		skip ( ( len + 1 ) & ~1 );
	}

	/* the "data" chunk must contain data in SMF format */
	if ( read_id() != MAKE_ID ( 'M', 'T', 'h', 'd' ) )
		goto invalid_format;

	return read_smf();
}

static void cleanup_file_data ( void )
{
	int i;
	struct event *event;

	for ( i = 0; i < num_tracks; ++i ) {
		event = tracks[i].first_event;

		while ( event ) {
			struct event *next = event->next;
			free ( event );
			event = next;
		}
	}

	num_tracks = 0;
	free ( tracks );
	tracks = NULL;
}

/*-----------------------------------------------
* Function: handle_big_sysex
*
* Description:  Description
*
*----------------------------------------------*/
static void handle_big_sysex ( snd_seq_event_t *ev )
{
	unsigned int length;
	ssize_t event_size;
	int err;

	length = ev->data.ext.len;

	if ( length > MIDI_BYTES_PER_SEC )
		ev->data.ext.len = MIDI_BYTES_PER_SEC;

	event_size = snd_seq_event_length ( ev );

	if ( event_size + 1 > snd_seq_get_output_buffer_size ( seq ) ) {
		err = snd_seq_drain_output ( seq );
		printd ( LogDebug, "drain output%d\n", err );
		err = snd_seq_set_output_buffer_size ( seq, event_size + 1 );
		printd ( LogDebug, "set output buffer size%d\n", err );
	}

	while ( length > MIDI_BYTES_PER_SEC ) {
		err = snd_seq_event_output ( seq, ev );
		printd ( LogDebug, "output event %d\n", err );
		err = snd_seq_drain_output ( seq );
		printd ( LogDebug, "drain output %d\n", err );
		err = snd_seq_sync_output_queue ( seq );
		printd ( LogDebug, "sync output %d\n", err );

		if ( sleep ( 1 ) )
			printd ( LogDebug, "aborted" );

		ev->data.ext.ptr += MIDI_BYTES_PER_SEC;
		length -= MIDI_BYTES_PER_SEC;
	}

	ev->data.ext.len = length;
}

/*-----------------------------------------------
* Function: play_midi
*
* Description:  Description
*
*----------------------------------------------*/
static void play_midi ( void )
{
	snd_seq_event_t ev;
	int i, max_tick, err;

	/* calculate length of the entire file */
	max_tick = -1;

	for ( i = 0; i < num_tracks; ++i ) {
		if ( tracks[i].end_tick > max_tick )
			max_tick = tracks[i].end_tick;
	}

	MyTempo = gMyInfo.Tempo;
	printd ( LogDebug, "MidiLooping Tempo=%d\n", MyTempo );


	// Here is where we loop.
	while ( MidiLooping ) {
		printd ( LogDebug, "ejk Starting loop\n" );

		/* initialize current position in each track */
		for ( i = 0; i < num_tracks; ++i )
			tracks[i].current_event = tracks[i].first_event;

		/* common settings for all our events */
		snd_seq_ev_clear ( &ev );
		ev.queue = queue;
		ev.source.port = DRLoop;
		ev.flags = SND_SEQ_TIME_STAMP_TICK;

		err = snd_seq_start_queue ( seq, queue, NULL );
//		printd(LogDebug, "start queue%d\n", err);
		/* The queue won't be started until the START_QUEUE event is
		 * actually drained to the kernel, which is exactly what we want. */

		for ( ;; ) {
			struct event* event = NULL;
			struct track* event_track = NULL;
			int i, min_tick = max_tick + 1;

			/* search next event */
			for ( i = 0; i < num_tracks; ++i ) {
				struct track *track = &tracks[i];
				struct event *e2 = track->current_event;

				if ( e2 && e2->tick < min_tick ) {
					min_tick = e2->tick;
					event = e2;
					event_track = track;
				}
			}

			while ( gLooperWaitForSync );

			if ( !event )
				break; /* end of song reached */

			/* advance pointer to next event */
			event_track->current_event = event->next;

			/* output the event */
			ev.type = event->type;
			ev.time.tick = event->tick;

			snd_seq_ev_set_subs ( &ev );

			switch ( ev.type ) {
			case SND_SEQ_EVENT_NOTEON:
			case SND_SEQ_EVENT_NOTEOFF:
			case SND_SEQ_EVENT_KEYPRESS:
				snd_seq_ev_set_fixed ( &ev );
				ev.data.note.channel = event->data.d[0];
				ev.data.note.note = event->data.d[1];
				ev.data.note.velocity = event->data.d[2];
				break;

			case SND_SEQ_EVENT_CONTROLLER:
				snd_seq_ev_set_fixed ( &ev );
				ev.data.control.channel = event->data.d[0];
				ev.data.control.param = event->data.d[1];
				ev.data.control.value = event->data.d[2];
				break;

			case SND_SEQ_EVENT_PGMCHANGE:
			case SND_SEQ_EVENT_CHANPRESS:
				snd_seq_ev_set_fixed ( &ev );
				ev.data.control.channel = event->data.d[0];
				ev.data.control.value = event->data.d[1];
				break;

			case SND_SEQ_EVENT_PITCHBEND:
				snd_seq_ev_set_fixed ( &ev );
				ev.data.control.channel = event->data.d[0];
				ev.data.control.value =
				    ( ( event->data.d[1] ) |
				      ( ( event->data.d[2] ) << 7 ) ) - 0x2000;
				break;

			case SND_SEQ_EVENT_SYSEX:
				snd_seq_ev_set_variable ( &ev, event->data.length, event->sysex );
				handle_big_sysex ( &ev );
				break;

			case SND_SEQ_EVENT_TEMPO:
				snd_seq_ev_set_fixed ( &ev );
//				ev.dest.client = SND_SEQ_CLIENT_SYSTEM;
				ev.dest.port = SND_SEQ_PORT_SYSTEM_TIMER;
				ev.data.queue.queue = queue;
				event->data.tempo = 60000000 / MyTempo;
				ev.data.queue.param.value = event->data.tempo;
				printd ( LogDebug, "ejk SND_SEQ_EVENT_TEMPO %d %d\n", event->data.tempo, MyTempo );
				break;

			default:
				printd ( LogDebug, "Invalid event type %d!", ev.type );
			}

			/* this blocks when the output pool has been filled */
//			err = snd_seq_event_output_buffer(seq, &ev);
//			err = snd_seq_event_output_direct(seq, &ev);

			snd_seq_nonblock ( seq, 1 );

			err = snd_seq_event_output ( seq, &ev );
//			printd(LogDebug, "output event%d\n", err);
		}

#if 1
		/* schedule queue stop at end of song */
		snd_seq_ev_set_fixed ( &ev );
		ev.type = SND_SEQ_EVENT_STOP;
		ev.time.tick = max_tick;
//		ev.dest.client = SND_SEQ_CLIENT_SYSTEM;
		ev.dest.port = SND_SEQ_PORT_SYSTEM_TIMER;
		ev.data.queue.queue = queue;
		err = snd_seq_event_output ( seq, &ev );
		printd ( LogDebug, "output event error %d %x\n",
		         err, seq );
#endif

		/* make sure that the sequencer sees all our events */
		do {
			err = snd_seq_drain_output ( seq );
			printd ( LogDebug, "drain output %d\n", err
			       );
			gLooperWaitForSync = true;
		} while ( err > 0 );

		/*
		 * There are three possibilities how to wait until all events have
		 * been played:
		 * 1) send an event back to us (like pmidi does), and wait for it;
		 * 2) wait for the EVENT_STOP notification for our queue which is sent
		 *    by the system timer port (this would require a subscription);
		 * 3) wait until the output pool is empty.
		 * The last is the simplest.
		 */

#if 0

		do {
			err = snd_seq_event_output_pending ( seq );
			printd ( LogDebug, "Pending output %d\n", err
			       );
			gLooperWaitForSync = true;
		} while ( err > 0 );

#endif

		err = snd_seq_sync_output_queue ( seq );
		gLooperWaitForSync = true;
		printd ( LogDebug, "After Sync %d %d\n", MidiLooping, err );
	} // end of while.

	/* give the last notes time to die away */
//	if (end_delay > 0)
//		sleep(end_delay);
	printd ( LogDebug, "Leaving play_midi\n" );
}

/*-----------------------------------------------
* Function: play_file
*
* Description:  Description
*
*----------------------------------------------*/
static int play_file ( void )
{
	int ok;

	printd ( LogDebug, "Play_file [%s]\n", file_name );

	file = fopen ( file_name, "rb" );

	if ( !file ) {
		printd ( LogDebug, "Cannot open [%s] - %s\n", file_name, strerror ( errno ) );
		return ( true );
	}

	printd ( LogDebug, "Play_File name %s\n", file_name );
	file_offset = 0;
	ok = 0;

	switch ( read_id() ) {
	case MAKE_ID ( 'M', 'T', 'h', 'd' ) :
		ok = read_smf();
		break;

	case MAKE_ID ( 'R', 'I', 'F', 'F' ) :
		ok = read_riff();
		break;

	default:
		errormsg ( "%s is not a Standard MIDI File\n", file_name );
		return ( true );
		break;
	}

	if ( file != stdin )
		fclose ( file );

	printd ( LogDebug, "About to Call Play Midi\n" );

	if ( ok )
		play_midi();

	printd ( LogDebug, "After call to Play Midi\n" );

	cleanup_file_data();
	return ( false );
}

/*--------------------------------------------
* Function:		alsa_loop_init
*
* Description:		Setup the Alsa input port.
*
*---------------------------------------------*/
int alsa_loop_init ( void )
{
	int ret;
	snd_seq_port_info_t * port_info = NULL;
	pthread_attr_t tattr;
	struct sched_param param;

	printd ( LogDebug, "alsa_loop_init\n" );

	/* initialized with default attributes */
	ret = pthread_attr_init ( &tattr );
	/* safe to get existing scheduling param */
	ret = pthread_attr_getschedparam ( &tattr, &param );

	/* set the priority; others are unchanged */

	param.sched_priority = 99;

	/* setting the new scheduling param */
	ret = pthread_attr_setschedparam ( &tattr, &param );

// testing

	/* Start midi thread */
	ret = pthread_create ( &g_alsa_midi_Loop_tid, &tattr, alsa_Loop_thread, NULL );

	return ( true );
}

void *alsa_Loop_thread ( void * context_ptr )
{

	printd ( LogDebug, "alsa_Loop_thread\n" );

	while ( 1 ) {

		if ( MidiLooping ) {
			printd ( LogDebug, "alsa_Loop_thread before play\n" );

			if ( play_file() ) {
				MidiLooping = false;
				printd ( LogDebug, "play error\n" );
			}

			printd ( LogDebug, "alsa_Loop_thread after play\n" );
		}
	}
}

/* prints an error message to stderr */
static void errormsg ( const char *msg, ... )
{
	va_list ap;

	va_start ( ap, msg );
	vfprintf ( stderr, msg, ap );
	va_end ( ap );
	fputc ( '\n', stderr );
}

#if 0
/* prints an error message to stderr, and dies */
static void printd ( LogDebug, const char *msg, ... )
{
	va_list ap;

	va_start ( ap, msg );
	vfprintd ( LogDebug, stderr, msg, ap );
	va_end ( ap );
	fputc ( '\n', stderr );
	exit ( EXIT_FAILURE );
}

/* memory allocation error handling */
static void check_mem ( void *p )
{
	if ( !p )
		printd ( LogDebug, "Out of memory" );
}

/* error handling for ALSA functions */
static void check_snd ( const char *operation, int err )
{
	if ( err < 0 )
		printd ( LogDebug, "Cannot %s - %s", operation, snd_strerror ( err ) );
}
#endif

