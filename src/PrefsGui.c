/*------------------------------------------------
|
|   File:   PrefsGui
|
|   Contains:
|
|
|   Written By:     Elias Keshishoglou on Sun May 19 18:56:58 PDT 2013
|
|   Copyright ©:   2013 Elias Keshishoglou all rights reserved.
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
//#include "../ChartFixer/ChartFixer.h"
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

#define StringColumn1 0
#define StringColumn2 1
#define NoteNameLength 12


extern GtkBuilder *gxml;

/*
 * Place Local prototypes here
 */
static GtkWidget *CreatePatchViewModel(void);
static GtkWidget *CreateStringViewModel(void);
static GtkWidget *CreatePortsViewModel(void);
static GtkWidget *CreateLayoutsViewModel(void);
GtkTreeModel *create_Chain_model(void);
static GtkTreeModel *CreatePortsModel(void);
static GtkWidget *CreateApplicationViewModel(void);
static GtkTreeModel *CreateApplicationModel(void);

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
GtkWidget *Prefs_ReConnect;
//  GtkWidget *About_Close;

/*--------------------------------------------------------------------
 * Function:        on_BasePath_changed
 *
 * Description:     <Description/Comments>
 *
 *------------------------------------------------*/
void on_BasePath_changed(GtkButton *button, gpointer user_data) {
    printd(LogDebug, "Base string Changed %s\n", gtk_entry_get_text(button));
    strcpy(&gMyInfo.BasePath, gtk_entry_get_text(button));
}

/*--------------------------------------------------------------------
 * Function:        Prefs_click_spin_handler
 *
 * Description:     Get data from number spin boxes.
 *
 *------------------------------------------------*/
gboolean Prefs_click_spin_handler(GtkWidget *widget, GdkEvent *event,
                                  gpointer user_data) {
    printd(LogDebug, "Prefs_click_spin_handler\n");

    gint value = gtk_spin_button_get_value_as_int(widget);
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
        //  SetTempo(value);
    }

    if (widget == Midi_Base) {
        gMyInfo.MidiBaseNote = value;
        printd(LogDebug, "Midi_Base  %x %d\n", widget, value);
    }

    return TRUE; /* stop event propagation */
}

/*--------------------------------------------------------------------
 * Function:        Prefs_click_text_handler
 *
 * Description:     Get data from text boxes.
 *
 *------------------------------------------------*/
gboolean Prefs_click_text_handler(GtkWidget *widget, GdkEvent *event,
                                  gpointer user_data) {

    const gchar *theString = gtk_entry_get_text(widget);

    printd(LogDebug, "Prefs_click_text_handler\n");
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
 * Function:        Prefs_Regenerate_handler
 *
 * Description:     Generate the Index File.
 *
 *------------------------------------------------*/
gboolean Prefs_Regenerate_handler(GtkWidget *widget, GdkEvent *event,
                                  gpointer user_data) {

    printd(LogDebug, "Prefs_Regenerate_handler\n");
    system("MusicApps.sh Reindex");
}


/*--------------------------------------------------------------------
 * Function:        Prefs_ReConnect_handler
 *
 * Description:     ReConnect connections
 *
 *------------------------------------------------*/
gboolean Prefs_ReConnect_handler(GtkWidget *widget, GdkEvent *event,
                                 gpointer user_data) {

    printd(LogDebug, "Prefs_Regenerate_handler\n");
    system("MusicApps.sh connect");
}

/*--------------------------------------------------------------------
 * Function:        Prefs_about_close_handler
 *
 * Description:     Handle the close button.
 *
 *------------------------------------------------*/
gboolean Prefs_about_close_handler(GtkWidget *widget, GdkEvent *event,
                                   gpointer user_data) {

    printd(LogDebug, "Prefs_about_close_handler\n");
    gtk_widget_destroy(user_data);

}

/*--------------------------------------------------------------------
 * Function:        InitGuiPrefs
 *
 * Description:     Load the widgets from glade for the
 * preferences user interface.
 *
 *------------------------------------------------*/
void InitGuiPrefs(void) {
    GtkWidget *Patch_Pane;
    GtkWidget *String_Pane;
    GtkWidget *Port_Pane;
    GtkWidget *Application_Pane;
    GtkWidget *ApplicationListView;
    GtkWidget *Layouts_Pane;
    GtkWidget *LayoutsListView;
    GtkWidget *PatchListView;
    GtkWidget *PortListView;
    GtkWidget *StringListView;
    GtkWidget *widget;
    GtkWidget *About_Box;
    GtkWidget *MidiOutput;

    char    VersionString[200];

    printd(LogDebug, "InitGuiPrefs\n");

    //   MidiOutput = GTK_WIDGET(gtk_builder_get_object(gxml,
    //                           "MidiOutputList"));

    PatchListView = CreatePatchViewModel();
    Patch_Pane = GTK_WIDGET(gtk_builder_get_object(gxml,
                            "PatchScroller"));

    StringListView = CreateStringViewModel();
    String_Pane = GTK_WIDGET(gtk_builder_get_object(gxml,
                             "StringWindow"));

    PortListView = CreatePortsViewModel();
    Port_Pane = GTK_WIDGET(gtk_builder_get_object(gxml,
                           "PortNames"));

    LayoutsListView = CreateLayoutsViewModel();
    Layouts_Pane = GTK_WIDGET(gtk_builder_get_object(gxml,
                              "Layouts"));

    ApplicationListView = CreateApplicationViewModel();
    Application_Pane = GTK_WIDGET(gtk_builder_get_object(gxml,
                                  "ApplicationNames"));


    //  About_Box = GTK_WIDGET(gtk_builder_get_object(gxml, "dialog-vbox1"));
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

#if 0
    /* Midi output port pane.
    */
    MidiOutput = GTK_WIDGET(gtk_builder_get_object(gxml,
                            "MidiOutputList"));
    printd(LogDebug, "GTK-WIDGET %x\n", MidiOutput);
    // gtk_list_box_insert(MidiOutput, "One", 0);
    // gtk_list_box_insert(MidiOutput, "Three", 2);
    // gtk_list_box_insert(MidiOutput, "Two", 1);
#endif

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

    Prefs_Regenerate = GTK_WIDGET(gtk_builder_get_object(gxml, "Prefs_Regenerate"));
    g_signal_connect(G_OBJECT(Prefs_Regenerate),
                     "button-press-event",
                     G_CALLBACK(Prefs_Regenerate_handler),
                     &Prefs_Regenerate);

    Prefs_ReConnect = GTK_WIDGET(gtk_builder_get_object(gxml, "Prefs_ReConnect"));
    g_signal_connect(G_OBJECT(Prefs_ReConnect),
                     "button-press-event",
                     G_CALLBACK(Prefs_ReConnect_handler),
                     &Prefs_ReConnect);

    //  About_Close = GTK_WIDGET(gtk_builder_get_object(gxml, "About_Close"));

    printd(LogDebug, "Main Gui Scroll Window %x %x\n", Patch_Pane, PatchListView);
#if 0
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(Patch_Pane),
                                        GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(Patch_Pane),
                                   GTK_POLICY_AUTOMATIC,
                                   TK_POLICY_AUTOMATIC);
#endif

    sprintf(VersionString, "V=%s B=%d D=%s %s", VERSION, MY_BUILD_NUMBER, __DATE__, __TIME__);
    gtk_about_dialog_set_version(About_Box, VersionString);

    //          g_signal_connect (G_OBJECT (Patch_Pane), "destroy",
    //          G_CALLBACK (on_window1_destroy), NULL);
    gtk_container_add(GTK_CONTAINER(Patch_Pane), PatchListView);
    gtk_widget_show_all(Patch_Pane);

    gtk_container_add(GTK_CONTAINER(String_Pane), StringListView);
    gtk_widget_show_all(String_Pane);

    gtk_container_add(GTK_CONTAINER(Port_Pane), PortListView);
    gtk_widget_show_all(Port_Pane);

    gtk_container_add(GTK_CONTAINER(Layouts_Pane), LayoutsListView);
    gtk_widget_show_all(Layouts_Pane);

    gtk_container_add(GTK_CONTAINER(Application_Pane), ApplicationListView);
    gtk_widget_show_all(Application_Pane);

    widget = GTK_WIDGET(gtk_builder_get_object(gxml, "BasePath"));
    g_signal_connect_data(G_OBJECT(widget), "changed",
                          G_CALLBACK(on_BasePath_changed), NULL, NULL, 0);
    gtk_entry_set_text(widget, gMyInfo.BasePath);
}

/*--------------------------------------------------------------------
* Function:     CreatePatchModel
*
* Description:      <Description/Comments>
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

    printd(LogDebug, "GtkTreeModel\n");

    for (Loop = 0; Loop < Max_Patches; Loop++) {
        gtk_list_store_append(treestore, &toplevel);
        gtk_list_store_set(treestore, &toplevel,
                           Name_COLUMN, gMyInfo.MyPatchInfo[Loop].Name,
                           Bank_COLUMN, gMyInfo.MyPatchInfo[Loop].BankSelect,
                           Patch_COLUMN, gMyInfo.MyPatchInfo[Loop].Patch,
                           Output_COLUMN, gMyInfo.OutPortName[gMyInfo.MyPatchInfo[Loop].OutPort],
                           Channel_COLUMN, gMyInfo.MyPatchInfo[Loop].Channel,
                           Command_COLUMN, CustomCommands[gMyInfo.MyPatchInfo[Loop].CustomCommand],
                           Chain_COLUMN, gMyInfo.MyPatchInfo[Loop].Chain, -1);
    }

    return GTK_TREE_MODEL(treestore);
}

/*--------------------------------------------------------------------
* Function:     CreateStringModel
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static GtkTreeModel *CreateStringModel(void) {
    GtkTreeIter toplevel, child;
    unsigned int Loop;
    GtkListStore *treestore;

    treestore = gtk_list_store_new(2,
                                   G_TYPE_UINT,
                                   G_TYPE_STRING);

    printd(LogDebug, "CreateStringModel\n");

    for (Loop = 0; Loop < TotalMaxStrings; Loop++) {
        gtk_list_store_append(treestore, &toplevel);
        gtk_list_store_set(treestore, &toplevel,
                           StringColumn1, Loop,
                           StringColumn2, GlobalNoteNames[gMyInfo.BaseStringName[Loop]],
                           -1);
    }

    return GTK_TREE_MODEL(treestore);
}

/*--------------------------------------------------------------------
* Function:     <Function name>
*
* Description:      <Description/Comments>
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
    printd(LogDebug, "PatchListEdited\n");

    Loop = atoi(path_string);
    printd(LogDebug, "cell_edited_callback %x, %d [ %s] %d %s\n", cell, Loop, new_text,
           user_data, path_string);

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
    case Chain_COLUMN:
    case Output_COLUMN:
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
        //        strcpy(gMyInfo.MyPatchInfo[Loop].Button, new_text);
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
            if (!strcmp((char *)CustomCommands[Loop1], new_text)) {
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
* Function:     create_combo_Output_model
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
GtkTreeModel *create_combo_Output_model(void) {
    GtkTreeModel *model1;
    GtkTreeIter iter;
    gint i;

    printd(LogDebug, "create_combo_Output_model\n");

    model1 = GTK_TREE_MODEL(gtk_list_store_new(1, G_TYPE_STRING));

    for (i = 0; i < MaxOutPorts; i++) {
        gtk_list_store_append(GTK_LIST_STORE(model1), &iter);
        gtk_list_store_set(GTK_LIST_STORE(model1), &iter, 0,
                           gMyInfo.OutPortName[i], -1);
    }

    return model1;
}

/*--------------------------------------------------------------------
* Function:     create_combo_model
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
GtkTreeModel *create_combo_model(void) {
    GtkTreeModel *model;
    GtkTreeIter iter;
    gint i;

    printd(LogDebug, "create_combo_model\n");
    model = GTK_TREE_MODEL(gtk_list_store_new(1, G_TYPE_STRING));

    for (i = 0; i < MaxCommands; i++) {
        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0,
                           CustomCommands[i], -1);
    }

    return model;
}

/*--------------------------------------------------------------------
* Function:     text_editing_started
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static void text_editing_started(GtkCellRenderer ATTRIBUTE_UNUSED *cell,
                                 GtkCellEditable *editable, const gchar *path, GCallback data) {
    printd(LogDebug, "text_editing_started\n");

    if (GTK_IS_ENTRY(editable)) {
        GtkEntry *entry = GTK_ENTRY(editable);
        GCallback cb_func = data;
    }
}

/*--------------------------------------------------------------------
* Function:     CreatePatchViewModel
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static GtkWidget *CreatePatchViewModel(void) {
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    GtkTreeModel *model, *combomodel, *OutPortModel, *ChainModel;
    GtkWidget *view;
    GtkTreeSelection *sel;

    printd(LogDebug, "CreatePatchViewModel\n");

    view = gtk_tree_view_new();

    combomodel = create_combo_model();
    OutPortModel = create_combo_Output_model();
    ChainModel = create_Chain_model();

    /* --- Name_COLUMN --- */
    // Can't change column width
    renderer = gtk_cell_renderer_text_new();

    g_object_set(renderer,
                 "editable", TRUE,
                 "editable-set", TRUE,
                 //                "width-chars", 300,
                 //                "wrap-width", 300,
                 NULL);

    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            Name_COLUMN, "Name", renderer, "text", Name_COLUMN,
            NULL);

    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(Name_COLUMN));
    g_signal_connect(renderer, "edited",
                     (GCallback) PatchListEdited, view);

#if 0
    col = gtk_tree_view_column_new_with_attributes((const gchar*)"Name",
            renderer,
            "editable", TRUE,
            "editable-set", TRUE,
            "width-chars", 300,
            "wrap-width", 300,
            NULL);

    gtk_tree_view_column_set_clickable(GTK_TREE_VIEW_COLUMN(col), TRUE);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_column_set_min_width(col, 300);

    //    g_object_set_data(G_OBJECT(renderer), "column",
    //                      GUINT_TO_POINTER(Name_COLUMN));

    //   g_signal_connect(G_OBJECT(renderer), "editing-started",
    //                    G_CALLBACK(text_editing_started), Button_COLUMN);
#endif

    /* --- Bank_COLUMN --- */
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            Bank_COLUMN, "Bank", renderer, "text", Bank_COLUMN,
            NULL);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(Bank_COLUMN));
    g_signal_connect(renderer, "edited", (GCallback) PatchListEdited, view);

    /* --- Patch_COLUMN --- */
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            Patch_COLUMN, "Patch", renderer, "text", Patch_COLUMN,
            NULL);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(Patch_COLUMN));
    g_signal_connect(renderer, "edited", (GCallback) PatchListEdited, view);

    renderer = gtk_cell_renderer_combo_new();
    g_object_set(renderer, "text-column", 0, "model", OutPortModel, "mode",
                 GTK_CELL_RENDERER_MODE_ACTIVATABLE, "editable", TRUE, "has-entry",
                 FALSE,
                 NULL);

    col = (GtkTreeViewColumn *)gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            Output_COLUMN, "Patch", renderer,
            "text", Output_COLUMN,
            // "width-chars", 300,
            //  "wrap-width", 300,
            //          "default-width", 300,
            (void *)NULL);

    //  gtk_tree_view_column_set_min_width(GTK_TREE_VIEW(col), 600);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(Output_COLUMN));
    g_signal_connect(renderer, "edited", (GCallback) PatchListEdited, view);
    g_signal_connect(G_OBJECT(renderer), "editing-started",
                     G_CALLBACK(text_editing_started), Output_COLUMN);

    /* --- Channel_COLUMN --- */
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    col = gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            Channel_COLUMN,
            "Channel", renderer,
            "text", Channel_COLUMN,
            NULL);

    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(Channel_COLUMN));
    g_signal_connect(renderer, "edited",
                     (GCallback) PatchListEdited, view);

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
                     (GCallback) PatchListEdited, view);
    g_signal_connect(G_OBJECT(renderer), "editing-started",
                     G_CALLBACK(text_editing_started), Command_COLUMN);

#if 1
    /* --- Patch_COLUMN --- */
    renderer = gtk_cell_renderer_combo_new();
    g_object_set(renderer, "text-column", 0, "model", ChainModel, "mode",
                 GTK_CELL_RENDERER_MODE_ACTIVATABLE,
                 "editable", TRUE,
                 "has-entry", FALSE,
                 NULL);
    col = (GtkTreeViewColumn *)gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            Chain_COLUMN, "Chain", renderer,
            "text", Chain_COLUMN,
            //          "default-width", 300,
            (void *)NULL);

    //  gtk_tree_view_column_set_min_width(GTK_TREE_VIEW(col), 600);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(Chain_COLUMN));
    g_signal_connect(renderer, "edited", (GCallback) PatchListEdited, view);
    g_signal_connect(G_OBJECT(renderer), "editing-started",
                     G_CALLBACK(text_editing_started), Chain_COLUMN);


#else
    /* --- Chain_COLUMN --- */
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    g_object_set(renderer, "text-column", 0,
                 "model", ChainModel, "mode",
                 GTK_CELL_RENDERER_MODE_ACTIVATABLE,
                 "editable", TRUE, "has-entry",
                 FALSE,
                 NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            Chain_COLUMN, "Chain", renderer,
            "text", Chain_COLUMN,
            NULL);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(Chain_COLUMN));
    g_signal_connect(renderer, "edited", (GCallback) PatchListEdited, view);
    //    g_object_set(renderer, "editable-set", TRUE, NULL);
    g_signal_connect(G_OBJECT(renderer), "editing-started",
                     G_CALLBACK(text_editing_started), Chain_COLUMN);
#endif

    // This is the large main patch list.
    model = CreatePatchModel();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

    return view;
}

/*--------------------------------------------------------------------
* Function:     create_combo_model
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
GtkTreeModel *create_NoteName_model(void) {
    GtkTreeModel *model;
    GtkTreeIter iter;
    gint i;

    printd(LogDebug, "create_combo_model\n");
    model = GTK_TREE_MODEL(gtk_list_store_new(1, G_TYPE_STRING));

    for (i = 0; i <= NoteNameLength; i++) {
        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0,
                           GlobalNoteNames[i], -1);
    }

    return model;
}

/*--------------------------------------------------------------------
* Function:     create_Chain_model
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
GtkTreeModel *create_Chain_model(void) {
    GtkTreeModel *model;
    GtkTreeIter iter;
    gint i;

    printd(LogDebug, "create_Chain_model\n");
    model = GTK_TREE_MODEL(gtk_list_store_new(1, G_TYPE_STRING));
    gtk_list_store_append(GTK_LIST_STORE(model), &iter);
    gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0,
                       "None", -1);

    for (i = 0; i < Max_Patches; i++) {
        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0,
                           gMyInfo.MyPatchInfo[i].Name, -1);
    }

    return model;
}

/*--------------------------------------------------------------------
* Function:     StringListEdited
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static void StringListEdited(GtkCellRendererText * cell, gchar * path_string,
                             gchar * new_text, gpointer user_data) {
    GtkTreeView *treeview = (GtkTreeView *) user_data;
    GtkTreeModel *model;
    GtkTreeIter iter;

    guint column;
    int Loop;
    int Loop1;

    GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
    printd(LogDebug, "StringListEdited\n");

    Loop = atoi(path_string);
    printd(LogDebug, "cell_edited_callback %x, %d [ %s] %d %s\n", cell, Loop, new_text,
           user_data, path_string);

    printd(LogDebug, "StringListEdited %x\n", treeview);

    // Loop, new_text

    /* Column number is passed as renderer object data */
    gpointer columnptr = g_object_get_data(G_OBJECT(cell), "column");

    column = GPOINTER_TO_UINT(columnptr);
    printd(LogDebug, "StringListEdited Column %x %d\n", columnptr, column);

    /* Get the iterator */
    model = gtk_tree_view_get_model(treeview);

    gtk_tree_model_get_iter_from_string(model, &iter, path_string);
    printd(LogDebug, "StringListEdited Model Iter %x %x\n", model, iter);

    switch (column) {
    case StringColumn1:
        break;

    case StringColumn2:
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, column,
                           new_text, -1);
        for (Loop1 = 0; Loop1 <= NoteNameLength; Loop1++) {
            printd(LogDebug, "Assignment %d,[%s] [%s]\n", Loop1, GlobalNoteNames[Loop1], new_text);
            if (!strcmp((char *)GlobalNoteNames[Loop1], new_text)) {
                printd(LogDebug, "FOUND %d,%s %s\n", Loop1, GlobalNoteNames[Loop1], new_text);
                gMyInfo.BaseStringName[Loop] = Loop1;
            }
        }
        break;
    }
}

/*--------------------------------------------------------------------
* Function:     CreateStringViewModel
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static GtkWidget *CreateStringViewModel(void) {
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    GtkTreeModel *model, *combomodel;
    GtkWidget *view;
    GtkTreeSelection *sel;

    printd(LogDebug, "CreateStringViewModel\n");

    view = gtk_tree_view_new();
    combomodel = create_NoteName_model();

    /* --- String Number --- */
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", FALSE, NULL);

    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            StringColumn1, "Str #", renderer, "text", StringColumn1,
            NULL);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(StringColumn1));
    //    g_signal_connect(renderer, "edited", (GCallback) StringListEdited, view);

    /* --- String Name --- */
    renderer = gtk_cell_renderer_combo_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    //    g_object_set(renderer, "editable-set", TRUE, NULL);
    g_object_set(renderer, "text-column", 0,
                 "model", combomodel, "mode",
                 GTK_CELL_RENDERER_MODE_ACTIVATABLE,
                 "editable", TRUE, "has-entry",
                 FALSE,
                 NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            StringColumn2, "Str Name",
            renderer, "text", StringColumn2,
            NULL);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(StringColumn2));
    g_signal_connect(renderer, "edited",
                     (GCallback) StringListEdited, view);
    g_signal_connect(G_OBJECT(renderer), "editing-started",
                     G_CALLBACK(text_editing_started), StringColumn2);

    // This is the large main patch list.
    model = CreateStringModel();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

    return view;
}

/*--------------------------------------------------------------------
* Function:     PortsListEdited
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static void PortsListEdited(GtkCellRendererText * cell, gchar * path_string,
                            gchar * new_text, gpointer user_data) {
    GtkTreeView *treeview = (GtkTreeView *) user_data;
    GtkTreeModel *model;
    GtkTreeIter iter;

    guint column;
    int Loop;
    int Loop1;

    GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
    printd(LogDebug, "PortsListEdited\n");

    Loop = atoi(path_string);
    printd(LogDebug, "cell_edited_callback %x, %d [ %s] %d %s\n", cell, Loop, new_text,
           user_data, path_string);

    printd(LogDebug, "PortsListEdited %x\n", treeview);

    // Loop, new_text

    /* Column number is passed as renderer object data */
    gpointer columnptr = g_object_get_data(G_OBJECT(cell), "column");

    column = GPOINTER_TO_UINT(columnptr);
    printd(LogDebug, "PortsListEdited Column %x %d\n", columnptr, column);

    /* Get the iterator */
    model = gtk_tree_view_get_model(treeview);

    gtk_tree_model_get_iter_from_string(model, &iter, path_string);
    printd(LogDebug, "PortsListEdited Model Iter %x %x\n", model, iter);

    switch (column) {
    case StringColumn1:
        break;

    case StringColumn2:
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, column,
                           new_text, -1);
        strcpy(&gMyInfo.OutPortName[Loop], new_text);
        printf("PortsListEdited Copied\n %d %s %s\n",
               Loop,
               new_text,
               gMyInfo.OutPortName[Loop]);
        break;
    }
}

/*--------------------------------------------------------------------
* Function:     CreatePortsModel
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static GtkTreeModel *CreatePortsModel(void) {
    GtkTreeIter toplevel, child;
    unsigned int Loop;
    GtkListStore *treestore;

    treestore = gtk_list_store_new(2,
                                   G_TYPE_UINT,
                                   G_TYPE_STRING);

    printd(LogDebug, "CreatePortsModel\n");

    for (Loop = 0; Loop < TotalMaxStrings; Loop++) {
        gtk_list_store_append(treestore, &toplevel);
        gtk_list_store_set(treestore, &toplevel,
                           StringColumn1, Loop,
                           StringColumn2, gMyInfo.OutPortName[Loop],
                           -1);
    }
    return GTK_TREE_MODEL(treestore);
}

/*--------------------------------------------------------------------
* Function:     CreatePortsViewModel
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static GtkWidget *CreatePortsViewModel(void) {
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    GtkTreeModel *model;
    GtkWidget *view;
    GtkTreeSelection *sel;

    printd(LogDebug, "CreatePortsViewModel\n");

    view = gtk_tree_view_new();

    /* --- Port Number --- */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            StringColumn1, "Port #", renderer, "text", StringColumn1,
            NULL);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(StringColumn1));
    g_signal_connect(renderer, "edited", (GCallback) PortsListEdited, view);

    /* --- Port Name --- */
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    //    g_object_set(renderer, "editable-set", TRUE, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            StringColumn2, "Port Name", renderer, "text", StringColumn2,
            NULL);
    g_signal_connect(renderer, "edited", (GCallback) PortsListEdited, view);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(StringColumn2));
    g_signal_connect(G_OBJECT(renderer), "editing-started",
                     G_CALLBACK(text_editing_started), StringColumn2);


    // This is the large main patch list.
    model = CreatePortsModel();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

    return view;
}

/*--------------------------------------------------------------------
* Function:     CreateLayoutModel
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static GtkTreeModel *CreateLayoutModel(void) {
    GtkTreeIter toplevel, child;
    unsigned int Loop;
    GtkListStore *treestore;

    treestore = gtk_list_store_new(2,
                                   G_TYPE_UINT,
                                   G_TYPE_STRING);

    printd(LogDebug, "CreateLayoutModel\n");

    for (Loop = 0; Loop < Max_Layouts; Loop++) {
        gtk_list_store_append(treestore, &toplevel);
        gtk_list_store_set(treestore, &toplevel,
                           StringColumn1, Loop,
                           StringColumn2, gMyInfo.LayoutPresets[Loop].Name,
                           -1);
    }

    return GTK_TREE_MODEL(treestore);
}





/*--------------------------------------------------------------------
* Function:     LayoutsListEdited
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static void LayoutsListEdited(GtkCellRendererText * cell, gchar * path_string,
                              gchar * new_text, gpointer user_data) {
    GtkTreeView *treeview = (GtkTreeView *) user_data;
    GtkTreeModel *model;
    GtkTreeIter iter;

    guint column;
    int Loop;
    int Loop1;

    GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
    printd(LogDebug, "LayoutsListEdited\n");

    Loop = atoi(path_string);
    printd(LogDebug, "cell_edited_callback %x, %d [ %s] %d %s\n", cell, Loop, new_text,
           user_data, path_string);

    printd(LogDebug, "LayoutsListEdited %x\n", treeview);

    // Loop, new_text

    /* Column number is passed as renderer object data */
    gpointer columnptr = g_object_get_data(G_OBJECT(cell), "column");

    column = GPOINTER_TO_UINT(columnptr);
    printd(LogDebug, "LayoutsListEdited Column %x %d\n", columnptr, column);

    /* Get the iterator */
    model = gtk_tree_view_get_model(treeview);

    gtk_tree_model_get_iter_from_string(model, &iter, path_string);
    printd(LogDebug, "LayoutsListEdited Model Iter %x %x\n", model, iter);

    switch (column) {
    case StringColumn1:
        break;

    case StringColumn2:
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, column,
                           new_text, -1);
        printf("LayoutsListEdited Copy \n %d %s\n",
               Loop,
               new_text);
        strcpy(&gMyInfo.LayoutPresets[Loop].Name, new_text);
        break;
    }
}


/*--------------------------------------------------------------------
* Function:     CreateLayoutsViewModel
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static GtkWidget *CreateLayoutsViewModel(void) {
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    GtkTreeModel *model;
    GtkWidget *view;
    GtkTreeSelection *sel;

    printd(LogDebug, "CreateLayoutsViewModel\n");

    view = gtk_tree_view_new();

    /* --- Port Number --- */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            StringColumn1, "Layout #", renderer, "text", StringColumn1,
            NULL);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(StringColumn1));
    g_signal_connect(renderer, "edited", (GCallback) LayoutsListEdited, view);

    /* --- Port Name --- */
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    //    g_object_set(renderer, "editable-set", TRUE, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            StringColumn2, "Layout Name", renderer, "text", StringColumn2,
            NULL);
    g_signal_connect(renderer, "edited", (GCallback) LayoutsListEdited, view);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(StringColumn2));
    g_signal_connect(G_OBJECT(renderer), "editing-started",
                     G_CALLBACK(text_editing_started), StringColumn2);


    // This is the large main patch list.
    model = CreateLayoutModel();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

    return view;
}





/*--------------------------------------------------------------------
* Function:     ApplicationsListEdited
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static void ApplicationsListEdited(GtkCellRendererText * cell, gchar * path_string,
                                   gchar * new_text, gpointer user_data) {
    GtkTreeView *treeview = (GtkTreeView *) user_data;
    GtkTreeModel *model;
    GtkTreeIter iter;

    guint column;
    int Loop;
    int Loop1;

    GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
    printd(LogDebug, "ApplicationsListEdited\n");

    Loop = atoi(path_string);
    printd(LogDebug, "cell_edited_callback %x, %d [ %s] %d %s\n", cell, Loop, new_text,
           user_data, path_string);

    printd(LogDebug, "ApplicationsListEdited %x\n", treeview);

    // Loop, new_text

    /* Column number is passed as renderer object data */
    gpointer columnptr = g_object_get_data(G_OBJECT(cell), "column");

    column = GPOINTER_TO_UINT(columnptr);
    printd(LogDebug, "ApplicationsListEdited Column %x %d\n", columnptr, column);

    /* Get the iterator */
    model = gtk_tree_view_get_model(treeview);

    gtk_tree_model_get_iter_from_string(model, &iter, path_string);
    printd(LogDebug, "ApplicationsListEdited Model Iter %x %x\n", model, iter);

    switch (column) {
    case StringColumn1:
        break;

    case StringColumn2:
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, column,
                           new_text, -1);
        printf("ApplicationsListEdited Copy \n %d %s\n",
               Loop,
               new_text);
        strcpy(&gMyInfo.Apps[Loop].Name, new_text);
        break;
    }
}




/*--------------------------------------------------------------------
* Function:     CreateApplicationViewModel
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static GtkWidget *CreateApplicationViewModel(void) {
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    GtkTreeModel *model;
    GtkWidget *view;
    GtkTreeSelection *sel;

    printd(LogDebug, "CreateApplicationViewModel\n");

    view = gtk_tree_view_new();

    /* --- Port Number --- */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            StringColumn1, "Application #", renderer, "text", StringColumn1,
            NULL);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(StringColumn1));
    g_signal_connect(renderer, "edited", (GCallback) ApplicationsListEdited, view);

    /* --- Port Name --- */
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    //    g_object_set(renderer, "editable-set", TRUE, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            StringColumn2, "Application Name", renderer, "text", StringColumn2,
            NULL);
    g_signal_connect(renderer, "edited", (GCallback) ApplicationsListEdited, view);
    g_object_set_data(G_OBJECT(renderer), "column",
                      GUINT_TO_POINTER(StringColumn2));
    g_signal_connect(G_OBJECT(renderer), "editing-started",
                     G_CALLBACK(text_editing_started), StringColumn2);


    // This is the large main patch list.
    model = CreateApplicationModel();
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

    return view;
}

/*--------------------------------------------------------------------
* Function:     CreateApplicationModel
*
* Description:      <Description/Comments>
*
*------------------------------------------------*/
static GtkTreeModel *CreateApplicationModel(void) {
    GtkTreeIter toplevel, child;
    unsigned int Loop;
    GtkListStore *treestore;

    treestore = gtk_list_store_new(2,
                                   G_TYPE_UINT,
                                   G_TYPE_STRING);

    printd(LogDebug, "CreateStringModel\n");

    for (Loop = 0; Loop < MaxApps; Loop++) {
        gtk_list_store_append(treestore, &toplevel);
        gtk_list_store_set(treestore, &toplevel,
                           StringColumn1, Loop,
                           StringColumn2, gMyInfo.Apps[Loop].Name,
                           -1);
    }

    return GTK_TREE_MODEL(treestore);
}


