/*---------------------------------------------------------------------
|
|	File: 	Connections
|
|	Contains:
|
|
|	Written By: 	Elias Keshishoglou on Sat May 11 20:18:30 PDT 2013
|
|	Copyright ©: 	2013 Elias Keshishoglou all rights reserved.
|
|	This program is free software; you can redistribute it and/or
|	modify it under the terms of the GNU General Public License
|	as published by the Free Software Foundation; either version 2
|	of the License, or (at your option) any later version.
|
|	This program is distributed in the hope that it will be useful,
|	but WITHOUT ANY WARRANTY; without even the implied warranty of
|	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|	GNU General Public License for more details.
|
|	You should have received a copy of the GNU General Public License
|	along with this program; if not, write to the Free Software
|	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
|
|
|---------------------------------------------------------------------*/

#define Connections_c

/*
 * Place Includes Here
 */

#include "Connections.h"
#include "LiveMusicApp.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <locale.h>
#include <sys/ioctl.h>
#include <alsa/asoundlib.h>
#include "aconfig.h"
// #include "gettext.h"

int	 gDevices;
int gPorts;
PortsInfo *thePorts;
/*
 * Place defines and Typedefs here
 */

typedef void (*action_func_t)(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
                              snd_seq_port_info_t *pinfo, int count);

/*
 * Place Local prototypes here
 */

static void print_port_and_subs(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
                                snd_seq_port_info_t *pinfo, int count);
static void do_search_port(snd_seq_t *seq, int perm, action_func_t do_action);

/*
 * Place Static variables here
 */

#define LIST_INPUT	1
#define LIST_OUTPUT	2

int InitConnections(void) {
	snd_seq_t *seq;

	if (snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
		fprintf(stderr, ("can't open sequencer\n"));
		return 1;
	}

	thePorts = &theInPorts;
	gDevices = 0;

	printd(LogDebug, "Search Inputs\n");
	do_search_port(seq, LIST_INPUT, print_port_and_subs);

	thePorts = &theOutPorts;
	gDevices = 0;

	printd(LogDebug, "Search Outputs\n");
	do_search_port(seq, LIST_OUTPUT, print_port_and_subs);
	snd_seq_close(seq);
	return (0);
}

#if 0
static void error_handler(const char *file, int line, const char *function,
                          int err, const char *fmt, ...) {
	va_list arg;

	if (err == ENOENT) /* Ignore those misleading "warnings" */
		return;
	va_start(arg, fmt);
	fprintf(stderr, "ALSA lib %s:%i:(%s) ", file, line, function);
	vfprintf(stderr, fmt, arg);
	if (err)
		fprintf(stderr, ": %s", snd_strerror(err));
	putc('\n', stderr);
	va_end(arg);
}

static void usage(void) {

}
#endif

/*
 * check permission (capability) of specified port
 */

#define LIST_INPUT	1
#define LIST_OUTPUT	2

#define perm_ok(pinfo,bits) ((snd_seq_port_info_get_capability(pinfo) & (bits)) == (bits))

static int check_permission(snd_seq_port_info_t *pinfo, int perm) {
	if (perm) {
		if (perm & LIST_INPUT) {
			if (perm_ok(pinfo,
			            SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ))
				goto __ok;
		}
		if (perm & LIST_OUTPUT) {
			if (perm_ok(pinfo,
			            SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE))
				goto __ok;
		}
		return 0;
	}
__ok:
	if (snd_seq_port_info_get_capability(
	            pinfo) & SND_SEQ_PORT_CAP_NO_EXPORT)
		return 0;
	return 1;
}

/*
 * list subscribers of specified type
 */
static void list_each_subs(snd_seq_t *seq, snd_seq_query_subscribe_t *subs,
                           int type, const char *msg) {
	int count = 0;
	snd_seq_query_subscribe_set_type(subs, type);
	snd_seq_query_subscribe_set_index(subs, 0);
	while (snd_seq_query_port_subscribers(seq, subs) >= 0) {
		const snd_seq_addr_t *addr;
		if (count++ == 0)
			printd(LogDebug, "\t%s: ", msg);
		else
			printd(LogDebug, ", ");
		addr = snd_seq_query_subscribe_get_addr(subs);
		printd(LogDebug, "%d:%d", addr->client, addr->port);
		if (snd_seq_query_subscribe_get_exclusive(subs))
			printd(LogDebug, "[ex]");
		if (snd_seq_query_subscribe_get_time_update(subs))
			printd(LogDebug, "[%s:%d]",
			       (snd_seq_query_subscribe_get_time_real(subs) ?
			        "real" :
			        "tick"),
			       snd_seq_query_subscribe_get_queue(subs));
		snd_seq_query_subscribe_set_index(subs,
		                                  snd_seq_query_subscribe_get_index(subs) + 1);
	}
	if (count > 0)
		printd(LogDebug, "\n");
}

/*
 * list subscribers
 */
static void list_subscribers(snd_seq_t *seq, const snd_seq_addr_t *addr) {
	snd_seq_query_subscribe_t *subs;
	snd_seq_query_subscribe_alloca(&subs);
	snd_seq_query_subscribe_set_root(subs, addr);
	list_each_subs(seq, subs, SND_SEQ_QUERY_SUBS_READ, ("Connecting To"));
	list_each_subs(seq, subs, SND_SEQ_QUERY_SUBS_WRITE, ("Connected From"));
}

/*
 * search all ports
 */

static void do_search_port(snd_seq_t *seq, int perm, action_func_t do_action) {
	snd_seq_client_info_t *cinfo;
	snd_seq_port_info_t *pinfo;
	int count;

	snd_seq_client_info_alloca(&cinfo);
	snd_seq_port_info_alloca(&pinfo);
	snd_seq_client_info_set_client(cinfo, -1);
	while (snd_seq_query_next_client(seq, cinfo) >= 0) {
		/* reset query info */
		snd_seq_port_info_set_client(pinfo,
		                             snd_seq_client_info_get_client(cinfo));
		snd_seq_port_info_set_port(pinfo, -1);
		count = 0;
		while (snd_seq_query_next_port(seq, pinfo) >= 0) {
			if (check_permission(pinfo, perm)) {
				do_action(seq, cinfo, pinfo, count);
				count++;
			}
		}
	}
}

static void print_port(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
                       snd_seq_port_info_t *pinfo, int count) {

	if (!count) {
		gDevices = thePorts->NumDevices;
		thePorts->NumDevices++;
		memcpy(thePorts->Devices[gDevices].Name,
		       snd_seq_client_info_get_name(cinfo), 100);
		thePorts->Devices[gDevices].ID = snd_seq_client_info_get_client(cinfo);
		thePorts->Devices[gDevices].NumPorts = 0;
#if 0
		printf(("client %d: '%s' [type=%s]\n"),
		       snd_seq_client_info_get_client(cinfo),
		       snd_seq_client_info_get_name(cinfo),
		       (snd_seq_client_info_get_type(cinfo) == SND_SEQ_USER_CLIENT ?
		        ("user") : ("kernel")));
#endif
	}
	gPorts = thePorts->Devices[gDevices].NumPorts;
	(thePorts->Devices[gDevices].NumPorts)++;
	memcpy(thePorts->Devices[gDevices].Ports[gPorts].Name,
	       snd_seq_port_info_get_name(pinfo), 100);
	thePorts->Devices[gDevices].Ports[gPorts].ID = snd_seq_port_info_get_port(
	            pinfo);
#if 0
	printd(LogDebug, "  %3d '%-16s'\n",
	       snd_seq_port_info_get_port(pinfo),
	       snd_seq_port_info_get_name(pinfo));
	printf("*** %d %d ***\n", gDevices, gPorts);
#endif

}

static void print_port_and_subs(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
                                snd_seq_port_info_t *pinfo, int count) {
	print_port(seq, cinfo, pinfo, count);
	list_subscribers(seq, snd_seq_port_info_get_addr(pinfo));
}

/*
 * remove all (exported) connections
 */
static void remove_connection(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
                              snd_seq_port_info_t *pinfo, int count) {
	snd_seq_query_subscribe_t *query;
	snd_seq_port_info_t *port;
	snd_seq_port_subscribe_t *subs;

	snd_seq_query_subscribe_alloca(&query);
	snd_seq_query_subscribe_set_root(query, snd_seq_port_info_get_addr(pinfo));
	snd_seq_query_subscribe_set_type(query, SND_SEQ_QUERY_SUBS_READ);
	snd_seq_query_subscribe_set_index(query, 0);

	snd_seq_port_info_alloca(&port);
	snd_seq_port_subscribe_alloca(&subs);

	while (snd_seq_query_port_subscribers(seq, query) >= 0) {
		const snd_seq_addr_t *sender = snd_seq_query_subscribe_get_root(query);
		const snd_seq_addr_t *dest = snd_seq_query_subscribe_get_addr(query);

		if (snd_seq_get_any_port_info(seq, dest->client, dest->port, port) < 0
		        || !(snd_seq_port_info_get_capability(port)
		             & SND_SEQ_PORT_CAP_SUBS_WRITE)
		        || (snd_seq_port_info_get_capability(port)
		            & SND_SEQ_PORT_CAP_NO_EXPORT)) {
			snd_seq_query_subscribe_set_index(query,
			                                  snd_seq_query_subscribe_get_index(query) + 1);
			continue;
		}
		snd_seq_port_subscribe_set_queue(subs,
		                                 snd_seq_query_subscribe_get_queue(query));
		snd_seq_port_subscribe_set_sender(subs, sender);
		snd_seq_port_subscribe_set_dest(subs, dest);
		if (snd_seq_unsubscribe_port(seq, subs) < 0) {
			snd_seq_query_subscribe_set_index(query,
			                                  snd_seq_query_subscribe_get_index(query) + 1);
		}
	}
}

static void remove_all_connections(snd_seq_t *seq) {
	do_search_port(seq, 0, remove_connection);
}

/*
 * main..
 */
#if 0

enum {
	SUBSCRIBE, UNSUBSCRIBE, LIST, REMOVE_ALL
};

static const struct option long_option[] = {
	{ "disconnect", 0, NULL, 'd' },
	{
		"input", 0, NULL, 'i'
	},
	{ "output", 0, NULL, 'o' },
	{
		"real", 1, NULL,
		'r'
	},
	{ "tick", 1, NULL, 't' },
	{ "exclusive", 0, NULL, 'e' },
	{
		"list", 0, NULL, 'l'
	},
	{ "removeall", 0, NULL, 'x' },
	{
		NULL, 0, NULL,
		0
	},
};

int Temp(int argc, char **argv) {
	int c;
	snd_seq_t *seq;
	int queue = 0, convert_time = 0, convert_real = 0, exclusive = 0;
	int command = SUBSCRIBE;
	int list_perm = 0;
	int client;
	int list_subs = 0;
	snd_seq_port_subscribe_t *subs;
	snd_seq_addr_t sender, dest;

#ifdef ENABLE_NLS
	setlocale(LC_ALL, "");
	textdomain(PACKAGE);
#endif

	while ((c = getopt_long(argc, argv, "dior:t:elx", long_option, NULL)) != -1) {
		switch (c) {
		case 'd':
			command = UNSUBSCRIBE;
			break;
		case if (snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
					fprintf(stderr, ("can't open sequencer\n"));
					return 1;
				}
			'i':
			command = LIST;
			list_perm |= LIST_INPUT;
			do_search_port(seq, list_perm,
			               list_subs ? print_port_and_subs : print_port);
			snd_seq_close(seq);
			break;
		case 'o':
			command = LIST;
			list_perm |= LIST_OUTPUT;
			break;
		case 'e':
			exclusive = 1;
			break;
		case 'r':
			queue = atoi(optarg);
			convert_time = 1;
			convert_real = 1;
			break;
		case 't':
			queue = atoi(optarg);
			convert_time = 1;
			convert_real = 0;
			break;
		case 'l':
			list_subs = 1;
			break;
		case 'x':
			command = REMOVE_ALL;
			break;
		default:
			usage();
			exit(1);
		}
	}

	if (snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
		fprintf(stderr, ("can't open sequencer\n"));
		return 1;
	}

	snd_lib_error_set_handler(error_handler);

	switch (command) {
	case LIST:
		do_search_port(seq, list_perm,
		               list_subs ? print_port_and_subs : print_port);
		snd_seq_close(seq);
		return 0;
	case REMOVE_ALL:
		remove_all_connections(seq);
		snd_seq_close(seq);
		return 0;
	}

	/* connection or disconnection */

	if (optind + 2 > argc) {
		snd_seq_close(seq);
		usage();
		exit(1);
	}

	if ((client = snd_seq_client_id(seq)) < 0) {
		snd_seq_close(seq);
		fprintf(stderr, ("can't get client id\n"));
		return 1;
	}

	/* set client info */
	if (snd_seq_set_client_name(seq, "ALSA Connector") < 0) {
		snd_seq_close(seq);
		fprintf(stderr, ("can't set client info\n"));
		return 1;
	}

	/* set subscription */
	if (snd_seq_parse_address(seq, &sender, argv[optind]) < 0) {
		snd_seq_close(seq);
		fprintf(stderr, ("invalid sender address %s\n"), argv[optind]);
		return 1;
	}
	if (snd_seq_parse_address(seq, &dest, argv[optind + 1]) < 0) {
		snd_seq_close(seq);
		fprintf(stderr, ("invalid destination address %s\n"), argv[optind + 1]);
		return 1;
	}
	snd_seq_port_subscribe_alloca(&subs);
	snd_seq_port_subscribe_set_sender(subs, &sender);
	snd_seq_port_subscribe_set_dest(subs, &dest);
	snd_seq_port_subscribe_set_queue(subs, queue);
	snd_seq_port_subscribe_set_exclusive(subs, exclusive);
	snd_seq_port_subscribe_set_time_update(subs, convert_time);
	snd_seq_port_subscribe_set_time_real(subs, convert_real);

	if (command == UNSUBSCRIBE) {
		if (snd_seq_get_port_subscription(seq, subs) < 0) {
			snd_seq_close(seq);
			return 1;
		}
		if (snd_seq_unsubscribe_port(seq, subs) < 0) {
			snd_seq_close(seq);
			return 1;
		}
	} else {
		if (snd_seq_get_port_subscription(seq, subs) == 0) {
			snd_seq_close(seq);
			return 1;
		}
		if (snd_seq_subscribe_port(seq, subs) < 0) {
			snd_seq_close(seq);
			return 1;
		}
	}

	snd_seq_close(seq);

	return 0;
}

#endif
