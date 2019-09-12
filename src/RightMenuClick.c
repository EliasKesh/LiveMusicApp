/*---------------------------------------------------------------------
|
|	File: 	RightMenuClick
|
|	Contains:
|
|
|	Written By: 	Elias Keshishoglou on Thu Sep 25 13:12:39 PDT 2014
|
|	Copyright �: 	2014 Elias Keshishoglou all rights reserved.
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

#define RightMenuClick_c

/*
 * Place Includes Here
 */
#include <gtk/gtk.h>
#include "LiveMusicApp.h"

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
 * Function:		view_popup_menu
 * Description:
 *
 *---------------------------------------------------------------------*/
void view_popup_menu(GtkWidget *treeview, GdkEventButton *event,
                     gpointer userdata) {
	GtkWidget *menu, *menuitem;

	menu = gtk_menu_new();

	menuitem = gtk_menu_item_new_with_label("Do something");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
//	g_signal_connect(menuitem, "activate",
//	                 (GCallback ) view_popup_menu_onDoSomething, 1);

	menuitem = gtk_menu_item_new_with_label("or something");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
//	g_signal_connect(menuitem, "activate",
//	                 (GCallback ) view_popup_menu_onDoSomething, 2);

	gtk_widget_show_all(menu);

	/* Note: event can be NULL here when called from view_onPopupMenu;
	 *  gdk_event_get_time() accepts a NULL argument */
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
	               (event != NULL) ? event->button : 0,
	               gdk_event_get_time((GdkEvent*) event));
}

/*--------------------------------------------------------------------
 * Function:		Patch_Popup_CB
 * Description:
 *
 *---------------------------------------------------------------------*/
void Patch_Popup_CB(GtkWidget *menuitem, gpointer userdata) {
	/* we passed the view as userdata when we connected the signal */
//	GtkTreeView *treeview = GTK_TREE_VIEW(userdata);
//	g_print("Do something! %d\n", (int)userdata);
	strcpy(gMyInfo.LayoutPresets[CurrentLayout].Presets[CurrentPreset], gMyInfo.MyPatchInfo[(int)userdata].Name);
	SetUpMainButtons(&gMyInfo.MyPatchInfo);
}

/*--------------------------------------------------------------------
 * Function:		CreatePatchPopupMenu
 * Description:
 *
 *---------------------------------------------------------------------*/
void CreatePatchPopupMenu(void) {
	GtkWidget *menuitem;
	int	Loop;
	PatchPopupMenu = gtk_menu_new();

	for (Loop = 0; Loop < Max_Patches; Loop++) {
		menuitem = gtk_menu_item_new_with_label(gMyInfo.MyPatchInfo[Loop].Name);
		gtk_menu_shell_append(GTK_MENU_SHELL(PatchPopupMenu), menuitem);
		g_signal_connect(menuitem, "activate",
		                 (GCallback ) Patch_Popup_CB, Loop);
	}

#if 0
	for (Loop = 0; Loop < MaxPresetButtons - 1; Loop++) {
		menuitem = gtk_menu_item_new_with_label(gMyInfo.MyPatchInfo[Loop].Name);
		gtk_menu_shell_append(GTK_MENU_SHELL(PatchPopupMenu), menuitem);
		g_signal_connect(menuitem, "activate",
		                 (GCallback ) Patch_Popup_CB, Loop);
	}
#endif

}

/*--------------------------------------------------------------------
 * Function:		ShowPatchListSelect
 * Description:		Bring up the list of Popup's
 *
 *
 *---------------------------------------------------------------------*/
int	 ShowPatchListSelect(GtkWidget *Temp, int Current) {

	GtkWidget *treeview;
	printd(LogInfo, "On Butting Pressed CB\n");

	/* single click with the right mouse button? */
	printf("view_onButtonPressed");
//	if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
	if (1 ) {
		gtk_widget_show_all(PatchPopupMenu);

		/* Note: event can be NULL here when called from view_onPopupMenu;
		 *  gdk_event_get_time() accepts a NULL argument */
		gtk_menu_popup(GTK_MENU(PatchPopupMenu), NULL, NULL, NULL, NULL,
		               0, 0);

		return TRUE; /* we handled this */
	}

	return FALSE; /* we did not handle this */
}

#if 0

/*--------------------------------------------------------------------
 * Function:		view_popup_menu_onDoSomething
 * Description:
 *
 *---------------------------------------------------------------------*/
void view_popup_menu_onDoSomething(GtkWidget *menuitem, int userdata) {
	/* we passed the view as userdata when we connected the signal */
//	GtkTreeView *treeview = GTK_TREE_VIEW(userdata);
//	g_print("Do something! %d\n", (int)userdata);

	switch ((int) userdata) {
	case 1:
		break;

	case 2:
		break;
	}
}

/*--------------------------------------------------------------------
 * Function:		view_onButtonPressed
 * Description:
 *
 *---------------------------------------------------------------------*/
gboolean view_onButtonPressed(GtkWidget *treeview, GdkEventButton *event,
                              gpointer userdata) {
	printd(LogInfo, "On Butting Pressed CB\n");

	/* single click with the right mouse button? */
	if (event->type == GDK_BUTTON_PRESS && event->button == 3) {

		view_popup_menu(treeview, event, userdata);

		return TRUE; /* we handled this */
	}

	return FALSE; /* we did not handle this */
}

gboolean view_onPopupMenu(GtkWidget *treeview, gpointer userdata) {
	view_popup_menu(treeview, NULL, userdata);

	return TRUE; /* we handled this */
}

/*--------------------------------------------------------------------
 * Function:		create_Popup_view
 * Description:		Here is where is initialize the Popup
 *
 *---------------------------------------------------------------------*/
void create_Popup_view(GtkWidget *view) {
//	GtkWidget *view;
	printd(LogInfo, "Before TreeView\n");

//	view = gtk_tree_view_new();

	g_signal_connect(view, "button-press-event",
	                 (GCallback ) view_onButtonPressed, NULL);
	g_signal_connect(view, "popup-menu", (GCallback ) view_onPopupMenu, NULL);
	printd(LogInfo, "After TreeView\n");
}
#endif
