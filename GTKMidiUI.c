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


//#define UsingNewButtons	1

#ifdef UsingNewButtons
#define GLADE_FILE ResourceDirectory"GTKMidiUI.glade.Buttons"
#else
#define GLADE_FILE ResourceDirectory"GTKMidiUI.glade"
#endif


snd_seq_t *seq;
int seqPort;
//	GladeXML *gxml;
GtkBuilder *gxml;
GtkWidget* MainStatus;
GtkWidget* CurrentModeWid;
GtkWidget	*TempoDraw;
unsigned int CurrentMode;
guint		MainStatusid;
GtkWidget *ModeSwitchButton;
GtkWidget *MainButtons[Max_Main_Buttons];

void	CreateMainButtons(void);
void	SetUpMainButtons(PatchInfo  *MyPatchInfo);
void PrintDataStructure(GTKMidiInfo *myInfo);
void CheckConnectionData(GTKMidiInfo *myInfo);
void CreateHTMLGuide(GTKMidiInfo *myInfo);
void apply_font_to_widget (GtkWidget *widget, gchar *fload);
int DoPatch( PatchInfo *thePatch);
void ToggleTempo(void);
static gboolean	tempo_handler(GtkWidget *widget);
void UpdateStatus(char *String);
void	 IncrementMode(void);

/* Used to Toggle the Tempo GUI display.
 */
char	TempoState;
static gboolean	time_handler(GtkWidget *widget);

#define MaxStatusHold 4
char	HoldStatus[MaxStatusHold][50];
char	HoldStatusIndex;

//PatchInfo  DefaultPatchInfo[] = {

/*--------------------------------------------------------------------
* Function:		apply_font_to_widget
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void apply_font_to_widget (GtkWidget *widget, gchar *fload) {
#if 0
	GtkStyle *style = gtk_style_new();
	gdk_font_unref(style->font_desc);
	style->font_desc = gdk_font_load(fload);
	style->font_desc = gdk_font_load("12x24");
//	gtk_style_set_font(style,fload);
	gtk_widget_set_style(GTK_WIDGET(widget), style);
#else
PangoFontDescription	*pfd;
pfd = pango_font_description_from_string (fload);

if(GTK_IS_LABEL(widget))
gtk_widget_modify_font ( widget, pfd);
else
gtk_widget_modify_font ( GTK_WIDGET (gtk_bin_get_child (GTK_BIN(widget))), pfd);

pango_font_description_free (pfd);
#endif
}

/*--------------------------------------------------------------------
* Function:		SwitchToTab
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void SwitchToTab(char Tab) {
	GtkWidget	*NoteBookPane;
		NoteBookPane = GTK_WIDGET (gtk_builder_get_object (gxml, "MainTab" ) );
		gtk_notebook_set_current_page(GTK_NOTEBOOK(NoteBookPane), Tab);
		printf("Switch to Tab %d\n",Tab);
}

/*--------------------------------------------------------------------
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void on_button_clicked (GtkButton *button, gpointer	user_data ){
	DoPatch(&gMyInfo.MyPatchInfo[(char )GetModePreset(user_data)]);
	printf("User data %d\n", (int) user_data);
}

void on_modebutton_clicked (GtkButton *button, gpointer	user_data ){

	IncrementMode();
	printf("Increment Mode called from Buttons \n");

}

void on_About_clicked (GtkButton *button, gpointer	user_data ){
    GtkWidget *window;

		window = GTK_WIDGET (gtk_builder_get_object  (gxml, "AboutDialog") );
		gtk_widget_show (window);

 //   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
//	gtk_window_set_title(GTK_WINDOW(window), "About LiveMusic");

}

/*--------------------------------------------------------------------
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void on_hscale1_value_changed (GtkWidget *widget, gpointer user_data)
{
		gdouble val;

		val = gtk_range_get_value (GTK_RANGE(widget));

		/* print to screen */
		g_print("Range value: %d\n", (guint)val);
// ejk		SendMidi(0,DefaultMidiChannel, 07, (int)val);
 }

/*--------------------------------------------------------------------
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void on_window1_destroy (GtkWidget *widget, gpointer user_data)
{
	/* break gtk_main() loop */
	gtk_main_quit();
}

/*--------------------------------------------------------------------
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
gboolean tab_focus_callback( GtkNotebook *notebook,
                             gint *arg1,
                             gpointer data ) {
  //  GtkTreeView* view = (GtkTreeView *)data;
//printf("tab_focus_callback %x %x %x\n", notebook, arg1, data);
  return true;
}

/*--------------------------------------------------------------------
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
int main (int argc, char *argv[]) {
	GtkWidget *main_window;
	GtkWidget *main_tab;
	GtkWidget *widget;
	GError	 *error = NULL;
	GtkWidget		*ChordWidget;
	
	
		CurrentMode = 0;
		/* initialize the GTK+ library */
		gtk_init (&argc, &argv);
		gtk_rc_parse( MAINPREFS_FILE);
		/*
		create an instance of the GladeXML object and build widgets within
		the window1 root node.
		*/
//		gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
		/* Get Preferences is there are there otherise set defaults.
		 */
		InitPref();

		gxml = gtk_builder_new ();
		if (!gtk_builder_add_from_file (gxml, GLADE_FILE, &error)) {
			g_warning ("Couldn't load builder file: %s", error->message);
			g_error_free (error);
		}
		/* get the window widget from the glade XML file */
		main_window = GTK_WIDGET (gtk_builder_get_object  (gxml, "window1") );
			g_signal_connect (G_OBJECT (main_window), "destroy", 
			G_CALLBACK (on_window1_destroy), NULL);

		main_tab = GTK_WIDGET (gtk_builder_get_object  (gxml, "MainTab") );
		g_signal_connect( GTK_NOTEBOOK( main_tab ), "switch-page", 
                     (GCallback) tab_focus_callback, gxml);

			/* connect signals */
//	glade_xml_signal_connect (gxml, "on_hscale1_value_changed",
//			G_CALLBACK(on_hscale1_value_changed));

		/* setup and initialize our statusbar */
		MainStatus = GTK_WIDGET (gtk_builder_get_object (gxml, "MainStatusBar") );
		CurrentModeWid = GTK_WIDGET (gtk_builder_get_object (gxml, "CurrentMode") );

		/* Clear the Status bar buffer.
		 */
		HoldStatusIndex = 0;
		memset(HoldStatus, 0, sizeof (HoldStatus));
		TempoDraw = GTK_WIDGET (gtk_builder_get_object (gxml, "Tempo"));

		widget = GTK_WIDGET (gtk_builder_get_object (gxml, "AboutButton"));
		g_signal_connect_data (G_OBJECT (widget), "clicked", 
			G_CALLBACK (on_About_clicked), NULL, NULL, 0);

//		memset(&thePorts, 0, sizeof (PortsInfo));
		PrintDataStructure(&gMyInfo);


		/* Set up the GUI for making changes to the preferences.
		 */
		InitGuiPrefs();

// exit(0);
		CreateHTMLGuide(&gMyInfo);

		/*
		 * Initialize the WebKit (HTML) engine
		 */
		InitHTML(gxml);
printf("After InitHTML\n");

		/* Call the Jackd
		 * jackd -R -t5000 -dalsa -Chw:$AudioInHW$DeviceAdder -Phw:$AudioOutHW$DeviceAdder -r44100 -p256 -n3
		 */
//		system( );
		/*
		 * Set up the buttons test and patches.
		 */		
		CreateMainButtons();
		SetUpMainButtons(&gMyInfo.MyPatchInfo);

		ModeSwitchButton = GTK_WIDGET (gtk_builder_get_object  (gxml, "ModeSwitchButton"));
		//gtk_label_set_text(GTK_LABEL(GTK_BIN(myButton)->child), gMyInfo.MyPatchInfo[Loop].Name);
		g_signal_connect_data (G_OBJECT (ModeSwitchButton), "clicked", 
			G_CALLBACK (on_modebutton_clicked), NULL, NULL, 0);
	
		/*
		 * Set up the Midi Sequencer port
		 */
		MyAlsaInit();
printf("After MyAlsaInit\n");
		
		/* Set up the connections between applications.
		 */
		InitConnections();

		ChordWidget = GTK_WIDGET (gtk_builder_get_object  (gxml, "ChordFrame") );
		ChorderMain(ChordWidget);

		/* Set up a timer for Tempo.
		 */
		g_timeout_add(Timer1Ticks, (GSourceFunc) time_handler, (gpointer) gxml);
		gMyInfo.TempoTimerID = 0;
		SetTempo(90);
		/* show the main window */
		gtk_widget_show_all (main_window);
		gtk_widget_modify_font(CurrentModeWid, pango_font_description_from_string("Sans Bold 16"));
		gtk_label_set_text (CurrentModeWid, theModes[0]);

	  /* begin main GTK loop */
		gtk_main ();


		WritePref();

		return 0;
}

/*--------------------------------------------------------------------
* Function:		Update the display status
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void UpdateStatus(char *String) {
GtkStyle *hold;
char	DisString[160];

	printf("Status Update %d\n", HoldStatusIndex);
	printf("%s\n", &HoldStatus[0]);
	printf("%s\n", &HoldStatus[1]);
	printf("%s\n", &HoldStatus[2]);
	printf("%s\n", &HoldStatus[3]);

	switch (HoldStatusIndex) {
		case 0:
			sprintf(DisString, "[%12s] [%12s] [%12s] [%12s]", &HoldStatus[1], &HoldStatus[2], &HoldStatus[3], String);
		break;

		case 1:
			sprintf(DisString, "[%12s] [%12s] [%12s] [%12s]", &HoldStatus[2], &HoldStatus[3], &HoldStatus[0], String);
		break;

		case 2:
			sprintf(DisString, "[%12s] [%12s] [%12s] [%12s]", &HoldStatus[3], &HoldStatus[0], &HoldStatus[1], String);
		break;

		case 3:
			sprintf(DisString, "[%12s] [%12s] [%12s] [%12s]", &HoldStatus[0], &HoldStatus[1], &HoldStatus[2], String);
		break;

	}
	
	strncpy(&HoldStatus[HoldStatusIndex++], String,50);
	if (HoldStatusIndex >= MaxStatusHold)
			HoldStatusIndex = 0;

	gtk_widget_modify_font(MainStatus, pango_font_description_from_string("Sans Bold 16"));
		gtk_label_set_text (MainStatus, DisString);
}

/*--------------------------------------------------------------------
* Function:		Set the tempo
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void SetTempo(unsigned char NewTempo) {

//	SendMidi(SND_SEQ_EVENT_TEMPO, char Port, char Channel, char Controller, NewTempo);

	/* Tell the timer to stop.
	 */
	 if (gMyInfo.TempoTimerID)
		g_source_remove(gMyInfo.TempoTimerID);
	 
	/* Store the tempo information.
	 */
	gMyInfo.Tempo = NewTempo;
	gMyInfo.TempoReload = (500 * 60) / NewTempo;
	printf("New Tempo Val  %d\n", gMyInfo.TempoReload);

	/* Start the new timer.
	 */
	gMyInfo.TempoTimerID = g_timeout_add(gMyInfo.TempoReload, 
		(GSourceFunc) tempo_handler, (gpointer) gxml);

	gMyInfo.Timer1Count = 0;
}

/*--------------------------------------------------------------------
* Function:		Tempo callback.
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
static gboolean	tempo_handler(GtkWidget *widget) {

	/* HANDE Tempo Midi 
	 */
	ToggleTempo();

return TRUE;
}

/*--------------------------------------------------------------------
* Function:		Timer Callback
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
static gboolean	time_handler(GtkWidget *widget) {


//  gtk_widget_queue_draw(widget);

return TRUE;
}

/*--------------------------------------------------------------------
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void ToggleTempo(void) {
GdkColor fgcolor;
GdkColor bgcolor;

	if (TempoState) {
		gdk_color_parse ("white", &bgcolor);
		gdk_color_parse ("Black", &fgcolor);
		TempoState = 0;
	}
	else {
		gdk_color_parse ("black", &bgcolor);
		gdk_color_parse ("white", &fgcolor);
		TempoState = 1;
	}
	
//	SendMidi(SND_SEQ_EVENT_TEMPO, TempoPort, DefaultMidiChannel, SND_SEQ_EVENT_TEMPO, 10);

	gtk_widget_modify_bg (TempoDraw, GTK_STATE_NORMAL, &bgcolor);
	gtk_widget_modify_fg (TempoDraw, GTK_STATE_NORMAL, &fgcolor);
//	SND_SEQ_EVENT_TEMPO
}

/*--------------------------------------------------------------------
* Function:		CreateMainButtons
*
* Description:		Build the main button screen
*---------------------------------------------------------------------*/
void	CreateMainButtons(void) {
	int			Loop, Row = 0, Col = 0;
	GtkWidget *ButtonFrame;
	GtkWidget *Table;

#ifndef UsingNewButtons
	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		
		MainButtons[Loop] = GTK_WIDGET (gtk_builder_get_object  (gxml, gMyInfo.MyPatchInfo[Loop].Button));
		g_signal_connect_data (G_OBJECT (MainButtons[Loop]), "clicked", 
			G_CALLBACK (on_button_clicked), Loop, NULL, 0);
	}
#else
	ButtonFrame = GTK_WIDGET (gtk_builder_get_object  (gxml, "ButtonFrame") );
	printf("Button Frame %x\n", ButtonFrame);

	Table = gtk_table_new(6,5,false);
	printf("Table %x\n", Table);

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
		printf("Loop %d %d %d %x\n", Loop, Col, Row, MainButtons[Loop]);
	}

	gtk_container_add (GTK_CONTAINER (ButtonFrame), Table);
#endif
}

/*--------------------------------------------------------------------
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void	SetUpMainButtons(PatchInfo  *myPatchInfo) {
	GtkWidget *myButton;
	int			Loop;

	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		myButton = MainButtons[Loop];
		printf("SetUpMainButtons: %s %x\n", gMyInfo.MyPatchInfo[Loop].Button, myButton);
		printf("Loop %d gMyInfo %x Patch %d\n",Loop, &gMyInfo, GetModePreset(Loop));
	gtk_label_set_text(GTK_LABEL(GTK_BIN(myButton)->child), gMyInfo.MyPatchInfo[GetModePreset(Loop)].Name);
	}
}

/*--------------------------------------------------------------------
* Function:		DoPatch
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
int DoPatch( PatchInfo *thePatch) {
char	Next;
PatchInfo *NextPatch;
char	NextCommand = 1;

	NextPatch = thePatch;

	do {
		SendMidiPatch(NextPatch);
		UpdateStatus(NextPatch->Name);

		NextCommand = NextPatch->Chain;
		NextPatch = &gMyInfo.MyPatchInfo[NextCommand];
	} while(NextCommand);
}

/*--------------------------------------------------------------------
* Function:		PrintDataStructure
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
 entry1 = GTK_WIDGET (gtk_builder_get_object  (xml, "entry1") );

#endif


/*--------------------------------------------------------------------
* Function:		CreateHTMLGuide
*
* Description:		Writes and HTML file for the presets.
*
*---------------------------------------------------------------------*/
void CreateHTMLGuide(GTKMidiInfo *myInfo) {
FILE	*MyFile;
char	FileName[255];
char	Loop;
char	Loop1;

	strcpy(FileName, myInfo->BasePath);
	strcat(FileName, "/FootSwitch.html");

	MyFile = fopen(FileName, "w");
	if ( !MyFile )
		return;

	fprintf(MyFile, "<html><head>\n<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\n<title>FootSwitchController\n</title>\n</head><body><h3>Foot Switch Controller&nbsp; - Elias<br> </h3>");
	fprintf(MyFile, "<table style=\"text-align: left; width: 849px; height: 83px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n<tbody>\n<tr>\n");
	Loop1 = 0;
	for (Loop = 0; Loop < Max_Patches; Loop++) {
		printf("Create HTML %d %d\n",Loop ,  Loop1);
	if (Loop1 == 0) {
fprintf(MyFile, "<td style=\"vertical-align: top; font-weight: bold; width: 75px;\"><big><big><big>%d</big></big></big><br>\n</td>\n", Loop);
fprintf(MyFile, "<td style=\"vertical-align: top; font-weight: bold; width: 75px;\">\n");
fprintf(MyFile, "<table style=\"text-align: left; width: 716px; height: 32px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n<tbody>\n<tr>\n");
	}

fprintf(MyFile, "<td style=\"vertical-align: top; font-weight: bold; width: 75px;\">%d - %s<br>\n</td>\n",Loop, myInfo->MyPatchInfo[Loop].Name);

	if (++Loop1 == 5) {
	printf("Loop %d %d\n", Loop, Loop1);
	fprintf(MyFile,"</td>\n</tr>\n</tbody>\n</table>\n</td>\n</tr>\n");
	Loop1 = 0;
	if (((Loop +1) % 10 ) == 0 )
		fprintf(MyFile,"</tbody>\n</table>\n<table style=\"text-align: left; width: 849px; height: 83px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n<tbody>\n<tr>");
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
void RaiseWindowsNum( char AppNumber ) {
//	printf("Raise Window %s\n", gMyInfo.Apps[AppNumber].Name);
	RaiseWindows(&gMyInfo.Apps[AppNumber].Name);
}

/*--------------------------------------------------------------------
* Function:		ModeSwitchPatch
*
* Description:		Based on the current mode convert an index.
*
*---------------------------------------------------------------------*/
int	 GetModePreset(int Value) {
int		NewValue;

	switch(CurrentMode) {
		case ModeSwitch:
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
			NewValue = Value;
		break;

		default:
			NewValue = Value;		
		break;
	}
}

/*--------------------------------------------------------------------
* Function:		IncrementMode
*
* Description:		Based on the current mode do a different.
*
*---------------------------------------------------------------------*/
void	 IncrementMode(void) {
		if (++CurrentMode > ModeLastItem)
			CurrentMode = 0;

		gtk_widget_modify_font(CurrentModeWid, pango_font_description_from_string("Sans Bold 16"));
		gtk_label_set_text (CurrentModeWid, theModes[CurrentMode]);
		SetUpMainButtons(&gMyInfo.MyPatchInfo);

	printf("Increment Mode \n",CurrentMode);
}

/*--------------------------------------------------------------------
* Function:		ModeSwitchPatch
*
* Description:		Based on the current mode do a different.
*
*---------------------------------------------------------------------*/
int	 ModeSwitchPatch(int MidiIn) {

	/* If the Midi command was a mode changes.
	 */
	if (MidiIn == ModeSwitchKey)
		IncrementMode();
	else
		DoPatch(&gMyInfo.MyPatchInfo[preModePractice[GetModePreset(MidiIn)]]);

// ejk event_ptr->data.control.value > 127 || event_ptr->data.control.value < 0 ? "???": gm_get_instrument_name(event_ptr->data.control.value));
return(0);
}
