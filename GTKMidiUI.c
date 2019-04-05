/*---------------------------------------------------------------------
 *      Revision Date:   2012/01/15 22:52:40
 *
 *      Contains:       Main code for te Live Music Application
 *
 *      Written by:     Elias Keshishoglou
 *
 *      Date:           Jan 15, 2012
 *
 *      Copyright © 2012 Elias Keshishoglou.  All rights reserved.
 *
 *      Change History (located at end of file)
 *
 *---------------------------------------------------------------------*/

#define GTKMidiUI_c

#include <gtk/gtk.h>
// #include "/usr/include/gtk-3.0/gtk/gtkcssprovider.h"
#include "GTKMidiUI.h"

#include "stdlib.h"
#include <string.h>
#include "stdbool.h"
#include "AlsaUtils.h"
#include "HTML.h"
#include "PrefsFile.h"
#include "Connections.h"
// #include <gdk/gdkkeysyms.h>
#include "getopt.h"
#include "Timers.h"
#include "Player.h"
#include "SooperOSC.h"

//#define UsingNewButtons	1

#ifdef UsingNewButtons
#define GLADE_FILE ResourceDirectory"GTKMidiUI.glade.Buttons"
#else
#define GLADE_FILE ResourceDirectory"LiveMusicApp.glade"
#endif
#define Icon_FILE ResourceDirectory"LiveIcon.png"
#define MaxTabs	5

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
theImageButtons TempoDraw;

// The Scale widgets.
GtkWidget *VScale1, *VScale2, *VScale3, *VScale4;
GtkAdjustment *Adjustment1, *Adjustment2, *Adjustment3, *Adjustment4;

// Images we use for the custom buttons.
GdkPixbuf *NoteBButtonOnImage;
GdkPixbuf *NoteBButtonOffImage;

// The Area we display the tempo
GtkWidget *TempoChild;

// Standard font description we use across the program
PangoFontDescription *Tempofont_desc;

// Hold the tempo string so we do not draw at inturrupt time.
char TempStrBuf[100];

GtkWidget *NoteBookPane;

// The button size my change based on the screen size.
int ButtonSize;

// Foot switch layout.
int KeyLayout = 1;

int RunLogLevel = LogDebug;

/*
 * Place Local prototypes here
 */
tPatchIndex LayoutSwitchPatch(tPatchIndex MidiIn, char DoAction);

void CreateTabButtons(void);
void CreateMainButtons(void);
void SetUpMainButtons(PatchInfo *MyPatchInfo);
//void PrintDataStructure(GTKMidiInfo *myInfo);
void CheckConnectionData(GTKMidiInfo *myInfo);
void CreateHTMLGuide(GTKMidiInfo *myInfo);
void apply_font_to_widget(GtkWidget *widget, gchar *fload);
tPatchIndex DoPatch(PatchInfo *thePatch);
void ToggleTempo(void);
void UpdateStatus(char *String);
void IncrementMode(void);
gint button_press_notify_cb(GtkWidget *entries, GdkEventKey *event,
                            GtkWidget *widget);
void VScale1_Changed(GtkAdjustment *adj);
void VScale2_Changed(GtkAdjustment *adj);
void VScale3_Changed(GtkAdjustment *adj);
void VScale4_Changed(GtkAdjustment *adj);
tPatchIndex GetModePreset(tPatchIndex Value);

#define MaxStatusHold 8
char HoldStatus[MaxStatusHold][50];
int HoldStatusIndex;
int MainButtonCountOn;

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

	if (RunLogLevel >= LogLevel)
		printf( "L%d-%s", LogLevel, p);

	return NULL;
}

GdkPixbuf *create_pixbuf(const gchar * filename) {
	GdkPixbuf *pixbuf;
	GError *error = NULL;
#if 0
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if (!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	return pixbuf;
#endif
	return (NULL);
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
 * Function:		SwitchToTab
 *
 * Description:		<Description/Comments>
 *
 *---------------------------------------------------------------------*/
void SwitchToTab(char Tab) {
	PreviousTab = gtk_notebook_get_current_page(GTK_NOTEBOOK(NoteBookPane));
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
	tPatchIndex PatchIndex;

	PatchIndex = LayoutSwitchPatch(user_data, true);
//    printd(LogInfo, "User data %d\n", (int) user_data);
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
	//	PatchIndex = LayoutSwitchPatch(user_data, true);
	IncrementMode();
	MyImageButtonSetText(theButton, gMyInfo.LayoutPresets[CurrentLayout].Name);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonDownImage);
	return TRUE; /* stop event propagation */
}

gboolean layout_release_handler(GtkWidget *widget,
                                GdkEvent *event,
                                gpointer user_data) {
	theImageButtons *theButton;
	theButton = (theImageButtons *) user_data;
	//	PatchIndex = LayoutSwitchPatch(user_data, true);

	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonUpImage);
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
//	printd(LogInfo, "In Destroy\n");
//	WritePrefs();
	gtk_main_quit();
}

/*--------------------------------------------------------------------
 * Function:		on_Tempo_Button
 *
 * Description:		Turn the Metrognome on or off
 *
 *---------------------------------------------------------------------*/
void on_Tempo_Button(GtkWidget *widget, gpointer user_data) {
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
	//  GtkTreeView* view = (GtkTreeView *)data;
//printd(LogInfo,"tab_focus_callback %x %x %x\n", notebook, arg1, data);
	//  GtkTreeView* view = (GtkTreeView *)data;

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
			gtk_image_set_from_pixbuf(GTK_IMAGE(MainButtons[Loop].Image),
			                          MainButtons[Loop].ButtonUpImage);
			MainButtons[Loop].State = 0;
			MainButtonCountOn = 0;
//			}
		}
	} else if (MainButtonCountOn)
		MainButtonCountOn--;
}

/*--------------------------------------------------------------------
 * Function:            GTKIdel_cb
 *
 * Description: Startup some Gui.
 *---------------------------------------------------------------------*/
int GTKIdel_cb(gpointer data) {

	HIDPoll();

	// Can't call this from the thread so the
	// thread sets teh structure and then the action
	// gets performed here.
	// Expression control of active GUI sliders.
	if (AlsaEvent.data.control.param == MIDI_CTL_MSB_MAIN_VOLUME) {
		switch (gMyInfo.ExpreP1Slider) {
		case Slider1:
			SetVolume1(AlsaEvent.data.control.value);
			break;

		case Slider2:
			SetVolume2(AlsaEvent.data.control.value);
			break;

		case Slider3:
			SetVolume3(AlsaEvent.data.control.value);
			break;

		case Slider4:
			SetVolume4(AlsaEvent.data.control.value);
			break;

		default:
			break;
		}
		AlsaEvent.data.control.param = 0;
	}


#if 0
	if (gUpdateTempo) {
		MyImageButtonSetText(&TempoDraw, TempStrBuf);
//		gtk_label_set_text((TempoChild), TempStrBuf);
		gUpdateTempo = 0;
	}

	if (SysCallString[0]) {
		system(SysCallString);

		SysCallString[0] = 0;
		printd(LogInfo, "IN Idle\n");
	}
#endif
	return (true);
}

/*--------------------------------------------------------------------
 * Function:            parse_cmdline
 *
 * Description: 	Deal with command line arguments.
 *---------------------------------------------------------------------*/
void parse_cmdline(int argc, char *argv[]) {
	int c;
	int digit_optind = 0;

	while (1)  {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{ "PrintLevel", required_argument, &verbose_flag, 1 },
			{ "verbose", no_argument, &verbose_flag, 1 },
			{ "FontSize", required_argument, 0, 'f' },
			{ "jack", required_argument, 0, 'j' },

			/* Not Used	*/
			{ "Layout", required_argument, 0, 'l' },
			{ "size", required_argument, 0, 's' },
			{ "append", no_argument, 0, 0 },
			{ "delete", required_argument, 0, 0 },
			{ "create", required_argument, 0, 'c' },
			{ "file", required_argument, 0, 0 },
			{ 0, 0, 0, 0 }
		};

		c = getopt_long(argc, argv, "abcj:dsfP:l:012",
		                long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			printd(LogInfo, "option %s", long_options[option_index].name);
			if (optarg)
				printd(LogInfo, " with arg %s", optarg);
			printd(LogInfo, "\n");
			break;

		case '0':
		case '1':
		case '2':
			if (digit_optind != 0 && digit_optind != this_option_optind)
				printd(LogInfo, "digits occur in two different argv-elements.\n");
			digit_optind = this_option_optind;
			printd(LogInfo, "option %c\n", c);
			break;

		case 'a':
			printd(LogInfo, "option a\n");
			break;

		case 'b':
			printd(LogInfo, "option b\n");
			break;

		case 'c':
			printd(LogInfo, "option c with value `%s'\n", optarg);
			break;

		case 'd':
			printd(LogInfo, "option d with value `%s'\n", optarg);
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

		case 's':
			printd(LogInfo, "size %s\n", optarg);
			break;

		case '?':
			break;

		case 'P':
			RunLogLevel = atoi(optarg);
			printd(LogInfo, "Layout %d\n", KeyLayout);
			break;

		default:
			printd(LogInfo, "?? getopt returned character code 0%o ??\n", c);
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
 * Function:            main
 *
 * Description:         This is where it all starts.
 *
 *---------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
	GtkWidget *main_window;
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
	verbose_flag = 0;
	KeyLayout = 1;

	parse_cmdline(argc, argv);
	/* Handle any HID pedals,
	Must be called before gtk_init
	*/
	InitHIDGrab();

	/* initialize the GTK+ library */
	gtk_init(&argc, &argv);
//	gtk_rc_parse( MAINPREFS_FILE);
	myScreen = gdk_screen_get_default();
	printd(LogInfo, "Screen Size %d %d\n", gdk_screen_get_width(myScreen), gdk_screen_get_height(myScreen));
	ScreenSize = 0;
	ButtonSize = 95;
	strcpy(JackName, "default");

	if (gdk_screen_get_width(myScreen) > 1400) {
		ScreenSize = 1;
		ButtonSize = 115;
	}

	if (gdk_screen_get_width(myScreen) > 1800) {
		ScreenSize = 2;
		ButtonSize = 150;
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

	sprintf(TempStrBuf, "%s.%d", GLADE_FILE, KeyLayout);

	if (!gtk_builder_add_from_file(gxml, TempStrBuf, &error)) {
		g_warning("Couldn't load builder file: %s", error->message);
		g_error_free(error);
	}

	/*
	 * get the window widget from the glade XML file
	 */
	main_window = GTK_WIDGET(gtk_builder_get_object(gxml, "window1"));

#if 1
	/*------------- CSS  --------------------------------------------------------------------------------------------------*/
	provider = gtk_css_provider_new();
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);

	gtk_style_context_add_provider_for_screen(screen,
	        GTK_STYLE_PROVIDER(provider),
	        GTK_STYLE_PROVIDER_PRIORITY_USER);
#if 0
	gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider),
	                                " GtkWindow {\n"
	                                "   background-image: url('./LiveMusicRes/WindowBackground.png');\n"
	                                "}\n", -1, NULL);
#else
// #define CSSFileName "./LiveMusicRes/LiveMusicApp.css"
#define CSSFileName "./LiveMusicRes/LiveMusicApp.css"

	printd(LogInfo, "ejk About to load\n");
	gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), CSSFileName, &err);
//	   printd(LogInfo, "ejk After load error %s\n", err->message);
#endif
	g_object_unref(provider);

	/*----------------------------------------------------------------------------------------------------------------------*/
#endif

	g_signal_connect(G_OBJECT (main_window), "destroy",
	                 G_CALLBACK (on_window1_destroy), NULL);
	gtk_window_set_title(GTK_WINDOW(main_window), "LiveMusicApp");
	gtk_window_set_icon(GTK_WINDOW(main_window), create_pixbuf(Icon_FILE));

	BButtonX = ButtonSize;
	BButtonY = (int) ((float) ButtonSize * 0.60);
	MButtonX = (int) ((float) ButtonSize * 0.95);
	MButtonY = (int) ((float) ButtonSize * 0.42);

	MainButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
	                        "./LiveMusicRes/MainSwitchOn.png", MButtonX, MButtonY, NULL, NULL);
	MainButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
	                         "./LiveMusicRes/MainSwitchOff.png", MButtonX, MButtonY, NULL, NULL);
	PatchButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
	                         "./LiveMusicRes/PatchSwitchOn.png", BButtonX, BButtonY, NULL, NULL);
	PatchButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
	                          "./LiveMusicRes/PatchSwitchOff.png", BButtonX, BButtonY, NULL, NULL);
	NoteBButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
	                         "./LiveMusicRes/NoteBSwitchOn.png", MButtonX, MButtonY, NULL, NULL);
	NoteBButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
	                          "./LiveMusicRes/NoteBSwitchOff.png", MButtonX, MButtonY, NULL, NULL);
//	GdkPixbuf *gdk_pixbuf_scale_simple (const GdkPixbuf *src, 135,65,  GDK_INTERP_NEAREST);
	NoteBookPane = GTK_WIDGET(gtk_builder_get_object(gxml, "MainTab"));

	/*
	 * Open the persistent main tab.
	 */
	g_signal_connect(GTK_NOTEBOOK( NoteBookPane ), "switch-page",
	                 (GCallback ) tab_focus_callback, gxml);

	/*
	 * Setup and initialize our statusbar and Mode indicator
	 */
	MainStatus = GTK_WIDGET(gtk_builder_get_object(gxml, "MainStatusBar"));
//	CurrentLayoutWid = GTK_WIDGET(
//		gtk_builder_get_object(gxml, "CurrentLayout"));

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

	MyImageButtonInit(&TempoDraw, EventBox, MainButtonOnImage, MainButtonOffImage);

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
	g_signal_connect_data(G_OBJECT(widget), "clicked",
	                      G_CALLBACK(on_About_clicked), NULL, NULL, 0);

	/*
	 * The OK Button on the About Box
	 */
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

	/*
	 * Set up the Midi Sequencer ports
	 */
	MyAlsaInit();
	printd(LogInfo, "After MyAlsaInit\n");
	SetAlsaMasterVolume(90);
	SetAlsaCaptureVolume(90);
	MyOSCInit();

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
	CreateTabButtons();

	/* Get the Mode switch button,
	 */
	EventBox = GTK_WIDGET(
	               gtk_builder_get_object(gxml, "LayoutEvent"));
	printd(LogInfo, "LayoutEvent %x\n", (unsigned int) EventBox);
	MyImageButtonInit(&LayoutButton, EventBox, MainButtonOnImage,
	                  MainButtonOffImage);
	MyImageButtonSetText(&LayoutButton, gMyInfo.LayoutPresets[0].Name);

	g_signal_connect(G_OBJECT(EventBox),
	                 "button-press-event",
	                 G_CALLBACK(layout_click_handler),
	                 &LayoutButton);
	g_signal_connect(G_OBJECT(EventBox),
	                 "button-release-event",
	                 G_CALLBACK(layout_release_handler),
	                 &LayoutButton);

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
	 * Set the initial Volumes.
	 */
	printd(LogInfo, "Setting Default Volumes\n");
	SetVolume1(gMyInfo.AnalogVolume);
	SetVolume2(gMyInfo.MidiVolume);
//       create_Patch_Popup_view(main_window);
	printd(LogInfo, "Entering gtk_main\n");


	MyTimerInit();

	g_idle_add(GTKIdel_cb, main_window);

	/*
	 * Set up the Live Player
	 */
	PlayerWidget = GTK_WIDGET(gtk_builder_get_object(gxml, "PlayerFrame"));
	LivePlayerInit(main_window, PlayerWidget);

	WaitingforMidi = WaitingforMidiHold = 0;

	/*
	 * Show the main window and let the show begin.
	 */
	gtk_widget_show_all(main_window);

	/*
	 * And they're off.
	 */
	gtk_main();

	/*
	 * After we quit we should write back the changes.
	 */
	CloseHIDGrab();
	WritePrefs();
	MyAlsaClose();
	MyOSCClose();
	LivePlayerClose();
	printd(LogInfo, "Closing LiveApp\n");


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
#define UsingImageButtons
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
//	PatchIndex = LayoutSwitchPatch(user_data, true);
	LayoutSwitchPatch(Loop, true);

//	gtk_image_set_from_pixbuf(GTK_IMAGE(MainButtons[Loop].Image),
//		MainButtons[Loop].ButtonDownImage);
	return TRUE; /* stop event propagation */
}

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
	for (Loop = 0; Loop < MaxTabs; Loop++) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(TabButtons[Loop].Image),
		                          TabButtons[Loop].ButtonUpImage);
	}
	Loop = (int) user_data;
	gtk_image_set_from_pixbuf(GTK_IMAGE(TabButtons[Loop].Image),
	                          TabButtons[Loop].ButtonDownImage);
	gtk_notebook_set_current_page(GTK_NOTEBOOK(NoteBookPane), Loop);

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
#if 0
#ifndef UsingNewButtons

	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		sprintf(Buffer, "button%d", Loop + 1);
		MainButtons[Loop] = GTK_WIDGET(gtk_builder_get_object(gxml, Buffer));
#if 0
		MainButtonImage = gtk_image_new_from_file(ResourceDirectory"MainButtonImage.png");
		g_object_set (MainButtons[Loop],
		              "image", MainButtonImage,
		              NULL);
#endif
#if 0
		"label", &text,
		"use-stock", &use_stock,
		"use-underline", &use_underline,
#endif
//		gtk_container_add(MainButtons[Loop], MainButtonImage);
//		gtk_button_set_image(MainButtons[Loop],MainButtonImage);
//		gtk_settings_set_property_value(MainButtons[Loop], "always-show-image", TRUE);
//	gtk_button_set_always_show_image(MainButtons[Loop], TRUE);
		g_signal_connect_data(G_OBJECT(MainButtons[Loop]), "clicked",
		                      G_CALLBACK(on_button_clicked), Loop, NULL, 0);
	}
#else
	ButtonFrame = GTK_WIDGET (gtk_builder_get_object (gxml, "ButtonFrame") );
	printd(LogInfo, "Button Frame %x\n", ButtonFrame);

	Table = gtk_table_new(6, 5, false);
	printd(LogInfo, "Table %x\n", Table);

	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		MainButtons[Loop] = gtk_button_new_with_label (gMyInfo.MyPatchInfo[GetModePreset(Loop)].Name);
		gtk_button_set_image(MainButtons[Loop], MainButtonImage);
		gtk_button_set_always_show_image(MainButtons[Loop], TRUE);
		g_signal_connect (MainButtons[Loop], "clicked",
		                  G_CALLBACK (on_button_clicked), (void *)Loop);
//		gtk_widget_show(MainButtons[Loop]);
		gtk_widget_set_usize(MainButtons[Loop], 120, 120);
//		gtk_table_attach_defaults(GTK_TABLE(Table), MainButtons[Loop],
//			Col, Col + 1, Row, Row + 1);
		gtk_table_attach(GTK_TABLE(Table), MainButtons[Loop],
		                 Col, Col + 1, Row, Row + 1, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);
		if (Col++ > 4) {
			Col = 0;
			Row++;
		}
		printd(LogInfo, "Loop %d %d %d %x\n", Loop, Col, Row, MainButtons[Loop]);
	}
	g_signal_connect_data(G_OBJECT(MainButtons[Loop]), "clicked",
	                      G_CALLBACK(on_button_clicked), Loop, NULL, 0);

	gtk_container_add (GTK_CONTAINER (ButtonFrame), Table);
#endif
#endif
	GtkWidget *Label;

	VScale1 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale1"));
	Adjustment1 = (GtkAdjustment *) (gtk_builder_get_object(gxml, "adjustment1"));
	g_signal_connect(G_OBJECT (VScale1), "value_changed",
	                 G_CALLBACK (VScale1_Changed), NULL);
	Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label1"));
	gtk_label_set_text((Label), (gchar *)gMyInfo.MyPatchInfo[Slider1].Name);


	VScale2 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale2"));
	Adjustment2 = (GtkAdjustment *) (gtk_builder_get_object(gxml, "adjustment2"));
	g_signal_connect(G_OBJECT (VScale2), "value_changed",
	                 G_CALLBACK (VScale2_Changed), NULL);
	Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label2"));
	gtk_label_set_text((Label), (gchar *)gMyInfo.MyPatchInfo[Slider2].Name);

	VScale3 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale3"));
	Adjustment3 = (GtkAdjustment *) (gtk_builder_get_object(gxml, "adjustment3"));
	g_signal_connect(G_OBJECT (VScale3), "value_changed",
	                 G_CALLBACK (VScale3_Changed), NULL);
	Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label3"));
	gtk_label_set_text((Label), (gchar *)gMyInfo.MyPatchInfo[Slider3].Name);

	VScale4 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale4"));
	Adjustment4 = (GtkAdjustment *) (gtk_builder_get_object(gxml, "adjustment4"));
	g_signal_connect(G_OBJECT (VScale4), "value_changed",
	                 G_CALLBACK (VScale4_Changed), NULL);
	Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label4"));
	gtk_label_set_text((Label), (gchar *)gMyInfo.MyPatchInfo[Slider4].Name);

}

/*--------------------------------------------------------------------
 * Function:		VScale1_Changed
 *
 * Description:		Volume Sliders changed.
 *
 *---------------------------------------------------------------------*/
void VScale1_Changed(GtkAdjustment *adj) {
	int ThisPatchNum;
	PatchInfo *ThisPatch;
	unsigned int NewValue;

	/* Set the number of decimal places to which adj->value is rounded */
	//   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
	NewValue = gtk_adjustment_get_value(Adjustment1);
	printd(LogInfo, "\nVscale 1 %d\n", NewValue);
	ThisPatchNum = Slider1;
	ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];

//	gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment1);

	if (ThisPatch->OutPort == InternalPort)
		MyOSCJackVol(NewValue, 0);
	else
		SendMidi(SND_SEQ_EVENT_CONTROLLER,
		         ThisPatch->OutPort,
		         ThisPatch->Channel,
		         ThisPatch->Patch,
		         (char) NewValue);
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

	/* Set the number of decimal places to which adj->value is rounded */
	//   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
	NewValue = gtk_adjustment_get_value(Adjustment2);
	printd(LogInfo, "\nVscale 2 %d\n", NewValue);
	ThisPatchNum = Slider2;
	ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];
	gMyInfo.MidiVolume = (char) gtk_adjustment_get_value(Adjustment2) * 1.25;

#if 0
	if (ThisPatch->OutPort == InternalPort)
		MyOSCJackVol(NewValue, 0);
	else
		SendMidi(SND_SEQ_EVENT_CONTROLLER,
		         ThisPatch->OutPort,
		         ThisPatch->Channel,
		         ThisPatch->Patch,
		         (char) NewValue);
#endif
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

	/* Set the number of decimal places to which adj->value is rounded */
	//   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
	NewValue = gtk_adjustment_get_value(Adjustment3);
	printd(LogInfo, "\nVscale 3 %d\n", NewValue);
	ThisPatchNum = Slider3;
	ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];

//	gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment3);

	if (ThisPatch->OutPort == InternalPort)
		MyOSCJackVol(NewValue, 0);
	else
		SendMidi(SND_SEQ_EVENT_CONTROLLER,
		         ThisPatch->OutPort,
		         ThisPatch->Channel,
		         ThisPatch->Patch,
		         (char) NewValue);
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
	printd(LogInfo, "\nVscale 4 %d\n", NewValue);
	ThisPatchNum = Slider4;
	ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];

//	gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment4);

	if (ThisPatch->OutPort == InternalPort)
		MyOSCJackVol(NewValue, 0);
	else
		SendMidi(SND_SEQ_EVENT_CONTROLLER,
		         ThisPatch->OutPort,
		         ThisPatch->Channel,
		         ThisPatch->Patch,
		         (char) NewValue);
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
	gtk_adjustment_set_value(Adjustment2, Value);
	gMyInfo.MidiVolume = (Value * 1.25);
	gtk_range_set_adjustment(VScale2, Adjustment2);
	return (Value);
}

/*--------------------------------------------------------------------
 * Function:		SetVolume3
 *
 * Description:
 *---------------------------------------------------------------------*/
int SetVolume3(int Value) {
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
	gtk_adjustment_set_value(Adjustment4, Value);
//	gMyInfo.MidiVolume = Value;
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
//		printd(LogInfo, "Loop %d gMyInfo [%s] Patch %d\n", Loop,
//			gMyInfo.MyPatchInfo[GetModePreset(Loop)].Name, GetModePreset(Loop));
		PatchIndex = LayoutSwitchPatch(Loop, FALSE);
//		printd(LogInfo, "SetUpMainButtons: %d %d\n", Loop, PatchIndex);

		if (PatchIndex >= 0 && PatchIndex < Max_Patches) {
			StringLen = strlen(gMyInfo.MyPatchInfo[PatchIndex].Name);
//	        printd(LogInfo, "---%*s%*s---\n",10+strlen(s)/2,s,10-strlen(s)/2,"");
			sprintf(String, "       %02d       \n%*s", Loop + 1, 7 + StringLen / 2,
			        gMyInfo.MyPatchInfo[PatchIndex].Name);
			MyImageButtonSetText(&MainButtons[Loop], String);
//			gtk_label_set_text((MainButtons[Loop].Label), String);
//       	  gdk_color_parse ("green", &color);
//        	  gtk_widget_modify_fg (myChild, GTK_STATE_NORMAL, &color);
		}
	}

#else
	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		myButton = MainButtons[Loop];
//		printd(LogInfo, "Loop %d gMyInfo [%s] Patch %d\n", Loop,
//			gMyInfo.MyPatchInfo[GetModePreset(Loop)].Name, GetModePreset(Loop));
		PatchIndex = LayoutSwitchPatch(Loop, FALSE);
//		printd(LogInfo, "SetUpMainButtons: %d %d\n", Loop, PatchIndex);

		if (PatchIndex >= 0 && PatchIndex < Max_Patches) {
			sprintf(String, "%02d-%s", Loop + 1,
			        gMyInfo.MyPatchInfo[PatchIndex].Name);
			myChild = gtk_bin_get_child(GTK_BIN(myButton));
			gtk_label_set_text((myChild), String);
//       	  gdk_color_parse ("green", &color);
//        	  gtk_widget_modify_fg (myChild, GTK_STATE_NORMAL, &color);
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

	do {
		SendMidiPatch(NextPatch);
		UpdateStatus(NextPatch->Name);

		NextCommand = FindString(fsPatchNames, NextPatch->Chain);
		NextPatch = &gMyInfo.MyPatchInfo[NextCommand];

		if (NextCommand != -1)
			usleep(150000);

	} while (NextCommand != -1);
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
G_OBJECT_set(G_OBJECT(button), "label", "goodbye", NULL);

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
 * Function:		RaiseWindowsNum
 *
 * Description:		Bring the App/Window into the forground.
 *
 *---------------------------------------------------------------------*/
void RaiseWindowsNum(int AppNumber) {
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

	NewValue = FindString(fsPatchNames,
	                      gMyInfo.LayoutPresets[CurrentLayout].Presets[Value]);
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

	if (gMyInfo.LayoutPresets[++CurrentLayout].Name[0] == 0) {
		CurrentLayout = 0;
	}

//	printd(LogInfo, "IncrementMode %d %s", CurrentLayout,
//		gMyInfo.LayoutPresets[CurrentLayout].Name);
//	gtk_widget_override_font(CurrentLayoutWid,
//		pango_font_description_from_string("Sans Bold 16"));
//	gtk_label_set_text(CurrentLayoutWid, LayoutPresets[CurrentLayout].Name);
	SetUpMainButtons(&gMyInfo.MyPatchInfo);

	printd(LogInfo, "Increment Mode %d\n", CurrentLayout);
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
	int Loop;

	LastPatch = MidiIn;
	LastAbsPatch = GetModePreset(MidiIn);
//	printd(LogInfo, "In LayoutSwitchPatch Mid In %d %d %d\n", MidiIn, LastAbsPatch,
//	       &gMyInfo.MyPatchInfo[(char) LastAbsPatch]);
	if (MidiIn >= Max_Patches) {
		printd(LogError, "MidiIn %d >= %d\n", MidiIn, Max_Patches);
		return (0);
	}

//	printd(LogDebug,"Old LastPatch %d\n", LastPatch);
	LastPatch = MidiIn;

	/*
	 * Toggle the on screen buttons if the midi input is used.
	 */
	if (DoAction) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(MainButtons[MidiIn].Image),
		                          MainButtons[MidiIn].ButtonDownImage);
		MainButtons[MidiIn].State = 1;
		MainButtonCountOn = 2;
	}

	RetVal = GetModePreset(MidiIn);
	if (RetVal >= Max_Patches) {
//		printd(LogError, "GetModePreset %d >= %d\n", MidiIn, Max_Patches);
		return (0);
	}
	if (gMyInfo.MyPatchInfo[RetVal].CustomCommand == cmdPreset) {
//              printd(LogInfo, "LayoutSwitchPatch Preset M%d R%d D%d\n", MidiIn,
//                      RetVal, DoAction);

		for (Loop = 0; Loop < MaxPresetButtons; Loop++) {
			if (gMyInfo.MyPatchInfo[RetVal].Patch == (Loop + 1))
				if (gMyInfo.WebPresets.thePreset[Loop] != -1)
					RetVal = gMyInfo.WebPresets.thePreset[Loop];
		}
	}

//		DoPatch(&gMyInfo.MyPatchInfo[preModePractice[GetModePreset(MidiIn)]]);
	if (DoAction) {
		if (RetVal >= 0 && RetVal < Max_Patches)
			DoPatch(&gMyInfo.MyPatchInfo[(int) RetVal]);
	}
//	printd(LogInfo, "LayoutSwitchPatch M%d R%d D%d\n", MidiIn, RetVal,
//		DoAction);
// ejk event_ptr->data.control.value > 127 || event_ptr->data.control.value < 0 ? "???": gm_get_instrument_name(event_ptr->data.control.value));
	return (RetVal);
}

/*--------------------------------------------------------------------
 * Function:		GuitarMidiPreset
 *
 * Description:		Wait for a preset from the Midi Guitar.
 *
 *---------------------------------------------------------------------*/
int GuitarMidiPreset(void) {

	printd(LogInfo, "GuitarMidiPreset Start\n");

	// MyOSCJackVol(NewValue);
	MyOSCJackMute(1, 0);
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
	tPatchIndex PatchChange;

	printd(LogInfo, "GuitarMidiPresetComplete Start %d\n", MidiNote);

	MyOSCJackMute(0, 0);

	PatchChange = MidiNote - gMyInfo.MidiBaseNote;
	if (PatchChange >= 0 && PatchChange < Max_Patches)
		LayoutSwitchPatch(PatchChange, TRUE);
//		DoPatch(&gMyInfo.MyPatchInfo[PatchChange]);

	printd(LogInfo, "GuitarMidiPresetComplete end Patch %d %d\n", gMyInfo.MidiBaseNote,
	       PatchChange);

	if (WaitingforMidiHold == 0)
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
 * Function:		FindString
 *
 * Description:	Find the offset into a list of strings.
 *---------------------------------------------------------------------*/
int FindString(int StringList, char *String) {
	int Loop;

	if (StringList == fsPatchNames) {
		for (Loop = 0; Loop < Max_Patches; Loop++) {
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

