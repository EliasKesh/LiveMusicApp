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
#include "/usr/include/gtk-3.0/gtk/gtkcssprovider.h"
// #include <glade/glade.h>
#include "GTKMidiUI.h"

#include "stdlib.h"
#include <string.h>
#include "stdbool.h"
#include "AlsaUtils.h"
#include "HTML.h"
#include "PrefsFile.h"
#include "Connections.h"
#include <gdk/gdkkeysyms.h>
#include "getopt.h"
#include "Timers.h"

//#define UsingNewButtons	1

#ifdef UsingNewButtons
#define GLADE_FILE ResourceDirectory"GTKMidiUI.glade.Buttons"
#else
#define GLADE_FILE ResourceDirectory"GTKMidiUI.glade"
#endif
#define Icon_FILE ResourceDirectory"LiveIcon.png"
#define MaxTabs	4
/*
 * Place Global variables here
 */
snd_seq_t *seq;
int seqPort;
//	GladeXML *gxml;
GtkWidget* MainStatus;
//GtkWidget* CurrentLayoutWid;
guint MainStatusid;
theImageButtons LayoutButton;
// GtkWidget *MainButtons[Max_Main_Buttons];
theImageButtons MainButtons[Max_Main_Buttons];
theImageButtons TabButtons[MaxTabs];
theImageButtons TempoDraw;

tPatchIndex GetModePreset(tPatchIndex Value);
GtkWidget *VScale1, *VScale2, *VScale3;

GdkPixbuf *NoteBButtonOnImage;
GdkPixbuf *NoteBButtonOffImage;


GtkAdjustment *Adjustment1, *Adjustment2, *Adjustment3;
tPatchIndex LayoutSwitchPatch(tPatchIndex MidiIn, char DoAction);
GtkWidget *TempoChild;
PangoFontDescription *Tempofont_desc;
char TempStrBuf[10];
GtkWidget *NoteBookPane;
int	ButtonSize;



/*
 * Place Local prototypes here
 */

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

#define MaxStatusHold 4
char HoldStatus[MaxStatusHold][50];
int	 HoldStatusIndex;
int	MainButtonCountOn;

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
	return NULL;
}

GdkPixbuf *create_pixbuf(const gchar * filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;
#if 0
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if(!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	return pixbuf;
#endif
	return(NULL);
}

/*--------------------------------------------------------------------
 * Function:            apply_font_to_widget
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
		gtk_widget_override_font(widget, pfd);
	else
		gtk_widget_override_font(GTK_WIDGET(gtk_bin_get_child (GTK_BIN(widget))),
			pfd);

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
	gpointer user_data)
{
	theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	printf("layout_click %x\n", theButton);
	//	PatchIndex = LayoutSwitchPatch(user_data, true);
	IncrementMode();
	MyImageButtonSetText(theButton, gMyInfo.LayoutPresets[CurrentLayout].Name);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
		theButton->ButtonDownImage);
	return TRUE; /* stop event propagation */
}

gboolean layout_release_handler(GtkWidget *widget,
	GdkEvent *event,
	gpointer user_data)
{
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
//	printf("In Destroy\n");
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
	}
		else {
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
 * Description: Make sure the buttons arr all up.
 *---------------------------------------------------------------------*/
void ClearMainButtons(void) {
int	Loop;

	if (MainButtonCountOn == 1) {
		for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
			if (MainButtons[Loop].State) {
			gtk_image_set_from_pixbuf(GTK_IMAGE(MainButtons[Loop].Image),
				MainButtons[Loop].ButtonUpImage);
			MainButtons[Loop].State = 0;
			MainButtonCountOn = 0;
			}
		}
	}
	else
		if (MainButtonCountOn)
			MainButtonCountOn--;

}

/*--------------------------------------------------------------------
 * Function:            GTKIdel_cb
 *
 * Description: Startup some Gui.
 *---------------------------------------------------------------------*/
int GTKIdel_cb(gpointer data) {

	if (gUpdateTempo) {
		MyImageButtonSetText(&TempoDraw, TempStrBuf);
//		gtk_label_set_text((TempoChild), TempStrBuf);
		gUpdateTempo = 0;
	}

#if 0
	if (SysCallString[0]) {
		system(SysCallString);

		SysCallString[0] = 0;
		printf("IN Idle\n");
	}
#endif
	return (true);
}

void parse_cmdline(int argc, char *argv[])
{
    int c;
    int digit_optind = 0;

    while (1)
      {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] =
        {
          {"size", required_argument, 0, 's'},
          {"append", no_argument, 0, 0},
          {"delete", required_argument, 0, 0},
          {"verbose", 0, 0, 0},
          {"create", required_argument, 0, 'c'},
          {"FontSize", required_argument, 0, 'f'},
          {"file", required_argument, 0, 0},
          {0, 0, 0, 0}
        };

        c = getopt_long (argc, argv, "abc:dsf:012",
                     long_options, &option_index);
        if (c == -1)
      break;

        switch (c)
          {
          case 0:
            printf ("option %s", long_options[option_index].name);
            if (optarg)
              printf (" with arg %s", optarg);
            printf ("\n");
            break;

          case '0':
          case '1':
          case '2':
            if (digit_optind != 0 && digit_optind != this_option_optind)
              printf ("digits occur in two different argv-elements.\n");
            digit_optind = this_option_optind;
            printf ("option %c\n", c);
            break;

          case 'a':
            printf ("option a\n");
            break;

          case 'b':
            printf ("option b\n");
            break;

          case 'c':
            printf ("option c with value `%s'\n", optarg);
            break;

          case 'd':
             printf ("option d with value `%s'\n", optarg);
             break;

          case 'f':
             ButtonSize = atoi(optarg);
             printf ("Font Size %d\n", ButtonSize);
            break;

          case 's':
             printf ("size %s\n", optarg);
             break;

          case '?':
            break;

          default:
            printf ("?? getopt returned character code 0%o ??\n", c);
            break;
          }
      }

    if (optind < argc)
      {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
        printf ("%s ", argv[optind++]);
        printf ("\n");
      }

 //   exit (0);    // whatever you want
}
/*--------------------------------------------------------------------
 * Function:            main
 *
 * Description:         This is where it all starts.
 *
 *---------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
	GtkWidget *main_window;
	GtkWidget *main_tab;
	GtkWidget *widget;
	GError *error = NULL;
	GtkWidget *ChordWidget;
	GtkWidget *EventBox;
	GError *err = NULL;
	/*----- CSS ----------- */
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;
	/*-----------------------*/
	int	BButtonX, BButtonY, MButtonX, MButtonY;
	int	Loop;
	GdkScreen *myScreen;
	/*
	 * Let's setup some variables.
	 * CurrentLayout is the default to start with
	 * When we start are
	 */
	CurrentLayout = 0;
	WaitingforMidi = 0;
	ButtonSize = 115;


	parse_cmdline(argc, argv);
	/* initialize the GTK+ library */
	gtk_init(&argc, &argv);
//	gtk_rc_parse( MAINPREFS_FILE);
	myScreen = gdk_screen_get_default ();
	printf("Screen Size %d %d\n", gdk_screen_get_width(myScreen), gdk_screen_get_height(myScreen));

	/*
	 * Initialize the XML reader/writer and set some basic values here.
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
	/*------------- CSS  --------------------------------------------------------------------------------------------------*/
	provider = gtk_css_provider_new();
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);

	gtk_style_context_add_provider_for_screen(screen,
		GTK_STYLE_PROVIDER(provider),
		GTK_STYLE_PROVIDER_PRIORITY_USER);

	gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider),
		" GtkWindow {\n"
			"   background-image: url('./LiveMusicRes/WindowBackground.png');\n"
			"}\n", -1, NULL);
	g_object_unref(provider);
	/*----------------------------------------------------------------------------------------------------------------------*/
	g_signal_connect(G_OBJECT (main_window), "destroy",
		G_CALLBACK (on_window1_destroy), NULL);
	gtk_window_set_title(GTK_WINDOW(main_window), "LiveMusicApp");
	gtk_window_set_icon(GTK_WINDOW(main_window), create_pixbuf(Icon_FILE));

	BButtonX = ButtonSize;
	BButtonY  = (int)((float)ButtonSize * 0.6);
	MButtonX = (int)((float)ButtonSize * 1.0);
	MButtonY  = (int)((float)ButtonSize * 0.4);

	MainButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
		"./LiveMusicRes/MainSwitchOn.png", MButtonX, MButtonY, NULL, NULL);
	MainButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
		"./LiveMusicRes/MainSwitchOff.png", MButtonX, MButtonY, NULL, NULL);
	PatchButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
		"./LiveMusicRes/PatchSwitchOn.png", BButtonX, BButtonY, NULL, NULL);
	PatchButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
		"./LiveMusicRes/PatchSwitchOff.png", BButtonX, BButtonY, NULL, NULL);
	NoteBButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
		"./LiveMusicRes/NoteBSwitchOn.png", MButtonX, MButtonY,  NULL, NULL);
	NoteBButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
		"./LiveMusicRes/NoteBSwitchOff.png", MButtonX, MButtonY,  NULL, NULL);

	GdkPixbuf *PatchButtonOffImage;
	GdkPixbuf *NoteBButtonOnImage;

//	GdkPixbuf *gdk_pixbuf_scale_simple (const GdkPixbuf *src, 135,65,  GDK_INTERP_NEAREST);
	NoteBookPane = GTK_WIDGET(gtk_builder_get_object(gxml, "MainTab"));

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

	MyImageButtonInit(&TempoDraw, EventBox, MainButtonOnImage,	MainButtonOffImage);

	if (gMyInfo.MetronomeOn) {
		MyImageButtonSetText(&TempoDraw, "On");
		gtk_image_set_from_pixbuf(GTK_IMAGE(TempoDraw.Image),
			TempoDraw.ButtonDownImage);
	}
	else {
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
	SetAlsaMasterVolume(84);
	SetAlsaCaptureVolume(84);

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
	printf("LayoutEvent %x\n", (unsigned int)EventBox);
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
	 * Show the main window and let the show begin.
	 */
	gtk_widget_show_all(main_window);
//	gtk_widget_override_font(CurrentLayoutWid,
//		pango_font_description_from_string("Sans Bold 16"));
//	gtk_label_set_text(CurrentLayoutWid, LayoutPresets[0].Name);

	/*
	 * Set the initial Volumes.
	 */
	printd(LogInfo, "Setting Default Volumes\n");
	SetVolume1(gMyInfo.AnalogVolume);
	SetVolume2(gMyInfo.MidiVolume);
//       create_Patch_Popup_view(main_window);
	printd(LogInfo, "Enterint gtk_main\n");

	MyTimerInit();
//	g_idle_add(GTKIdel_cb, main_window);

	/*
	 * And they're off.
	 */
	gtk_main();

	/*
	 * After we quit we should write back the changes.
	 */
	WritePrefs();
	MyAlsaClose();
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
			sprintf(DisString, "<span font=\"12\" color='#%lx'><b>%12s\n%12s\n%12s\n%12s</b></span>",gMyInfo.StatusTextColor,
				(char*) &HoldStatus[1],(char*) &HoldStatus[2], (char*) &HoldStatus[3], String);
			break;

		case 1:
			sprintf(DisString, "<span font=\"12\" color='#%lx'><b>%12s\n%12s\n%12s\n%12s</b></span>",gMyInfo.StatusTextColor,
				 (char*)  &HoldStatus[2], (char*) &HoldStatus[3], (char*) &HoldStatus[0], String);
			break;

		case 2:
			sprintf(DisString, "<span font=\"12\" color='#%lx'><b>%12s\n%12s\n%12s\n%12s</b></span>",gMyInfo.StatusTextColor,
				(char*) &HoldStatus[3],	(char*) &HoldStatus[0], (char*) &HoldStatus[1], String);
			break;

		case 3:
			sprintf(DisString, "<span font=\"12\" color='#%lx'><b>%12s\n%12s\n%12s\n%12s</b></span>",gMyInfo.StatusTextColor,
				(char*) &HoldStatus[0],	(char*) &HoldStatus[1], (char*) &HoldStatus[2], String);
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
	gtk_widget_override_font(MainStatus,
		pango_font_description_from_string("Sans Bold 12"));
//	gtk_label_set_text(MainStatus, DisString);
	gtk_label_set_markup( (GtkLabel *) MainStatus, DisString);
}
#define UsingImageButtons
gboolean click_handler(GtkWidget *widget,
	GdkEvent *event,
	gpointer user_data)
{
	int Loop;
	GdkEvent	*theEvent;

	theEvent  = gtk_get_current_event();
printf("Event %x %x\n", theEvent->button.state, GDK_CONTROL_MASK);

	Loop = (int) user_data;
	CurrentPreset = Loop;
	if (theEvent->button.state & GDK_CONTROL_MASK) {
		printf("We have Control Down\n");
		ShowPatchListSelect(GTK_WINDOW(widget),Loop);

	}
//	PatchIndex = LayoutSwitchPatch(user_data, true);
	LayoutSwitchPatch(Loop, true);

//	gtk_image_set_from_pixbuf(GTK_IMAGE(MainButtons[Loop].Image),
//		MainButtons[Loop].ButtonDownImage);
	return TRUE; /* stop event propagation */
}

gboolean release_handler(GtkWidget *widget,
	GdkEvent *event,
	gpointer user_data)
{
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
	gpointer user_data)
{
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
int	Loop;
char Buffer[40];
GtkWidget *MainButtonImage;
GtkWidget *EventBox;


	for (Loop = 0; Loop < MaxTabs; Loop++) {
		sprintf(Buffer, "NTab%d", Loop + 1);
		EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, Buffer));
		MyImageButtonInit(&TabButtons[Loop], EventBox, NoteBButtonOnImage,NoteBButtonOffImage);

		g_signal_connect(G_OBJECT(EventBox),
			"button-press-event",
			G_CALLBACK(Notebook_click_handler),
			Loop);
	}

	MyImageButtonSetText(&TabButtons[0], "Patch");
	MyImageButtonSetText(&TabButtons[1], "Chart");
	MyImageButtonSetText(&TabButtons[2], "Chords");
	MyImageButtonSetText(&TabButtons[3], "Prefs");
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
	printd(LogInfo,"Button Frame %x\n", ButtonFrame);

	Table = gtk_table_new(6,5,false);
	printd(LogInfo,"Table %x\n", Table);

	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
		MainButtons[Loop] = gtk_button_new_with_label (gMyInfo.MyPatchInfo[GetModePreset(Loop)].Name);
		gtk_button_set_image(MainButtons[Loop],MainButtonImage);
		gtk_button_set_always_show_image(MainButtons[Loop], TRUE);
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
	g_signal_connect_data(G_OBJECT(MainButtons[Loop]), "clicked",
		G_CALLBACK(on_button_clicked), Loop, NULL, 0);

	gtk_container_add (GTK_CONTAINER (ButtonFrame), Table);
#endif
#endif

#if 1
	VScale1 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale1"));
	Adjustment1 = (GtkAdjustment *) GTK_WIDGET(gtk_builder_get_object(gxml, "adjustment1"));
	g_signal_connect(G_OBJECT (VScale1), "value_changed",
		G_CALLBACK (VScale1_Changed), NULL);

	VScale2 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale2"));
	Adjustment2 = (GtkAdjustment *)GTK_WIDGET(gtk_builder_get_object(gxml, "adjustment2"));
	g_signal_connect(G_OBJECT (VScale2), "value_changed",
		G_CALLBACK (VScale2_Changed), NULL);

	VScale3 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale3"));
	Adjustment3 = (GtkAdjustment *)GTK_WIDGET(gtk_builder_get_object(gxml, "adjustment3"));
#endif
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
	gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment1);
#if 1
	SendMidi(SND_SEQ_EVENT_CONTROLLER,
		gMyInfo.MyPatchInfo[Slider1].OutPort,
		gMyInfo.MyPatchInfo[Slider1].Channel,
		gMyInfo.MyPatchInfo[Slider1].Patch,
		(char) gMyInfo.AnalogVolume);
#endif
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
	gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment2);
#if 1
	SendMidi(SND_SEQ_EVENT_CONTROLLER,
		gMyInfo.MyPatchInfo[Slider2].OutPort,
		gMyInfo.MyPatchInfo[Slider2].Channel,
		gMyInfo.MyPatchInfo[Slider2].Patch,
		(char) gMyInfo.AnalogVolume);
#endif
}

/*--------------------------------------------------------------------
 * Function:		VScale3_Changed
 *
 * Description:		Volume Sliders changed.
 *
 *---------------------------------------------------------------------*/
void VScale3_Changed(GtkAdjustment *adj) {
	/* Set the number of decimal places to which adj->value is rounded */
	//   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
//    printf("\nVscale 1 %f\n", Adjustment2->value);
	gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment3);
	SendMidi(SND_SEQ_EVENT_CONTROLLER,
		gMyInfo.MyPatchInfo[Slider3].OutPort,
		gMyInfo.MyPatchInfo[Slider3].Channel,
		gMyInfo.MyPatchInfo[Slider3].Patch,
		(char) gMyInfo.AnalogVolume);
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
	int		StringLen;

#ifdef UsingImageButtons
	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
//		printd(LogInfo, "Loop %d gMyInfo [%s] Patch %d\n", Loop,
//			gMyInfo.MyPatchInfo[GetModePreset(Loop)].Name, GetModePreset(Loop));
		PatchIndex = LayoutSwitchPatch(Loop, FALSE);
//		printd(LogInfo, "SetUpMainButtons: %d %d\n", Loop, PatchIndex);

		if (PatchIndex >= 0 && PatchIndex < Max_Patches) {
			StringLen = strlen(gMyInfo.MyPatchInfo[PatchIndex].Name);
//	        printf("---%*s%*s---\n",10+strlen(s)/2,s,10-strlen(s)/2,"");
			sprintf(String, "       %02d       \n%*s", Loop + 1,7+StringLen/2,
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
	int	Next;
	PatchInfo *NextPatch;
	int	 NextCommand = 1;

	NextPatch = thePatch;

	do {
		SendMidiPatch(NextPatch);
		UpdateStatus(NextPatch->Name);

		NextCommand = FindString(fsPatchNames, NextPatch->Chain);
		NextPatch = &gMyInfo.MyPatchInfo[NextCommand];
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
	int	 Loop;
	int	 Loop1;

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

//	printf("IncrementMode %d %s", CurrentLayout,
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

//    printd(LogInfo, "In LayoutSwitchPatch Mid In %d %d %d\n", MidiIn, GetModePreset(MidiIn),
//           &gMyInfo.MyPatchInfo[(char) GetModePreset(MidiIn)]);
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
		MainButtonCountOn = 15;
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
	tPatchIndex PatchChange;

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

	PatchChange = MidiNote - gMyInfo.MidiBaseNote;
	if (PatchChange >= 0 && PatchChange < Max_Patches)
		DoPatch(&gMyInfo.MyPatchInfo[PatchChange]);

	printf("GuitarMidiPresetComplete end Patch %d %d\n", gMyInfo.MidiBaseNote,
		PatchChange);
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
#endif
	return (FALSE);
}

/*--------------------------------------------------------------------
 * Function:		SetVolume1
 *
 * Description:	Change the Volume Slider based on midi input.
 *---------------------------------------------------------------------*/
int SetVolume1(int Value) {
	gtk_adjustment_set_value(Adjustment1, Value);

	gMyInfo.AnalogVolume = Value;
	gtk_range_set_adjustment(VScale1, Adjustment1);

#if 1
	SendMidi(SND_SEQ_EVENT_CONTROLLER,
		gMyInfo.MyPatchInfo[Slider1].OutPort,
		gMyInfo.MyPatchInfo[Slider1].Channel,
		gMyInfo.MyPatchInfo[Slider1].Patch,
		(char) Value);
#endif
	return (Value);
}

/*--------------------------------------------------------------------
 * Function:		SetVolume2
 *
 * Description:	Change the Volume Slider based on midi input.
 *---------------------------------------------------------------------*/
int SetVolume2(int Value) {
	gtk_adjustment_set_value(Adjustment2, Value);
	gMyInfo.MidiVolume = Value;
	gtk_range_set_adjustment(VScale2, Adjustment2);

#if 1
	SendMidi(SND_SEQ_EVENT_CONTROLLER,
		gMyInfo.MyPatchInfo[Slider2].OutPort,
		gMyInfo.MyPatchInfo[Slider2].Channel,
		gMyInfo.MyPatchInfo[Slider2].Patch,
		(char) Value);
#endif
	return (Value);
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
return(0);
}

