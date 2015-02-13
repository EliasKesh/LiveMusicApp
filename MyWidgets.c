/*---------------------------------------------------------------------
|
|	File: 	MyWidgets
|
|	Contains: 	
|
|
|	Written By: 	Elias Keshishoglou on Tue Feb 10 15:53:13 PST 2015
|
|	Copyright �: 	2015 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#define MyWidgets_c

/*
 * Place Includes Here
 */

#include "MyWidgets.h"



/*
 * Place defines and Typedefs here
 */


/*
 * Place Local prototypes here
 */


/*
 * Place Static variables here
 */

/*--------------------------------------------------------------------
 * Function:            MyImageButtonInit
 *
 * Description:         Create a new button from an eventbox..
 *---------------------------------------------------------------------*/
int	MyImageButtonInit( theImageButtons *theButton, GtkWidget *EventBox,GdkPixbuf *On, GdkPixbuf *Off) {
	GtkWidget *overlay;

    /*
       * A GtkImage doesn't have a window, so we need to put it inside
       * a GtkEventBox so we can capture events.
       */
   GtkWidget *image = gtk_image_new ();

        overlay = gtk_overlay_new ();

	  gtk_container_add(GTK_CONTAINER(overlay), image);
	  gtk_container_add(GTK_CONTAINER(EventBox), overlay);
		theButton->Label = gtk_label_new("Hello There");;
		MyImageButtonSetText(theButton->Label, "Hello There");
//	          gtk_label_set_markup(GTK_LABEL(theButton->Label),
 //             "<span font=\"16\" color=\"white\"><b>Hello There:</b></span>");

   gtk_overlay_add_overlay (GTK_OVERLAY (overlay), theButton->Label);
   	   theButton->EventBox = EventBox;
	theButton->Image = image;
	theButton->Overlay = overlay;
	theButton->ButtonUpImage = On;
	theButton->ButtonDownImage = Off;
	gtk_image_set_from_pixbuf (GTK_IMAGE(theButton->Image), theButton->ButtonUpImage);

#if 0
	g_signal_connect(G_OBJECT(EventBox),
		"button-release-event",
		G_CALLBACK(normal_release_handler),
		&theButton);
#endif
}

/*--------------------------------------------------------------------
 * Function:            MyImageButtonSetText
 *
 * Description:         Change or Set the text of a button..
 *---------------------------------------------------------------------*/
int	MyImageButtonSetText( theImageButtons *theButton, char *String) {
char		FormatString[100];

// https://developer.gnome.org/pango/stable/PangoMarkupFormat.html
//sprintf(FormatString, "<span font=\"10\" color=\"white\"><b>%s</b></span>", String);
//sprintf(FormatString, "<span size=\"12800\" color=\"white\"><b>%s</b></span>", String);
sprintf(FormatString, "<span font=\"10\" color=\"white\"><b>%s</b></span>", String);
	gtk_label_set_markup(GTK_LABEL(theButton->Label),FormatString);
}

gboolean normal_release_handler(GtkWidget *widget,
	GdkEvent *event,
	gpointer user_data)
{
	theImageButtons *theButton;
	theButton = (theImageButtons *) user_data;
	//	PatchIndex = LayoutSwitchPatch(user_data, true);

	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
		theButton->ButtonUpImage);
	return TRUE; /* stop event propagation */
}
