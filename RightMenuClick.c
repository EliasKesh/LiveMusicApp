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
 |
 |---------------------------------------------------------------------*/

#define RightMenuClick_c

/*
 * Place Includes Here
 */
#include <gtk/gtk.h>

#include "GTKMidiUI.h"

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
 * Function:		view_popup_menu_onDoSomething
 * Description:
 *
 *---------------------------------------------------------------------*/
void view_popup_menu_onDoSomething(GtkWidget *menuitem, gpointer userdata) {
	/* we passed the view as userdata when we connected the signal */
//	GtkTreeView *treeview = GTK_TREE_VIEW(userdata);
	g_print("Do something! %d\n", (int)userdata);

	switch ((int) userdata) {
		case 1:
			break;

		case 2:
			break;
	}
}

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
	g_signal_connect(menuitem, "activate",
		(GCallback ) view_popup_menu_onDoSomething, 1);

	menuitem = gtk_menu_item_new_with_label("or something");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(menuitem, "activate",
		(GCallback ) view_popup_menu_onDoSomething, 2);

//	g_signal_connect(menuitem, "activate",
//		(GCallback ) view_popup_menu_onDoSomething, treeview);

	gtk_widget_show_all(menu);

	/* Note: event can be NULL here when called from view_onPopupMenu;
	 *  gdk_event_get_time() accepts a NULL argument */
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
		(event != NULL) ? event->button : 0,
		gdk_event_get_time((GdkEvent*) event));
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
		g_print("Single right click on the tree view.\n");
#if 0
		/* optional: select row if no row is selected or only
		 *  one other row is selected (will only do something
		 *  if you set a tree selection mode as described later
		 *  in the tutorial) */
		if (1) {
			GtkTreeSelection *selection;

			selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

			/* Note: gtk_tree_selection_count_selected_rows() does not
			 *   exist in gtk+-2.0, only in gtk+ >= v2.2 ! */
			if (gtk_tree_selection_count_selected_rows(selection) <= 1) {
				GtkTreePath *path;

				/* Get tree path for row that was clicked */
				if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
						(gint) event->x,
						(gint) event->y,
						&path, NULL, NULL, NULL)) {
					gtk_tree_selection_unselect_all(selection);
					gtk_tree_selection_select_path(selection, path);
					gtk_tree_path_free(path);
				}
			}
		} /* end of optional bit */
#endif
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


















/*--------------------------------------------------------------------
 * Function:		Patch_Popup_CB
 * Description:
 *
 *---------------------------------------------------------------------*/
void Patch_Popup_CB(GtkWidget *menuitem, gpointer userdata) {
	/* we passed the view as userdata when we connected the signal */
//	GtkTreeView *treeview = GTK_TREE_VIEW(userdata);
	g_print("Do something! %d\n", (int)userdata);
	strcpy(gMyInfo.LayoutPresets[CurrentLayout].Presets[CurrentPreset], gMyInfo.MyPatchInfo[(int)userdata].Name);
	SetUpMainButtons(&gMyInfo.MyPatchInfo);
}

/*--------------------------------------------------------------------
 * Function:		view_Patch_popup_menu
 * Description:
 *
 *---------------------------------------------------------------------*/
void view_Patch_popup_menu(GtkWidget *treeview, GdkEventButton *event,
	gpointer userdata) {
	GtkWidget *menu, *menuitem;
	int	Loop;
	menu = gtk_menu_new();

	for (Loop = 0; Loop< Max_Patches; Loop++) {
		menuitem = gtk_menu_item_new_with_label(gMyInfo.MyPatchInfo[Loop].Name);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
		g_signal_connect(menuitem, "activate",
			(GCallback ) Patch_Popup_CB, Loop);
	}
	gtk_widget_show_all(menu);

	/* Note: event can be NULL here when called from view_onPopupMenu;
	 *  gdk_event_get_time() accepts a NULL argument */
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
		0, NULL);
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
		g_print("Single right click on the tree view.\n");
#if 0
		/* optional: select row if no row is selected or only
		 *  one other row is selected (will only do something
		 *  if you set a tree selection mode as described later
		 *  in the tutorial) */
		if (1) {
			GtkTreeSelection *selection;

			selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

			/* Note: gtk_tree_selection_count_selected_rows() does not
			 *   exist in gtk+-2.0, only in gtk+ >= v2.2 ! */
			if (gtk_tree_selection_count_selected_rows(selection) <= 1) {
				GtkTreePath *path;

				/* Get tree path for row that was clicked */
				if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
						(gint) event->x,
						(gint) event->y,
						&path, NULL, NULL, NULL)) {
					gtk_tree_selection_unselect_all(selection);
					gtk_tree_selection_select_path(selection, path);
					gtk_tree_path_free(path);
				}
			}
		} /* end of optional bit */
#endif
		view_Patch_popup_menu(treeview, NULL, Current);

		return TRUE; /* we handled this */
	}

	return FALSE; /* we did not handle this */
}

