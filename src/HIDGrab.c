/*---------------------------------------------------------------------
|
| File:   HIDGrab
|
| Contains: Routines to get a HID device detach
    it from the kernel so it can be used to control
    Midi and OSC .
|
|
| Written By:   Elias Keshishoglou
|
| Copyright �:  2019 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#define HIDGrab_c

/*
 * Place Includes Here
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include "LiveMusicApp.h"

/*
 * Place defines and Typedefs here
 */


//#define VENDORID  0x05d5
//#define PRODUCTID 0x6782

// Footswitch
#define VENDORID  0x0c45
#define PRODUCTID 0x7403
#define SCN_BCD_SZ 1000
/*
 * Place Local prototypes here
 */

int HIDPoll(void);
int CloseHIDGrab(void);
int setNonblocking(int fd);
int InitHIDGrab(void);

/*
 * Place Static variables here
 */

int theHID_fd = -1;
gint theOldPane;

/*--------------------------------------------------------------------
 * Function:            HIDPoll
 *
 * Description:
 *
 *---------------------------------------------------------------------*/
int HIDPoll() {
  struct input_event ev[64];
  int size = sizeof(struct input_event);
  int rd;
  int value;
  int i;
  int result = 0;
  int KeyCode;

#if 0
  if ((rd = read(theHID_fd, ev, size * 64)) < size) {
    break;
  }
#endif

  rd = read(theHID_fd, ev, size * 64);
//  printf("Read %d %x\n", rd, theHID_fd);
  if (rd != -1) {
    for (i = 0; i < (rd / sizeof(struct input_event)); i++) {
      if (ev[i].code != 69)
        switch (ev[i].type) {
        case EV_SYN:
//        printf("---------------------------------------\n");
          break;

        case EV_KEY:
          KeyCode = ev[i].code;
          switch (ev[i].value) {
          case 0:
            printd(LogDebug, "Key Code: %d released\n", ev[i].code);
            if (KeyCode == 48) {
//              theOldPane = gtk_notebook_get_current_page(GTK_NOTEBOOK(NoteBookPaneS));
//              gtk_notebook_set_current_page(GTK_NOTEBOOK(NoteBookPaneS), 1);
              SwitchToTab(PreviousTab);

              WaitingforMidi = 0;
              WaitingforMidiHold = 0;
            }

            break;
          case 1:
            printd(LogDebug, "Key Code: %d pressed\n", ev[i].code);

            if (KeyCode == 48) {
              SwitchToTab(0);

//              gtk_notebook_set_current_page(GTK_NOTEBOOK(NoteBookPaneS), theOldPane);
              WaitingforMidi = 1;
              WaitingforMidiHold = 1;
            }

            break;

          case 2:
//          printd(LogDebug,"Key pressed continue\n", ev[i].code);
            break;

          default:
            printd(LogDebug, "UnKnown value: type %d, code %d, value %d\n",
                   ev[i].type,
                   ev[i].code,
                   ev[i].value);
            break;
          }

          break;
        }
    }
  }
}

/*--------------------------------------------------------------------
 * Function:            CloseHIDGrab
 *
 * Description:
 *
 *---------------------------------------------------------------------*/
int CloseHIDGrab(void) {

  ioctl(theHID_fd, EVIOCGRAB, 1);
  close(theHID_fd);
  return 0;
}

/*--------------------------------------------------------------------
 * Function:            setNonblocking
 *
 * Description:
 *
 *---------------------------------------------------------------------*/
int setNonblocking(int fd) {
  int flags;

  /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
  /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
  if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
    flags = 0;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
  /* Otherwise, use the old way of doing it */
  flags = 1;
  return ioctl(fd, FIOBIO, &flags);
#endif
}

/*--------------------------------------------------------------------
 * Function:            InitHIDGrab
 *
 * Description:
 *
 *---------------------------------------------------------------------*/
int InitHIDGrab() {
  int count = 0;
  struct dirent **files = NULL;
  struct input_id id;
  int result = 0;
  char name[256] = "Unknown";
  char path[64];
  int scan_fd = -1;

  count = scandir("/dev/input", &files, 0, 0) - 1;
  while ( count >= 0 ) {
    if ( scan_fd == -1 && strncmp(files[count]->d_name, "event", 5) == 0 ) {
      sprintf(path, "/dev/input/%s", files[count]->d_name);
      printd(LogDebug, "/dev/input/%s ", files[count]->d_name);
      scan_fd = open(path, O_RDONLY);
      if ( scan_fd >= 0 ) {
        if ( ioctl(scan_fd, EVIOCGID, (void *)&id) < 0 )
          perror("ioctl EVIOCGID");
        else {
          printd(LogDebug, "V=%x D=%x\n", id.vendor, id.product);
          if ( id.vendor == VENDORID && id.product == PRODUCTID )
            printd(LogDebug, "scanner attached to %s\n", path);
          else {
            close(scan_fd);
            scan_fd = -1;
          }
        }
      } else {
        fprintf(stderr, "Error opening %s", path);
        perror("");
      }
    }
    free(files[count--]);
  }
  free(files);

  if ( scan_fd >= 0 )
    ioctl(scan_fd, EVIOCGRAB);
  else {
    printd(LogDebug, "scanner not found or couldn't be opened\n");
    return 0;
  }

  theHID_fd = open(path, O_RDONLY);
  if (theHID_fd == -1) {
    printd(LogDebug, "Failed to open event device.\n");
  }

  result = ioctl(theHID_fd, EVIOCGNAME(sizeof(name)), name);
  printd(LogDebug, "Reading From : %s (%s) %d\n", path, name, theHID_fd);
  setNonblocking(theHID_fd);
  result = ioctl(theHID_fd, EVIOCGRAB, 1);
  printd(LogDebug, "Getting exclusive access: %d\n", result);
}

//  NoteBookPaneS = (GtkWidget *)GTK_WIDGET(gtk_builder_get_object(gxml, "MainTab"));

#if 0

char keycodelist(int scancode) {
  char ret = '-';
  return (unsigned char)scancode;
  switch (scancode) {
  case 0x02: ret = '1'; break;
  case 0x03: ret = '2'; break;
  case 0x04: ret = '3'; break;
  case 0x05: ret = '4'; break;
  case 0x06: ret = '5'; break;
  case 0x07: ret = '6'; break;
  case 0x08: ret = '7'; break;
  case 0x09: ret = '8'; break;
  case 0x0a: ret = '9'; break;
  case 0x0b: ret = '0'; break;
  case 0x0c: ret = '-'; break;

  case 0x10: ret = 'q'; break;
  case 0x11: ret = 'w'; break;
  case 0x12: ret = 'e'; break;
  case 0x13: ret = 'r'; break;
  case 0x14: ret = 't'; break;
  case 0x15: ret = 'y'; break;
  case 0x16: ret = 'u'; break;
  case 0x17: ret = 'i'; break;
  case 0x18: ret = 'o'; break;
  case 0x19: ret = 'p'; break;

  case 0x1e: ret = 'a'; break;
  case 0x1f: ret = 's'; break;
  case 0x20: ret = 'd'; break;
  case 0x21: ret = 'f'; break;
  case 0x22: ret = 'g'; break;
  case 0x23: ret = 'h'; break;
  case 0x24: ret = 'j'; break;
  case 0x25: ret = 'k'; break;
  case 0x26: ret = 'l'; break;

  case 0x2c: ret = 'z'; break;
  case 0x2d: ret = 'x'; break;
  case 0x2e: ret = 'c'; break;
  case 0x2f: ret = 'v'; break;
  case 0x30: ret = 'b'; break;
  case 0x31: ret = 'n'; break;
  case 0x32: ret = 'm'; break;
  default: break;
  }
  return ret;
}

//read a barcode from the scanner.
//reads as long as *loopcond!=0 (if loopcond is NULL then read
//forever). If termination condition is met, returns NULL.
//read all characters from barcode untill we read 0x28 (carriage
//return).
char* readScanner(int *loopcond) {
  static char barcode[SCN_BCD_SZ];
  char code[SCN_BCD_SZ];
  int i = 0;
  struct input_event ev;

  while ( loopcond == NULL ? 1 : *loopcond ) {
    read(scan_fd, &ev, sizeof(struct input_event));
    if ( ev.type == 1 && ev.value == 1 ) {
      if ( ev.code == 28 ) { //carriage return
        code[i] = 0;
        strcpy(barcode, code);
        return barcode;
      } else {
        if ( ev.code != 0 ) {
          code[i++] = keycodelist(ev.code);
          if ( i == SCN_BCD_SZ - 1 ) { printf("Barcode buffer full\n"); return NULL;}
        }
      }
    }
  }
  return NULL;
}
#endif
