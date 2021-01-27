/*------------------------------------------------
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
|------------------------------------------------*/

#ifndef MyWidgets_h
#define MyWidgets_h
/*
 * Place Includes here.
 */
#include <gtk/gtk.h>

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
	char			State;
} theImageButtons;

#include "LiveMusicApp.h"

int	MyImageButtonInit( theImageButtons *theButton, GtkWidget *EventBox, GdkPixbuf *On, GdkPixbuf *Off);
int	MyImageButtonSetText( theImageButtons *theButton, char *String);
gboolean normal_release_handler(GtkWidget *widget,	GdkEvent *event, gpointer user_data);

/*
 * Place Static variables here.
 */


#endif
