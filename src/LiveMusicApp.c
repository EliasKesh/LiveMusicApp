/*---------------------------------------------------------------------
|      Revision Date:   2012/01/15 22:52:40
|
|      Contains:       Main code for the Live Music Application
|
|      Written by:     Elias Keshishoglou
|
|      Date:           Jan 15, 2012
|
|		Version:        1.3.4
|
|      Copyright © 2012 Elias Keshishoglou.  All rights reserved.
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

#define LiveMusicApp_c

#include "LiveMusicApp.h"

#include "stdlib.h"
#include <string.h>
#include "stdbool.h"
#include "AlsaUtils.h"
#include "HTML.h"
#include "PrefsFile.h"
#include "Connections.h"
#include "getopt.h"
#include "Timers.h"
#include "Player.h"
#include "SooperOSC.h"
#include "math.h"
#include "PrefsGui.h"
#include <unistd.h>
#include "config.h"
#include <pwd.h>

//#define UsingNewButtons	1

#ifdef UsingNewButtons
#define GLADE_FILE GetResourceDir("LiveMusicApp.glade.Buttons",FileLocConfig)
#else
#define GLADE_FILE "LiveMusicApp.glade"
#endif
#define Icon_FILE GetResourceDir("LiveIcon.png",FileLocConfig)
#define MaxTabs	5
#define UsingImageButtons

/* Max number of history messages.
*/
#define MaxStatusHold 8
#define CSSFileName GetResourceDir("LiveMusicApp.css",FileLocConfig)
#define HistoryFileName	"LiveMusicHistory"
#define MaxKeyTimeout	3

// export LIBGL_ALWAYS_SOFTWARE=1

/*
 * Place Global variables here
 */

// The Status Display about what patch was selected.
GtkWidget* MainStatus;
guint MainStatusid;

// The structure to hold the custom buttons.
theImageButtons LayoutButton;
theImageButtons MainButtons[Max_Main_Buttons];
theImageButtons TabButtons[MaxTabs];

// The Scale widgets.
GtkWidget *VScale1, *VScale2, *VScale3, *VScale4;
GtkAdjustment *Adjustment1, *Adjustment2, *Adjustment3, *Adjustment4;

// Images we use for the custom buttons.
GdkPixbuf *NoteBButtonOnImage;
GdkPixbuf *NoteBButtonOffImage;

// The Area we display the tempo
GtkWidget *TempoChild;

/* The entire pane we keep the tabs in.
*/
GtkWidget *NoteBookPane;

// The button size my change based on the screen size.
int ButtonSize;

// Foot switch layout.
int KeyLayout = 1;

/* Printf Level.
*/
int RunLogLevel = LogNone;

/* Array to hold the recent status messages.
*/
char HoldStatus[MaxStatusHold][50];
int HoldStatusIndex;

/* Boolean, send internal metronome to midi.
*/
int MainButtonCountOn;

/* Open a history file, I forget what I did
at rehearsal.
*/
FILE 	*FileHistory;
char *ResourceFileName[250];

/* Update the Tabs in GTK context.
*/

#define MuteCountDelay  8;
char  RemoveMuteCount;
char *homedir;

/*
 * Place Local prototypes here
 */
tPatchIndex LayoutSwitchPatch(tPatchIndex MidiIn, char DoAction);
int 	JackMaster;
void CreateTabButtons(void);
void CreateMainButtons(void);
void ToggleTempo(void);
void UpdateStatus(char *String);
void VScale1_Changed(GtkAdjustment *adj);
void VScale2_Changed(GtkAdjustment *adj);
void VScale3_Changed(GtkAdjustment *adj);
void VScale4_Changed(GtkAdjustment *adj);
tPatchIndex GetModePreset(tPatchIndex Value);
int CloseHistoryFile(void);
int InitHistoryFile(void);
gint key_press_cb(GtkWidget *widget, GdkEventKey *kevent, gpointer data);
gboolean tab_focus_callback(GtkNotebook *notebook, gint *arg1, gpointer data);
void on_Tempo_Button(GtkWidget *widget, GdkEvent *event, gpointer user_data);
void on_About_clicked(GtkButton *button, gpointer user_data);
gboolean layout_click_handler(GtkWidget *widget,
                              GdkEvent *event,
                              gpointer user_data);
gboolean layout_release_handler(GtkWidget *widget,
                                GdkEvent *event,
                                gpointer user_data);
void on_window1_destroy(GtkWidget *widget, gpointer user_data);

int InitJackTransport(void);
int CloseJackTransport(void);

#if 0
GdkPixbuf *create_pixbuf(const gchar * filename);
//void PrintDataStructure(LiveMusicInfo *myInfo);
void CheckConnectionData(LiveMusicInfo *myInfo);
void apply_font_to_widget(GtkWidget *widget, gchar *fload);
void on_hscale1_value_changed(GtkWidget *widget, gpointer user_data);
void CreateHTMLGuide(LiveMusicInfo *myInfo);
gint button_press_notify_cb(GtkWidget *entries, GdkEventKey *event,
                            GtkWidget *widget);
#endif

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

	if (RunLogLevel >= LogLevel || (LogLevel == LogTest) )
		printf( "L%d %s", LogLevel, p);

	return NULL;
}

/*--------------------------------------------------------------------
 * Function:		GetResourceDir
 *
 * Description:		Return the full path for the file.
 *
 *---------------------------------------------------------------------*/
char *GetResourceDir(char *FileName, char WhichLoc) {

	strcpy(ResourceFileName, homedir);

	if (WhichLoc == FileLocConfig)
		strcat(ResourceFileName, "/.config/LiveMusicApp/");

	if (WhichLoc == FileLocTunes)
		strcat(ResourceFileName, "/MySongs/");

	strcat(ResourceFileName, FileName);
	return (ResourceFileName);
}


/*--------------------------------------------------------------------
 * Function:		Check for the .config dirs
 *
 * Description:
 *
 *---------------------------------------------------------------------*/
void CheckForStartupDirs(void) {
	struct stat s;
	int err;
	char FileString[255];
	char CommandString[400];
	char *UserName;

	/* Get home directory.
	*/
	if ((homedir = getenv("HOME")) == NULL) {
		homedir = getpwuid(getuid())->pw_dir;
	}

	UserName = getenv("USER");

	NewInstall = 0;
	sprintf(FileString, "%s/.config/LiveMusicApp", homedir);
	err = stat(FileString, &s);
	if (-1 == err) {

		printf("Not Found %s\n", FileString);
		sprintf(CommandString, "rsync -avrx --chown=%s:%s /usr/share/LiveMusicApp %s/.config/", UserName, UserName, homedir);
		system(CommandString);
	}


	sprintf(FileString, "%s/MySongs", homedir);
	err = stat(FileString, &s);
	if (-1 == err) {
		sprintf(CommandString, "mv %s/.config/LiveMusicApp/MySongs %s/", homedir, homedir);
		system(CommandString);
		NewInstall = 1;
	}
}

/*--------------------------------------------------------------------
 * Function:            main
 *
 * Description:         This is where it all starts.
 *
 *---------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
	// GtkWidget *theMainWindow;
	GtkWidget *widget;
	GError *error = NULL;
	GtkWidget *ChordWidget;
	GtkWidget *PlayerWidget;
	GtkWidget *EventBox;
	GError *err = NULL;
	/*----- CSS ----------- */
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;
	/*-----------------------*/
	int BButtonX, BButtonY, MButtonX, MButtonY;
	int Loop;
	GdkScreen *myScreen;

	/*
	 * Let's setup some variables.
	 * CurrentLayout is the default to start with
	 * When we start are
	 */
	CurrentLayout = 0;
	WaitingforMidi = 0;
	GenerateHFile = 0;
	KeyLayout = 1;
	JackMaster = 0;
	RemoveMuteCount = 0;
	gMyInfo.TimerCount = 0;
	gMyInfo.TabSwitch = NULLSwitch;
	gMyInfo.RaiseSwitch = NULLSwitch;
	gMyInfo.NextDeskSwitch = NULLSwitch;
	gMyInfo.PrevDeskSwitch = NULLSwitch;
	gMyInfo.GoToDeskSwitch = Max_Patches;


	CheckForStartupDirs();

	// printf("Home Dir %s\n",homedir );

	GetResourceDir("./MyFile.png", FileLocConfig);

	/* Default name for the jack client.
	*/
	strcpy(JackName, "default");

	parse_cmdline(argc, argv);

	printf("Build date  : %s:%s\n", __DATE__, __TIME__);
	printf("Build Number %d\n", MY_BUILD_NUMBER);

	/* History saves the charts you open.
	I forget what I worked on at rehearsal,
	this helps me remeber.
	*/
	InitHistoryFile();

	/* Handle any HID pedals,
	Must be called before gtk_init
	*/
//	InitHIDGrab();

	/* initialize the GTK+ library */
	gtk_init(&argc, &argv);

	myScreen = gdk_screen_get_default();
	printd(LogInfo, "Screen Size %d %d\n", gdk_screen_get_width(myScreen), gdk_screen_get_height(myScreen));
	ScreenSize = 0;
	ButtonSize = 95;

	/* Based on the sreen, size the buttons.
	*/
	if (gdk_screen_get_width(myScreen) > 1400) {
		ScreenSize = 1;
		ButtonSize = 115;
	}

	if (gdk_screen_get_width(myScreen) > 1800) {
		ScreenSize = 2;
		ButtonSize = 130;
	}
	printd(LogInfo, "Button Size %d %d\n", ButtonSize);

#if 0
	GtkCssProvider *provider = gtk_css_provider_new();
//	gtk_css_provider_load_from_data(provider, ".frame{border:10px solid red;}", -1, NULL);
	gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (provider),
	                                 "* {border-color: #CC0000}",
	                                 -1, NULL);
	gtk_css_provider_load_from_data (provider,
	                                 "}\n", -1, NULL);
	FaderControl.png
	.scale.slider,
	.scale.slider.horizontal {
background - image: -gtk - scaled(url("assets/scale-slider-horz-dark.png"), url("assets/scale-slider-horz-dark@2.png"));
	}

	/*---------------- CSS ----------------------------------------------------------------------------------------------------*/
	provider = gtk_css_provider_new ();
	display = gdk_display_get_default ();
	screen = gdk_display_get_default_screen (display);
	gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	gsize bytes_written, bytes_read;

	const gchar* home = "YourPathHere"; // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

	GError *error = 0;

	gtk_css_provider_load_from_path (provider,
	                                 g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),
	                                 NULL);
	g_object_unref (provider);
	/*-------------------------------------------------------------------------------------------------------------------------*/

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#endif

	/*
	 * Initialize the XML reader/writer and set some basic values here.
	 */
	InitPref();

	/*
	 create an instance of the GladeXML object and build widgets within
	 the window1 root node.
	 */
	gxml = gtk_builder_new();

	/* Choose the glad file based on the layout we are using.
	*/
	sprintf(TempStrBuf, "%s.%d", GLADE_FILE, KeyLayout);
	if (!gtk_builder_add_from_file(gxml, GetResourceDir(TempStrBuf, FileLocConfig), &error)) {
		g_warning("Couldn't load builder file: %s", error->message);
		g_error_free(error);
	}

	/*
	 * get the window widget from the glade XML file
	 */
	theMainWindow = GTK_WIDGET(gtk_builder_get_object(gxml, "window1"));

	/*------------- CSS  --------------------------------------------------------------------------------------------------*/
	provider = gtk_css_provider_new();
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);

	gtk_style_context_add_provider_for_screen(screen,
	        GTK_STYLE_PROVIDER(provider),
	        GTK_STYLE_PROVIDER_PRIORITY_USER);

	printd(LogInfo, "ejk About to load\n");
	gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), CSSFileName, &err);

	g_object_unref(provider);


	/* Connect the close button.
	*/
	g_signal_connect(G_OBJECT (theMainWindow), "destroy", G_CALLBACK (on_window1_destroy), NULL);
	gtk_window_set_title(GTK_WINDOW(theMainWindow), "LiveMusicApp_1");
//	gtk_window_set_icon(GTK_WINDOW(theMainWindow), create_pixbuf(Icon_FILE));
	gtk_window_set_icon(GTK_WINDOW(theMainWindow), NULL);


	/* Get the button sizes.
	*/
	BButtonX = ButtonSize;
	BButtonY = (int) ((float) ButtonSize * 0.65);
	MButtonX = (int) ((float) ButtonSize * 0.80);
	MButtonY = (int) ((float) ButtonSize * 0.6);

	MainButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
	                        GetResourceDir("MainSwitchOn.png", FileLocConfig), MButtonX, MButtonY, NULL, NULL);
	MainButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
	                         GetResourceDir("MainSwitchOff.png", FileLocConfig), MButtonX, MButtonY, NULL, NULL);
	PatchButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
	                         GetResourceDir("PatchSwitchOn.png", FileLocConfig), BButtonX, BButtonY, NULL, NULL);
	PatchButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
	                          GetResourceDir("PatchSwitchOff.png", FileLocConfig), BButtonX, BButtonY, NULL, NULL);
	NoteBButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
	                         GetResourceDir("NoteBSwitchOn.png", FileLocConfig), MButtonX, MButtonY, NULL, NULL);
	NoteBButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
	                          GetResourceDir("NoteBSwitchOff.png", FileLocConfig), MButtonX, MButtonY, NULL, NULL);
//	GdkPixbuf *gdk_pixbuf_scale_simple (const GdkPixbuf *src, 135,65,  GDK_INTERP_NEAREST);
	NoteBookPane = GTK_WIDGET(gtk_builder_get_object(gxml, "MainTab"));

	/*
	 * Open the persistent main tab.
	 */
	g_signal_connect(GTK_NOTEBOOK( NoteBookPane ), "switch-page",
	                 (GCallback ) tab_focus_callback, gxml);

#if 0
	g_signal_connect(G_OBJECT(theMainWindow), "key_press_event", G_CALLBACK(key_press_cb), theMainWindow);
	gMyInfo.KeyPatchValue = 0;
#endif

	/*
	 * Setup and initialize our status bar and Mode indicator
	 */
	MainStatus = GTK_WIDGET(gtk_builder_get_object(gxml, "MainStatusBar"));

	/*
	 * Clear the Status bar buffer.
	 */
	HoldStatusIndex = 0;
	memset(HoldStatus, 0, sizeof(HoldStatus));

	/*
	 * Get the metronome button loaded and displayed.
	 */
	EventBox = GTK_WIDGET(
	               gtk_builder_get_object(gxml, "Tempo"));


	/* Set up my buttons.
	*/
	MyImageButtonInit(&TempoDraw, EventBox, MainButtonOnImage, MainButtonOffImage);

	/* Update the metronome button.
	*/
	if (gMyInfo.MetronomeOn) {
		MyImageButtonSetText(&TempoDraw, "On");
		gtk_image_set_from_pixbuf(GTK_IMAGE(TempoDraw.Image),
		                          TempoDraw.ButtonDownImage);
	} else {
		MyImageButtonSetText(&TempoDraw, "Off");
		gtk_image_set_from_pixbuf(GTK_IMAGE(TempoDraw.Image),
		                          TempoDraw.ButtonUpImage);
	}

	g_signal_connect(G_OBJECT(EventBox),
	                 "button-press-event",
	                 G_CALLBACK(on_Tempo_Button),
	                 &TempoDraw);

	/*
	 * The about window.
	 */
	widget = GTK_WIDGET(gtk_builder_get_object(gxml, "AboutButton"));
	g_signal_connect_data(G_OBJECT(widget), "clicked", G_CALLBACK(on_About_clicked), NULL, NULL, 0);

#if 0
	/*
	 * The OK Button on the About Box
	 */
	g_signal_connect(G_OBJECT(theMainWindow),
	                 "key_press_event",
	                 G_CALLBACK(
	                     button_press_notify_cb),
	                 NULL);
	/*
	 * Create the HTML page from our preferences.
	 */
	CreateHTMLGuide(&gMyInfo);
#endif

	/*
	 * Initialize the WebKit (HTML) engine
	 */
	InitHTML(gxml);
	printd(LogInfo, "After InitHTML\n");

	/*
	 * Set up the Midi Sequencer ports
	 */
	MyAlsaInit();
	printd(LogInfo, "After MyAlsaInit\n");

	/* OSC controls sooperlooper and jack-volume.
	*/
	MyOSCInit();

	/* If the options are for Jack Master.
	*/
	if (JackMaster)
		InitJackTransport();

	/*
	 * Set up the buttons, text and patches.
	 */
	CreateMainButtons();
	SetUpMainButtons(&gMyInfo.MyPatchInfo);
	CreateTabButtons();

	/* Get the Mode switch button,
	 */
	EventBox = GTK_WIDGET(
	               gtk_builder_get_object(gxml,
	                                      "LayoutEvent"));
	printd(LogInfo, "LayoutEvent %x\n", (unsigned int) EventBox);
	MyImageButtonInit(&LayoutButton, EventBox, MainButtonOnImage,
	                  MainButtonOffImage);

	MyImageButtonSetText(&LayoutButton, gMyInfo.LayoutPresets[0].Name);

	g_signal_connect(G_OBJECT(EventBox),
	                 "button-press-event",
	                 G_CALLBACK(
	                     layout_click_handler),
	                 &LayoutButton);
	g_signal_connect(G_OBJECT(EventBox),
	                 "button-release-event",
	                 G_CALLBACK(
	                     layout_release_handler),
	                 &LayoutButton);

	/*
	 * Set up the connections between applications.
	 */
	InitConnections();

	/*
	 * Set the up Chord page in case we need it later.
	 */
	ChordWidget = GTK_WIDGET(gtk_builder_get_object(gxml, "ChordFrame"));
	ChorderMain(theMainWindow, ChordWidget);

	/*
	 * Set the initial Volumes.
	 */
	printd(LogInfo, "Setting Default Volumes\n");
	SetVolume1(gMyInfo.AnalogVolume);
	SetVolume2(gMyInfo.MidiVolume);

	printd(LogInfo, "Entering gtk_main\n");

	/*
	 * Start the timers
	 */
	MyTimerInit();

	/*
	 * Add and Idle for non interrupt based processing.
	 */
//	g_idle_add(GTKIdel_cb, theMainWindow);

	/*
	 * Set up the Live Player
	 */
	PlayerWidget = GTK_WIDGET(gtk_builder_get_object(gxml, "PlayerFrame"));
	LivePlayerInit(theMainWindow, PlayerWidget);

	WaitingforMidi = WaitingforMidiHold = 0;

	/*
	 * Set up the GUI for making changes to the preferences.
	 */
	InitGuiPrefs();

	/*
	 * Create the popup Menu's (ctrl-click) now that we have the presets.
	 */
	CreatePatchPopupMenu ();

	/*
	 * Show the main window and let the show begin.
	 */
	gtk_widget_show_all(theMainWindow);

	/*
	 * And they're off.
	 */
	printd(LogInfo, "And we're off \n");
	gtk_main();

	/*
	 * After we quit we should write back the changes.
	 */
	CloseHIDGrab();
	WritePrefs();
	MyAlsaClose();
	MyOSCClose();
	CloseJackTransport();
	LivePlayerClose();
	printd(LogInfo, "Closing LiveApp\n");

	return 0;
}

/*--------------------------------------------------------------------
 * Function:            GTKIdel_cb
 *
 * Description: Startup some Gui.
 *---------------------------------------------------------------------*/
int GTKIdel_cb(gpointer data) {

	/* Can't call this from the thread so the
	 thread sets the structure and then the action
	 gets performed here.
	 Expression control of active GUI sliders.
	*/
//	printd(LogDebug, "GTKIdel_cb %d %d\n", AlsaEvent.data.control.param, gMyInfo.ExpreP1Slider);

	if (AlsaEvent.data.control.param == MIDI_CTL_MSB_MAIN_VOLUME) {
		switch (gMyInfo.MyPatchInfo[gMyInfo.ExpreP1Slider].Channel) {
		case Slider1:
			printd(LogDebug, "GTKIdel_cb Slider1 %d \n", Slider1);
			SetVolume1(AlsaEvent.data.control.value / 1.28);
			break;

		case Slider2:
			printd(LogDebug, "GTKIdel_cb Slider2 %d \n", Slider2);
			SetVolume2(AlsaEvent.data.control.value / 1.28);
			break;

		case Slider3:
			printd(LogDebug, "GTKIdel_cb Slider3 %d \n", Slider3);
			SetVolume3(AlsaEvent.data.control.value / 1.28);
			break;

		case Slider4:
			printd(LogDebug, "GTKIdel_cb Slider4 %d \n", Slider4);
//			SetScale4Label(gMyInfo.MyPatchInfo[3].Name);

		default:
//			printd(LogInfo, "GTKIdel_cb: %d\n", AlsaEvent.data.control.param);
			printd(LogDebug, "GTKIdel_cb Default\n");
//			SetScale4Label(gMyInfo.MyPatchInfo[gMyInfo.ExpreP1Slider].Name);
			SetVolume4(AlsaEvent.data.control.value / 1.28);
			break;
		}

		/* Clear it until next message set
		*/
		AlsaEvent.data.control.param = 0;
	}


	/* Check to see if a TAB was switched.
	*/
	if (gMyInfo.TabSwitch != NULLSwitch) {
		printd(LogDebug, "TabSwitch %d\n", gMyInfo.GuitarMidiCallParam1);
		SwitchToTab(gMyInfo.TabSwitch);
		gMyInfo.TabSwitch = NULLSwitch;
		printd(LogDebug, "TabSwitch End %d\n", gMyInfo.GuitarMidiCallParam1);
	}

	/* Check to see if a Window was switched.
	*/
	if (gMyInfo.RaiseSwitch != NULLSwitch) {
		printd(LogDebug, "RaiseSwitch %d\n", gMyInfo.GuitarMidiCallParam1);
		RaiseWindowsNum(gMyInfo.RaiseSwitch);
		gMyInfo.RaiseSwitch = NULLSwitch;
		printd(LogDebug, "RaiseSwitch End %d\n", gMyInfo.GuitarMidiCallParam1);
	}

	/* Can't call this in the alsa thread.
	*/
	if (gMyInfo.GuitarMidiCall == GuitarMidiCallStart) {
		printd(LogDebug, "GuitarMidiCallStart %d\n", gMyInfo.GuitarMidiCallParam1);
		GuitarMidiPreset(gMyInfo.GuitarMidiCallParam1);
		gMyInfo.GuitarMidiCall = GuitarMidiCallNo;
	}

	if (gMyInfo.GuitarMidiCall == GuitarMidiCallComplete) {
		printd(LogDebug, "GuitarMidiCallComplete %d\n", gMyInfo.GuitarMidiCallParam1);
		GuitarMidiPresetComplete(gMyInfo.GuitarMidiCallParam1);
		gMyInfo.GuitarMidiCall = GuitarMidiCallNo;
	}

	if (gMyInfo.LayoutCall) {
		printd(LogDebug, "LayoutCall %d\n", gMyInfo.LayoutCall);
		LayoutSwitchPatch(gMyInfo.LayoutCallParam1, gMyInfo.LayoutCallParam2);
		gMyInfo.LayoutCall = FALSE;
		gMyInfo.LayoutCallParam1 = 0;
		gMyInfo.LayoutCallParam2 = 0;

	}


	if (gMyInfo.NextDeskSwitch) {
		NextDesktop();
		gMyInfo.NextDeskSwitch = FALSE;
	}

	if (gMyInfo.PrevDeskSwitch) {
		PrevDesktop();
		gMyInfo.PrevDeskSwitch = FALSE;
	}

	if (gMyInfo.GoToDeskSwitch) {
		GoToDesktop(gMyInfo.GoToDeskSwitch);
		gMyInfo.GoToDeskSwitch = Max_Patches;
	}

	if (gMyInfo.IncrementSwitch) {
		IncrementMode();
		gMyInfo.IncrementSwitch = NULL;
	}

	if (RemoveMuteCount) {
		if (RemoveMuteCount-- == 1 ) {
			MyOSCJackMute(0, 0);
			RemoveMuteCount = 0;
		}
	}

	/* If new information about the slider.
	*/
	if (gMyInfo.SliderUpdate > 0 && gMyInfo.SliderUpdate < Max_Patches) {
		printd(LogDebug, "SliderUpdate %d\n", gMyInfo.SliderUpdate);
		if (gMyInfo.MyPatchInfo[gMyInfo.SliderUpdate].CustomCommand == cmdSetExpr) {
			printd(LogDebug, "SliderUpdate \n");
			SetScale4Label(gMyInfo.MyPatchInfo[gMyInfo.SliderUpdate].Name);
		}
		gMyInfo.SliderUpdate = 0;
	}

	/* If the timer went off, update the metronome.
	*/
	if (UIUpdateFromTimer == TRUE) {
		UIUpdateFromTimer = FALSE;
		printd(LogDebug, "UIUpdateFromTimer 1\n");
		MyImageButtonSetText(&TempoDraw, TempoUpdateString);

		/* Make sure the buttons are all up after being pressed.
		*/
		ClearMainButtons();
		/* With these off doesn't
		seem to lock up.
		*/
		if (JackMaster)
			jack_poll();

		MyOSCPoll(FALSE);
		PlayerPoll(TRUE);
//		HIDPoll();

		/*  Turn Pedal lights off
		*/
		SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
		         DrumMidiChannel, 04, (int) PedalLED3Off );

		SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort,
		         DrumMidiChannel, 04, (int) PedalLED4Off );

	}

#ifdef AcceptTimedKeyboard
	gMyInfo.TimerCount++;

	// If we have a key in the works.
	if (gMyInfo.KeyPatchValue != 0 ) {
		if (gMyInfo.KeyIsValue) {
			printd(LogDebug, "Idle %d\n", gMyInfo.KeyIsValue);
			if ( (gMyInfo.TimerCount - gMyInfo.KeyTimer) > MaxKeyTimeout ) {
				gMyInfo.KeyIsValue = 0;
				printd(LogDebug, "Final Key %d\n", gMyInfo.KeyPatchValue);
				if (gMyInfo.KeyPatchValue > 0 && gMyInfo.KeyPatchValue < LayoutSwitchPatch)
					LayoutSwitchPatch(gMyInfo.KeyPatchValue - 1, true);

				gMyInfo.KeyPatchValue = 0;
			}
		} else {
			printd(LogDebug, "We have letter %c\n", gMyInfo.KeyChar);
			gMyInfo.KeyPatchValue = 0;

			// Space will toggle player
			if (gMyInfo.KeyPatchValue == ' ') {
				plLoopToggle();
			}

		}
	}
#endif
//	printd(LogDebug, "GTKIdel_cb out\n");
	return (FALSE);
}

/*--------------------------------------------------------------------
 * Function:		SwitchToTab
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void SwitchToTab(char Tab) {
	int Loop;

	// Check for a valid tab number
	if (Tab < 0 || Tab >= MaxApps)
		return;

	// Clear all of the tab buttons.
	for (Loop = 0; Loop < MaxTabs; Loop++) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(TabButtons[Loop].Image),
		                          TabButtons[Loop].ButtonUpImage);
	}

	Loop = Tab;
	gtk_image_set_from_pixbuf(GTK_IMAGE(TabButtons[Loop].Image),
	                          TabButtons[Loop].ButtonDownImage);

//	PreviousTab = gtk_notebook_get_current_page(GTK_NOTEBOOK(NoteBookPane));
	gtk_notebook_set_current_page(GTK_NOTEBOOK(NoteBookPane), Tab);
	printd(LogDebug, "Switch to Tab %x %d %d\n", NoteBookPane, PreviousTab, Tab);
}

/*--------------------------------------------------------------------
 * Function:		on_button_clicked
 *
 * Description:		When the uses presses one of the main screen patch
 * 	buttons we get dispatched here.
 *
 *---------------------------------------------------------------------*/
void on_button_clicked(GtkButton *button, gpointer user_data) {
//	tPatchIndex PatchIndex;

	printd(LogInfo, "on_button_clicked %d\n", (int) user_data);
//	PatchIndex = LayoutSwitchPatch(user_data, true);
	LayoutSwitchPatch(user_data, true);
}

/*--------------------------------------------------------------------
 * Function:		on_layoutbutton_clicked
 *
 * Description:		When the mode button is pressed from the screen we call
 * 	this.
 *
 *---------------------------------------------------------------------*/
gboolean layout_click_handler(GtkWidget *widget,
                              GdkEvent *event,
                              gpointer user_data) {
	theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	printd(LogInfo, "layout_click %x\n", theButton);
	IncrementMode();
	MyImageButtonSetText(theButton, gMyInfo.LayoutPresets[CurrentLayout].Name);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image), theButton->ButtonDownImage);
	return TRUE; /* stop event propagation */
}

/*--------------------------------------------------------------------
 * Function:		layout_release_handler
 *
 * Description:
 *
 *---------------------------------------------------------------------*/
gboolean layout_release_handler(GtkWidget *widget,
                                GdkEvent *event,
                                gpointer user_data) {
	theImageButtons *theButton;
	theButton = (theImageButtons *) user_data;
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image), theButton->ButtonUpImage);
	return TRUE; /* stop event propagation */
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
 * Function:		on_window1_destroy
 *
 * Description:		That's IT, I'm outta here.
 *
 *---------------------------------------------------------------------*/
void on_window1_destroy(GtkWidget *widget, gpointer user_data) {

	/* break gtk_main() loop */
//	printd(LogInfo, "In Destroy\n");
	WritePrefs();
	gtk_main_quit();
}

/*--------------------------------------------------------------------
 * Function:		on_Tempo_Button
 *
 * Description:		Turn the Metrognome on or off
 *
 *---------------------------------------------------------------------*/
void on_Tempo_Button(GtkWidget *widget, GdkEvent *event, gpointer user_data) {

	// Put Dialog here.
	if (event->button.state & GDK_CONTROL_MASK) {
	}

	if (gMyInfo.MetronomeOn) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(TempoDraw.Image),
		                          TempoDraw.ButtonUpImage);
		gMyInfo.MetronomeOn = 0;
	} else {
		gtk_image_set_from_pixbuf(GTK_IMAGE(TempoDraw.Image),
		                          TempoDraw.ButtonDownImage);
		gMyInfo.MetronomeOn = 1;
	}
}

/*--------------------------------------------------------------------
 * Function:		tab_focus_callback
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
gboolean tab_focus_callback(GtkNotebook *notebook, gint *arg1, gpointer data) {
	SetUpMainButtons(&gMyInfo.MyPatchInfo);
	return true;
}

/*--------------------------------------------------------------------
 * Function:            ClearMainButtons
 *
 * Description: Make sure the buttons are all up.
 *---------------------------------------------------------------------*/
void ClearMainButtons(void) {
	int Loop;

	if (MainButtonCountOn == 1) {
		for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
//			if (MainButtons[Loop].State) {
			gtk_image_set_from_pixbuf(GTK_IMAGE(
			                              MainButtons[Loop].Image),
			                          MainButtons[Loop].ButtonUpImage);
			MainButtons[Loop].State = 0;
			MainButtonCountOn = 0;
//			}
		}
	} else if (MainButtonCountOn)
		MainButtonCountOn--;
}


/*--------------------------------------------------------------------
 * Function:            parse_cmdline
 *
 * Description: 	Deal with command line arguments.
 *---------------------------------------------------------------------*/
void parse_cmdline(int argc, char *argv[]) {
	int c;
	int digit_optind = 0;

	printf("Arg 0 %s\n", argv[0]);
	while (1)  {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{ "verbose", no_argument, 0, 'v' },
			{ "FontSize", required_argument, 0, 'f' },
			{ "jack", required_argument, 0, 'j' },
			{ "enable jack", no_argument, 0, 'e' },
			{ "layout", required_argument, 0, 'l' },
			{ "IncludeFile", no_argument, 0, 'i' },
//			{ "IncludeFile", required_argument, &GenerateHFile, 1 },
			{ 0, 0, 0, 0 }
		};

		c = getopt_long(argc, argv, "?hiev:f:j:l:",
		                long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'i':
			printf("IncludeFile \n");
			GenerateHFile = 1;
			break;

		case 'f':
			ButtonSize = atoi(optarg);
			printd(LogInfo, "Font Size %d\n", ButtonSize);
			break;

		case 'j':
			strncpy(JackName, optarg, MaxStringPortName);
			printd(LogInfo, "JackName %s\n", JackName);
			break;

		case 'l':
			KeyLayout = atoi(optarg);
			printd(LogInfo, "Layout %d\n", KeyLayout);
			break;

		case 'v':
			RunLogLevel = atoi(optarg);
			printd(LogInfo, "RunLogLevel 1-no -> 6-all %d\n", RunLogLevel);
			break;

		case 'e':
			JackMaster = 1;
			printd(LogInfo, "JackMaster off\n");
			break;

		default:
		case 'h':
			printf("Live Music CLI Usage\n");
			printf(" v verbose - Debug output level\n");
			printf(" f FontSize - Font Size for smaller screens.\n");
			printf(" j jack - Jack master name.\n");
			printf(" e enable-jack - Connect to jackserver.\n");
			printf(" l Layout - Glade layout file.\n");
			printf(" i IncludeFile - Generate include file on exit.\n");

			exit(0);
			break;
		}
	}

	if (optind < argc) {
		printd(LogInfo, "non-option ARGV-elements: ");
		while (optind < argc)
			printd(LogInfo, "%s ", argv[optind++]);
		printd(LogInfo, "\n");
	}
}

/*--------------------------------------------------------------------
 * Function:		Update the display status
 * Description:		raw the previous patches on the screen to make
 * 	sure that they completed. It's hard to tell with a foot pedal .
 *
 *---------------------------------------------------------------------*/
void UpdateStatus(char *String) {
	GtkStyle *hold;
	char DisString[500];
	int Loop;
	int StringOff = 0;

	for (Loop = 0; Loop < MaxStatusHold - 1; Loop++) {
		strcpy(HoldStatus[Loop], HoldStatus[Loop + 1]);
//	printd(LogDebug,"String off %d %s\n", StringOff, HoldStatus[Loop]);
		StringOff += sprintf((DisString + StringOff),
		                     "<span font=\"12\" color='#%lx'><b>%12s\n</b></span>",
		                     gMyInfo.StatusTextColor,
		                     (char*) &HoldStatus[Loop]);

	}
	strcpy(HoldStatus[MaxStatusHold - 1], String);
	sprintf((DisString + StringOff),
	        "<span font=\"12\" color='#%lx'><b>%12s\n</b></span>",
	        gMyInfo.StatusTextColor,
	        (char*) String);

	/* Actually draw the text to the window.
	 */
	gtk_widget_override_font(MainStatus,
	                         pango_font_description_from_string("Sans Bold 12"));
	gtk_label_set_markup((GtkLabel *) MainStatus, DisString);

}

/*--------------------------------------------------------------------
 * Function:		Main Button Handler
 *
 *---------------------------------------------------------------------*/
gboolean click_handler(GtkWidget *widget,
                       GdkEvent *event,
                       gpointer user_data) {
	int Loop;
	GdkEvent *theEvent;

	theEvent = gtk_get_current_event();
	printd(LogInfo, "Event %x %x\n", theEvent->button.state, GDK_CONTROL_MASK);

	Loop = (int) user_data;
	CurrentPreset = Loop;
	if (theEvent->button.state & GDK_CONTROL_MASK) {
		printd(LogInfo, "We have Control Down\n");
		ShowPatchListSelect(GTK_WINDOW(widget), Loop);
	}

	/* Only call make the call if we are not adding,
	a new patch to the button.
	*/
	else
		LayoutSwitchPatch(Loop, true);

	return TRUE; /* stop event propagation */
}

/*--------------------------------------------------------------------
 * Function:		release_handler
 *
 * Description:		Clear the button
 *---------------------------------------------------------------------*/
gboolean release_handler(GtkWidget *widget,
                         GdkEvent *event,
                         gpointer user_data) {
	int Loop;

	Loop = (int) user_data;
	gtk_image_set_from_pixbuf(GTK_IMAGE(MainButtons[Loop].Image),
	                          MainButtons[Loop].ButtonUpImage);
	return TRUE; /* stop event propagation */
}

/*--------------------------------------------------------------------
 * Function:		Notebook_click_handler
 *
 * Description:		Build the notebook tab
 *---------------------------------------------------------------------*/
gboolean Notebook_click_handler(GtkWidget *widget,
                                GdkEvent *event,
                                gpointer user_data) {
	int Loop;

	Loop = (int) user_data;
//	gtk_notebook_set_current_page(GTK_NOTEBOOK(NoteBookPane), Loop);
	SwitchToTab(Loop);
	PreviousTab = CurrentTab;
	CurrentTab = Loop;

	return TRUE; /* stop event propagation */
}

/*--------------------------------------------------------------------
 * Function:		CreateTabButtons
 *
 * Description:		Build the notebook tab
 *---------------------------------------------------------------------*/
void CreateTabButtons(void) {
	int Loop;
	char Buffer[40];
	GtkWidget *MainButtonImage;
	GtkWidget *EventBox;

	for (Loop = 0; Loop < MaxTabs; Loop++) {
		sprintf(Buffer, "NTab%d", Loop + 1);
		EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, Buffer));
		MyImageButtonInit(&TabButtons[Loop], EventBox, NoteBButtonOnImage, NoteBButtonOffImage);

		g_signal_connect(G_OBJECT(EventBox),
		                 "button-press-event",
		                 G_CALLBACK(Notebook_click_handler),
		                 Loop);
	}

	MyImageButtonSetText(&TabButtons[0], "Patch");
	MyImageButtonSetText(&TabButtons[1], "Chart");
	MyImageButtonSetText(&TabButtons[2], "Player");
	MyImageButtonSetText(&TabButtons[3], "Chords");
	MyImageButtonSetText(&TabButtons[4], "Prefs");
	gtk_image_set_from_pixbuf(GTK_IMAGE(TabButtons[0].Image),
	                          TabButtons[0].ButtonDownImage);

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
	GtkWidget *MainButtonImage;
	GtkWidget *EventBox;

#ifdef UsingImageButtons
	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		sprintf(Buffer, "eventbox%d", Loop + 1);
		EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, Buffer));
//		gtk_widget_get_usize(EventBox);

		MyImageButtonInit(&MainButtons[Loop], EventBox, PatchButtonOnImage,
		                  PatchButtonOffImage);

		g_signal_connect(G_OBJECT(EventBox),
		                 "button-press-event",
		                 G_CALLBACK(click_handler),
		                 Loop);
		g_signal_connect(G_OBJECT(EventBox),
		                 "button-release-event",
		                 G_CALLBACK(release_handler),
		                 Loop);
	}
#endif
	GtkWidget *Label;

	VScale1 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale1"));
	Adjustment1 = (GtkAdjustment *) (gtk_builder_get_object(gxml, "adj_Analog"));
	g_signal_connect(G_OBJECT (VScale1), "value_changed",
	                 G_CALLBACK (VScale1_Changed), NULL);
	Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label1"));
	gtk_label_set_text((Label), (gchar *)gMyInfo.MyPatchInfo[gMyInfo.HardSlider[0]].Name);


	VScale2 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale2"));
	Adjustment2 = (GtkAdjustment *) (gtk_builder_get_object(gxml, "adj_Midi"));
	g_signal_connect(G_OBJECT (VScale2), "value_changed",
	                 G_CALLBACK (VScale2_Changed), NULL);
	Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label2"));
	gtk_label_set_text((Label), (gchar *)gMyInfo.MyPatchInfo[gMyInfo.HardSlider[1]].Name);

	VScale3 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale3"));
	Adjustment3 = (GtkAdjustment *) (gtk_builder_get_object(gxml, "adj_Master"));
	g_signal_connect(G_OBJECT (VScale3), "value_changed",
	                 G_CALLBACK (VScale3_Changed), NULL);
	Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label3"));
	gtk_label_set_text((Label), (gchar *)gMyInfo.MyPatchInfo[gMyInfo.HardSlider[2]].Name);

	VScale4 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale4"));
	Adjustment4 = (GtkAdjustment *) (gtk_builder_get_object(gxml, "adj_Express"));
	g_signal_connect(G_OBJECT (VScale4), "value_changed",
	                 G_CALLBACK (VScale4_Changed), NULL);
	Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label4"));
	gtk_label_set_text((Label), (gchar *)gMyInfo.MyPatchInfo[gMyInfo.HardSlider[3]].Name);

}

/*--------------------------------------------------------------------
 * Function:		SetScale4Label
 *
 * Description:		Volume 4 Label
 *
 *---------------------------------------------------------------------*/
void SetScale4Label(char *String) {
	GtkWidget *Label;

	Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label4"));
	printd(LogDebug, "SetScale4Label %d %s\n", Label, String);
	gtk_label_set_text((Label), (gchar *)String);
}

/*--------------------------------------------------------------------
 * Function:		VScale1_Changed
 *
 * Description:		Volume Sliders changed.
 * 				All values from 0-127
 *
 *---------------------------------------------------------------------*/
void VScale1_Changed(GtkAdjustment *adj) {
	int ThisPatchNum;
	PatchInfo *ThisPatch;
	unsigned int NewValue;

	/* Set the number of decimal places to which adj->value is rounded */
	//   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
	NewValue = gtk_adjustment_get_value(Adjustment1);
	printd(LogDebug, "Vscale 1 %d\n", NewValue);
	ThisPatchNum = gMyInfo.HardSlider[0];
	ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];
//	gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment1);

	if (ThisPatch->OutPort == InternalPort)
		MyOSCJackVol(NewValue, 0);
	else
		SendMidi(SND_SEQ_EVENT_CONTROLLER,
		         ThisPatch->OutPort,
		         ThisPatch->Channel,
		         ThisPatch->Patch,
		         (char) NewValue * 1.27);

	printd(LogDebug, "Vscale 1 End %d\n", NewValue);

}

/*--------------------------------------------------------------------
 * Function:		VScale2_Changed
 *
 * Description:		Volume Sliders changed.
 *
 *---------------------------------------------------------------------*/
void VScale2_Changed(GtkAdjustment *adj) {
	int ThisPatchNum;
	PatchInfo *ThisPatch;
	unsigned int NewValue;
	unsigned int LogValue;

	/* Set the number of decimal places to which adj->value is rounded */
	//   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
	NewValue = gtk_adjustment_get_value(Adjustment2);
	printd(LogInfo, "Vscale 2 %d\n", NewValue);
	ThisPatchNum = gMyInfo.HardSlider[1];
	ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];
	gMyInfo.MidiVolume = (char) gtk_adjustment_get_value(Adjustment2) * 1.25;

	LogValue = (int)( pow(NewValue, 0.6) * 6.35);

	SendMidi(SND_SEQ_EVENT_CONTROLLER,
	         ThisPatch->OutPort,
	         ThisPatch->Channel,
	         ThisPatch->Patch,
	         (char) LogValue);
}

/*--------------------------------------------------------------------
 * Function:		VScale3_Changed
 *
 * Description:		Volume Sliders changed.
 *
 *---------------------------------------------------------------------*/
void VScale3_Changed(GtkAdjustment *adj) {
	int ThisPatchNum;
	PatchInfo *ThisPatch;
	unsigned int NewValue;
	unsigned int LogValue;

	/* Set the number of decimal places to which adj->value is rounded */
	//   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
	NewValue = gtk_adjustment_get_value(Adjustment3);
//	printd(LogInfo, "Vscale 3 %d\n", NewValue);
	ThisPatchNum = gMyInfo.HardSlider[2];
	ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];

	LogValue = (int)( pow(NewValue, 0.6) * 6.35);

	if (ThisPatch->OutPort == InternalPort)
		MyOSCJackVol(LogValue, 0);
	else
		SendMidi(SND_SEQ_EVENT_CONTROLLER,
		         ThisPatch->OutPort,
		         ThisPatch->Channel,
		         ThisPatch->Patch,
		         (char) LogValue * 1.27);
}
/*--------------------------------------------------------------------
 * Function:		VScale4_Changed
 *
 * Description:		Volume Sliders changed.
 *
 *---------------------------------------------------------------------*/
void VScale4_Changed(GtkAdjustment *adj) {
	int ThisPatchNum;
	PatchInfo *ThisPatch;
	unsigned int NewValue;

	/* Set the number of decimal places to which adj->value is rounded */
	//   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
	NewValue = gtk_adjustment_get_value(Adjustment4);
	ThisPatchNum = gMyInfo.ExpreP1Slider;
	ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];
	printd(LogInfo, "Vscale 4 %d P=%d\n",
	       NewValue, ThisPatch->Channel);

//	gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment4);

	if (ThisPatch->OutPort == InternalPort)
		MyOSCJackVol(NewValue, 0);
	else
		SendMidi(SND_SEQ_EVENT_CONTROLLER,
		         ThisPatch->OutPort,
		         ThisPatch->Channel,
		         ThisPatch->Patch,
		         (char) NewValue * 1.27);
}

/*--------------------------------------------------------------------
 * Function:		SetVolume1
 *
 * Description:	Change the Volume Slider based on midi input.
 *---------------------------------------------------------------------*/
int SetVolume1(int Value) {
	gtk_adjustment_set_value(Adjustment1, Value);
	printd(LogDebug, "Slider 1\n");
	gMyInfo.AnalogVolume = Value;
	gtk_range_set_adjustment(VScale1, Adjustment1);

	return (Value);
}

/*--------------------------------------------------------------------
 * Function:		SetVolume2
 *
 * Description:	Change the Volume Slider based on midi input.
 *---------------------------------------------------------------------*/
int SetVolume2(int Value) {
	printd(LogDebug, "Slider 2 %x %d\n",
	       Adjustment2, Value);

	gtk_adjustment_set_value(Adjustment2, Value);
	gMyInfo.MidiVolume = (Value * 1.27);
	gtk_range_set_adjustment(VScale2, Adjustment2);
	return (Value);
}

/*--------------------------------------------------------------------
 * Function:		SetVolume3
 *
 * Description:
 *---------------------------------------------------------------------*/
int SetVolume3(int Value) {
	printd(LogDebug, "Slider 3 %x %d\n",
	       Adjustment2, Value);
	gtk_adjustment_set_value(Adjustment3, Value);
	gtk_range_set_adjustment(VScale3, Adjustment3);
	return (Value);
}

/*--------------------------------------------------------------------
 * Function:		SetVolume4
 *
 * Description:
 *---------------------------------------------------------------------*/
int SetVolume4(int Value) {
	printd(LogDebug, "Slider 4 %x %d\n",
	       Adjustment2, Value);
	gtk_adjustment_set_value(Adjustment4, Value);
	gtk_range_set_adjustment(VScale4, Adjustment4);
	return (Value);
}

/*--------------------------------------------------------------------
 * Function:		SetUpMainButtons
 *
 * Description:		Set the title of all of the buttons
 *
 *---------------------------------------------------------------------*/
void SetUpMainButtons(PatchInfo *myPatchInfo) {
	GtkWidget *myButton;
	GtkWidget *myChild;
	tPatchIndex Loop;
	tPatchIndex PatchIndex;
	GdkColor color;
	char String[PatchNameSize];
	int StringLen;

#ifdef UsingImageButtons
	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		PatchIndex = LayoutSwitchPatch(Loop, FALSE);
		printd(LogInfo, "SetUpMainButtons1:IMG %d %d \n", Loop, PatchIndex);

		if (PatchIndex >= 0 && PatchIndex < Max_Patches) {
			StringLen = strlen(gMyInfo.MyPatchInfo[PatchIndex].Name);
			sprintf(String, "     %02d     \n%*s",
			        Loop + 1,
			        (14 + StringLen) / 2,
			        gMyInfo.MyPatchInfo[PatchIndex].Name);
			printd(LogInfo, "SetUpMainButtons:IMG %d %d %s \n", Loop, PatchIndex, gMyInfo.MyPatchInfo[PatchIndex].Name);

			MyImageButtonSetText(&MainButtons[Loop], String);
		}
	}
#else
	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		myButton = MainButtons[Loop];
		PatchIndex = LayoutSwitchPatch(Loop, FALSE);

		if (PatchIndex >= 0 && PatchIndex < Max_Patches) {
			sprintf(String, "%02d-%s", Loop + 1,
			        gMyInfo.MyPatchInfo[PatchIndex].Name);
			printd(LogInfo, "SetUpMainButtons %d %s \n", PatchIndex, gMyInfo.MyPatchInfo[PatchIndex].Name);
			myChild = gtk_bin_get_child(GTK_BIN(myButton));
			gtk_label_set_text((myChild), String);
		}
	}
#endif
}

/*--------------------------------------------------------------------
 * Function:
 *
 * Description:         <Description/Comments>
 *
 *---------------------------------------------------------------------*/
tPatchIndex DoPatch(PatchInfo *thePatch) {
	int Next;
	PatchInfo *NextPatch;
	int NextCommand = 1;

	NextPatch = thePatch;
	WriteToHistory(NextPatch->Name);
	printd(LogInfo, "DoPatch: Start \n");

	// Execute a patch and any chain patches.
	do {
		printd(LogInfo, "DoPatch: %s \n", NextPatch->Name);
		SendMidiPatch(NextPatch);

		UpdateStatus(NextPatch->Name);

		if (NextPatch->Chain[0] != 0) {
			NextCommand = FindString(fsPatchNames, NextPatch->Chain);
			NextPatch = &gMyInfo.MyPatchInfo[NextCommand];

			if (NextCommand != -1)
				usleep(150000);
		}
		else
			NextCommand = -1;
		

	} while (NextCommand != -1);

	return (0);
}

/*--------------------------------------------------------------------
 * Function:		RaiseWindowsNum
 *
 * Description:		Bring the App/Window into the forground.
 *
 *---------------------------------------------------------------------*/
void RaiseWindowsNum(int AppNumber) {
	printd(LogInfo, "Raise Window %s\n", gMyInfo.Apps[AppNumber].Name);
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

	NewValue = FindString(fsPatchNames,
	                      gMyInfo.LayoutPresets[CurrentLayout].Presets[Value]);

	printd(LogInfo, "GetModePreset %d %d %d\n",
	       Value,
	       NewValue,
	       CurrentLayout
	      );
#if 0
	switch (CurrentLayout) {
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
#endif
//	printd(LogInfo, "Get Mode Preset Old %d New %d\n", Value, NewValue);
	return (NewValue);
}

/*--------------------------------------------------------------------
 * Function:		IncrementMode
 *
 * Description:		Based on the current mode do a different.
 *
 *---------------------------------------------------------------------*/
void IncrementMode(void) {

	// Check for wrap around.
	if (gMyInfo.LayoutPresets[++CurrentLayout].Name[0] == 0) {
		CurrentLayout = 0;
	}

//	printd(LogInfo, "IncrementMode %d %s", CurrentLayout,
//		gMyInfo.LayoutPresets[CurrentLayout].Name);
//	gtk_widget_override_font(CurrentLayoutWid,
//		pango_font_description_from_string("Sans Bold 16"));
//	gtk_label_set_text(CurrentLayoutWid, LayoutPresets[CurrentLayout].Name);
	SetUpMainButtons(&gMyInfo.MyPatchInfo);

	printd(LogDebug, "Increment Mode %d\n", CurrentLayout);
}

/*--------------------------------------------------------------------
 * Function:		LayoutSwitchPatch
 *
 * Description:		Based on the current mode do a different.
 *
 *---------------------------------------------------------------------*/
tPatchIndex LayoutSwitchPatch(tPatchIndex MidiIn, char DoAction) {
	tPatchIndex Preset;
	tPatchIndex RetVal;
	tPatchIndex FinalRetVal;
	int Loop;

	Preset = GetModePreset(MidiIn);
	printd(LogInfo, "In LayoutSwitchPatch Mid In %d %d %d\n", MidiIn, Preset,
	       &gMyInfo.MyPatchInfo[(char) Preset]);

	if (MidiIn >= Max_Patches) {
		printd(LogError, "MidiIn %d >= %d\n", MidiIn, Max_Patches);
		return (0);
	}

	/*
	 * Toggle the on screen buttons if the midi input is used.
	 */
	if (DoAction) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(MainButtons[MidiIn].Image),
		                          MainButtons[MidiIn].ButtonDownImage);
		MainButtons[MidiIn].State = 1;
		MainButtonCountOn = 2;
	}

	RetVal = Preset;
	if (RetVal >= Max_Patches || RetVal < 0) {
		printd(LogError, "GetModePreset %d >= %d\n", MidiIn, Max_Patches);
		return (0);
	}

	FinalRetVal = RetVal;

	/* If the command is a preset lookup the patch.
	*/
	if (gMyInfo.MyPatchInfo[RetVal].CustomCommand == cmdPreset) {
		printd(LogInfo, "LayoutSwitchPatch1 Preset M%d R%d D%d\n", MidiIn,
		       RetVal, DoAction);

		/* Walk thru the preset buttons.
		*/
		for (Loop = 0; Loop < MaxPresetButtons; Loop++) {

			/* Find which button it is.
			*/
			if (gMyInfo.MyPatchInfo[RetVal].Patch == (Loop + 1))
				if (gMyInfo.WebPresets.thePreset[Loop] != -1) {
					FinalRetVal = gMyInfo.WebPresets.thePreset[Loop];
				}
		}
	}

	if (FinalRetVal > Max_Patches)
		FinalRetVal = RetVal;

	/* If we should act on it then call for a change.
	*/
	if (DoAction) {
		if (FinalRetVal >= 0 && FinalRetVal < Max_Patches)
			DoPatch(&gMyInfo.MyPatchInfo[(int) FinalRetVal]);
	}

// ejk event_ptr->data.control.value > 127 || event_ptr->data.control.value < 0 ? "???": gm_get_instrument_name(event_ptr->data.control.value));
	return (FinalRetVal);
}

/*--------------------------------------------------------------------
 * Function:		GuitarMidiPreset
 *
 * Description:		Wait for a preset from the Midi Guitar.
 *
 *---------------------------------------------------------------------*/
int GuitarMidiPreset(char Wait) {

	printd(LogInfo, "GuitarMidiPreset Start\n");

	/* Switch to the patch tab so we can see
	what we want to select.
	*/
	SwitchToTab(0);

	// MyOSCJackVol(NewValue);
	MyOSCJackMute(1, 0);
	WaitingforMidi = 1;

	/* If we are wait for a specific release queue.
	*/
	if (Wait) {
		WaitingforMidiHold = 1;
	}

	return (0);
}

/*--------------------------------------------------------------------
 * Function:		GuitarMidiPresetComplete
 *
 * Description:		Wait for a preset from the Midi Guitar.
 *
 *---------------------------------------------------------------------*/
int GuitarMidiPresetComplete(tPatchIndex MidiNote) {
	tPatchIndex PatchChange;

//	printd(LogInfo, "GuitarMidiPresetComplete Start %d\n", MidiNote);

	if (MidiNote < Max_Patches) {
		PatchChange = MidiNote - gMyInfo.MidiBaseNote;
		if (PatchChange >= 0 && PatchChange < Max_Patches)
			LayoutSwitchPatch(PatchChange, TRUE);
//		DoPatch(&gMyInfo.MyPatchInfo[PatchChange]);
	}

	printd(LogInfo, "GuitarMidiPresetComplete end Patch %d %d %d\n", gMyInfo.MidiBaseNote,
	       PreviousTab, PatchChange);

	/* Switch back to what ever we were looking at.
	*/
	SwitchToTab(PreviousTab);

	if (WaitingforMidiHold == 0) {
		WaitingforMidi = 0;
		/* In case the note rings.
		*/
		RemoveMuteCount = MuteCountDelay;
	} else if (MidiNote >= Max_Patches) {
		WaitingforMidi = 0;
		MyOSCJackMute(0, 0);
		WaitingforMidiHold = 0;

	}

	return (0);
}

/*--------------------------------------------------------------------
 * Function:		key_press_cb
 *
 * Description:	Key input.
 *---------------------------------------------------------------------*/
gint key_press_cb(GtkWidget *widget, GdkEventKey *kevent, gpointer data)  {
	GtkWidget *btn = (GtkWidget *)data;
#ifdef AcceptTimedKeyboard

	if (kevent->type == GDK_KEY_PRESS)  {
		gMyInfo.KeyChar = kevent->keyval;

		// If it's a character.
		if (gMyInfo.KeyChar >= '0' && gMyInfo.KeyChar <= '9') {
			if ( (gMyInfo.TimerCount - gMyInfo.KeyTimer) < MaxKeyTimeout || gMyInfo.KeyPatchValue == 0 ) {
				gMyInfo.KeyPatchValue = (10 * gMyInfo.KeyPatchValue) + (gMyInfo.KeyChar - '0');
				gMyInfo.KeyIsValue = 1;
				printd(LogDebug, "Key Press %d %d\n", gMyInfo.KeyPatchValue, (gMyInfo.TimerCount - gMyInfo.KeyTimer));
			}
		} else
			gMyInfo.KeyPatchValue = gMyInfo.KeyChar;
	}

	gMyInfo.KeyTimer = gMyInfo.TimerCount;

	// void g_signal_emit_by_name(GObject *object, const gchar *name, ... );
	// g_signal_emit_by_name(G_OBJECT(btn), "clicked", NULL);
	// g_signal_emit_by_name(G_OBJECT(widget), "clicked", NULL);
#endif
	if (kevent->type == GDK_KEY_PRESS)  {
		gMyInfo.KeyChar = kevent->keyval;

		printd(LogDebug, "Key %d\n", gMyInfo.KeyChar);
		if (gMyInfo.KeyChar >= '0' && gMyInfo.KeyChar <= '9') {
			gMyInfo.KeyPatchValue = (10 * gMyInfo.KeyPatchValue) + (gMyInfo.KeyChar - '0');
		}

		if (gMyInfo.KeyChar == ' ') {
			plPausePlay();
		}

// NumLock 127
// 81 /
// 86 *
// 83 -
// 85 +
// 08 Back
// 82 .
//
		if (gMyInfo.KeyChar == 13) {
			LayoutSwitchPatch(gMyInfo.KeyPatchValue - 1, true);
			gMyInfo.KeyPatchValue = 0;
		}
	}

	return TRUE;
}

/*--------------------------------------------------------------------
 * Function:		FindString
 *
 * Description:	Find the offset into a list of strings.
 *---------------------------------------------------------------------*/
int FindString(int StringList, char *String) {
	int Loop;

//	if (String[0] == 0)
//		return(-1);

	if (StringList == fsPatchNames) {
		for (Loop = 0; Loop < Max_Patches; Loop++) {
//	printd(LogDebug,"FindString %d %s %s\n",
//			Loop, String,
//			gMyInfo.MyPatchInfo[Loop].Name);

			if (!strcmp(gMyInfo.MyPatchInfo[Loop].Name, String))
				return (Loop);
		}
		return (-1);
	}

	if (StringList == fsPortNames) {
		for (Loop = 0; Loop < MaxOutPorts; Loop++) {
			gMyInfo.OutPortName[Loop];
		}
	}

	if (StringList == fsAppNames) {
		for (Loop = 0; Loop < MaxApps; Loop++) {
			gMyInfo.Apps[Loop].Name;
		}

	}
	return (0);
}

/*--------------------------------------------------------------------
 * Function:		InitHistoryFile
 *
 * Description:		When the sliders are changed on the main screen.
 *
 *---------------------------------------------------------------------*/
int InitHistoryFile(void) {

//	FileHistory = fopen(GetResourceDir(HistoryFileName, FileLocTunes) , "a+");	
	FileHistory = fopen(GetResourceDir(HistoryFileName, FileLocConfig) , "a+");	
	printd(LogDebug,"File History %x\n", FileHistory);

	if (FileHistory) {
		fseek(FileHistory, 0, SEEK_END);
		printd(LogDebug, FileHistory, "*****************\n");
		fflush(FileHistory);
	}
}

int WriteToHistory(char *str) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	if (FileHistory) {
		fprintf(FileHistory, "%d-%d-%d %d:%d:%d->", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fprintf(FileHistory, "%s\n", str);
		fflush(FileHistory);
	}
}

int CloseHistoryFile(void) {
	fflush(FileHistory);
	fclose(FileHistory);
}

#if 0
/*--------------------------------------------------------------------
 * Function:		show_motion_notify_cb
 *
 * Description:	CallBack function for motion.
 *---------------------------------------------------------------------*/
gint button_press_notify_cb(GtkWidget *entries, GdkEventKey *event,
                            GtkWidget *widget) {

#if 0
	switch (event->keyval) {
	case GDK_p:
		printd(LogInfo, "key pressed: %s\n", "p");
		break;
	case GDK_s:
		if (event->state & GDK_SHIFT_MASK) {
			printd(LogInfo, "key pressed: %s\n", "shift + s");
		} else if (event->state & GDK_CONTROL_MASK) {
			printd(LogInfo, "key pressed: %s\n", "ctrl + s");
		} else {
			printd(LogInfo, "key pressed: %s\n", "s");
		}
		break;
	case GDK_m:
		if (event->state & GDK_SHIFT_MASK) {
			printd(LogInfo, "key pressed: %s\n", "shift + m");
		} else if (event->state & GDK_CONTROL_MASK) {
			printd(LogInfo, "key pressed: %s\n", "ctrl + m");
		} else {
			printd(LogInfo, "key pressed: %s\n", "m");
		}
		break;

	default:
		return FALSE;
	}
#endif
	return (FALSE);
}

/*--------------------------------------------------------------------
 * Function:		CreateHTMLGuide
 *
 * Description:		Writes and HTML file for the presets.
 *
 *---------------------------------------------------------------------*/
void CreateHTMLGuide(LiveMusicInfo *myInfo) {
	FILE *MyFile;
	char FileName[255];
	int Loop;
	int Loop1;

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
 * Function:            create_pixbuf
 *
 * Description:		Load an icon file for the application
 *
 *---------------------------------------------------------------------*/
GdkPixbuf *create_pixbuf(const gchar * filename) {
	GdkPixbuf *pixbuf;
	GError *error = NULL;
#if 0
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if (!pixbuf)
		fprintf(stderr, "%s\n", error->message);
	g_error_free(error);
}
return pixbuf;
#endif
return (NULL);
}


/*--------------------------------------------------------------------
 * Function:		CheckConnectionData
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void CheckConnectionData(LiveMusicInfo *myInfo) {

}
/*--------------------------------------------------------------------
 * Function:            apply_font_to_widget
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void apply_font_to_widget(GtkWidget *widget, gchar *fload) {
	PangoFontDescription *pfd;
	pfd = pango_font_description_from_string(fload);

	if (GTK_IS_LABEL(widget))
		gtk_widget_override_font(widget, pfd);
	else
		gtk_widget_override_font(GTK_WIDGET(gtk_bin_get_child (GTK_BIN(widget))),
		                         pfd);

	pango_font_description_free(pfd);
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
#endif
