/*---------------------------------------------
|
|   File:   MyWidgets
|
|   Contains:
|
|
|   Written By:     Elias Keshishoglou on Tue Feb 10 15:53:13 PST 2015
|
|   Copyright �:  2015 Elias Keshishoglou all rights reserved.
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
int MyImageButtonInit(theImageButtons *theButton, GtkWidget *EventBox, GdkPixbuf *On, GdkPixbuf *Off) {
    GtkWidget *overlay;

    /*
     * A GtkImage doesn't have a window, so we need to put it inside
     * a GtkEventBox so we can capture events.
     */
    GtkWidget *image = gtk_image_new();

    overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(overlay), image);
    gtk_container_add(GTK_CONTAINER(EventBox), overlay);
    theButton->Label = gtk_label_new("!!PROB!!");

    MyImageButtonSetText(theButton, "!!PROB!!");
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), GTK_WIDGET(theButton->Label));
    theButton->EventBox = EventBox;
    theButton->Image = image;
    theButton->Overlay = overlay;
    theButton->ButtonUpImage = Off;
    theButton->ButtonDownImage = On;
    gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image), theButton->ButtonUpImage);

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
int MyImageButtonSetText(theImageButtons *theButton, char *String) {
    char        FormatString[200];
    char        FontSize = 12;
    // https://developer.gnome.org/pango/stable/PangoMarkupFormat.html

    //  sprintf(FormatString, "<span font=\"10\" color='#%lx'><b>%s</b></span>",
    //      gMyInfo.ButtonTextColor, String);
    //    sprintf(FormatString, "<span  face=\"monospace\" font=\"12\" color='#%lx'><b>%s</b></span>",
    //   sprintf(FormatString, "<span  face=\"Ubuntu Mono\" font=\"14\" color='#%lx'><b>%s</b></span>",
    //    sprintf(FormatString, "<span  face=\"Noto Sans Mono\" font=\"14\" color='#%lx'><b>%s</b></span>",
    //    sprintf(FormatString, "<span  face=\"Sans Sarif\" font=\"12\" color='#%lx'><b>%s</b></span>",
    //
    if (ScreenSize == 0) {
        FontSize = 9;
    }

    sprintf(FormatString, "<span  face=\"Bitstream Vera Sans Mono\" font=\"%d\" color='#%lx'><b>%s</b></span>",
            FontSize,
            gMyInfo.ButtonTextColor, String);

    //printd(LogDebug, "MyImageButtonSetText %x %s \n", theButton, String);
    //  printf("MyImageButtonSetText %x %s \n", theButton, String);

    gtk_label_set_markup((theButton->Label), (gchar *)FormatString);

    //  g_idle_add(GTKIdel_cb, theMainWindow);
    return (0);
}

/*--------------------------------------------
 * Function:            MyImageButtonSetText2
 *
 * Description:         Change or Set the text of a button..
 *---------------------------------------------*/
int MyImageButtonSetText2(theImageButtons *theButton, int Number, char *String) {
    char        FormatString[200];
    int         StrLen;
    char        FontSize = 12;
    char        StringOff = 14;

    if (ScreenSize == 0) {
        FontSize = 9;
        StringOff = 9;
    }

    StrLen = strlen(String);
    sprintf(FormatString, "<span face=\"Bitstream Vera Sans Mono\" font=\"%d\" color='#%lx'><b>      %03d      </b></span>\n<span  face=\"Bitstream Vera Sans Mono\" font=\"12\" color='#%lx'><b>%*s</b></span>",
            FontSize,
            0xc07000,
            Number,
            gMyInfo.ButtonTextColor,
            (StringOff + StrLen) / 2,
            String);

    gtk_label_set_markup((theButton->Label), (gchar *)FormatString);

    //  g_idle_add(GTKIdel_cb, theMainWindow);
    return (0);
}

/*--------------------------------------------
 * Function:            normal_release_handler
 *
 * Description:         Change or Set the text of a button..
 *---------------------------------------------*/
gboolean normal_release_handler(GtkWidget *widget,
                                GdkEvent *event,
                                gpointer user_data) {
    theImageButtons *theButton;
    theButton = (theImageButtons *) user_data;
    //  PatchIndex = LayoutSwitchPatch(user_data, true);

    gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
                              theButton->ButtonUpImage);

    return TRUE; /* stop event propagation */
}
