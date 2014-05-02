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


#define GLADE_FILE "GTKMidiUI.glade"
#define MAINPREFS_FILE ".GTKMidi"

#define DefaultMidiChannel		1
#define MidiProgramChange		32

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


void	SetUpMainButtons(PatchInfo  *PatchInfo);
void PrintDataStructure(GTKMidiInfo *myInfo);
void CheckConnectionData(GTKMidiInfo *myInfo);
void CreateHTMLGuide(GTKMidiInfo *myInfo);
void apply_font_to_widget (GtkWidget *widget, gchar *fload);
int DoPatch( PatchInfo *thePatch);
void ToggleTempo(void);
static gboolean	tempo_handler(GtkWidget *widget);
void UpdateStatus(char *String);
void ConnectSignals(void);
void	 IncrementMode(void);


enum {ToDesktop0 = 60, ToDesktop1,ToDesktop2,ToDesktop3,ToDesktop4,ToDesktop5,
	ToAnalogApp, ToMidiSoundApp, ToLooperApp, ToTransportApp, ToMidiControl };
/* Used to Toggle the Tempo GUI display.
 */
char	TempoState;
static gboolean	time_handler(GtkWidget *widget);

#define MaxStatusHold 4
char	HoldStatus[MaxStatusHold][50];
char	HoldStatusIndex;

//PatchInfo  DefaultPatchInfo[] = {
GTKMidiInfo  GlobalInfo = { 
	{
// 00	Button		Title			Bank		Patch	Outport		Chan	Custom->Chain
	{"button1",	"Elias1",		0xff,		1,		RakarrakPort,	2,	NoCustom,	0 },
	{"button2",	"Dist",		0xff,		2,		RakarrakPort,	2,	NoCustom,	0 },
	{"button3",	"Funk Chorus",0xff,		3,		RakarrakPort,	2,	NoCustom,	0 },
	{"button4",	"ThumpBass",	0xff,		8,		RakarrakPort,	2,	NoCustom,	0 },
	{"button5",	"Chorus",		0xff,		11,		RakarrakPort,	2,	NoCustom,	0 },
	{"button6",	"Jazz",		0xff,		27,		RakarrakPort,	2,	NoCustom,	0 },
	{"button7",	"Heavenly",	0xff,		30,		RakarrakPort,	2,	NoCustom,	0 },
	{"button8",	"Synth",		0xff,		15,		RakarrakPort,	2,	NoCustom,	0 },
	{"button9",	"Extreme",		0xff,		26,		RakarrakPort,	2,	NoCustom,	0 },
	{"button10",	"Change",		0xff,		0,		RakarrakPort,	2,	ToNextDesktop,0},
// 10	Button		Title		Bank	Patch	Outport			
	{"button11",	"Piano",		SFDSF,		63,		FluidPort,		1,	NoCustom,	0 },
	{"button12",	"Honky",		SF32GM,	3,		FluidPort,		1,	NoCustom,	0},
	{"button13",	"Breath",		SFFluidGM,	53,	FluidPort,			1,	NoCustom,	0},
	{"button14",	"De-Tuned Organ",SFA340Low,	16,	FluidPort,		1,	NoCustom,	0},
	{"button15",	"Flute",		SFFlute,	73,	FluidPort,			1,	NoCustom,	0},
	{"button16",	"Synth Lead",	SFA340Base,	24,	FluidPort,		1,	NoCustom,	0},
	{"button17",	"Walky",		SFDSF,		3,		FluidPort,		1,	NoCustom,	0},
	{"button18",	"Trumpet",		SFDSF,		66,		FluidPort,		1,	NoCustom,	0},
	{"button19",	"Steel Drums",SFFluidGM,	114,FluidPort,		1,	NoCustom,	0},
	{"button20",	"Strings",		SFDSF,		85,		FluidPort,		1,	NoCustom,	0},
// 20	Button		Title		Bank	Patch	Outport			
	{"button21",	"Drums",		128,		26,		FluidPort,		1,	NoCustom,	0},
	{"button22",	"Slap Bass",	SFDSF,		24,		FluidPort,		1,	NoCustom,	0},
	{"button23",	"Synth Hard",	SFDSF,		60,		FluidPort,		1,	NoCustom,	0},
	{"button24",	"Marimba",		SFDSF,		12,		FluidPort,		1,	NoCustom,	0},
	{"button25",	"Holdsworth", SFMusica,	48,		FluidPort,		1,	NoCustom,	0},
	{"button26",	"Steel Drums",SFDSF,		114,	FluidPort,		1,	NoCustom,	0},
	{"button27",	"Low Synth",	SFDSF,		45,		FluidPort,		1,	NoCustom,	0},
	{"button28",	"Mid0",		0xff,		0,		GI20Port,		1,	NoCustom,	0},
	{"button29",	"Mid-24",		0xff,		2,		GI20Port,		1,	NoCustom,	0},
	{"button30",	"Mid Screen",	SFDSF,		0,		FluidPort,		1,	NoCustom,	0},
// 30	Button		Title		Bank	Patch	Outport			
	{"NoButton",	"Rak Eff6",	116,		11,		FluidPort,		1,	Controller,	0},
	{"NoButton",	"Rak Eff7",	116,		13,		FluidPort,		1,	Controller,	0},
	{"NoButton",	"Rak Eff8",	116,		15,		FluidPort,		1,	Controller,	0},
	{"NoButton",	"Rak Eff9",	116,		17,		FluidPort,		1,	Controller,	0},
	{"NoButton",	"Rak Eff10",	116,		18,		FluidPort,		1,	Controller,	0},
	{"NoButton",	"Rak Eff1",	116,		0,		FluidPort,		1,	Controller,	0},
	{"NoButton",	"Rak Eff2",	116,		2,		FluidPort,		1,	Controller,	0},
	{"NoButton",	"Rak Eff3",	116,		4,		FluidPort,		1,	Controller,	0},
	{"NoButton",	"Rak Eff4",	116,		6,		FluidPort,		1,	Controller,	0},
	{"NoButton",	"Rak Eff5",	116,		8,		FluidPort,		1,	Controller,	0},
// 40	Button		Title		Bank	Patch	Outport			
	{"NoButton",	"TransStart",	0xff,		0,		TransportPort,	1,	TransStart,	0},
	{"NoButton",	"TransCont",	0xff,		0,		TransportPort,	1,	TransCont,	0},
	{"NoButton",	"TransStop",	0xff,		0,		TransportPort,	1,	TransStop,	0},
	{"NoButton",	"TransBack",	0xff,		0,		TransportPort,	1,	TransStart,	0},
	{"NoButton",	"TransPos",	0xff,		0,		TransportPort,	1,	TransStart,	0},
	{"NoButton",	"Tap",			116,		121,	FluidPort,		1,	Controller,	0},
	{"NoButton",	"Midi Tog",	116,		122,	FluidPort,		1,	Controller,	0},
	{"NoButton",	"Tuner",		116,		123,	FluidPort,		1,	Controller,	0},
	{"NoButton",	"AnaOnOff",	116,		124,	FluidPort,		1,	Controller,	0},
	{"NoButton",	"AnaOnOff",	116,		124,	FluidPort,		1,	Controller,	0},
// 50	Button		Title		Bank	Patch	Outport			
	{"NoButton",	"LP Undo",		0xff,		70,		LooperPort,	1,	NoCustom,	0},
	{"NoButton",	"LP 1",		0xff,		71,		LooperPort,	1,	NoCustom,	ToLooperApp},
	{"NoButton",	"LP 2",		0xff,		72,		LooperPort,	1,	NoCustom,	ToLooperApp},
	{"NoButton",	"LP 3",		0xff,		73,		LooperPort,	1,	NoCustom,	ToLooperApp},
	{"NoButton",	"LP 4",		0xff,		74,		LooperPort,	1,	NoCustom,	ToLooperApp},
	{"NoButton",	"LP Rec",		0xff,		75,		LooperPort,	1,	NoCustom,	0},
	{"NoButton",	"LP Pause",	0xff,		76,		LooperPort,	1,	NoCustom,	0},
	{"NoButton",	"LP Tog",		0xff,		77,		LooperPort,	1,	NoCustom,	0},
	{"NoButton",	"LP Tap",		0xff,		78,		LooperPort,	1,	NoCustom,	0},
	{"NoButton",	"LP All",		0xff,		79,		LooperPort,	1,	NoCustom,	0},
// 60	Button		Title		Bank	Patch	Outport			
	{"NoButton",	"AnalogApp",	0xff,		AnalogApp,		0,		1,	RaiseApp,	0},
	{"NoButton",	"MidiSoundApp",0xff,		MidiSoundApp,	0,		1,	RaiseApp,	0},
	{"NoButton",	"LooperApp",	0xff,		LooperApp,		0,		1,	RaiseApp,	0},
	{"NoButton",	"MidiControl",0xff,		MidiControl,	0,		1,	RaiseApp,	0},
	{"NoButton",	"MP3Player",	0xff,		MP3Player,		0,		1,	RaiseApp,	0},
	{"NoButton",	"TabPlayer",	0xff,		TabPlayer,		0,		1,	RaiseApp,	0},
	{"NoButton",	"PreTab",		0xff,		0		,		0,		1,	SwitchTab,	0},
	{"NoButton",	"ScoreTab",	0xff,		1,				0,		1,	SwitchTab,	0},

	{"NoButton",	"TransportApp",0xff,		TransportApp,	0,		1,	RaiseApp,	0},
\

	{"NoButton",	"Switch0",		0xff,		0,			0,		1,	ToDesktop,	0},
	{"NoButton",	"Switch1",		0xff,		1,			0,		1,	ToDesktop,	0},
	{"NoButton",	"Switch2",		0xff,		2,			0,		1,	ToDesktop,	0},
	{"NoButton",	"Switch3",		0xff,		3,			0,		1,	ToDesktop,	0},
	{"NoButton",	"Switch4",		0xff,		4,			0,		1,	ToDesktop,	0},
	{"NoButton",	"Switch5",		0xff,		5,			0,		1,	ToDesktop,	0},


	{"NoCustom",	"Elias1",		0xff,		0,		FluidPort,		2,	ToDesktop, 0}
  },
"/home/Dropbox/FusionBlue/ChartsHTML/",
// Number and name of the output ports
	6,
	{"Fluid", "Rakarrak", "Looper", "Transport", "GI20", "Tempo" },
	
	{"USB Audio CODEC", 0 },
	{"FLUID", 0 },
	{"sooperlooper", 0 },
	{"audacity", 0 },
	{"MidiLink", 0 },
	{"Tempo", 0 },
};


/*--------------------------------------------------------------------
* Function:		apply_font_to_widget
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void apply_font_to_widget (GtkWidget *widget, gchar *fload) {
	GtkStyle *style = gtk_style_new();
	gdk_font_unref(style->font_desc);
	style->font_desc = gdk_font_load(fload);
	style->font_desc = gdk_font_load("12x24");
//	gtk_style_set_font(style,fload);
	gtk_widget_set_style(GTK_WIDGET(widget), style);
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
		gtk_notebook_set_current_page(NoteBookPane, Tab);
		printf("Switch to Tab %d\n",Tab);
}

/*--------------------------------------------------------------------
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void on_button_clicked (GtkButton *button, gpointer	user_data ){
	DoPatch(&gMyInfo.MyPatchInfo[(char )GetModePreset(user_data - 1)]);
	printf("User data %d\n", user_data);
}

void on_modebutton_clicked (GtkButton *button, gpointer	user_data ){

	IncrementMode();
	printf("Increment Mode called from Buttons \n");

}

void on_About_clicked (GtkButton *button, gpointer	user_data ){
    GtkWidget *window;

	printf("About Button %d\n", user_data);
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

void
on_window1_destroy (GtkWidget *widget, gpointer user_data)
{
	/* break gtk_main() loop */
	gtk_main_quit();
}

void ConnectSignals(void) {
	char Loop;
	char ButtonName[255];
	GtkWidget *widget;
	GError	 *error = NULL;

	for (Loop = 1; Loop <= Max_Main_Buttons; Loop++ ) {
		sprintf(ButtonName, "button%d", Loop);

		widget = GTK_WIDGET (gtk_builder_get_object (gxml, ButtonName));
		g_signal_connect_data (G_OBJECT (widget), "clicked", 
			G_CALLBACK (on_button_clicked), Loop, NULL, 0);

		printf("Loop %d, Name %s %x %x\n",Loop, ButtonName, widget, gxml);
	}

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
printf("tab_focus_callback %x %x %x\n", notebook, arg1, data);
  return true;
}

/*--------------------------------------------------------------------
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
int
main (int argc, char *argv[]) {
	GtkWidget *main_window;
	GtkWidget *main_tab;
	GtkWidget *widget;
	GError	 *error = NULL;

	CurrentMode = 0;
		/* initialize the GTK+ library */
		gtk_init (&argc, &argv);
		gtk_rc_parse( MAINPREFS_FILE);
		/*
		create an instance of the GladeXML object and build widgets within
		the window1 root node.
		*/
//		gxml = glade_xml_new (GLADE_FILE, NULL, NULL);

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


		HoldStatusIndex = 0;
		memset(HoldStatus, 0, sizeof (HoldStatus));
		TempoDraw = GTK_WIDGET (gtk_builder_get_object (gxml, "Tempo"));

		ConnectSignals();

		widget = GTK_WIDGET (gtk_builder_get_object (gxml, "AboutButton"));
		g_signal_connect_data (G_OBJECT (widget), "clicked", 
			G_CALLBACK (on_About_clicked), NULL, NULL, 0);



		memcpy(&gMyInfo, &GlobalInfo, sizeof (GTKMidiInfo));
		strcpy(&gMyInfo.Apps[AnalogApp].Name, "rakarrack" );
		strcpy(&gMyInfo.Apps[MidiSoundApp].Name, "qsynth" );
		strcpy(&gMyInfo.Apps[LooperApp].Name, "slgui" );
		strcpy(&gMyInfo.Apps[TransportApp].Name, "" );
		strcpy(&gMyInfo.Apps[MidiControl].Name, "LiveMusic" );
		strcpy(&gMyInfo.Apps[MP3Player].Name, "clementine" );
		strcpy(&gMyInfo.Apps[TabPlayer].Name, "Tux" );
//		memset(&thePorts, 0, sizeof (PortsInfo));
		PrintDataStructure(&gMyInfo);

//		InitPref();
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
		SetUpMainButtons(&gMyInfo.MyPatchInfo);
printf("After SetUpMainButtons\n");	
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

		/* Set up a timer for Tempo.
		 */
		g_timeout_add(Timer1Ticks, (GSourceFunc) time_handler, (gpointer) gxml);
		gMyInfo.TempoTimerID = 0;
		SetTempo(60);
		/* show the main window */
		gtk_widget_show (main_window);
		gtk_widget_modify_font(CurrentModeWid, pango_font_description_from_string("Sans Bold 16"));
		gtk_label_set_text (CurrentModeWid, "Switch");

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
			sprintf(DisString, "[%15s] [%15s] [%15s] [%15s]", &HoldStatus[1], &HoldStatus[2], &HoldStatus[3], String);
		break;

		case 1:
			sprintf(DisString, "[%15s] [%15s] [%15s] [%15s]", &HoldStatus[2], &HoldStatus[3], &HoldStatus[0], String);
		break;

		case 2:
			sprintf(DisString, "[%15s] [%15s] [%15s] [%15s]", &HoldStatus[3], &HoldStatus[0], &HoldStatus[1], String);
		break;

		case 3:
			sprintf(DisString, "[%15s] [%15s] [%15s] [%15s]", &HoldStatus[0], &HoldStatus[1], &HoldStatus[2], String);
		break;

	}
	
	strncpy(&HoldStatus[HoldStatusIndex++], String,100);
	if (HoldStatusIndex >= MaxStatusHold)
			HoldStatusIndex = 0;

	gtk_widget_modify_font(MainStatus, pango_font_description_from_string("Sans Bold 20"));
		gtk_label_set_text (MainStatus, DisString);
}

/*--------------------------------------------------------------------
* Function:		Set the tempo
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void SetTempo(unsigned char NewTempo) {

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
	gMyInfo.TempoTimerID = g_timeout_add(gMyInfo.TempoReload, (GSourceFunc) tempo_handler, (gpointer) gxml);

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
* Function:		<Function name>
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void	SetUpMainButtons(PatchInfo  *PatchInfo) {
	GtkWidget *myButton;
	int			Loop;

	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		myButton = GTK_WIDGET (gtk_builder_get_object  (gxml, gMyInfo.MyPatchInfo[Loop].Button));
		
		printf("SetUpMainButtons: %s %x\n", gMyInfo.MyPatchInfo[Loop].Button, myButton);
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
void PrintDataStructure(GTKMidiInfo *myInfo) {
int		Loop;
int		Loop1;
PortsInfo	*thePorts;

	printf("Main Information\n");

	for (Loop = 0; Loop < Max_Patches; Loop++) {
		printf("ID=%d %s\n", Loop, myInfo->MyPatchInfo[Loop].Name);
	}

	printf("BaseName %s\n", myInfo->BasePath);

	for (Loop = 0; Loop < myInfo->NumOutPorts; Loop++) {
		printf("Ports %s\n", myInfo->OutPortName[Loop]);
	}

	printf("SoundGen %s\n", myInfo->SoundGen.Name);
	printf("AnalogEffects %s\n", myInfo->AnalogEffects.Name);
	printf("Looper %s\n", myInfo->Looper.Name);
	printf("Transport %s\n", myInfo->Transport.Name);
	printf("MidiController %s\n", myInfo->MidiController.Name);

	thePorts = &theInPorts;
	for ( Loop = 0; Loop < thePorts->NumDevices; Loop++ ) {
		printf("In Name [%s]\n", thePorts->Devices[Loop].Name);
		for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
			printf("In SName %d, [%s]\n",Loop1, thePorts->Devices[Loop].Ports[Loop1].Name);
		}
	}

	thePorts = &theOutPorts;
	for ( Loop = 0; Loop < thePorts->NumDevices; Loop++ ) {
		printf("Out Name [%s]\n", thePorts->Devices[Loop].Name);
		for (Loop1 = 0; Loop1 < thePorts->Devices[Loop].NumPorts; Loop1++) {
			printf("Out SName %d, [%s]\n",Loop1, thePorts->Devices[Loop].Ports[Loop1].Name);
		}
	}

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
		if (CurrentMode++ > ModeLastItem)
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
