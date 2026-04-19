/*---------------------------------------------------------------------
|
|	File: 	sfizz_pty_launcher
|
|	Contains: 	
|
|
|	Written By: 	Elias Keshishoglou on Sun Apr 19 07:39:21 EEST 2026
|
|	Copyright : 	2026 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#define sfizz_pty_launcher_c

/*
 * Place Includes Here
 */

#define _XOPEN_SOURCE 600
#include <fcntl.h>
#include <pty.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


/*
 * Place defines and Typedefs here
 */


/*
 * Place Local prototypes here
 */


/*
 * Place Static variables here
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <sfz-file> <pw-jack-path>\n", argv[0]);
        return 1;
    }

    const char *sfz = argv[1];
    const char *sfizz = argv[2];

    // small delay to let pipewire/jack graph settle
    usleep(500000); // 0.5s

    char *args[] = {
        (char *)"pw-jack",
        (char *)sfizz,
        (char *)sfz,
        NULL
    };

    execvp("pw-jack", args);

    perror("execvp failed");
    return 1;
}

