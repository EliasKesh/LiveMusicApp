/*---------------------------------------------------------------------
 |
 |	File: 	PrefsGui
 |
 |	Contains:
 |
 |
 |	Written By: 	Elias Keshishoglou on Sun May 19 18:56:58 PDT 2013
 |
 |	Copyright ©: 	2013 Elias Keshishoglou all rights reserved.
 |
 |
 |---------------------------------------------------------------------*/

#define PrefsGui_c

/*
 * Place Includes Here
 */

#include <gtk/gtk.h>

#include "stdlib.h"
#include <string.h>
#include "stdbool.h"
#include "GTKMidiUI.h"
#include "PrefsGui.h"

/*
 * Place defines and Typedefs here
 */

extern GtkBuilder *gxml;

/*
 * Place Local prototypes here
 */
static GtkWidget *CreatePatchViewModel(void);

/*
 * Place Static variables here
 */
enum {
	Button_COLUMN = 0,
	Name_COLUMN,
	Bank_COLUMN,
	Patch_COLUMN,
	Output_COLUMN,
	Channel_COLUMN,
	Command_COLUMN,
	Chain_COLUMN,
	NumCOLUMN
};

/*--------------------------------------------------------------------
 * Function:		on_BasePath_changed
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void on_BasePath_changed(GtkButton *button, gpointer user_data) {
//	DoPatch(&gMyInfo.MyPatchInfo[(char )user_data - 1]);
	printd(LogDebug, "Base string %s\n", gtk_entry_get_text(button));
	strcpy(&gMyInfo.BasePath, gtk_entry_get_text(button));
}

/*--------------------------------------------------------------------
 * Function:		InitGuiPrefs
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void InitGuiPrefs(void) {
	GtkWidget *main_window;
	GtkWidget *view;
	GtkWidget *widget;

	view = CreatePatchViewModel();

	main_window = GTK_WIDGET(gtk_builder_get_object(gxml, "PatchScroller"));
	printd(LogDebug, "Main Gui Scroll Window %x %x\n", main_window, view);
#if 0
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (main_window),
	                                     GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (main_window),
	                                GTK_POLICY_AUTOMATIC,
	                                TK_POLICY_AUTOMATIC);
#endif

//			g_signal_connect (G_OBJECT (main_window), "destroy",
//			G_CALLBACK (on_window1_destroy), NULL);
	gtk_container_add(GTK_CONTAINER(main_window), view);
	gtk_widget_show_all(main_window);

	widget = GTK_WIDGET(gtk_builder_get_object(gxml, "BasePath"));
	g_signal_connect_data(G_OBJECT(widget), "changed",
	                      G_CALLBACK(on_BasePath_changed), NULL, NULL, 0);
	gtk_entry_set_text(widget, gMyInfo.BasePath);

}

/*--------------------------------------------------------------------
 * Function:		<Function name>
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static GtkTreeModel *CreatePatchModel(void) {
	GtkTreeIter toplevel, child;
	unsigned int Loop;
#if 1
	GtkListStore *treestore;

	treestore = gtk_list_store_new(NumCOLUMN,
	                               G_TYPE_STRING,
	                               G_TYPE_STRING,
	                               G_TYPE_UINT,
	                               G_TYPE_UINT,
	                               G_TYPE_CHAR,
	                               G_TYPE_CHAR,
	                               G_TYPE_STRING,
	                               G_TYPE_STRING);
#else
	GtkTreeStore *treestore;
	treestore = gtk_tree_store_new (NumCOLUMN,
	                                G_TYPE_STRING,
	                                G_TYPE_STRING,
	                                G_TYPE_UINT,
	                                G_TYPE_UINT,
	                                G_TYPE_CHAR,
	                                G_TYPE_CHAR,
	                                G_TYPE_STRING,
	                                G_TYPE_CHAR );

#endif
	for (Loop = 0; Loop < Max_Patches; Loop++) {
//		gtk_tree_store_append(treestore, &toplevel,NULL);
		gtk_list_store_append(treestore, &toplevel);
		gtk_list_store_set(treestore, &toplevel,
		                   //		gtk_tree_store_set (treestore, &toplevel,
//		Button_COLUMN,gMyInfo.MyPatchInfo[Loop].Index,
		                   Name_COLUMN, gMyInfo.MyPatchInfo[Loop].Name,
		                   Bank_COLUMN, gMyInfo.MyPatchInfo[Loop].BankSelect,
		                   Patch_COLUMN, gMyInfo.MyPatchInfo[Loop].Patch,
		                   Output_COLUMN, gMyInfo.MyPatchInfo[Loop].OutPort,
		                   Channel_COLUMN, gMyInfo.MyPatchInfo[Loop].Channel,
		                   Command_COLUMN, CustomCommands[gMyInfo.MyPatchInfo[Loop].CustomCommand],
		                   Chain_COLUMN, gMyInfo.MyPatchInfo[Loop].Chain, -1);
	}

	return GTK_TREE_MODEL(treestore);
}

/*--------------------------------------------------------------------
 * Function:		<Function name>
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static void PatchListEdited(GtkCellRendererText * cell, gchar * path_string,
                            gchar * new_text, gpointer user_data) {
	GtkTreeView *treeview = (GtkTreeView *) user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;

	guint column;
	int Loop;
	int Loop1;

	GtkTreePath *path = gtk_tree_path_new_from_string(path_string);

	Loop = atoi(path_string);
	printd(LogDebug, "cell_edited_callback %x, %d [%s] %d %s\n", cell, Loop, new_text,
	       (int) user_data, path_string);

	printd(LogDebug, "PatchListEdited %x\n", treeview);

	/* Column number is passed as renderer object data */
	gpointer columnptr = g_object_get_data(G_OBJECT(cell), "column");

	column = GPOINTER_TO_UINT(columnptr);
	printd(LogDebug, "PatchListEdited Column %x %d\n", columnptr, column);

	/* Get the iterator */
	model = gtk_tree_view_get_model(treeview);

	gtk_tree_model_get_iter_from_string(model, &iter, path_string);
	printd(LogDebug, "PatchListEdited Model Iter %x %x\n", model, iter);

	/* Update the model */
	switch (column) {
	case Name_COLUMN:
	case Command_COLUMN:
		gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, new_text,
		                   -1);
		break;

	default:
		gtk_list_store_set(GTK_LIST_STORE(model), &iter, column,
		                   atoi(new_text), -1);
		break;
	}

	switch (column) {
	case Button_COLUMN:
		//		strcpy(gMyInfo.MyPatchInfo[Loop].Button, new_text);
		break;

	case Name_COLUMN:
		strcpy(gMyInfo.MyPatchInfo[Loop].Name, new_text);
		break;

	case Bank_COLUMN:
		gMyInfo.MyPatchInfo[Loop].BankSelect = atoi(new_text);
		break;

	case Patch_COLUMN:
		gMyInfo.MyPatchInfo[Loop].Patch = atoi(new_text);
		break;

	case Output_COLUMN:
		gMyInfo.MyPatchInfo[Loop].OutPort = atoi(new_text);
		break;

	case Channel_COLUMN:
		gMyInfo.MyPatchInfo[Loop].Channel = atoi(new_text);
		break;

	case Command_COLUMN:
		for (Loop1 = 0; Loop1 <= MaxCommands; Loop1++) {
			if (strstr((char *)&CustomCommands[Loop1], new_text)) {
				gMyInfo.MyPatchInfo[Loop].CustomCommand = Loop1;
			}
		}
		break;
	case Chain_COLUMN:
		strcpy(gMyInfo.MyPatchInfo[Loop].Chain, new_text);
		break;

	}
}

/*--------------------------------------------------------------------
 * Function:		create_combo_model
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
GtkTreeModel *create_combo_model(void) {
	GtkTreeModel *model;
	GtkTreeIter iter;
	gint i;

	model = GTK_TREE_MODEL(gtk_list_store_new(1, G_TYPE_STRING));

	for (i = 0; i < MaxCommands; i++) {

		gtk_list_store_append(GTK_LIST_STORE(model), &iter);
		gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, CustomCommands[i],
		                   -1);
	}

	return model;
}

/*--------------------------------------------------------------------
 * Function:		text_editing_started
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static void text_editing_started(GtkCellRenderer ATTRIBUTE_UNUSED *cell,
                                 GtkCellEditable *editable, const gchar *path, GCallback data) {
	printd(LogDebug, "text_editing_started\n");
	if (GTK_IS_ENTRY(editable)) {
		GtkEntry *entry = GTK_ENTRY(editable);
		GCallback cb_func = data;
//               g_signal_connect(GTK_OBJECT(entry), "activate", (GCallback)cb_func, (char *)xstrdup(path));
	}
}

/*--------------------------------------------------------------------
 * Function:		row_activated_cb
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void row_activated_cb(GtkTreeView *tree_view, GtkTreePath *path,
                      GtkTreeViewColumn *column) {
	GtkTreeIter iter;
	PangoStyle style;
	GtkWidget *window;
	GtkTreeModel *model;

	model = gtk_tree_view_get_model(tree_view);
	gtk_tree_model_get_iter(model, &iter, path);
#if 1
	gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
	                   //		      FUNC_COLUMN, &func,
//		      STYLE_COLUMN, &style,
	                   -1);
#endif
	printd(LogDebug, "row_activated_cb1 %x %x %x [%x]\n", tree_view, path, column, iter);

	gtk_tree_store_set(GTK_TREE_STORE(model), &iter, PANGO_STYLE_NORMAL,
	                   PANGO_STYLE_ITALIC, -1);

}

/*--------------------------------------------------------------------
 * Function:		CreatePatchViewModel
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static GtkWidget *CreatePatchViewModel(void) {
	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;
	GtkTreeModel *model, *combomodel;
	GtkWidget *view;
	GtkTreeSelection *sel;

	view = gtk_tree_view_new();

	combomodel = create_combo_model();
#if 0
	/* --- Button_COLUMN --- */
	renderer = gtk_cell_renderer_text_new ();
//  g_object_set(renderer, "editable", TRUE, NULL);
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	        Button_COLUMN, "Button", renderer,
	        "text", Button_COLUMN,
	        NULL);
	g_object_set_data(G_OBJECT(renderer), "column", GUINT_TO_POINTER(Button_COLUMN));
#endif

	/* --- Name_COLUMN --- */
	renderer = gtk_cell_renderer_text_new();
	g_object_set(renderer, "editable", TRUE, NULL);
	g_object_set(renderer, "editable-set", TRUE, NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Name_COLUMN, "Name", renderer, "text", Name_COLUMN,
	        NULL);

//	g_signal_connect(renderer, "toggled", (GCallback) PatchListEdited, view);
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Name_COLUMN));
	g_signal_connect(G_OBJECT(renderer), "editing-started",
	                 G_CALLBACK(text_editing_started), Button_COLUMN);

	/* --- Bank_COLUMN --- */
	renderer = gtk_cell_renderer_text_new();
	g_object_set(renderer, "editable", TRUE, NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Bank_COLUMN, "Bank", renderer, "text", Bank_COLUMN,
	        NULL);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Bank_COLUMN));
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);

	/* --- Patch_COLUMN --- */
	renderer = gtk_cell_renderer_text_new();
	g_object_set(renderer, "editable", TRUE, NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Patch_COLUMN, "Patch", renderer, "text", Patch_COLUMN,
	        NULL);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Patch_COLUMN));
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);

	/* --- Output_COLUMN --- */
	renderer = gtk_cell_renderer_text_new();
	g_object_set(renderer, "editable", TRUE, NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Output_COLUMN, "Port", renderer, "text", Output_COLUMN,
	        NULL);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Output_COLUMN));
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);

	/* --- Channel_COLUMN --- */
	renderer = gtk_cell_renderer_text_new();
	g_object_set(renderer, "editable", TRUE, NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Channel_COLUMN, "Channel", renderer, "text", Channel_COLUMN,
	        NULL);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Channel_COLUMN));
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);

	/* --- Command_COLUMN --- */
	renderer = gtk_cell_renderer_combo_new();
//  g_object_set(renderer, "editable", TRUE, NULL);
	g_object_set(renderer, "text-column", 0, "model", combomodel, "mode",
	             GTK_CELL_RENDERER_MODE_ACTIVATABLE, "editable", TRUE, "has-entry",
	             FALSE,
	             NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Command_COLUMN, "Command", renderer, "text", Command_COLUMN,
	        NULL);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Command_COLUMN));
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);
	g_signal_connect(renderer, "clicked", (GCallback ) PatchListEdited, view);
	g_signal_connect(G_OBJECT(renderer), "editing-started",
	                 G_CALLBACK(text_editing_started), Button_COLUMN);

	/* --- Chain_COLUMN --- */
	renderer = gtk_cell_renderer_text_new();
	g_object_set(renderer, "editable", TRUE, NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Chain_COLUMN, "Chain", renderer,
	        "text", Chain_COLUMN,
	        NULL);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Chain_COLUMN));
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);
	g_object_set(renderer, "editable-set", TRUE, NULL);

	/* connect a cell data function */
//  gtk_tree_view_column_set_cell_data_func(col, renderer, age_cell_data_func, NULL, NULL);

	model = CreatePatchModel();
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

//	g_signal_connect (view, "row_activated", G_CALLBACK (row_activated_cb), model);

//  g_object_unref(model); /* destroy model automatically with view */

//  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
//                              GTK_SELECTION_NONE);
	return view;
}

#if 0
----------------------------------------------------------------
NOT USED
----------------------------------------------------------------
void cell_toggled_callback (GtkCellRendererToggle *cell,
                            gchar *path_string,
                            GtkTreeView *tree_view) {

	GtkTreeModel *model = gtk_tree_view_get_model (tree_view);
	GtkTreeModelFilter *filter_model;
	//g_print("CB(cell_toggled:tree_view %s\n",G_OBJECT_TYPE_NAME( model ) );
	char *model_class = G_OBJECT_TYPE_NAME( model );
	GtkTreeIter iter;
	GtkTreePath *path, *filter_path;
	gboolean column_value, new_column_value;

	path = gtk_tree_path_new_from_string( path_string );

	// get the real model, & path if model is a GtkTreeModelFilter
	if ( !strcmp( "GtkTreeModelFilter", model_class ) ) {
		filter_model = model;
		model = gtk_tree_model_filter_get_model( GTK_TREE_MODEL_FILTER( model ) );
		filter_path = path;

		path = gtk_tree_model_filter_convert_path_to_child_path( filter_model,
		        filter_path);
	}

	if ( !gtk_tree_model_get_iter( GTK_TREE_MODEL( model ), &iter, path ) ) {
		g_warning ("%s: could not find path", G_STRLOC);
		return;
	}

	gtk_tree_model_get (GTK_TREE_MODEL (model), &iter,
	                    COL_NEED, &column_value, -1);

	// toggle the column_value 0=>1; 1=>0;
	new_column_value = column_value ? 0 : 1;

	gtk_list_store_set( GTK_LIST_STORE( model ), &iter,
	                    COL_NEED, new_column_value, -1);
	//gtk_list_store_set_value( GTK_LIST_STORE( model ), &iter,
	//          COL_NEED, new_column_value );

	//gtk_tree_model_row_changed( model, path, &iter );
	gtk_tree_path_free (path);

	// get the real model, & path if model is a GtkTreeModelFilter
	if ( !strcmp( "GtkTreeModelFilter", model_class ) ) {
		gtk_tree_model_filter_refilter( filter_model );
		gtk_tree_path_free (filter_path);

	}

	return;
}

void cell_edited_callback (GtkCellRendererText *cell,
                           gchar *path_string,
                           gchar *new_text,
                           gpointer user_data) {
	int Loop;

	Loop = atoi(path_string);
	printd(LogDebug, "cell_edited_callback %x, %d [%s] %d %s\n", cell, Loop, new_text, (int)user_data, path_string );
#if 0
	guint column, row;
	GtkTreeIter iter;
	GtkTreeModel *model;

	model = gtk_tree_view_get_model (treeview);
	if (gtk_tree_model_get_iter_from_string (model, &iter, path_string))
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, column, new_text, -1);

#endif
//   gtk_label_set_text(GTK_LABEL(cell), new_text);

	switch ((int) user_data) {
	case Button_COLUMN:
//		strcpy(gMyInfo.MyPatchInfo[Loop].Button, new_text);
		break;

	case Name_COLUMN:
		strcpy(gMyInfo.MyPatchInfo[Loop].Name, new_text);
		break;
	case Bank_COLUMN:
		gMyInfo.MyPatchInfo[Loop].BankSelect = atoi(new_text);

		break;
	case Patch_COLUMN:
		gMyInfo.MyPatchInfo[Loop].Patch = atoi(new_text);

		break;

	case Output_COLUMN:
		gMyInfo.MyPatchInfo[Loop].OutPort = atoi(new_text);
		break;
	case Channel_COLUMN:
		gMyInfo.MyPatchInfo[Loop].Channel = atoi(new_text);

		break;
	case Command_COLUMN:
		gMyInfo.MyPatchInfo[Loop].CustomCommand = atoi(new_text);

		break;
	case Chain_COLUMN:
		gMyInfo.MyPatchInfo[Loop].Chain = atoi(new_text);
		break;

	}
}
g_object_set( renderer, "activatable", TRUE, NULL);

void cell_toggled_callback (GtkCellRendererToggle *cell,
                            gchar *path_string,
                            gpointer user_data) {
	printd(LogDebug, "cell_toggled_callback %x, %s %x\n", cell, path_string, user_data);
}
void labour_cell_edited (GtkCellRendererText *cell,
                         gchar *path_string,
                         gchar *new_text,
                         gpointer model) {
	int Loop;

	Loop = atoi(path_string);
	printd(LogDebug, "labour_cell_edited\n");

	if (!model) {
		g_error("Unable to get model from cell renderer");
	}
	GtkTreeIter row;
	gtk_tree_model_get_iter_from_string(model, &row, path_string);
	guint column_no = GPOINTER_TO_UINT(g_object_get_data(
	                                       G_OBJECT(cell), "column_no"));
	g_print("Column :%d, new text: \"%s\"\n", column_no, new_text);

};

#endif
#if 0
/*
 * ------------------------------------------------------------------------
 */

void
view_popup_menu_onDoSomething (GtkWidget *menuitem, gpointer userdata) {
	/* we passed the view as userdata when we connected the signal */
	GtkTreeView *treeview = GTK_TREE_VIEW(userdata);

	g_print ("Do something!\n");
}


void view_popup_menu (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
	GtkWidget *menu, *menuitem;

	menu = gtk_menu_new();

	menuitem = gtk_menu_item_new_with_label("Do something");

	g_signal_connect(menuitem, "activate",
	                 (GCallback) view_popup_menu_onDoSomething, treeview);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

	gtk_widget_show_all(menu);

	/* Note: event can be NULL here when called from view_onPopupMenu;
	 *  gdk_event_get_time() accepts a NULL argument */
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
	               (event != NULL) ? event->button : 0,
	               gdk_event_get_time((GdkEvent*)event));
}


gboolean view_onButtonPressed (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
	/* single click with the right mouse button? */
	if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {
		g_print ("Single right click on the tree view.\n");

		/* optional: select row if no row is selected or only
		 *  one other row is selected (will only do something
		 *  if you set a tree selection mode as described later
		 *  in the tutorial) */
		if (1) {
			GtkTreeSelection *selection;

			selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

			/* Note: gtk_tree_selection_count_selected_rows() does not
			 *   exist in gtk+-2.0, only in gtk+ >= v2.2 ! */
			if (gtk_tree_selection_count_selected_rows(selection)  <= 1) {
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

		view_popup_menu(treeview, event, userdata);

		return TRUE; /* we handled this */
	}

	return FALSE; /* we did not handle this */
}


gboolean view_onPopupMenu (GtkWidget *treeview, gpointer userdata) {
	view_popup_menu(treeview, NULL, userdata);

	return TRUE; /* we handled this */
}

int	ShowPatchListSelect(int	Starting) {
	GtkWidget *view;

	view = gtk_tree_view_new();

	g_signal_connect(view, "button-press-event", (GCallback) view_onButtonPressed, NULL);
	g_signal_connect(view, "popup-menu", (GCallback) view_onPopupMenu, NULL);

}
#endif
enum {
	LIST_ITEM = 0,
	N_COLUMNS
};

#if 0
static void init_list(GtkWidget *list) {
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",
	         renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

	gtk_tree_view_set_model(GTK_TREE_VIEW(list),
	                        GTK_TREE_MODEL(store));

	g_object_unref(store);
}
#endif

#if 0
static void add_to_list(GtkWidget *list, const gchar *str) {
	GtkListStore *store;
	GtkTreeIter iter;

	store = GTK_LIST_STORE(gtk_tree_view_get_model
	                       (GTK_TREE_VIEW(list)));

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}
#endif

void  OnPatchSelected(GtkWidget *widget, int	Starting) {
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;

	printd(LogDebug, "On Patch Changed\n");
	if (gtk_tree_selection_get_selected(
	            GTK_TREE_SELECTION(widget), &model, &iter)) {

		gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
		printd(LogDebug, "On Patch Changed in Selection %s\n", value);

		strcpy(gMyInfo.LayoutPresets[CurrentLayout].Presets[Starting], value);
//    gtk_label_set_text(GTK_LABEL(label), value);
		g_free(value);
	}

}
#if 0
int	ShowPatchListSelect(GtkWidget *view, int	Starting) {
	GtkWidget *window;
	GtkWidget *list;

	GtkWidget *vbox;
	GtkWidget *label;
	GtkTreeSelection *selection;
	int Loop;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_widget_set_size_request(window, 270, 250);
	gtk_window_set_title(GTK_WINDOW(window), "List View");

	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

	vbox = gtk_vbox_new(FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);

	label = gtk_label_new("Select Patch");
	gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

	gtk_container_add(GTK_CONTAINER(window), vbox);

	init_list(list);
	for (Loop = 0; Loop < Max_Patches; Loop++) {
		add_to_list(list, gMyInfo.MyPatchInfo[Loop].Name);
	}

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	gtk_tree_view_set_activate_on_single_click (GTK_TREE_VIEW(list), TRUE);
	g_signal_connect(selection, "changed",
	                 G_CALLBACK(OnPatchSelected), Starting);

	gtk_widget_show_all(window);
	printd(LogDebug, "Selected %d\n", selection);
	return (selection);
}


#endif