/*------------------------------------------------
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
|------------------------------------------------*/

#define PrefsGui_c

/*
 * Place Includes Here
 */

#include <gtk/gtk.h>

#include "stdlib.h"
#include <string.h>
#include "stdbool.h"
#include "LiveMusicApp.h"
#include "PrefsGui.h"
#include "../ChartFixer/ChartFixer.h"
#include "config.h"
/*
 * Place defines and Typedefs here
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

extern GtkBuilder *gxml;

/*
 * Place Local prototypes here
 */
static GtkWidget *CreatePatchViewModel(void);

/*
 * Place Static variables here
 */

GtkWidget *Analog_Volume;
GtkWidget *Midi_Volume;
GtkWidget *Record_Beats;
GtkWidget *Count_Beats;
GtkWidget *Beats_Measure;
GtkWidget *Tempo_pref;
GtkWidget *Midi_Base;
GtkWidget *OSC_Address;
GtkWidget *OSCLoopPort;
GtkWidget *OSCVolumePort;
GtkWidget *OSCHydrogenPort;
GtkWidget *About_Close;

GtkWidget *Prefs_Regenerate;
//	GtkWidget *About_Close;

/*--------------------------------------------------------------------
 * Function:		on_BasePath_changed
 *
 * Description:		<Description/Comments>
 *
 *------------------------------------------------*/
void on_BasePath_changed(GtkButton *button, gpointer user_data) {
//	DoPatch(&gMyInfo.MyPatchInfo[(char )user_data-1]);
	printd(LogDebug, "Base string Changed %s\n", gtk_entry_get_text(button));
	strcpy(&gMyInfo.BasePath, gtk_entry_get_text(button));
}

/*--------------------------------------------------------------------
 * Function:		Prefs_click_spin_handler
 *
 * Description:		Get data from number spin boxes.
 *
 *------------------------------------------------*/
gboolean Prefs_click_spin_handler(GtkWidget *widget, GdkEvent *event,
                                  gpointer user_data) {
//	theImageButtons *theButton;

//	theButton = (theImageButtons *) user_data;
	gint value = gtk_spin_button_get_value_as_int (widget);
	if (widget == Analog_Volume) {
		gMyInfo.AnalogVolume = value;
		printd(LogDebug, "Analog_Volume  %x %d\n", widget, value);
	}

	if (widget == Midi_Volume) {
		gMyInfo.MidiVolume = value;
		printd(LogDebug, "Midi_Volume  %x %d\n", widget, value);
	}

	if (widget == Record_Beats) {
		gMyInfo.LoopRecBeats = value;
		printd(LogDebug, "Record_Beats  %x %d\n", widget, value);
	}

	if (widget == Count_Beats) {
		gMyInfo.CountInBeats = value;
		printd(LogDebug, "Count_Beats  %x %d\n", widget, value);
	}

	if (widget == Beats_Measure) {
		gMyInfo.BeatsPerMeasure = value;
		printd(LogDebug, "Beats_Measure  %x %d\n", widget, value);
	}

	if (widget == Tempo_pref) {
		gMyInfo.Tempo = value;
		printd(LogDebug, "Tempo_pref  %x %d\n", widget, value);

		// if (WeAreRunning)
		// 	SetTempo(value);
	}

	if (widget == Midi_Base) {
		gMyInfo.MidiBaseNote = value;
		printd(LogDebug, "Midi_Base  %x %d\n", widget, value);
	}

	return TRUE; /* stop event propagation */
}

/*--------------------------------------------------------------------
 * Function:		Prefs_click_text_handler
 *
 * Description:		Get data from text boxes.
 *
 *------------------------------------------------*/
gboolean Prefs_click_text_handler(GtkWidget *widget, GdkEvent *event,
                                  gpointer user_data) {

	const gchar *theString = gtk_entry_get_text (widget);

	if (widget == OSC_Address) {
		strncpy(gMyInfo.OSCIPAddress, theString, 20);
		printd(LogDebug, "OSC_Address  %x %s\n", widget, theString);
	}

	if (widget == OSCLoopPort) {
		strncpy(gMyInfo.OSCPortNumLooper, theString, 6);
		printd(LogDebug, "OSCLoopPort  %x %s\n", widget, theString);
	}

	if (widget == OSCHydrogenPort) {
		strncpy(gMyInfo.OSCPortNumHydrogen, theString, 6);
		printd(LogDebug, "OSCHydrogenPort  %x %s\n", widget, theString);
	}

	if (widget == OSCVolumePort) {
		strncpy(gMyInfo.OSCPortNumJackVol, theString, 6);
		printd(LogDebug, "OSCVolumePort  %x %s\n", widget, theString);
	}

	return TRUE; /* stop event propagation */
}


/*--------------------------------------------------------------------
 * Function:		Prefs_Regenerate_handler
 *
 * Description:		Generate the Index File.
 *
 *------------------------------------------------*/
gboolean Prefs_Regenerate_handler(GtkWidget *widget, GdkEvent *event,
                                  gpointer user_data) {

	printd (LogDebug, "Prefs_Regenerate_handler\n");
//	gtk_widget_destroy(user_data);
//	DoChartFix(gMyInfo.BasePath,
//	           1, 1, 1, 1);
	system("ReIndex.sh");

}

/*--------------------------------------------------------------------
 * Function:		Prefs_about_close_handler
 *
 * Description:		Handle the close button.
 *
 *------------------------------------------------*/
gboolean Prefs_about_close_handler(GtkWidget *widget, GdkEvent *event,
                                   gpointer user_data) {

	printd (LogDebug, "Prefs_about_close_handler\n");
	gtk_widget_destroy(user_data);

}

/*--------------------------------------------------------------------
 * Function:		InitGuiPrefs
 *
 * Description:		Load the widgets from glade for the
 * preferences user interface.
 *
 *------------------------------------------------*/
void InitGuiPrefs(void) {
	GtkWidget *Patch_Pane;
	GtkWidget *view;
	GtkWidget *widget;
	GtkWidget *About_Box;
	GtkWidget *MidiOutput;
	char 	VersionString[200];

	MidiOutput = GTK_WIDGET(gtk_builder_get_object(gxml,
	                        "MidiOutputList"));


	view = CreatePatchViewModel();

	Patch_Pane = GTK_WIDGET(gtk_builder_get_object(gxml,
	                        "PatchScroller"));
	//	About_Box = GTK_WIDGET(gtk_builder_get_object(gxml, "dialog-vbox1"));
	About_Box = GTK_WIDGET(gtk_builder_get_object(gxml,
	                       "AboutDialog"));

	Analog_Volume = GTK_WIDGET(gtk_builder_get_object(gxml,
	                           "Analog_Volume"));
	g_signal_connect(G_OBJECT(Analog_Volume),
	                 "value-changed",
	                 G_CALLBACK(Prefs_click_spin_handler),
	                 &Analog_Volume);
	gtk_spin_button_set_value(Analog_Volume,
	                          gMyInfo.AnalogVolume);

	Midi_Volume = GTK_WIDGET(gtk_builder_get_object(gxml,
	                         "Midi_Volume"));
	g_signal_connect(G_OBJECT(Midi_Volume),
	                 "value-changed",
	                 G_CALLBACK(Prefs_click_spin_handler),
	                 &Midi_Volume);
	gtk_spin_button_set_value(Midi_Volume,
	                          gMyInfo.MidiVolume);

	Record_Beats = GTK_WIDGET(gtk_builder_get_object(gxml,
	                          "Record_Beats"));
	g_signal_connect(G_OBJECT(Record_Beats),
	                 "value-changed",
	                 G_CALLBACK(Prefs_click_spin_handler),
	                 &Record_Beats);
	gtk_spin_button_set_value(Record_Beats, gMyInfo.LoopRecBeats);

	Count_Beats = GTK_WIDGET(gtk_builder_get_object(gxml,
	                         "Count_Beats"));
	g_signal_connect(G_OBJECT(Count_Beats),
	                 "value-changed",
	                 G_CALLBACK(Prefs_click_spin_handler),
	                 &Count_Beats);
	gtk_spin_button_set_value(Count_Beats, gMyInfo.CountInBeats);

	Beats_Measure = GTK_WIDGET(gtk_builder_get_object(gxml,
	                           "Beats_Measure"));
	g_signal_connect(G_OBJECT(Beats_Measure),
	                 "value-changed",
	                 G_CALLBACK(Prefs_click_spin_handler),
	                 &Beats_Measure);
	gtk_spin_button_set_value(Beats_Measure, gMyInfo.BeatsPerMeasure);

	Tempo_pref = GTK_WIDGET(gtk_builder_get_object(gxml,
	                        "Tempo_pref"));
	g_signal_connect(G_OBJECT(Tempo_pref),
	                 "value-changed",
	                 G_CALLBACK(Prefs_click_spin_handler),
	                 &Tempo_pref);
	gtk_spin_button_set_value(Tempo_pref, gMyInfo.Tempo);

	Midi_Base = GTK_WIDGET(gtk_builder_get_object(gxml,
	                       "Midi_Base"));
	g_signal_connect(G_OBJECT(Midi_Base),
	                 "value-changed",
	                 G_CALLBACK(Prefs_click_spin_handler),
	                 &Midi_Base);
	gtk_spin_button_set_value(Midi_Base, gMyInfo.MidiBaseNote);

	/* Midi output port pane.
	*/
	MidiOutput = GTK_WIDGET(gtk_builder_get_object(gxml,
	                        "MidiOutputList"));
	printd (LogDebug, "GTK-WIDGET % x\n", MidiOutput);
	// gtk_list_box_insert(MidiOutput, "One", 0);
	// gtk_list_box_insert(MidiOutput, "Three", 2);
	// gtk_list_box_insert(MidiOutput, "Two", 1);

	OSC_Address = GTK_WIDGET(gtk_builder_get_object(gxml,
	                         "OSC_Address"));
	g_signal_connect(G_OBJECT(OSC_Address),
	                 "changed",
	                 G_CALLBACK(Prefs_click_text_handler),
	                 &OSC_Address);
	gtk_entry_set_text(OSC_Address, gMyInfo.OSCIPAddress);

	OSCLoopPort = GTK_WIDGET(gtk_builder_get_object(gxml,
	                         "OSCLoopPort"));
	g_signal_connect(G_OBJECT(OSCLoopPort),
	                 "changed",
	                 G_CALLBACK(Prefs_click_text_handler),
	                 &OSCLoopPort);
	gtk_entry_set_text(OSCLoopPort, gMyInfo.OSCPortNumLooper);

	OSCVolumePort = GTK_WIDGET(gtk_builder_get_object(gxml,
	                           "OSCVolumePort"));
	g_signal_connect(G_OBJECT(OSCVolumePort),
	                 "changed",
	                 G_CALLBACK(Prefs_click_text_handler),
	                 &OSCVolumePort);
	gtk_entry_set_text(OSCVolumePort, gMyInfo.OSCPortNumJackVol);

	OSCHydrogenPort = GTK_WIDGET(gtk_builder_get_object(gxml,
	                             "OSCHydrogenPort"));
	g_signal_connect(G_OBJECT(OSCHydrogenPort),
	                 "changed",
	                 G_CALLBACK(Prefs_click_text_handler),
	                 &OSCHydrogenPort);
	gtk_entry_set_text(OSCHydrogenPort, gMyInfo.OSCPortNumHydrogen);

	About_Close = GTK_WIDGET(gtk_builder_get_object(gxml, "About_Close"));
	// g_signal_connect(G_OBJECT(About_Close),
	//                  "button-press-event",
	//                  G_CALLBACK(Prefs_about_close_handler),
	//                  &About_Box);

	Prefs_Regenerate = GTK_WIDGET(gtk_builder_get_object(gxml, "Prefs_Regenerate"));
	g_signal_connect(G_OBJECT(Prefs_Regenerate),
	                 "button-press-event",
	                 G_CALLBACK(Prefs_Regenerate_handler),
	                 &Prefs_Regenerate);

//	About_Close = GTK_WIDGET(gtk_builder_get_object(gxml, "About_Close"));

	printd(LogDebug, "Main Gui Scroll Window %x %x\n", Patch_Pane, view);
#if 0
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (Patch_Pane),
	                                     GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (Patch_Pane),
	                                GTK_POLICY_AUTOMATIC,
	                                TK_POLICY_AUTOMATIC);
#endif

	sprintf(VersionString, "V=%s B=%d D=%s %s", VERSION, MY_BUILD_NUMBER, __DATE__, __TIME__ );
	gtk_about_dialog_set_version(About_Box, VersionString);

	//			g_signal_connect (G_OBJECT (Patch_Pane), "destroy",
	//			G_CALLBACK (on_window1_destroy), NULL);
	gtk_container_add(GTK_CONTAINER(Patch_Pane), view);
	gtk_widget_show_all(Patch_Pane);

	widget = GTK_WIDGET(gtk_builder_get_object(gxml, "BasePath"));
	g_signal_connect_data(G_OBJECT(widget), "changed",
	                      G_CALLBACK(on_BasePath_changed), NULL, NULL, 0);
	gtk_entry_set_text(widget, gMyInfo.BasePath);
}

/*--------------------------------------------------------------------
* Function:		CreatePatchModel
*
* Description:		<Description/Comments>
*
*------------------------------------------------*/
static GtkTreeModel *CreatePatchModel(void) {
	GtkTreeIter toplevel, child;
	unsigned int Loop;
	GtkListStore *treestore;
	treestore = gtk_list_store_new(NumCOLUMN,
	                               G_TYPE_STRING,
	                               G_TYPE_STRING,
	                               G_TYPE_UINT,
	                               G_TYPE_UINT,
	                               G_TYPE_STRING,
	                               G_TYPE_CHAR,
	                               G_TYPE_STRING,
	                               G_TYPE_STRING);

	for (Loop = 0; Loop < Max_Patches; Loop++) {
		// printf(" % d % d % s\n", Loop, gMyInfo.MyPatchInfo[Loop].OutPort,
		// 	gMyInfo.OutPortName[gMyInfo.MyPatchInfo[Loop].OutPort]);
		//		gtk_tree_store_append(treestore, &toplevel,NULL);
		gtk_list_store_append(treestore, &toplevel);
		gtk_list_store_set(treestore, &toplevel,
		                   //		gtk_tree_store_set (treestore, &toplevel,
		                   //		Button_COLUMN,gMyInfo.MyPatchInfo[Loop].Index,
		                   Name_COLUMN, gMyInfo.MyPatchInfo[Loop].Name,
		                   Bank_COLUMN, gMyInfo.MyPatchInfo[Loop].BankSelect,
		                   Patch_COLUMN, gMyInfo.MyPatchInfo[Loop].Patch,
		                   Output_COLUMN, gMyInfo.OutPortName[gMyInfo.MyPatchInfo[Loop].OutPort],
		                   //		                   Output_COLUMN, gMyInfo.MyPatchInfo[Loop].OutPort,
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
*------------------------------------------------*/
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
	printd(LogDebug, "cell_edited_callback % x, % d [ % s] % d % s\n", cell, Loop, new_text,
	       user_data, path_string);

	printd(LogDebug, "PatchListEdited % x\n", treeview);

	/* Column number is passed as renderer object data */
	gpointer columnptr = g_object_get_data(G_OBJECT(cell), "column");

	column = GPOINTER_TO_UINT(columnptr);
	printd(LogDebug, "PatchListEdited Column % x % d\n", columnptr, column);

	/* Get the iterator */
	model = gtk_tree_view_get_model(treeview);

	gtk_tree_model_get_iter_from_string(model, &iter, path_string);
	printd(LogDebug, "PatchListEdited Model Iter % x % x\n", model, iter);

	/* Update the model */
	switch (column) {
	case Name_COLUMN:
	case Command_COLUMN:
		gtk_list_store_set(GTK_LIST_STORE(model), &iter, column,
		                   new_text, -1);
		break;

	case Output_COLUMN:
		//		printf("IN Output % s\n", new_text);
		gtk_list_store_set(GTK_LIST_STORE(model), &iter, column,
		                   new_text, -1);
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
		CreatePatchPopupMenu();
		break;

	case Bank_COLUMN:
		gMyInfo.MyPatchInfo[Loop].BankSelect = atoi(new_text);
		break;

	case Patch_COLUMN:
		gMyInfo.MyPatchInfo[Loop].Patch = atoi(new_text);
		break;

	case Output_COLUMN:
		// gMyInfo.MyPatchInfo[Loop].OutPort = atoi(new_text);
		for (Loop1 = 0; Loop1 <= MaxOutPorts; Loop1++) {
			if (strstr((char *)&gMyInfo.OutPortName[Loop1], new_text)) {
				gMyInfo.MyPatchInfo[Loop].OutPort = Loop1;
			}
		}
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
* Function:		create_combo_Output_model
*
* Description:		<Description/Comments>
*
*------------------------------------------------*/
GtkTreeModel *create_combo_Output_model(void) {
	GtkTreeModel *model1;
	GtkTreeIter iter;
	gint i;

	model1 = GTK_TREE_MODEL(gtk_list_store_new(1, G_TYPE_STRING));

	for (i = 0; i < MaxOutPorts; i++) {
		gtk_list_store_append(GTK_LIST_STORE(model1), &iter);
		gtk_list_store_set(GTK_LIST_STORE(model1), &iter, 0,
		                   gMyInfo.OutPortName[i], -1);
	}

	return model1;
}

/*--------------------------------------------------------------------
* Function:		create_combo_model
*
* Description:		<Description/Comments>
*
*------------------------------------------------*/
GtkTreeModel *create_combo_model(void) {
	GtkTreeModel *model;
	GtkTreeIter iter;
	gint i;

	model = GTK_TREE_MODEL(gtk_list_store_new(1, G_TYPE_STRING));

	for (i = 0; i < MaxCommands; i++) {
		gtk_list_store_append(GTK_LIST_STORE(model), &iter);
		gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0,
		                   CustomCommands[i], -1);
	}

	return model;
}

/*--------------------------------------------------------------------
* Function:		text_editing_started
*
* Description:		<Description/Comments>
*
*------------------------------------------------*/
static void text_editing_started(GtkCellRenderer ATTRIBUTE_UNUSED *cell,
                                 GtkCellEditable *editable, const gchar *path, GCallback data) {
	printd(LogDebug, "text_editing_started\n");

	if (GTK_IS_ENTRY(editable)) {
		GtkEntry *entry = GTK_ENTRY(editable);
		GCallback cb_func = data;
		//               g_signal_connect(GTK_OBJECT(entry), "activate", (GCallback)cb_func, (char *)xstrdup(path));
	}
}

#if 0
/*--------------------------------------------------------------------
* Function:		text_editing_cancelled
*
* Description:		<Description/Comments>
*
*------------------------------------------------*/
static void text_editing_cancelled(GtkCellEditable *editable,
	gpointer user_data) {
	printd(LogDebug, "text_editing_cancelled\n");
}
#endif

/*--------------------------------------------------------------------
* Function:		row_activated_cb
*
* Description:		<Description/Comments>
*
*------------------------------------------------*/
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
	printd(LogDebug, "row_activated_cb1 % x % x % x [ % x]\n", tree_view, path, column, iter);

	gtk_tree_store_set(GTK_TREE_STORE(model), &iter, PANGO_STYLE_NORMAL,
	                   PANGO_STYLE_ITALIC, -1);

}

/*--------------------------------------------------------------------
* Function:		CreatePatchViewModel
*
* Description:		<Description/Comments>
*
*------------------------------------------------*/
static GtkWidget *CreatePatchViewModel(void) {
	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;
	GtkTreeModel *model, *combomodel, *OutPortModel;
	GtkWidget *view;
	GtkTreeSelection *sel;

	view = gtk_tree_view_new();

	combomodel = create_combo_model();
	OutPortModel = create_combo_Output_model();

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
	//	g_object_set(renderer, "width-chars", 400, NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Name_COLUMN, "Name", renderer, "text", Name_COLUMN,
	        NULL);

	//	g_signal_connect(renderer, "toggled", (GCallback) PatchListEdited, view);
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Name_COLUMN));
	g_signal_connect(G_OBJECT(renderer), "editing-started",
	                 G_CALLBACK(text_editing_started), Button_COLUMN);
	// g_signal_connect(G_OBJECT(renderer), "editing-done",
	//                  G_CALLBACK(text_editing_cancelled), Button_COLUMN);



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
#if 0
	renderer = gtk_cell_renderer_text_new();
	g_object_set(renderer, "editable", TRUE, NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Output_COLUMN, "Port", renderer, "text", Output_COLUMN,
	        NULL);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Output_COLUMN));
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);
#else
	renderer = gtk_cell_renderer_combo_new();
	g_object_set(renderer, "text-column", 0, "model", OutPortModel, "mode",
	             GTK_CELL_RENDERER_MODE_ACTIVATABLE, "editable", TRUE, "has-entry",
	             FALSE,
	             NULL);
	col = (GtkTreeViewColumn *)gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Output_COLUMN, "OutPort", renderer, 
	        "text", Output_COLUMN,
//	        "default-width", 300,
	        (void *)NULL);

//	gtk_tree_view_column_set_min_width(GTK_TREE_VIEW(col), 600);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Output_COLUMN));
	g_signal_connect(renderer, "edited", (GCallback ) PatchListEdited, view);
	g_signal_connect(G_OBJECT(renderer), "editing-started",
        G_CALLBACK(text_editing_started), Output_COLUMN);

	// g_signal_connect(G_OBJECT(renderer), "editing-done",
	//                  G_CALLBACK(text_editing_cancelled), Output_COLUMN);


#endif
	/* --- Channel_COLUMN --- */
	renderer = gtk_cell_renderer_text_new();
//	gtk_tree_view_column_set_min_width (GTK_TREE_VIEW(renderer), 30);

	g_object_set(renderer, "editable", TRUE, NULL);
	col = gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Channel_COLUMN, 
	        "Channel", renderer, 
	        "text", Channel_COLUMN,
	        NULL);
#if 0	
    g_object_set(G_OBJECT(col),
                 "min-width", 100,
                 "visible", TRUE,
                 (void *) 0);
#endif
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Channel_COLUMN));
	g_signal_connect(renderer, "edited",
	                 (GCallback ) PatchListEdited, view);

	/* --- Command_COLUMN --- */
	renderer = gtk_cell_renderer_combo_new();
	g_object_set(renderer, "editable", TRUE, NULL);
	g_object_set(renderer, "text-column", 0,
	             "model", combomodel, "mode",
	             GTK_CELL_RENDERER_MODE_ACTIVATABLE,
	             "editable", TRUE, "has-entry",
	             FALSE,
	             NULL);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
	        Command_COLUMN, "Command",
	        renderer, "text", Command_COLUMN,
	        NULL);
	g_object_set_data(G_OBJECT(renderer), "column",
	                  GUINT_TO_POINTER(Command_COLUMN));
	g_signal_connect(renderer, "edited",
	                 (GCallback ) PatchListEdited, view);
	// g_signal_connect(renderer, "clicked",
	//                  (GCallback ) PatchListEdited, view);
	g_signal_connect(G_OBJECT(renderer), "editing-started",
	                 G_CALLBACK(text_editing_started), Command_COLUMN);
	// g_signal_connect(G_OBJECT(renderer), "editing-done",
	//                  G_CALLBACK(text_editing_cancelled), Command_COLUMN);

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
