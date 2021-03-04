/*---------------------------------------------
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
|	Version:        1.3.4
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
|---------------------------------------------*/

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

/*--------------------------------------------
 * Function:            MyImageButtonInit
 *
 * Description:         Create a new button from an eventbox..
 *---------------------------------------------*/
int	MyImageButtonInit( theImageButtons *theButton, GtkWidget *EventBox, GdkPixbuf *On, GdkPixbuf *Off) {
    GtkWidget *overlay;

    /*
       * A GtkImage doesn't have a window, so we need to put it inside
       * a GtkEventBox so we can capture events.
       */
    GtkWidget *image = gtk_image_new ();

    overlay = gtk_overlay_new ();
    gtk_container_add(GTK_CONTAINER(overlay), image);
    gtk_container_add(GTK_CONTAINER(EventBox), overlay);
    theButton->Label = gtk_label_new("!!PROB!!");

    MyImageButtonSetText(theButton, "!!PROB!!");

//	          gtk_label_set_markup(GTK_LABEL(theButton->Label),
//             "<span font=\"16\" color=\"white\"><b>Hello There:</b></span>");

    gtk_overlay_add_overlay (GTK_OVERLAY (overlay), GTK_WIDGET(theButton->Label));
    theButton->EventBox = EventBox;
    theButton->Image = image;
    theButton->Overlay = overlay;
    theButton->ButtonUpImage = Off;
    theButton->ButtonDownImage = On;
    gtk_image_set_from_pixbuf (GTK_IMAGE(theButton->Image), theButton->ButtonUpImage);

#if 0
    g_signal_connect(G_OBJECT(EventBox),
                     "button-release-event",
                     G_CALLBACK(normal_release_handler),
                     &theButton);
#endif
    return (0);
}

/*--------------------------------------------
 * Function:            MyImageButtonSetText
 *
 * Description:         Change or Set the text of a button..
 *---------------------------------------------*/
int	MyImageButtonSetText( theImageButtons *theButton, char *String) {
    char		FormatString[200];
// https://developer.gnome.org/pango/stable/PangoMarkupFormat.html

//	sprintf(FormatString, "<span font=\"10\" color='#%lx'><b>%s</b></span>",
//		gMyInfo.ButtonTextColor, String);
    sprintf(FormatString, "<span  face=\"monospace\" font=\"10\" color='#%lx'><b>%s</b></span>",
            gMyInfo.ButtonTextColor, String);

    printd(LogDebug, "MyImageButtonSetText %x %s \n", theButton, String);
//	printf("MyImageButtonSetText %x %s \n", theButton, String);

    gtk_label_set_markup((theButton->Label), (gchar *)FormatString);

//	g_idle_add(GTKIdel_cb, theMainWindow);
    return (0);
}

gboolean normal_release_handler(GtkWidget *widget,
                                GdkEvent *event,
                                gpointer user_data) {
    theImageButtons *theButton;
    theButton = (theImageButtons *) user_data;
    //	PatchIndex = LayoutSwitchPatch(user_data, true);

    gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
                              theButton->ButtonUpImage);

    return TRUE; /* stop event propagation */
}
