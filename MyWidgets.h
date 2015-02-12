/*---------------------------------------------------------------------
|
|	File: 	MyWidgets
|
|	Contains: 	
|
|
|	Written By: 	Elias Keshishoglou on Tue Feb 10 15:53:15 PST 2015
|
|	Copyright �: 	2015 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#ifndef MyWidgets_h
#define MyWidgets_h
/*
 * Place Includes here.
 */
#include "GTKMidiUI.h"


/*
 * Place Global prototypes here.
 */
typedef struct {
	GtkWidget 	*Label;
	GtkWidget 	*Overlay;
	GtkWidget 	*EventBox;
	GtkWidget 	*Image;
	GdkPixbuf 	*ButtonUpImage;
	GdkPixbuf 	*ButtonDownImage;

} theImageButtons;

int	MyImageButtonInit( theImageButtons *theButton, GtkWidget *EventBox,GdkPixbuf *On, GdkPixbuf *Off);
int	MyImageButtonSetText( theImageButtons *theButton, char *String);

/*
 * Place Static variables here.
 */


#endif
