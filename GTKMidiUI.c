/*---------------------------------------------------------------------
 *	Revision Date:	 2012/01/15 22:52:40
 *
 *	Contains:	<Type in description>
 *
 *	Written by:	Elias Kesh
 *
 *	Date:		<Creation Date>
 *
 *	Copyright © 2012 Elias Kesh.  All rights reserved.
 *
 *	Change History (located at end of file)
 *
 *---------------------------------------------------------------------*/

#define GTKMidiUI_c

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "stdlib.h"
#include <string.h>
#include "stdbool.h"
#include "GTKMidiUI.h"
#include "AlsaUtils.h"
#include "HTML.h"
#include "PrefsFile.h"
#include "Connections.h"
#include <gdk/gdkkeysyms.h>

//#define UsingNewButtons	1

#ifdef UsingNewButtons
#define GLADE_FILE ResourceDirectory"GTKMidiUI.glade.Buttons"
#else
#define GLADE_FILE ResourceDirectory"GTKMidiUI.glade"
#endif

/*
 * Place Global variables here
 */
snd_seq_t *seq;
int seqPort;
//	GladeXML *gxml;
GtkBuilder *gxml;
GtkWidget* MainStatus;
GtkWidget* CurrentModeWid;
GtkWidget *TempoDraw;
unsigned int CurrentMode;
guint MainStatusid;
GtkWidget *ModeSwitchButton;
GtkWidget *MainButtons[Max_Main_Buttons];
tPatchIndex GetModePreset(tPatchIndex Value);
GtkWidget *VScale1, *VScale2, *VScale3;
GtkAdjustment *Adjustment1, *Adjustment2, *Adjustment3;



/* Used to Toggle the Tempo GUI display.
 */
char TempoState;
// static gboolean time_handler(GtkWidget *widget);

/*
 * Place Local prototypes here
 */

void CreateMainButtons(void);
void SetUpMainButtons(PatchInfo *MyPatchInfo);
void PrintDataStructure(GTKMidiInfo *myInfo);
void CheckConnectionData(GTKMidiInfo *myInfo);
void CreateHTMLGuide(GTKMidiInfo *myInfo);
void apply_font_to_widget(GtkWidget *widget, gchar *fload);
tPatchIndex DoPatch(PatchInfo *thePatch);
void ToggleTempo(void);
static gboolean tempo_handler(GtkWidget *widget);
void UpdateStatus(char *String);
void IncrementMode(void);
gint button_press_notify_cb(GtkWidget *entries, GdkEventKey *event,
                            GtkWidget *widget);
void VScale1_Changed(GtkAdjustment *adj);
void VScale2_Changed(GtkAdjustment *adj);


#define MaxStatusHold 4
char HoldStatus[MaxStatusHold][50];
char HoldStatusIndex;

/*--------------------------------------------------------------------
 * Function:		printd
 *
 * Description:		Loggable print command.
 *
 *---------------------------------------------------------------------*/
char *printd(char LogLevel, const char *fmt, ...) {
    va_list ap;
    char p[512]; /* check for null in real code */
    va_start(ap, fmt);
    vsnprintf(p, 512, fmt, ap);
    va_end(ap);
    printf("L%d-%s", LogLevel, p);
    return p;
}
/*--------------------------------------------------------------------
 * Function:		apply_font_to_widget
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void apply_font_to_widget(GtkWidget *widget, gchar *fload) {
#if 0
    GtkStyle *style = gtk_style_new();
    gdk_font_unref(style->font_desc);
    style->font_desc = gdk_font_load(fload);
    style->font_desc = gdk_font_load("12x24");
//	gtk_style_set_font(style,fload);
    gtk_widget_set_style(GTK_WIDGET(widget), style);
#else
    PangoFontDescription *pfd;
    pfd = pango_font_description_from_string(fload);

    if (GTK_IS_LABEL(widget))
        gtk_widget_modify_font(widget, pfd);
    else
        gtk_widget_modify_font(GTK_WIDGET(gtk_bin_get_child (GTK_BIN(widget))), pfd);

    pango_font_description_free(pfd);
#endif
}

/*--------------------------------------------------------------------
 * Function:		SwitchToTab
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void SwitchToTab(char Tab) {
    GtkWidget *NoteBookPane;
    NoteBookPane = GTK_WIDGET(gtk_builder_get_object(gxml, "MainTab"));
    gtk_notebook_set_current_page(GTK_NOTEBOOK(NoteBookPane), Tab);
    printd(LogInfo, "Switch to Tab %d\n", Tab);
}

/*--------------------------------------------------------------------
 * Function:		on_button_clicked
 *
 * Description:		When the uses presses one of the main screen patch
 * 	buttons we get dispatched here.
 *
 *---------------------------------------------------------------------*/
void on_button_clicked(GtkButton *button, gpointer user_data) {
	tPatchIndex	PatchIndex;

	PatchIndex = ModeSwitchPatch(user_data);
	DoPatch(&gMyInfo.MyPatchInfo[(char) PatchIndex]);
    printd(LogInfo, "User data %d\n", (int) user_data);
}

/*--------------------------------------------------------------------
 * Function:		on_modebutton_clicked
 *
 * Description:		When the mode button is pressed from the screen we call
 * 	this.
 *
 *---------------------------------------------------------------------*/
void on_modebutton_clicked(GtkButton *button, gpointer user_data) {

    IncrementMode();
    printd(LogInfo, "Increment Mode called from Buttons \n");
}

/*--------------------------------------------------------------------
 * Function:		on_About_clicked
 *
 * Description:		It's all about ME !
 *
 *---------------------------------------------------------------------*/
void on_About_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window;

    window = GTK_WIDGET(gtk_builder_get_object(gxml, "AboutDialog"));
    gtk_widget_show(window);
}

/*--------------------------------------------------------------------
 * Function:		on_hscale1_value_changed
 *
 * Description:		When the sliders are changed on the main screen.
 *
 *---------------------------------------------------------------------*/
void on_hscale1_value_changed(GtkWidget *widget, gpointer user_data) {
    gdouble val;

    val = gtk_range_get_value(GTK_RANGE(widget));

    /* print to screen */
    printd(LogInfo, "Range value: %d\n", (guint) val);
    SendMidi(SND_SEQ_EVENT_CONTROLLER, 0, DefaultMidiChannel, 07, (int) val);
}

/*--------------------------------------------------------------------
 * Function:		on_window1_destroy
 *
 * Description:		That's IT, I'm outta here.
 *
 *---------------------------------------------------------------------*/
void on_window1_destroy(GtkWidget *widget, gpointer user_data) {
    /* break gtk_main() loop */
    gtk_main_quit();
}

/*--------------------------------------------------------------------
 * Function:		on_Tempo_Button
 *
 * Description:		Turn the Metrognome on or off
 *
 *---------------------------------------------------------------------*/
void on_Tempo_Button(GtkWidget *widget, gpointer user_data) {
    if (gMyInfo.MetronomeOn)
        gMyInfo.MetronomeOn = 0;
    else
        gMyInfo.MetronomeOn = 1;
}

/*--------------------------------------------------------------------
 * Function:		tab_focus_callback
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
gboolean tab_focus_callback(GtkNotebook *notebook, gint *arg1, gpointer data) {
    //  GtkTreeView* view = (GtkTreeView *)data;
//printd(LogInfo,"tab_focus_callback %x %x %x\n", notebook, arg1, data);
    return true;
}

/*--------------------------------------------------------------------
 * Function:		main
 *
 * Description:		This is where it all starts.
 *
 *---------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    GtkWidget *main_window;
    GtkWidget *main_tab;
    GtkWidget *widget;
    GError *error = NULL;
    GtkWidget *ChordWidget;

    /*
     * Let's setup some variables.
     */
    CurrentMode = 0;
    WaitingforMidi = 0;

    /* initialize the GTK+ library */
    gtk_init(&argc, &argv);
    gtk_rc_parse( MAINPREFS_FILE);

    /*
     * Get Preferences is there are there otherwise set defaults.
     */
    InitPref();
    /*
     create an instance of the GladeXML object and build widgets within
     the window1 root node.
     */
    gxml = gtk_builder_new();
    if (!gtk_builder_add_from_file(gxml, GLADE_FILE, &error)) {
        g_warning("Couldn't load builder file: %s", error->message);
        g_error_free(error);
    }

    /*
     * get the window widget from the glade XML file
     */
    main_window = GTK_WIDGET(gtk_builder_get_object(gxml, "window1"));
    g_signal_connect(G_OBJECT (main_window), "destroy",
                     G_CALLBACK (on_window1_destroy), NULL);
    gtk_window_set_title(GTK_WINDOW(main_window), "LiveMusicApp");

    /*
     * Open the persistent main tab.
     */
    main_tab = GTK_WIDGET(gtk_builder_get_object(gxml, "MainTab"));
    g_signal_connect(GTK_NOTEBOOK( main_tab ), "switch-page",
                     (GCallback ) tab_focus_callback, gxml);

    /*
     * Setup and initialize our statusbar and Mode indicator
     */
    MainStatus = GTK_WIDGET(gtk_builder_get_object(gxml, "MainStatusBar"));
    CurrentModeWid = GTK_WIDGET(gtk_builder_get_object(gxml, "CurrentMode"));

    /*
     * Clear the Status bar buffer.
     */
    HoldStatusIndex = 0;
    memset(HoldStatus, 0, sizeof(HoldStatus));
    TempoDraw = GTK_WIDGET(gtk_builder_get_object(gxml, "Tempo"));
    g_signal_connect_data(G_OBJECT(TempoDraw), "clicked",
                          G_CALLBACK(on_Tempo_Button), NULL, NULL, 0);

    /*
     * The about window.
     */
    widget = GTK_WIDGET(gtk_builder_get_object(gxml, "AboutButton"));
    g_signal_connect_data(G_OBJECT(widget), "clicked",
                          G_CALLBACK(on_About_clicked), NULL, NULL, 0);

    g_signal_connect(G_OBJECT(main_window), "key_press_event",
                     G_CALLBACK(button_press_notify_cb), NULL);


    /*
     * Debug, this prints out the main internal data structure.
     */
//		PrintDataStructure(&gMyInfo);
    /*
     * Set up the GUI for making changes to the preferences.
     */
    InitGuiPrefs();

    /*
     * Create the HTML page from our preferences.
     */
    CreateHTMLGuide(&gMyInfo);

    /*
     * Initialize the WebKit (HTML) engine
     */
    InitHTML(gxml);

    printd(LogInfo, "After InitHTML\n");

    /* Call the Jackd
     * jackd -R -t5000 -dalsa -Chw:$AudioInHW$DeviceAdder -Phw:$AudioOutHW$DeviceAdder -r44100 -p256 -n3
     */
//		system( );
//
    /*
     * Set up the buttons, text and patches.
     */
    CreateMainButtons();
    SetUpMainButtons(&gMyInfo.MyPatchInfo);


    /* Get the Mode switch button,
     */
    ModeSwitchButton = GTK_WIDGET(
                           gtk_builder_get_object(gxml, "ModeSwitchButton"));
    //gtk_label_set_text(GTK_LABEL(GTK_BIN(myButton)->child), gMyInfo.MyPatchInfo[Loop].Name);
    g_signal_connect_data(G_OBJECT(ModeSwitchButton), "clicked",
                          G_CALLBACK(on_modebutton_clicked), NULL, NULL, 0);

    /*
     * Set up the Midi Sequencer port
     */
    MyAlsaInit();
    printd(LogInfo, "After MyAlsaInit\n");

    /*
     * Set up the connections between applications.
     */
    InitConnections();

    /*
     * Set the up Chord page in case we need it later.
     */
    ChordWidget = GTK_WIDGET(gtk_builder_get_object(gxml, "ChordFrame"));
    ChorderMain(main_window, ChordWidget);

    /*
     * Set up a timer for Tempo.
     */
//		g_timeout_add(Timer1Ticks, (GSourceFunc) time_handler, (gpointer) gxml);
    gMyInfo.TempoTimerID = 0;
    SetTempo(90);

    /*
     * Show the main window and let the show begin.
     */
    gtk_widget_show_all(main_window);
    gtk_widget_modify_font(CurrentModeWid,
                           pango_font_description_from_string("Sans Bold 16"));
    gtk_label_set_text(CurrentModeWid, theModes[0]);

    /*
     * Set the initial Volumes.
     */
    SetVolume1(gMyInfo.AnalogVolume);
    SetVolume2(gMyInfo.MidiVolume);
//	create_Popup_view(main_window);
    /*
     * And they're off.
     */
    gtk_main();

    /*
     * After we quit we should write back the changes.
     */
    WritePref();

    return 0;
}

/*--------------------------------------------------------------------
 * Function:		Update the display status
 * Description:		raw the previous patches on the screen to make
 * 	sure that they completed. It's hard to tell with a foot pedal .
 *
 *---------------------------------------------------------------------*/
void UpdateStatus(char *String) {
    GtkStyle *hold;
    char DisString[160];

#if 0
    printd(LogInfo,"Status Update %d\n", HoldStatusIndex);
    printd(LogInfo,"%s\n", &HoldStatus[0]);
    printd(LogInfo,"%s\n", &HoldStatus[1]);
    printd(LogInfo,"%s\n", &HoldStatus[2]);
    printd(LogInfo,"%s\n", &HoldStatus[3]);
#endif

    /* Based on where we are in the circlular buffer.
     */
    switch (HoldStatusIndex) {
    case 0:
        sprintf(DisString, "[%12s] [%12s] [%12s] [%12s]", &HoldStatus[1],
                &HoldStatus[2], &HoldStatus[3], String);
        break;

    case 1:
        sprintf(DisString, "[%12s] [%12s] [%12s] [%12s]", &HoldStatus[2],
                &HoldStatus[3], &HoldStatus[0], String);
        break;

    case 2:
        sprintf(DisString, "[%12s] [%12s] [%12s] [%12s]", &HoldStatus[3],
                &HoldStatus[0], &HoldStatus[1], String);
        break;

    case 3:
        sprintf(DisString, "[%12s] [%12s] [%12s] [%12s]", &HoldStatus[0],
                &HoldStatus[1], &HoldStatus[2], String);
        break;
    }

    /* Copy the patch names the the buffer.
     */
    strncpy(&HoldStatus[HoldStatusIndex++], String, 50);

    /* Reset the circular list of we have to.
     */
    if (HoldStatusIndex >= MaxStatusHold)
        HoldStatusIndex = 0;

    /* Actually draw the text to the window.
     */
    gtk_widget_modify_font(MainStatus,
                           pango_font_description_from_string("Sans Bold 16"));
    gtk_label_set_text(MainStatus, DisString);
}

/*--------------------------------------------------------------------
 * Function:		Set the tempo
 * Description:		Set te tempo to a new value. Re-setup the timer
 * 	interrupts to handle double te tempo.
 *
 *---------------------------------------------------------------------*/
void SetTempo(unsigned char NewTempo) {

    /* Send out a message our tempo is changing.
     */
    SendMidi(SND_SEQ_EVENT_TEMPO, TempoPort, DefaultMidiChannel, 0,
             (int) NewTempo);

    /* Tell the timer to stop.
     */
    if (gMyInfo.TempoTimerID)
        g_source_remove(gMyInfo.TempoTimerID);

    /* Store the tempo information.
     */
    gMyInfo.Tempo = NewTempo;
//	gMyInfo.TempoReload = (500 * 60) / NewTempo;
    gMyInfo.TempoReload = ((500 * 5) / NewTempo);

    printd(LogInfo, "New Tempo %d Val  %d\n", NewTempo, gMyInfo.TempoReload);

    /* Start the new timer.
     */
    gMyInfo.TempoTimerID = g_timeout_add(gMyInfo.TempoReload,
                                         (GSourceFunc) tempo_handler, (gpointer) gxml);

//	gMyInfo.Timer1Count = 0;

}

/*--------------------------------------------------------------------
 * Function:		Tempo callback.
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static gboolean tempo_handler(GtkWidget *widget) {

    /* HANDE Tempo Midi
     */
    ToggleTempo();

    return TRUE;
}

#if 0
/*--------------------------------------------------------------------
 * Function:		Timer Callback
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
static gboolean time_handler(GtkWidget *widget) {

//	printd(LogInfo," IN Timer call back\n");
    return TRUE;
}
#endif
/*--------------------------------------------------------------------
 * Function:		ToggleTempo
 *
 * Description:		Toggle based on the common tempo.
 * 	Tempostate is the current index * 24
 *
 *---------------------------------------------------------------------*/
void ToggleTempo(void) {
    GdkColor fgcolor;
    GdkColor bgcolor;
    char StrBuf[10];

    if (TempoState >= (gMyInfo.TempoMax * 12))
        TempoState = 0;

    if (!(TempoState % 12)) {
//		gdk_color_parse ("white", &bgcolor);
//		gdk_color_parse ("Black", &fgcolor);

        /* On the first beat plat a different sound.
         */
        if (gMyInfo.MetronomeOn)
            if (TempoState)
                SendMidi(SND_SEQ_EVENT_NOTEON, TempoPort,
                         DrumMidiChannel, 00, (int) 35);
            else
                SendMidi(SND_SEQ_EVENT_NOTEON, TempoPort,
                         DrumMidiChannel, 00, (int) 34);

        if (gMyInfo.MetronomeOn)
            sprintf(StrBuf, "On   %d", (TempoState / 24) + 1);
        else
            sprintf(StrBuf, "Off  %d", (TempoState / 24) + 1);

        PangoFontDescription *font_desc;
        font_desc = pango_font_description_from_string("Sans Bold 18");
        //	pango_font_description_set_size(font_desc,40*PANGO_SCALE);

        gtk_widget_modify_font(GTK_LABEL(GTK_BIN(TempoDraw)->child), font_desc);

        gtk_label_set_text(GTK_LABEL(GTK_BIN(TempoDraw)->child), StrBuf);
    } else {
//		gdk_color_parse ("black", &bgcolor);
//		gdk_color_parse ("white", &fgcolor);
//		SendMidi(SND_SEQ_EVENT_NOTEOFF, 0, DefaultMidiChannel, 07, (int)35);
    }
    SendMidi(SND_SEQ_EVENT_CLOCK, TempoPort, 0, 0, 0);

    /* Display the flasing count.
     */
//	gtk_widget_modify_bg (TempoDraw, GTK_STATE_NORMAL, &bgcolor);
//	gtk_widget_modify_fg (TempoDraw, GTK_STATE_NORMAL, &fgcolor);
//	gtk_widget_modify_font(TempoDraw, pango_font_description_from_string("Sans Bold 20"));
//    g_object_set(G_OBJECT(TempoDraw), "gtk-font-name", "Sans Bold 20",
//                 NULL);
    TempoState++;
}

/*--------------------------------------------------------------------
 * Function:		CreateMainButtons
 *
 * Description:		Build the main button screen
 *---------------------------------------------------------------------*/
void CreateMainButtons(void) {
    int Loop, Row = 0, Col = 0;
    GtkWidget *ButtonFrame;
    GtkWidget *Table;
    char Buffer[40];

#ifndef UsingNewButtons
    for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
        sprintf(Buffer, "button%d", Loop + 1);
        MainButtons[Loop] = GTK_WIDGET(gtk_builder_get_object(gxml, Buffer));
        g_signal_connect_data(G_OBJECT(MainButtons[Loop]), "clicked",
                              G_CALLBACK(on_button_clicked), Loop, NULL, 0);
    }
#else
    ButtonFrame = GTK_WIDGET (gtk_builder_get_object (gxml, "ButtonFrame") );
    printd(LogInfo,"Button Frame %x\n", ButtonFrame);

    Table = gtk_table_new(6,5,false);
    printd(LogInfo,"Table %x\n", Table);

    for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
        MainButtons[Loop] = gtk_button_new_with_label (gMyInfo.MyPatchInfo[GetModePreset(Loop)].Name);
        g_signal_connect (MainButtons[Loop], "clicked",
                          G_CALLBACK (on_button_clicked), (void *)Loop);
//		gtk_widget_show(MainButtons[Loop]);
        gtk_widget_set_usize(MainButtons[Loop],120,120);
//		gtk_table_attach_defaults(GTK_TABLE(Table), MainButtons[Loop],
//			Col, Col + 1, Row, Row + 1);
        gtk_table_attach(GTK_TABLE(Table), MainButtons[Loop],
                         Col, Col + 1, Row, Row + 1, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND,0,0);
        if (Col++ > 4) {
            Col = 0;
            Row++;
        }
        printd(LogInfo,"Loop %d %d %d %x\n", Loop, Col, Row, MainButtons[Loop]);
    }

    gtk_container_add (GTK_CONTAINER (ButtonFrame), Table);
#endif

    VScale1 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale1"));
    Adjustment1 = GTK_WIDGET(gtk_builder_get_object(gxml, "adjustment1"));
    gtk_signal_connect(GTK_OBJECT (VScale1), "value_changed",
                       GTK_SIGNAL_FUNC (VScale1_Changed), NULL);
//    scale_set_default_values (GTK_SCALE (VScale1));

    VScale2 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale2"));
    Adjustment2 = GTK_WIDGET(gtk_builder_get_object(gxml, "adjustment2"));
    gtk_signal_connect(GTK_OBJECT (VScale2), "value_changed",
                       GTK_SIGNAL_FUNC (VScale2_Changed), NULL);

    VScale3 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale3"));
    Adjustment3 = GTK_WIDGET(gtk_builder_get_object(gxml, "adjustment3"));

}

/*--------------------------------------------------------------------
 * Function:		VScale1_Changed
 *
 * Description:		Volume Sliders changed.
 *
 *---------------------------------------------------------------------*/
void VScale1_Changed(GtkAdjustment *adj) {
    /* Set the number of decimal places to which adj->value is rounded */
    //   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
    //   printf("\nVscale 1 %f\n", Adjustment1->value);
    SendMidi(SND_SEQ_EVENT_CONTROLLER,
    	gMyInfo.MyPatchInfo[Slider1].OutPort,
    	gMyInfo.MyPatchInfo[Slider1].Channel,
    	gMyInfo.MyPatchInfo[Slider1].Patch,
             (char) Adjustment1->value);
    gMyInfo.AnalogVolume = (char) Adjustment1->value;
}

/*--------------------------------------------------------------------
 * Function:		VScale2_Changed
 *
 * Description:		Volume Sliders changed.
 *
 *---------------------------------------------------------------------*/
void VScale2_Changed(GtkAdjustment *adj) {
    /* Set the number of decimal places to which adj->value is rounded */
    //   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
//    printf("\nVscale 1 %f\n", Adjustment2->value);
    SendMidi(SND_SEQ_EVENT_CONTROLLER,
    	gMyInfo.MyPatchInfo[Slider2].OutPort,
    	gMyInfo.MyPatchInfo[Slider2].Channel,
    	gMyInfo.MyPatchInfo[Slider2].Patch,
             (char) Adjustment2->value);
    gMyInfo.MidiVolume = (char) Adjustment2->value;
}

/*--------------------------------------------------------------------
 * Function:		SetUpMainButtons
 *
 * Description:		Set the title of all of the buttons
 *
 *---------------------------------------------------------------------*/
void SetUpMainButtons(PatchInfo *myPatchInfo) {
    GtkWidget *myButton;
    tPatchIndex Loop;
	tPatchIndex	PatchIndex;

    for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
        myButton = MainButtons[Loop];
//		printd(LogInfo,"SetUpMainButtons: %d %x\n", gMyInfo.MyPatchInfo[Loop].Index, myButton);
//		printd(LogInfo,"Loop %d gMyInfo %s Patch %d\n",Loop, gMyInfo.MyPatchInfo[GetModePreset(Loop)].Name, GetModePreset(Loop));
        PatchIndex = ModeSwitchPatch(Loop);
        gtk_label_set_text(GTK_LABEL(GTK_BIN(myButton)->child),
                           gMyInfo.MyPatchInfo[PatchIndex].Name);
    }
}

/*--------------------------------------------------------------------
 * Function:		DoPatch
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
tPatchIndex DoPatch(PatchInfo *thePatch) {
    char Next;
    PatchInfo *NextPatch;
    char NextCommand = 1;

    NextPatch = thePatch;

    do {
        SendMidiPatch(NextPatch);
        UpdateStatus(NextPatch->Name);

        NextCommand = NextPatch->Chain;
        NextPatch = &gMyInfo.MyPatchInfo[NextCommand];
    } while (NextCommand);
    return (0);
}

/*--------------------------------------------------------------------
 * Function:		CheckConnectionData
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void CheckConnectionData(GTKMidiInfo *myInfo) {

}

#if 0
gtk_object_set(GTK_OBJECT(button), "label", "goodbye", NULL);

gtk_label_set_text(GTK_LABEL(GTK_BIN(button)->child), "hello");
GladeXML* xml;
GtkWidget* entry1;

xml = glade_get_widget_tree (GTK_WIDGET (button1));
entry1 = GTK_WIDGET (gtk_builder_get_object (xml, "entry1") );

#endif

/*--------------------------------------------------------------------
 * Function:		CreateHTMLGuide
 *
 * Description:		Writes and HTML file for the presets.
 *
 *---------------------------------------------------------------------*/
void CreateHTMLGuide(GTKMidiInfo *myInfo) {
    FILE *MyFile;
    char FileName[255];
    char Loop;
    char Loop1;

    strcpy(FileName, myInfo->BasePath);
    strcat(FileName, "/aaFootSwitch.html");

    MyFile = fopen(FileName, "w");
    if (!MyFile)
        return;

    fprintf(MyFile,
            "<html><head>\n<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\n<title>FootSwitchController\n</title>\n</head><body><h3>Foot Switch Controller&nbsp; - Elias<br> </h3>");
    fprintf(MyFile,
            "<table style=\"text-align: left; width: 849px; height: 83px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n<tbody>\n<tr>\n");
    Loop1 = 0;
    for (Loop = 0; Loop < Max_Patches; Loop++) {
        printd(LogInfo, "Create HTML %d %d\n", Loop, Loop1);
        if (Loop1 == 0) {
            fprintf(MyFile,
                    "<td style=\"vertical-align: top; font-weight: bold; width: 75px;\"><big><big><big>%d</big></big></big><br>\n</td>\n",
                    Loop);
            fprintf(MyFile,
                    "<td style=\"vertical-align: top; font-weight: bold; width: 75px;\">\n");
            fprintf(MyFile,
                    "<table style=\"text-align: left; width: 716px; height: 32px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n<tbody>\n<tr>\n");
        }

        fprintf(MyFile,
                "<td style=\"vertical-align: top; font-weight: bold; width: 75px;\">%d - %s<br>\n</td>\n",
                Loop, myInfo->MyPatchInfo[Loop].Name);

        if (++Loop1 == 5) {
            printd(LogInfo, "Loop %d %d\n", Loop, Loop1);
            fprintf(MyFile, "</td>\n</tr>\n</tbody>\n</table>\n</td>\n</tr>\n");
            Loop1 = 0;
            if (((Loop + 1) % 10) == 0)
                fprintf(MyFile,
                        "</tbody>\n</table>\n<table style=\"text-align: left; width: 849px; height: 83px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n<tbody>\n<tr>");
        }

//		myInfo->OutPortName[Loop]
    }
    fclose(MyFile);
}

/*--------------------------------------------------------------------
 * Function:		RaiseWindowsNum
 *
 * Description:		Bring the App/Window into the forground.
 *
 *---------------------------------------------------------------------*/
void RaiseWindowsNum(char AppNumber) {
//	printd(LogInfo,"Raise Window %s\n", gMyInfo.Apps[AppNumber].Name);
    RaiseWindows(&gMyInfo.Apps[AppNumber].Name);
}

/*--------------------------------------------------------------------
 * Function:		GetModePreset
 *
 * Description:		Based on the current mode convert an index.
 *
 *---------------------------------------------------------------------*/
tPatchIndex GetModePreset(tPatchIndex Value) {
    tPatchIndex NewValue;

    switch (CurrentMode) {
    case ModeDefault:
        NewValue = Value;
        break;

    case ModeRehearsal:
        NewValue = Value;
        break;

    case ModePractice:
        NewValue = preModePractice[Value];
        break;

    case ModePerformance:
        NewValue = Value;
        break;

    case ModeLooper:
        NewValue = preModeLooper[Value];
        break;

    default:
        NewValue = Value;
        break;
    }
    printd(LogInfo, "Get Mode Preset Old %d New %d\n", Value, NewValue);
    return (NewValue);
}

/*--------------------------------------------------------------------
 * Function:		IncrementMode
 *
 * Description:		Based on the current mode do a different.
 *
 *---------------------------------------------------------------------*/
void IncrementMode(void) {
    if (++CurrentMode > ModeLastItem)
        CurrentMode = 0;

    gtk_widget_modify_font(CurrentModeWid,
                           pango_font_description_from_string("Sans Bold 16"));
    gtk_label_set_text(CurrentModeWid, theModes[CurrentMode]);
    SetUpMainButtons(&gMyInfo.MyPatchInfo);

    printd(LogInfo, "Increment Mode %d\n", CurrentMode);
}

/*--------------------------------------------------------------------
 * Function:		ModeSwitchPatch
 *
 * Description:		Based on the current mode do a different.
 *
 *---------------------------------------------------------------------*/
tPatchIndex ModeSwitchPatch(tPatchIndex MidiIn) {
    char Preset;

    printd(LogInfo, "In ModeSwitchPatch Mid In %d %d %d\n", MidiIn,
           GetModePreset(MidiIn),
           &gMyInfo.MyPatchInfo[(char) GetModePreset(MidiIn)]);

    /* If the Midi command was a mode changes.
     */
    switch (MidiIn) {
    case PresetMidiKey:
        GuitarMidiPreset();
        return(-1);
        break;

    case Preset1FButton:
        Preset = gMyInfo.WebPresets.thePreset1;
        if (Preset != -1)
        	return(Preset);
        else
        	return(MidiIn);
        break;

    case Preset2FButton:
        Preset = gMyInfo.WebPresets.thePreset2;
        if (Preset != -1)
        	return(Preset);
        else
        	return(MidiIn);
        break;

    case ModeSwitchKey:
        IncrementMode();
        break;

    default:
        return(GetModePreset(MidiIn));
        break;
    }
//		DoPatch(&gMyInfo.MyPatchInfo[preModePractice[GetModePreset(MidiIn)]]);

    printd(LogInfo, "ModeSwitchPatch %d\n", MidiIn);
// ejk event_ptr->data.control.value > 127 || event_ptr->data.control.value < 0 ? "???": gm_get_instrument_name(event_ptr->data.control.value));
    return (-1);
}

/*--------------------------------------------------------------------
 * Function:		GuitarMidiPreset
 *
 * Description:		Wait for a preset from the Midi Guitar.
 *
 *---------------------------------------------------------------------*/
int GuitarMidiPreset(void) {

    printf("GuitarMidiPreset Start\n");

    /* Set Audio Volume to zero
     */
    SendMidi(SND_SEQ_EVENT_CONTROLLER,
    	gMyInfo.MyPatchInfo[Slider1].OutPort,
    	gMyInfo.MyPatchInfo[Slider1].Channel,
    	gMyInfo.MyPatchInfo[Slider1].Patch,
    	0);

    /* Set Midi Volume to zero
     */
    SendMidi(SND_SEQ_EVENT_CONTROLLER,
    	gMyInfo.MyPatchInfo[Slider2].OutPort,
    	gMyInfo.MyPatchInfo[Slider2].Channel,
    	gMyInfo.MyPatchInfo[Slider2].Patch,
    	0);

    WaitingforMidi = 1;

    return (0);
}

/*--------------------------------------------------------------------
 * Function:		GuitarMidiPresetComplete
 *
 * Description:		Wait for a preset from the Midi Guitar.
 *
 *---------------------------------------------------------------------*/
int GuitarMidiPresetComplete(tPatchIndex MidiNote) {
	tPatchIndex	PatchChange;

    printf("GuitarMidiPresetComplete Start %d\n", MidiNote);

    /* Set Audio Volume back
     */
    SendMidi(SND_SEQ_EVENT_CONTROLLER,
    	gMyInfo.MyPatchInfo[Slider1].OutPort,
    	gMyInfo.MyPatchInfo[Slider1].Channel,
    	gMyInfo.MyPatchInfo[Slider1].Patch,
    	gMyInfo.AnalogVolume);

    /* Set Midi Volume back
     */
    SendMidi(SND_SEQ_EVENT_CONTROLLER,
    	gMyInfo.MyPatchInfo[Slider2].OutPort,
    	gMyInfo.MyPatchInfo[Slider2].Channel,
    	gMyInfo.MyPatchInfo[Slider2].Patch,
    	gMyInfo.MidiVolume);

    PatchChange = gMyInfo.MidiBaseNote - MidiNote;
    if (PatchChange >= 0 && PatchChange < Max_Patches)
    	DoPatch(&gMyInfo.MyPatchInfo[PatchChange]);

    WaitingforMidi = 0;

    return (0);
}

/*--------------------------------------------------------------------
 * Function:		show_motion_notify_cb
 *
 * Description:	CallBack function for motion.
 *---------------------------------------------------------------------*/
gint button_press_notify_cb(GtkWidget *entries, GdkEventKey *event,
                            GtkWidget *widget) {

    switch (event->keyval) {
    case GDK_p:
        printf("key pressed: %s\n", "p");
        break;
    case GDK_s:
        if (event->state & GDK_SHIFT_MASK) {
            printf("key pressed: %s\n", "shift + s");
        } else if (event->state & GDK_CONTROL_MASK) {
            printf("key pressed: %s\n", "ctrl + s");
        } else {
            printf("key pressed: %s\n", "s");
        }
        break;
    case GDK_m:
        if (event->state & GDK_SHIFT_MASK) {
            printf("key pressed: %s\n", "shift + m");
        } else if (event->state & GDK_CONTROL_MASK) {
            printf("key pressed: %s\n", "ctrl + m");
        } else {
            printf("key pressed: %s\n", "m");
        }
        break;

    default:
        return FALSE;
    }

    return (TRUE);
}

/*--------------------------------------------------------------------
 * Function:		SetVolume1
 *
 * Description:	Change the Volume Slider based on midi input.
 *---------------------------------------------------------------------*/
int SetVolume1(int Value) {
    Adjustment1->value = (float) Value;
    gMyInfo.AnalogVolume = Value;
    gtk_range_set_adjustment(VScale1, Adjustment1);

    SendMidi(SND_SEQ_EVENT_CONTROLLER,
    	gMyInfo.MyPatchInfo[Slider1].OutPort,
    	gMyInfo.MyPatchInfo[Slider1].Channel,
    	gMyInfo.MyPatchInfo[Slider1].Patch,
    	(char) Adjustment1->value);
}

/*--------------------------------------------------------------------
 * Function:		SetVolume2
 *
 * Description:	Change the Volume Slider based on midi input.
 *---------------------------------------------------------------------*/
int SetVolume2(int Value) {
    Adjustment2->value = Value;
    gMyInfo.MidiVolume = Value;
    gtk_range_set_adjustment(VScale2, Adjustment2);
    SendMidi(SND_SEQ_EVENT_CONTROLLER,
    	gMyInfo.MyPatchInfo[Slider2].OutPort,
    	gMyInfo.MyPatchInfo[Slider2].Channel,
    	gMyInfo.MyPatchInfo[Slider2].Patch,
        (char) Adjustment2->value);
}
