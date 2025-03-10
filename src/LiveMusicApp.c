/*---------------------------------------------
|      Revision Date:   2012/01/15 22:52:40
|
|      Contains:       Main code for the Live Music Application
|
|      Written by:     Elias Keshishoglou
|
|      Date:           Jan 15, 2012
|
|       Version:        1.8.4
|
|      Copyright  2012-2021 Elias Keshishoglou.  All rights reserved.
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
|   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
USA.
|
|
|---------------------------------------------*/

/* Used for allocating storage in the
include file.
*/
#define LiveMusicApp_c

#include "LiveMusicApp.h"

#include "APlayMidi.h"
#include "AlsaUtils.h"
#include "Connections.h"
#include "GenPrefs.h"
#include "HTML.h"
#include "Player.h"
#include "PrefsFile.h"
#include "PrefsGui.h"
#include "SooperOSC.h"
#include "Timers.h"
#include "config.h"
#include "getopt.h"
#include "libgen.h"
#include "math.h"
#include "stdbool.h"
#include "stdlib.h"
#include <pwd.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


// https://docs.gtk.org/gtk4/migrating-3to4.html
#define GLADE_FILE "LiveMusicApp.glade"  // GUI layout file
#define Icon_FILE GetResourceDir("LiveIcon.png", FileLocConfig)  // App icon
#define UsingImageButtons

/* Max number of history messages.
 */
#define MaxStatusHold 12
#define MaxUpdateLength 50
// #define CSSFileName GetResourceDir("LiveMusicApp.css",FileLocConfig)
#define HistoryFileName "/LiveMusicHistory.txt"

// export LIBGL_ALWAYS_SOFTWARE=1

/*
 * Place Global variables here
 */

// The Status Display about what patch was selected.
GtkWidget *MainStatus;  // Status bar widget
GtkWidget *PlayerCurWid;  // Current player widget
guint MainStatusid;  // Status message ID

// The structure to hold the custom buttons.
theImageButtons LayoutButton;  // Layout selection button
theImageButtons MainButtons[Max_Main_Buttons];  // Main patch buttons
theImageButtons TabButtons[tabpageMAX];  // Tab navigation buttons

// The Scale widgets.
GtkWidget *VScale1, *VScale2, *VScale3, *VScale4;  // Volume sliders
GtkAdjustment *Adjustment1, *Adjustment2, *Adjustment3, *Adjustment4;  // Slider adjustments

// Images we use for the custom buttons.
GdkPixbuf *NoteBButtonOnImage;  // Button pressed state
GdkPixbuf *NoteBButtonOffImage;  // Button released state

// The Area we display the tempo
GtkWidget *TempoChild;

/* The entire pane we keep the tabs in.
 */
GtkWidget *NoteBookPane;

// Foot switch layout.
int KeyLayout = 1;

/* Printf Level.
 */
int RunLogLevel;

/* Array to hold the recent status messages.
 */
char HoldStatus[MaxStatusHold][MaxUpdateLength];  // Status message history
int HoldStatusIndex;  // Current status message index

/* Boolean, send internal metronome to midi.
 */
int MainButtonCountOn;

/* Open a history file, I forget what I did
at rehearsal.
*/
FILE *FileHistory;  // History log file
char ResourceFileName[250];  // Resource file path
FILE *LogFile;  // Application log file
char OSCPortNumber[MaxStringPortName];  // OSC communication port

/* Update the Tabs in GTK context.
 */

#define MuteCountDelay 8;
char RemoveMuteCount;  // Counter for unmuting
char *homedir;  // User home directory
int VolumeControllerNum;  // Active volume controller

gint WinWidth, WinHeight;  // Main window dimensions

/*
 * Place Local prototypes here
 */
tPatchIndex LayoutSwitchPatch(tPatchIndex MidiIn, char DoAction);
int JackMaster;
void CreateTabButtons(void);
void CreateMainButtons(void);
void ToggleTempo(void);
void VScale1_Changed(GtkAdjustment *adj);
void VScale2_Changed(GtkAdjustment *adj);
void VScale3_Changed(GtkAdjustment *adj);
void VScale4_Changed(GtkAdjustment *adj);
tPatchIndex GetModePreset(tPatchIndex Value);
int CloseHistoryFile(void);
int InitHistoryFile(void);
gboolean tab_focus_callback(GtkNotebook *notebook, gint *arg1, gpointer data);
void on_Tempo_Button(GtkWidget *widget, GdkEvent *event, gpointer user_data);
void on_About_clicked(GtkButton *button, gpointer user_data);
gboolean layout_click_handler(GtkWidget *widget, GdkEvent *event,
                              gpointer user_data);
gboolean layout_release_handler(GtkWidget *widget, GdkEvent *event,
                                gpointer user_data);
void on_window1_destroy(GtkWidget *widget, gpointer user_data);

int InitJackTransport(void);
int CloseJackTransport(void);
#ifdef GTK_4
gboolean my_keypress_function(GtkWidget *widget, GdkEvent *event,
                              gpointer data);
#else
gboolean my_keypress_function(GtkWidget *widget, GdkEventKey *event,
                              gpointer data);
#endif
void CheckForStartupDirs(void);
void parse_cmdline(int argc, char *argv[]);

/*--------------------------------------------
 * Function:        printd
 *
 * Description:     Loggable print command.
 *
 *---------------------------------------------*/
char *printd(int LogLevel, const char *fmt, ...) {
    va_list ap;
    char p[512]; /* check for null in real code */
    va_start(ap, fmt);
    vsnprintf(p, 512, fmt, ap);
    va_end(ap);

    /* Always print LogTest messages.
     */
    if ((RunLogLevel | LogTest) & LogLevel) {
        printf("L%x %s", LogLevel, p);
    }

    if (LogFile != NULL) {
        fprintf(LogFile, "L%x %s", LogLevel, p);
    }

    return NULL;
}

#ifdef GTK_4
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Live Music App");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_widget_show(window);
}
#endif

/*--------------------------------------------
 * Function:            main
 *
 * Description:         This is where it all starts.
 *
 *---------------------------------------------*/
int main(int argc, char *argv[]) {
    // GtkWidget *theMainWindow;
    GtkWidget *widget;
    GError *error = NULL;
    GtkWidget *ChordWidget;
    GtkWidget *PlayerWidget;
    GtkWidget *EventBox;
    GError *err = NULL;
#if 0
    /*----- CSS ----------- */
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
#endif

    /*-----------------------*/
    int BButtonX, BButtonY, MButtonX, MButtonY;
    int Loop;
    //    GdkScreen *myScreen;
    WeAreRunning = FALSE;

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
    UsePipewire = 0;
    CmdPipewire = "";
    RemoveMuteCount = 0;
    RunLogLevel = 0;
    gMyInfo.TimerCount = 0;
    gMyInfo.TabSwitch = NULLSwitch;
    gMyInfo.RaiseSwitch = NULLSwitch;
    gMyInfo.NextDeskSwitch = NULLSwitch;
    gMyInfo.PrevDeskSwitch = NULLSwitch;
    gMyInfo.GoToDeskSwitch = Max_Patches;
    gMyInfo.MarkerUpdate = false;

    /* Let's see what's already installed.
     */
    LogFile = NULL;
    CheckForStartupDirs();
    GetResourceDir("./MyFile.png", FileLocConfig);
    LogFile = fopen(GetResourceDir("GuitarLog.txt", FileLocConfig), "w+");

    /* Default name for the jack client.
     */
    strcpy(JackName, "default");

    /* Print the build information.
     */
    printf("Build date  : %s:%s\n", __DATE__, __TIME__);
    printf("Build Number %d\n", MY_BUILD_NUMBER);
    printf("Version Number %s\n", MY_VERSION_NUMBER);

    /* Handle any HID pedals,
    Must be called before gtk_init
    */
    //  InitHIDGrab();
    ScreenSize = 2;
    ButtonSize = 140;

    strncpy(OSCPortNumber, MyOSCPortNumber, MaxStringPortName);

    /* initialize the GTK+ library */
    parse_cmdline(argc, argv);
#ifdef GTK_4
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.LiveMusicApp", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
#else
    gtk_init(&argc, &argv);
#endif
    // myScreen = gdk_screen_get_default();
    //    printd(LogInfo, "Screen Size %d %d\n", gdk_screen_get_width(myScreen),
    //    gdk_screen_get_height(myScreen));

    //    gtk_window_get_default_size (theMainWindow, &WinWidth,&WinHeight);

#if 0
    /* Based on the screen, size the buttons.
    */
    if (gdk_screen_get_width (myScreen) > 1400) {
        ScreenSize = 1;
        ButtonSize = 115;
    }

    if (gdk_screen_get_width (myScreen) > 1800) {
        ScreenSize = 2;
        ButtonSize = 130;
    }

    printd (LogInfo, "Button Size %d %d\n", ButtonSize);
#endif

    /*
     * Initialize the XML reader/writer and set some basic values here.
     */
    InitPref();

    /* History saves the charts you open.
    I forget what I worked on at rehearsal,
    this helps me remember.
    */
    InitHistoryFile();

    /*
     create an instance of the GladeXML object and build widgets within
     the window root node.
     */
    gxml = gtk_builder_new();

    /* Choose the glade file based on the layout we are using.
     */
    sprintf(TempStrBuf, "%s.%d", GLADE_FILE, KeyLayout);

    if (!gtk_builder_add_from_file(
            gxml, GetResourceDir(TempStrBuf, FileLocConfig), &error)) {
        g_warning("Couldn't load builder file: %s", error->message);
        g_error_free(error);
    }

    /*
     * get the window widget from the glade XML file
     */
    theMainWindow = GTK_WIDGET(gtk_builder_get_object(gxml, "window1"));

#if 0
    /*------------- CSS  ---------------------------*/
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen (display);

    gtk_style_context_add_provider_for_screen (screen,
            GTK_STYLE_PROVIDER (provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
#endif

    printd(LogInfo, "ejk About to load\n");

    /* Connect the close button.
     */
    g_signal_connect(G_OBJECT(theMainWindow), "destroy",
                     G_CALLBACK(on_window1_destroy), NULL);
    gtk_window_set_title(GTK_WINDOW(theMainWindow), "LiveMusicApp_1");

#if 1
    // desktop-file-install livemusicapp.desktop
    GdkPixbuf *pixbuf;
    pixbuf = gdk_pixbuf_new_from_file(Icon_FILE, &err);

    gtk_window_set_icon(GTK_WINDOW(theMainWindow), pixbuf);
    g_object_unref(pixbuf);
#else
    gtk_window_set_icon_name(GTK_WINDOW(theMainWindow), "guitar");
#endif

    /* Get the button sizes.
    B are the preset buttons
    M are the main application buttons */
    BButtonX = (int)((float)ButtonSize * 1.1);
    BButtonY = (int)((float)ButtonSize * 0.7);
    MButtonX = (int)((float)ButtonSize * 0.9);
    MButtonY = (int)((float)ButtonSize * 0.5);

    MainButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
        GetResourceDir("MainSwitchOn.png", FileLocConfig), MButtonX, MButtonY,
        0, 0);
    MainButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
        GetResourceDir("MainSwitchOff.png", FileLocConfig), MButtonX, MButtonY,
        0, 0);
    PatchButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
        GetResourceDir("PatchSwitchOn.png", FileLocConfig), BButtonX, BButtonY,
        0, 0);
    PatchButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
        GetResourceDir("PatchSwitchOff.png", FileLocConfig), BButtonX, BButtonY,
        0, 0);
    NoteBButtonOnImage = gdk_pixbuf_new_from_file_at_scale(
        GetResourceDir("NoteBSwitchOn.png", FileLocConfig), MButtonX, MButtonY,
        0, 0);
    NoteBButtonOffImage = gdk_pixbuf_new_from_file_at_scale(
        GetResourceDir("NoteBSwitchOff.png", FileLocConfig), MButtonX, MButtonY,
        0, 0);
    //  GdkPixbuf *gdk_pixbuf_scale_simple (const GdkPixbuf *src, 135,65,
    //  GDK_INTERP_NEAREST);
    NoteBookPane = GTK_WIDGET(gtk_builder_get_object(gxml, "MainTab"));

    /*
     * Open the persistent main tab.
     */
    g_signal_connect(GTK_NOTEBOOK(NoteBookPane), "switch-page",
                     (GCallback)tab_focus_callback, gxml);

    /*
     * Setup and initialize our status bar and Mode indicator
     */
    MainStatus = GTK_WIDGET(gtk_builder_get_object(gxml, "MainStatusBar"));
    PlayerCurWid = GTK_WIDGET(gtk_builder_get_object(gxml, "PlayTimeStat"));
    gtk_widget_set_tooltip_text(PlayerCurWid, "Player Countdown to markers.");

    /*
     * Clear the Status bar buffer.
     */
    HoldStatusIndex = 0;
    memset(HoldStatus, 0, sizeof(HoldStatus));

    /*
     * Get the metronome button loaded and displayed.
     */
    EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, "Tempo"));

    /* Set up my buttons.
     */
    MyImageButtonInit(&TempoDraw, EventBox, MainButtonOnImage,
                      MainButtonOffImage);

    SetMetronomeStatus(gMyInfo.MetronomeOn);

    g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                     G_CALLBACK(on_Tempo_Button), &TempoDraw);

    /*
     * The about window.
     */
    widget = GTK_WIDGET(gtk_builder_get_object(gxml, "AboutButton"));
    g_signal_connect_data(G_OBJECT(widget), "clicked",
                          G_CALLBACK(on_About_clicked), NULL, NULL, 0);

#if 0
    /*
     * The OK Button on the About Box
     */
    g_signal_connect (G_OBJECT (theMainWindow),
                      "key_press_event",
                      G_CALLBACK (
                          button_press_notify_cb),
                      NULL);
    /*
     * Create the HTML page from our preferences.
     */
    CreateHTMLGuide (&gMyInfo);
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
    InitMidiLooper();

    /* OSC controls sooperlooper and jack-volume.
     */
    MyOSCInit(OSCPortNumber);

    /* If the options are for Jack Master.
     */
    if (JackMaster) {
        InitJackTransport();
    }

    /*
     * Set up the buttons, text and patches.
     */
    CreateMainButtons();
    SetUpMainButtons(&gMyInfo.MyPatchInfo);
    CreateTabButtons();

    /* Get the Mode switch button,
     */
    EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, "LayoutEvent"));
    printd(LogInfo, "LayoutEvent %x\n", (void *)EventBox);
    MyImageButtonInit(&LayoutButton, EventBox, MainButtonOnImage,
                      MainButtonOffImage);

    MyImageButtonSetText(&LayoutButton, gMyInfo.LayoutPresets[0].Name);

    g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                     G_CALLBACK(layout_click_handler), &LayoutButton);
    g_signal_connect(G_OBJECT(EventBox), "button-release-event",
                     G_CALLBACK(layout_release_handler), &LayoutButton);
    /*
     * Set up the connections between applications.
     */
    InitConnections();

    /*
     * Set the up Chord page in case we need it later.
     */
    ChordWidget = GTK_WIDGET(gtk_builder_get_object(gxml, "ChordFrame"));
    InitChorder(theMainWindow, ChordWidget);

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
    //  g_idle_add(GTKIdel_cb, theMainWindow);

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
    CreatePatchPopupMenu();

#ifdef GTK_4
    gtk_widget_add_events(theMainWindow, 1024);
#else
    gtk_widget_add_events(theMainWindow, GDK_KEY_PRESS_MASK);
#endif
    g_signal_connect(G_OBJECT(theMainWindow), "key_press_event",
                     G_CALLBACK(my_keypress_function), NULL);

    DisableTextInput = FALSE;

    // Clear the LEDS
    //    for (Loop = 32; Loop < 64; Loop++)
    //        SetDAWLed(Loop, 1);

    /*
     * Show the main window and let the show begin.
     */
    gtk_widget_show_all(theMainWindow);
    //    MyOSCJackVol(127, 10);
    MyOSCJackVol(127, typeOSCVolGuitarL);
    MyOSCJackVol(127, typeOSCVolGuitarR);
    MyOSCJackVol(127, typeOSCVolMidi);
    MyOSCJackVol(127, typeOSCVolMP3);

    /*
     * And they're off.
     */
    printd(LogInfo, "And we're off \n");
    WeAreRunning = TRUE;
    gtk_main();
    WeAreRunning = FALSE;

    /*
     * After we quit we should write back the changes.
     */
    //  CloseHIDGrab();
    WritePrefs();
    CloseChorder();
    MyAlsaClose();
    MyOSCClose();
    CloseJackTransport();
    LivePlayerClose();

    /* No printd after this.
     */
    fclose(LogFile);

    return 0;
}

/*--------------------------------------------
 * Function:            parse_cmdline
 *
 * Description:     Deal with command line arguments.
 *---------------------------------------------*/
void parse_cmdline(int argc, char *argv[]) {
    int c;
    int digit_optind = 0;

    //  printf("Arg 0 %s\n", argv[0]);
    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"Version", no_argument, 0, 'v'},
            {"Carla", required_argument, 0, 'c'},
            {"Debug", required_argument, 0, 'd'},
            {"FontSize", required_argument, 0, 'f'},
            {"jack", required_argument, 0, 'j'},
            {"enable jack", no_argument, 0, 'e'},
            {"layout", required_argument, 0, 'l'},
            {"OSCPort", required_argument, 0, 'p'},
            {"IncludeFile", no_argument, 0, 'i'},
            {"Pipewire", no_argument, 0, 'w'},
            //          { "IncludeFile", required_argument, &GenerateHFile, 1 },
            {0, 0, 0, 0}};

        // Get the arguments
        c = getopt_long(argc, argv, "?hievwd:f:j:l:p:c:", long_options,
                        &option_index);

        // If no arguments, then break out.
        if (c == -1) {
            break;
        }

        // Do something with the information
        switch (c) {
        case 'i':
            printf("IncludeFile \n");
            GenerateHFile = 1;
            break;

        case 'f':
            ButtonSize = atoi(optarg);
            printd(LogInfo, "Font Size %d\n", ButtonSize);
            if (ButtonSize < 121) {
                ScreenSize = 1;
            }
            if (ButtonSize < 101) {
                ScreenSize = 0;
            }

            break;

        case 'p':
            strncpy(OSCPortNumber, optarg, MaxStringPortName);
            printd(LogInfo, "OSCPort %s\n", OSCPortNumber);
            break;

        case 'j':
            strncpy(JackName, optarg, MaxStringPortName);
            printd(LogInfo, "JackName %s\n", JackName);
            break;

        case 'l':
            KeyLayout = atoi(optarg);
            printd(LogInfo, "Layout %d\n", KeyLayout);
            break;

        case 'c':
            sscanf(optarg, "%x", &CarlaRackNum);
            printd(LogInfo, "The position of the mixer in the Carla Rack. %d\n",
                   CarlaRackNum);
            break;

        case 'd':
            //          RunLogLevel = atoi(optarg);
            sscanf(optarg, "%x", &RunLogLevel);
            printd(LogInfo, "RunLogLevel 1-no -> 6-all %d\n", RunLogLevel);
            break;

        case 'v':
            // printf("Build date  : %s:%s\n", __DATE__, __TIME__);
            // printf("Build Number %d\n", MY_BUILD_NUMBER);
            // printf("Version Number %s\n", MY_VERSION_NUMBER);
            break;

        case 'e':
            JackMaster = 1;
            printd(LogInfo, "JackMaster off\n");
            break;

        case 'w':
            UsePipewire = 1;
            CmdPipewire = "pw-jack ";
            printd(LogInfo, "UsePipewire on\n");
            break;

        default:
        case 'h':
            printf("Live Music CLI Usage\n");
            printf(" v Version - Print version and build info\n");
            printf(" f FontSize - Font Size for smaller screens.\n");
            printf(" j jack - Jack master name.\n");
            printf(" e enable-jack - Connect to jackserver.\n");
            printf(" l Layout - Glade layout file.\n");
            printf(" i IncludeFile - Generate include file on exit.\n");
            printf(" p OSCPort Number - Server Port number.\n");
            printf(" w pipewire - Use pipewire for playback \n");
            printf(" c carla - mixer position in rack \n");
            printf(" d debug - Debug output level hex 0xfff \n");
            printf("   LogTest 0x01,LogInfo 0x02,LogWarn 0x04,LogAlert 0x08\n");
            printf("   LogError 0x10,LogDebug 0x20,LogTimer 0x40\n");
            printf("   LogRealTime 0x100,LogMidi 0x200,LogPlayer 0x400\n");
            printf("   LogHTML 0x800 \n");

            exit(0);
            break;
        }
    }

    if (optind < argc) {
        printd(LogInfo, "non-option ARGV-elements: ");

        while (optind < argc) {
            printd(LogInfo, "%s ", argv[optind++]);
        }

        printd(LogInfo, "\n");
    }
}

/*--------------------------------------------
 * Function:            GTKIdel_cb
 *
 * Description: Startup some Gui.
 *---------------------------------------------*/
unsigned long int IdleCounter;
int GTKIdel_cb(gpointer data) {
    char ForString[400];

    /* Can't call this from the thread so the
    thread sets the structure and then the action
    gets performed here.
    Expression control of active GUI sliders.
    */
    IdleCounter++;

    if ((IdleCounter > 100)) {
        printd(LogError, "*** Timer Problem %d\n", IdleCounter);
    }

    printd(LogRealTime, "GTKIdel_cb %d %d\n", AlsaEvent.data.control.param,
           gMyInfo.ExpreP1Slider);

    // gMyInfo.SliderGUIValue
    //  if (AlsaEvent.data.control.param == MIDI_CTL_MSB_MAIN_VOLUME) {

    /* Calling GTK function from another thread is not good, so,
    we queue up the requests for UI updates and process them here in
    the idle thread.
    */
    if (gMyInfo.SliderGUIUpdate) {
        printd(LogDebug, "GTKIdel_cb slider %d \n", gMyInfo.ExpreP1Slider);

        switch (gMyInfo.SliderGUINumber) {
        //      switch (gMyInfo.MyPatchInfo[gMyInfo.ExpreP1Slider].Channel) {
        case Slider1:
            printd(LogTest, "GTKIdel_cb Slider1 %d \n", Slider1);
            SetVolume1(gMyInfo.SliderGUIValue / 1.27);
            break;

        case Slider2:
            printd(LogTest, "GTKIdel_cb Slider2 %d \n", Slider2);
            SetVolume2(gMyInfo.SliderGUIValue / 1.27);
            break;

        case Slider3:
            printd(LogTest, "GTKIdel_cb Slider3 %d \n", Slider3);
            SetVolume3(gMyInfo.SliderGUIValue / 1.27);
            break;

        case Slider4:
            printd(LogTest, "GTKIdel_cb Slider4 %d \n", Slider4);
            //          SetScale4Label(gMyInfo.MyPatchInfo[3].Name);

        default:
            //          printd(LogInfo, "GTKIdel_cb: %d\n",
            //          AlsaEvent.data.control.param);
            printd(LogTest, "GTKIdel_cb Default\n");
            //          SetScale4Label(gMyInfo.MyPatchInfo[gMyInfo.ExpreP1Slider].Name);
            SetVolume4(gMyInfo.SliderGUIValue / 1.26);
            break;
        }

        /* Clear it until next message set
         */
        AlsaEvent.data.control.param = 0;
        //      gMyInfo.SliderGUINumber = 0;
        gMyInfo.SliderGUIUpdate--;
    }

    if (gMyInfo.SetMP3PlayVolBool) {
        SetMP3PlayVol(gMyInfo.SetMP3PlayVolBool);
        gMyInfo.SetMP3PlayVolBool = 0;
    }

    if (gMyInfo.ScrollUpdate) {
        ScrollCtrl(gMyInfo.ScrollUpdate);
        gMyInfo.ScrollUpdate = 0;
    }

    if (gMyInfo.MarkerUpdate) {
        gMyInfo.MarkerUpdate = false;
        NewMarkerDialog();
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
        printd(LogDebug, "GuitarMidiCallStart %d\n",
               gMyInfo.GuitarMidiCallParam1);
        GuitarMidiPreset(gMyInfo.GuitarMidiCallParam1);
        gMyInfo.GuitarMidiCall = GuitarMidiCallNo;
    }

    if (gMyInfo.GuitarMidiCall == GuitarMidiCallComplete) {
        printd(LogDebug, "GuitarMidiCallComplete %d\n",
               gMyInfo.GuitarMidiCallParam1);
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
        IncrementLayoutMode();
        gMyInfo.IncrementSwitch = 0;
        MyImageButtonSetText(&LayoutButton,
                             gMyInfo.LayoutPresets[CurrentLayout].Name);
        gtk_image_set_from_pixbuf(GTK_IMAGE(LayoutButton.Image),
                                  LayoutButton.ButtonDownImage);
    }

    if (gMyInfo.PatchUpdate) {
        printd(LogDebug, "gMyInfo.PatchUpdate %d\n", gMyInfo.PatchUpdate);
        LayoutSwitchPatch(gMyInfo.PatchUpdate - 1, true);
        gMyInfo.PatchUpdate = 0;
    }

    if (RemoveMuteCount) {
        printd(LogTest, "RemoveMuteCount %d\n", RemoveMuteCount);

        if (RemoveMuteCount-- == 1) {
            MyOSCJackMute(0, 0xff);
            RemoveMuteCount = 0;
        }
    }

    /* If new information about the slider.
     */
    if (gMyInfo.SliderUpdate > 0 && gMyInfo.SliderUpdate < Max_Patches) {
        printd(LogDebug, "SliderUpdate %d\n", gMyInfo.SliderUpdate);

        if (gMyInfo.MyPatchInfo[gMyInfo.SliderUpdate].CustomCommand ==
            cmdSetExpr) {
            printd(LogDebug, "SliderUpdate \n");
            SetScale4Label(gMyInfo.MyPatchInfo[gMyInfo.SliderUpdate].Name);
        }
        gMyInfo.SliderUpdate = 0;
    }

    /* Set the player Tempo.
     */
    if (gMyInfo.Tempo != gMyInfo.LoopTempo) {
        SetLoopTempo(gMyInfo.Tempo);
    }

    /* Update the player UI and remmote mplayer.
     */
    PlayerPoll(TRUE);

    /* Clear the button.
     */
    MyImageButtonSetText(&TempoDraw, TempoUpdateString);

    /* If the timer went off, update the metronome.
     */
    if (UIUpdateFromTimer == TRUE) {
        UIUpdateFromTimer = FALSE;
        printd(LogRealTime, "Reset *** %d\n", IdleCounter);
        IdleCounter = 0;
        printd(LogRealTime, "UIUpdateFromTimer 1\n");

        /* Make sure the buttons are all up after being pressed.
         */
        ClearMainButtons();

        /* With these off doesn't
        seem to lock up.
        */
        if (JackMaster) {
            jack_poll();
        }

        MyOSCPoll(FALSE);
        //      HIDPoll();

        /* Handle the various LEDs using midi messages.
         */
        SendMidi(SND_SEQ_EVENT_CONTROLLER, DAWPort, 1,
                 MIDI_CTL_GENERAL_PURPOSE5, (int)0);
        SendMidi(SND_SEQ_EVENT_CONTROLLER, DAWPort, 1,
                 MIDI_CTL_GENERAL_PURPOSE6, (int)0);
        SendMidi(SND_SEQ_EVENT_CONTROLLER, DAWPort, 1,
                 MIDI_CTL_GENERAL_PURPOSE7, (int)0);
        SendMidi(SND_SEQ_EVENT_CONTROLLER, DAWPort, 1,
                 MIDI_CTL_GENERAL_PURPOSE8, (int)0);

        /*  Turn Pedal lights off
         */
        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort, DrumMidiChannel, 04,
                 (int)PedalLED3Off);
        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort, DrumMidiChannel, 04,
                 (int)PedalLED4Off);

    } else {
        printd(LogRealTime, "No Timer %d\n", IdleCounter);
    }

    // Update the player time if playing.
    gtk_widget_override_font(
        PlayerCurWid, pango_font_description_from_string("Sans Bold 16"));
    sprintf(ForString,
            "<span font=\"16\" color='#%lx'><b>%3.1f\n%s\n</b></span><span "
            "font=\"12\" color='#%lx'><b>LP %2.2f</b></span>",
            0x80ff00, PlayerDisTime, PlayerDisSection, 0xff8000,
            gMyInfo.LoopPosition);

    gtk_label_set_markup((GtkLabel *)PlayerCurWid, ForString);
    //  printd(LogDebug, "GTKIdel_cb out\n");
    return (FALSE);
    //  return (TRUE);
}

/*--------------------------------------------
 * Function:     GetResourceDir
 *
 * Description:      Return the full path for the file.
 *
 *---------------------------------------------*/
char *GetResourceDir(char *FileName, char WhichLoc) {

    /* Location of resource files needed.
     */
    if (WhichLoc == FileLocConfig) {
        strcpy(ResourceFileName, homedir);
        strcat(ResourceFileName, "/.config/LiveMusicApp/");
    }

    /* Default location of the Songs files.
     */
    if (WhichLoc == FileLocTunes) {
        strcpy(ResourceFileName, homedir);
        strcat(ResourceFileName, "/MySongs/");
    }

    /* The location the user selected. Must load prefs
    file before using this option.
    */
    if (WhichLoc == FileLocUser) {
        strcpy(ResourceFileName, (gMyInfo.BasePath));
        dirname(ResourceFileName);
        strcat(ResourceFileName, "/");
    }

    strcat(ResourceFileName, FileName);
    return (ResourceFileName);
}

/*--------------------------------------------
 * Function:     Check for the .config dirs
 *
 * Description:
 *
 *---------------------------------------------*/
void CheckForStartupDirs(void) {
    struct stat s;
    int err;
    char FileString[250];
    char CommandString[400];
    char *UserName;

    /* Get home directory.
     */
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    UserName = getenv("USER");

    //    NewInstall = 0;

    // Check for an existing config directory
    sprintf(FileString, "%s/.config/LiveMusicApp", homedir);
    err = lstat(FileString, &s);
    //    err = access(FileString, &s);

    // if not then copy the default from /usr/share
    if (err < 0) {
        // Copy LiveMusic Prefs folder the local location
        sprintf(CommandString,
                "rsync -avrx --chown=%s:%s /usr/local/share/LiveMusicApp "
                "%s/.config/",
                UserName, UserName, homedir);
        system(CommandString);
        printf("New Inst %s\n", CommandString);

        // Copy the Songs directory
        sprintf(CommandString,
                "rsync -avrx --chown=%s:%s "
                "/usr/local/share/LiveMusicApp/MySongs %s/",
                UserName, UserName, homedir);
        system(CommandString);
        printf("New Inst %s\n", CommandString);

        // Create a generic pref files.
        printf("Fill in local preferences\n");
        memcpy(&gMyInfo, &GlobalInfo, sizeof(LiveMusicInfo));
        strcpy(gMyInfo.BasePath, GetResourceDir("index.html", FileLocTunes));
        printf("Write to Preferences file\n");
        WritePrefs();

        //        NewInstall = 1;
    }
}

/*--------------------------------------------
 * Function:            my_keypress_function
 *
 * Description: Get keyboard events.
 *---------------------------------------------*/
#ifdef GTK_4
gboolean my_keypress_function(GtkWidget *widget, GdkEvent *event,
                              gpointer data) {
    int MyEvent = !keyval(event);
    int MyState = !state(event);
#else
gboolean my_keypress_function(GtkWidget *widget, GdkEventKey *event,
                              gpointer data) {
    int MyEvent = event->keyval;
    int MyState = event->state;
#endif

    printd(LogDebug, "Event %x %x\n", MyEvent, MyState);

    if (DisableTextInput) {
        printd(LogDebug, "Key Disabled\n");
        return (FALSE);
    }

    if (MyEvent >= 0x31 && MyEvent <= 0x39 && MyState == 4) {
        printd(LogDebug, "Patch\n");
        LayoutSwitchPatch(MyEvent - 0x31, true);
        return (TRUE);
    }

    switch (MyEvent) {

    case GDK_KEY_q:
        if (MyState == 4) {
            printd(LogDebug, "Found quit\n");
            gtk_window_close(GTK_WINDOW(theMainWindow));
        }

    case GDK_KEY_l:
        if (MyState == 4) {
            printd(LogDebug, "GDK_KEY_l\n");
            plLoopToggle();
        }

    //  if ( MyEvent == GDK_KEY_p && MyState == 0) {
    case GDK_KEY_space:
        if (MyState == 4) {
            printd(LogDebug, "GDK_KEY_space\n");
            plPausePlay();
        }

    case GDK_KEY_a:
        if (MyState == 4) {
            printd(LogDebug, "GDK_KEY_a\n");
            plSetA();
        }

    case GDK_KEY_b:
        if (MyState == 4) {
            printd(LogDebug, "GDK_KEY_b\n");
            plSetB();
        }

        // case GDK_KEY_Return:
        //  printd(LogDebug, "GDK_KEY_Return\n");
        //  return (TRUE);
        //  break;

        // case GDK_KEY_BackSpace:
        //  printd(LogDebug, "GDK_KEY_BackSpace\n");
        //  return (TRUE);
        //  break;

    case GDK_KEY_Tab:
        printd(LogDebug, "GDK_KEY_Tab\n");
        return (TRUE);
        break;

    case GDK_KEY_Multi_key:
        printd(LogDebug, "GDK_KEY_Multi_key\n");
        return (TRUE);
        break;

    case GDK_KEY_Page_Up:
        printd(LogDebug, "GDK_KEY_Page_Up\n");
        return (TRUE);
        break;

    case GDK_KEY_Page_Down:
        printd(LogDebug, "GDK_KEY_Page_Down\n");
        return (TRUE);
        break;

    case GDK_KEY_F1:
        printd(LogDebug, "GDK_KEY_F1\n");
        return (TRUE);
        break;

    case GDK_KEY_Control_L:
        printd(LogDebug, "GDK_KEY_Control_L\n");
        return (TRUE);
        break;

    case GDK_KEY_Meta_L:
        printd(LogDebug, "GDK_KEY_Meta_L\n");
        return (TRUE);
        break;

    case GDK_KEY_Super_L:
        printd(LogDebug, "GDK_KEY_Super_L\n");
        return (TRUE);
        break;

    case GDK_KEY_Begin:
        printd(LogDebug, "GDK_KEY_Begin\n");
        return (TRUE);
        break;

    case GDK_KEY_Alt_L:
        printd(LogDebug, "GDK_KEY_Alt_L\n");
        return (TRUE);
        break;

    case GDK_KEY_Down:
        printd(LogDebug, "GDK_KEY_Down\n");
        return (TRUE);
        break;

    case GDK_KEY_Up:
        printd(LogDebug, "GDK_KEY_Up\n");
        return (TRUE);
        break;

    case GDK_KEY_Right:
        printd(LogDebug, "GDK_KEY_Right\n");
        return (TRUE);
        break;

    case GDK_KEY_Num_Lock:
        printd(LogDebug, "GDK_KEY_Num_Lock\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Space:
        printd(LogDebug, "GDK_KEY_KP_Space\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Tab:
        printd(LogDebug, "GDK_KEY_KP_Tab\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Enter:
        printd(LogDebug, "GDK_KEY_KP_Enter\n");
        return TRUE;
        break;

    case GDK_KEY_KP_F1:
        printd(LogDebug, "GDK_KEY_KP_F1\n");
        return TRUE;
        break;

    case GDK_KEY_KP_F2:
        printd(LogDebug, "GDK_KEY_KP_F2\n");
        return TRUE;
        break;

    case GDK_KEY_KP_F3:
        printd(LogDebug, "GDK_KEY_KP_F3\n");
        return TRUE;
        break;

    case GDK_KEY_KP_F4:
        printd(LogDebug, "GDK_KEY_KP_F4\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Home:
        printd(LogDebug, "GDK_KEY_KP_Home\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Left:
        printd(LogDebug, "GDK_KEY_KP_Left\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Up:
        printd(LogDebug, "GDK_KEY_KP_Up\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Right:
        printd(LogDebug, "GDK_KEY_KP_Right\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Down:
        printd(LogDebug, "GDK_KEY_KP_Down\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Page_Up:
        printd(LogDebug, "GDK_KEY_KP_Page_Up\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Page_Down:
        printd(LogDebug, "GDK_KEY_KP_Page_Down\n");
        return TRUE;
        break;

    case GDK_KEY_KP_End:
        printd(LogDebug, "GDK_KEY_KP_End\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Begin:
        printd(LogDebug, "GDK_KEY_KP_Begin\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Insert:
        printd(LogDebug, "GDK_KEY_KP_Insert\n");
        return TRUE;
        break;

        // case GDK_KEY_KP_Delete:
        //  printd(LogDebug, "GDK_KEY_KP_Delete\n");
        //  return TRUE;
        //  break;

    case GDK_KEY_KP_Equal:
        printd(LogDebug, "GDK_KEY_KP_Equal\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Multiply:
        printd(LogDebug, "GDK_KEY_KP_Multiply\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Add:
        printd(LogDebug, "GDK_KEY_KP_Add\n");
        SetExpressionControl(VolumeControllerNum,
                             GetExpressionControl(VolumeControllerNum) + 5);
        return TRUE;
        break;

    case GDK_KEY_KP_Separator:
        printd(LogDebug, "GDK_KEY_KP_Separator\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Subtract:
        printd(LogDebug, "GDK_KEY_KP_Subtract\n");
        SetExpressionControl(VolumeControllerNum,
                             GetExpressionControl(VolumeControllerNum) - 5);
        return TRUE;
        break;

    case GDK_KEY_KP_Decimal:
        printd(LogDebug, "GDK_KEY_KP_Decimal\n");
        return TRUE;
        break;

    case GDK_KEY_KP_Divide:
        printd(LogDebug, "GDK_KEY_KP_Divide\n");
        return TRUE;
        break;

    case GDK_KEY_KP_0:
        printd(LogDebug, "GDK_KEY_KP_0\n");
        VolumeControllerNum = 1;
        return TRUE;
        break;

    case GDK_KEY_KP_1:
        printd(LogDebug, "GDK_KEY_KP_1\n");
        VolumeControllerNum = 2;
        return TRUE;
        break;

    case GDK_KEY_KP_2:
        printd(LogDebug, "GDK_KEY_KP_2\n");
        VolumeControllerNum = 3;
        return TRUE;
        break;

    case GDK_KEY_KP_3:
        printd(LogDebug, "GDK_KEY_KP_3\n");
        VolumeControllerNum = 4;
        return TRUE;
        break;

    case GDK_KEY_KP_4:
        printd(LogDebug, "GDK_KEY_KP_4\n");
        VolumeControllerNum = 5;
        return TRUE;
        break;

    case GDK_KEY_KP_5:
        printd(LogDebug, "GDK_KEY_KP_5\n");
        VolumeControllerNum = 6;
        return TRUE;
        break;

    case GDK_KEY_KP_6:
        printd(LogDebug, "GDK_KEY_KP_6\n");
        VolumeControllerNum = 7;
        return TRUE;
        break;

    case GDK_KEY_KP_7:
        printd(LogDebug, "GDK_KEY_KP_7\n");
        VolumeControllerNum = 8;
        return TRUE;
        break;

    case GDK_KEY_KP_8:
        printd(LogDebug, "GDK_KEY_KP_8\n");
        return TRUE;
        break;

    case GDK_KEY_KP_9:
        printd(LogDebug, "GDK_KEY_KP_9\n");
        return TRUE;
        break;
    }

    g_idle_add(GTKIdel_cb, theMainWindow);

    return FALSE;
}

/*--------------------------------------------
 * Function:        SwitchToTab
 *
 * Description:     <Description/Comments>
 *
 *---------------------------------------------*/
void SwitchToTab(char Tab) {
    int Loop;

    printd(LogDebug, "SwitchToTab %d\n", Tab);

    // Check for a valid tab number
    if (Tab < 0 || Tab >= tabpageMAX) {
        return;
    }

    if (Tab == tabpagePrefs) {
        DisableTextInput = TRUE;
    } else {
        DisableTextInput = FALSE;
    }

    // Clear all of the tab buttons.
    for (Loop = 0; Loop < tabpageMAX; Loop++) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(TabButtons[Loop].Image),
                                  TabButtons[Loop].ButtonUpImage);
    }

    Loop = Tab;
    gtk_image_set_from_pixbuf(GTK_IMAGE(TabButtons[Loop].Image),
                              TabButtons[Loop].ButtonDownImage);

    //  PreviousTab = gtk_notebook_get_current_page(GTK_NOTEBOOK(NoteBookPane));
    gtk_notebook_set_current_page(GTK_NOTEBOOK(NoteBookPane), Tab);
    printd(LogDebug, "Switch to Tab %x %d %d\n", NoteBookPane, PreviousTab,
           Tab);
}

#if 0
/*--------------------------------------------
 * Function:        on_button_clicked
 *
 * Description:     When the uses presses one of the main screen patch
 *  buttons we get dispatched here.
 *
 *---------------------------------------------*/
void on_button_clicked (GtkButton *button, gpointer user_data) {
    //  tPatchIndex PatchIndex;

    printd (LogInfo, "on_button_clicked %d\n", (int) user_data);
    //  PatchIndex = LayoutSwitchPatch(user_data, true);
    LayoutSwitchPatch (user_data, true);
    g_idle_add (GTKIdel_cb, theMainWindow);

}
#endif

/*--------------------------------------------
 * Function:        on_layoutbutton_clicked
 *
 * Description:     When the mode button is pressed from the screen we call
 *  this.
 *
 *---------------------------------------------*/
gboolean layout_click_handler(GtkWidget *widget, GdkEvent *event,
                              gpointer user_data) {
    theImageButtons *theButton;

    theButton = (theImageButtons *)user_data;
    printd(LogDebug, "layout_click %x\n", theButton);
    IncrementLayoutMode();
    MyImageButtonSetText(theButton, gMyInfo.LayoutPresets[CurrentLayout].Name);
    gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
                              theButton->ButtonDownImage);

    g_idle_add(GTKIdel_cb, theMainWindow);

    return TRUE; /* stop event propagation */
}

/*--------------------------------------------
 * Function:        layout_release_handler
 *
 * Description:
 *
 *---------------------------------------------*/
gboolean layout_release_handler(GtkWidget *widget, GdkEvent *event,
                                gpointer user_data) {
    theImageButtons *theButton;
    theButton = (theImageButtons *)user_data;
    gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
                              theButton->ButtonUpImage);
    g_idle_add(GTKIdel_cb, theMainWindow);
    return TRUE; /* stop event propagation */
}

/*--------------------------------------------
 * Function:        on_About_clicked
 *
 * Description:     It's all about ME !
 *
 *---------------------------------------------*/
void on_About_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window;

    window = GTK_WIDGET(gtk_builder_get_object(gxml, "AboutDialog"));
    gtk_widget_show(window);
}

/*--------------------------------------------
 * Function:        on_window1_destroy
 *
 * Description:     That's IT, I'm outta here.
 *
 *---------------------------------------------*/
void on_window1_destroy(GtkWidget *widget, gpointer user_data) {

    //  printd(LogInfo, "In Destroy\n");
    WritePrefs();
    gtk_main_quit();
}

/*--------------------------------------------
 * Function:        on_Tempo_Button
 *
 * Description:     Turn the Metrognome on or off
 *
 *---------------------------------------------*/
void on_Tempo_Button(GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    // Put Dialog here.
#ifdef GTK_4
    if (!state(event) & GDK_CONTROL_MASK) {
        printd(LogInfo, "CTRL and Tempo\n");
        //      gMyInfo.Tempo = 132;
    }
#else
    if (event->button.state & GDK_CONTROL_MASK) {
        printd(LogInfo, "CTRL and Tempo\n");
        //      gMyInfo.Tempo = 132;
    }
#endif

    if (gMyInfo.MetronomeOn) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(TempoDraw.Image),
                                  TempoDraw.ButtonUpImage);
        gMyInfo.MetronomeOn = 0;

    } else {
        gtk_image_set_from_pixbuf(GTK_IMAGE(TempoDraw.Image),
                                  TempoDraw.ButtonDownImage);
        gMyInfo.MetronomeOn = 1;
    }

    g_idle_add(GTKIdel_cb, theMainWindow);
}

/*--------------------------------------------
 * Function:        tab_focus_callback
 *
 * Description:     <Description/Comments>
 *
 *---------------------------------------------*/
gboolean tab_focus_callback(GtkNotebook *notebook, gint *arg1, gpointer data) {
    SetUpMainButtons(&gMyInfo.MyPatchInfo);
    return true;
}

/*--------------------------------------------
 * Function:            ClearMainButtons
 *
 * Description: Make sure the buttons are all up.
 *---------------------------------------------*/
void ClearMainButtons(void) {
    int Loop;

    if (MainButtonCountOn == 1) {
        for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
            //          if (MainButtons[Loop].State) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(MainButtons[Loop].Image),
                                      MainButtons[Loop].ButtonUpImage);
            MainButtons[Loop].State = 0;
            MainButtonCountOn = 0;
            //          }
        }

    } else if (MainButtonCountOn) {
        MainButtonCountOn--;
    }
}

/*--------------------------------------------
 * Function:        Update the display status
 * Description:     raw the previous patches on the screen to make
 *  sure that they completed. It's hard to tell with a foot pedal .
 *
 *---------------------------------------------*/
void UpdateStatus(char *String) {
    char DisString[MaxStatusHold * (MaxUpdateLength + 20)];
    //  char DisString[1500];
    int Loop;
    int StringOff = 0;

    /* Check for string length
     */
    if (strlen(String) > 11) {
        String[11] = 0;
    }

    /* Move everything up to add new entry on the bottom.
     */
    for (Loop = 0; Loop < MaxStatusHold - 1; Loop++) {
        strcpy(HoldStatus[Loop], HoldStatus[Loop + 1]);
        StringOff +=
            sprintf((DisString + StringOff),
                    "<span font=\"12\" color='#%lx'><b>%12s\n</b></span>",
                    gMyInfo.StatusTextColor, (char *)&HoldStatus[Loop]);
    }

    strcpy(HoldStatus[MaxStatusHold - 1], String);
    sprintf((DisString + StringOff),
            "<span font=\"12\" color='#%lx'><b>%12s\n</b></span>",
            gMyInfo.StatusTextColor, (char *)String);

    /* Actually draw the text to the window.
     */
    gtk_widget_override_font(
        MainStatus, pango_font_description_from_string("Sans Bold 12"));
    gtk_label_set_markup((GtkLabel *)MainStatus, DisString);
}

/*--------------------------------------------
 * Function:        SetMetronomeStatus
 *
 *---------------------------------------------*/
void SetMetronomeStatus(char State) {

    gMyInfo.MetronomeOn = State;
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
}

/*--------------------------------------------
 * Function:        Main Button Handler
 *
 *---------------------------------------------*/
gboolean click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    int Loop;
    GdkEvent *theEvent;
    int MyState;

    theEvent = gtk_get_current_event();

#ifdef GTK_4
    MyState = !state(theEvent);
#else
    MyState = theEvent->button.state;
#endif
    printd(LogInfo, "Event %x %x\n", MyState, GDK_CONTROL_MASK);

    Loop = (intptr_t)user_data;
    CurrentPreset = Loop;

    if (MyState & GDK_CONTROL_MASK) {
        printd(LogInfo, "We have Control Down\n");
        ShowPatchListSelect(GTK_WINDOW(widget), Loop);
    }

    /* Only call make the call if we are not adding,
    a new patch to the button.
    */
    else {
        LayoutSwitchPatch(Loop, true);
    }

    return TRUE; /* stop event propagation */
}

/*--------------------------------------------
 * Function:        release_handler
 *
 * Description:     Clear the button
 *---------------------------------------------*/
gboolean release_handler(GtkWidget *widget, GdkEvent *event,
                         gpointer user_data) {
    int Loop;

    Loop = (intptr_t)user_data;
    gtk_image_set_from_pixbuf(GTK_IMAGE(MainButtons[Loop].Image),
                              MainButtons[Loop].ButtonUpImage);

    g_idle_add(GTKIdel_cb, theMainWindow);

    return TRUE; /* stop event propagation */
}

/*--------------------------------------------
 * Function:        Notebook_click_handler
 *
 * Description:     Build the notebook tab
 *---------------------------------------------*/
gboolean Notebook_click_handler(GtkWidget *widget, GdkEvent *event,
                                gpointer user_data) {
    int Loop;

    Loop = (intptr_t)user_data;
    //  gtk_notebook_set_current_page(GTK_NOTEBOOK(NoteBookPane), Loop);
    SwitchToTab(Loop);
    PreviousTab = CurrentTab;
    CurrentTab = Loop;

    g_idle_add(GTKIdel_cb, theMainWindow);
    return TRUE; /* stop event propagation */
}

/*--------------------------------------------
 * Function:        CreateTabButtons
 *
 * Description:     Build the notebook tab
 *---------------------------------------------*/
void CreateTabButtons(void) {
    int Loop;
    char Buffer[40];
    GtkWidget *MainButtonImage;
    GtkWidget *EventBox;

    for (Loop = 0; Loop < tabpageMAX; Loop++) {
        sprintf(Buffer, "NTab%d", Loop + 1);
        EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, Buffer));
        MyImageButtonInit(&TabButtons[Loop], EventBox, NoteBButtonOnImage,
                          NoteBButtonOffImage);

        g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                         G_CALLBACK(Notebook_click_handler), (gpointer)Loop);
    }

    MyImageButtonSetText(&TabButtons[0], "Patch");
    MyImageButtonSetText(&TabButtons[1], "Chart");
    MyImageButtonSetText(&TabButtons[2], "Player");
    MyImageButtonSetText(&TabButtons[3], "Chords");
    MyImageButtonSetText(&TabButtons[4], "Prefs");
    gtk_image_set_from_pixbuf(GTK_IMAGE(TabButtons[0].Image),
                              TabButtons[0].ButtonDownImage);
}

/*--------------------------------------------
 * Function:        CreateMainButtons
 *
 * Description:     Build the main button screen
 *---------------------------------------------*/
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
        //      gtk_widget_get_usize(EventBox);

        MyImageButtonInit(&MainButtons[Loop], EventBox, PatchButtonOnImage,
                          PatchButtonOffImage);
        gtk_widget_set_tooltip_text(MainButtons[Loop].EventBox,
                                    "CTRL-Click to set button.");

        g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                         G_CALLBACK(click_handler), (gpointer)Loop);
        g_signal_connect(G_OBJECT(EventBox), "button-release-event",
                         G_CALLBACK(release_handler), (gpointer)Loop);
    }

#endif
    GtkWidget *Label;

    VScale1 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale1"));
    Adjustment1 = (GtkAdjustment *)(gtk_builder_get_object(gxml, "adj_Analog"));
    g_signal_connect(G_OBJECT(VScale1), "value_changed",
                     G_CALLBACK(VScale1_Changed), NULL);
    Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label1"));
    gtk_label_set_text(
        (Label), (gchar *)gMyInfo.MyPatchInfo[gMyInfo.HardSlider[0]].Name);

    VScale2 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale2"));
    Adjustment2 = (GtkAdjustment *)(gtk_builder_get_object(gxml, "adj_Midi"));
    g_signal_connect(G_OBJECT(VScale2), "value_changed",
                     G_CALLBACK(VScale2_Changed), NULL);
    Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label2"));
    gtk_label_set_text(
        (Label), (gchar *)gMyInfo.MyPatchInfo[gMyInfo.HardSlider[1]].Name);

    VScale3 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale3"));
    Adjustment3 = (GtkAdjustment *)(gtk_builder_get_object(gxml, "adj_Master"));
    g_signal_connect(G_OBJECT(VScale3), "value_changed",
                     G_CALLBACK(VScale3_Changed), NULL);
    Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label3"));
    gtk_label_set_text(
        (Label), (gchar *)gMyInfo.MyPatchInfo[gMyInfo.HardSlider[2]].Name);

    VScale4 = GTK_WIDGET(gtk_builder_get_object(gxml, "vscale4"));
    Adjustment4 =
        (GtkAdjustment *)(gtk_builder_get_object(gxml, "adj_Express"));
    g_signal_connect(G_OBJECT(VScale4), "value_changed",
                     G_CALLBACK(VScale4_Changed), NULL);
    Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label4"));
    gtk_label_set_text(
        (Label), (gchar *)gMyInfo.MyPatchInfo[gMyInfo.HardSlider[3]].Name);
}

/*--------------------------------------------
 * Function:        MidiToDB
 *
 * Description:     Convert Linear to Audio.
 *
 *---------------------------------------------*/
float MidiToDB(int Volume) {
    float LogValue;
    //    VolumeFloat = ((float)Volume / 127);
    //    VolumeFloat = Log(128/(Volume + 1)) * (-10);
    //    VolumeFloat=(exp((float)Volume/128)*74 - 74)/128;
    //    VolumeFloat=(exp((float)Volume/32)*2.5-2.5);
    //    VolumeFloat=(exp((float)Volume/42)*6.5-6.5);

    //    VolumeFloat=(exp((float)Volume/60)*17 - 17)/128;
    //    VolumeFloat=(exp((float)Volume/50)*11 - 11)/130;
    LogValue = (exp((float)Volume / 60) * 17 - 17);
    return (LogValue);
}

/*--------------------------------------------
 * Function:        SetScale4Label
 *
 * Description:     Volume 4 Label
 *
 *---------------------------------------------*/
void SetScale4Label(char *String) {
    GtkWidget *Label;

    Label = GTK_WIDGET(gtk_builder_get_object(gxml, "label4"));
    printd(LogDebug, "SetScale4Label %d %s\n", Label, String);
    gtk_label_set_text((Label), (gchar *)String);
}

/*--------------------------------------------
 * Function:        VScale1_Changed
 *
 * Description:     Volume Sliders changed.
 *              All values from 0-127
 *
 *---------------------------------------------*/
void VScale1_Changed(GtkAdjustment *adj) {
    int ThisPatchNum;
    PatchInfo *ThisPatch;
    unsigned int NewValue;

    /* Set the number of decimal places to which adj->value is rounded */
    //   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
    NewValue = gtk_adjustment_get_value(Adjustment1) * 1.27;
    printd(LogDebug, "Vscale 1 %d\n", NewValue);
    ThisPatchNum = gMyInfo.HardSlider[0];
    ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];
    //  gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment1);

    if (ThisPatch->OutPort == InternalPort) {
        MyOSCJackVol(NewValue, typeOSCVolGuitarL);
        MyOSCJackVol(NewValue, typeOSCVolGuitarR);
    } else
        SendMidi(SND_SEQ_EVENT_CONTROLLER, ThisPatch->OutPort,
                 ThisPatch->Channel, ThisPatch->Patch, (char)NewValue);

    printd(LogDebug, "Vscale 1 End %d\n", NewValue);
}

/*--------------------------------------------
 * Function:        VScale2_Changed
 *
 * Description:     Volume Sliders changed.
 *
 *---------------------------------------------*/
void VScale2_Changed(GtkAdjustment *adj) {
    int ThisPatchNum;
    PatchInfo *ThisPatch;
    unsigned int NewValue;
    unsigned int LogValue;

    /* Set the number of decimal places to which adj->value is rounded */
    //   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
    NewValue = gtk_adjustment_get_value(Adjustment2) * 1.27;
    ThisPatchNum = gMyInfo.HardSlider[1];
    ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];
    gMyInfo.MidiVolume = (char)gtk_adjustment_get_value(Adjustment2) * 1.25;

    LogValue = MidiToDB(NewValue);
    printd(LogInfo, "Vscale 2 %d %d \n", NewValue, LogValue);

    if (ThisPatch->OutPort == InternalPort) {
        MyOSCJackVol(NewValue, typeOSCVolMidi);
    } else
        SendMidi(SND_SEQ_EVENT_CONTROLLER, ThisPatch->OutPort,
                 ThisPatch->Channel, ThisPatch->Patch, (char)LogValue);
}

/*--------------------------------------------
 * Function:        VScale3_Changed
 *
 * Description:     Volume Sliders changed.
 *
 *---------------------------------------------*/
void VScale3_Changed(GtkAdjustment *adj) {
    int ThisPatchNum;
    PatchInfo *ThisPatch;
    unsigned int NewValue;
    unsigned int LogValue;

    /* Set the number of decimal places to which adj->value is rounded */
    //   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
    NewValue = gtk_adjustment_get_value(Adjustment3) * 1.27;
    ThisPatchNum = gMyInfo.HardSlider[2];
    ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];

    LogValue = MidiToDB(NewValue);
    printd(LogInfo, "Vscale 3 %d %d \n", NewValue, LogValue);

    if (ThisPatch->OutPort == InternalPort) {
        MyOSCJackVol(LogValue, typeOSCVolMaster);
    } else
        SendMidi(SND_SEQ_EVENT_CONTROLLER, ThisPatch->OutPort,
                 ThisPatch->Channel, ThisPatch->Patch, (char)LogValue);
}

/*--------------------------------------------
 * Function:        VScale4_Changed
 *
 * Description:     Volume Sliders changed.
 *
 *---------------------------------------------*/
void VScale4_Changed(GtkAdjustment *adj) {
    int ThisPatchNum;
    PatchInfo *ThisPatch;
    unsigned int NewValue;

    /* Set the number of decimal places to which adj->value is rounded */
    //   gtk_scale_set_digits (GTK_SCALE (VScale1), (gint) adj->value);
    NewValue = gtk_adjustment_get_value(Adjustment4) * 1.27;
    ThisPatchNum = gMyInfo.ExpreP1Slider;
    ThisPatch = &gMyInfo.MyPatchInfo[ThisPatchNum];
    printd(LogInfo, "Vscale 4 %d P=%d\n", NewValue, ThisPatch->Channel);

    //  gMyInfo.AnalogVolume = (char) gtk_adjustment_get_value(Adjustment4);

    if (ThisPatch->OutPort == InternalPort) {
        MyOSCJackVol(NewValue, typeOSCVolGuitarL);
        MyOSCJackVol(NewValue, typeOSCVolGuitarR);
    } else {
        SendMidi(SND_SEQ_EVENT_CONTROLLER, ThisPatch->OutPort,
                 ThisPatch->Channel, ThisPatch->Patch, (char)NewValue);
    }
}

/*--------------------------------------------
 * Function:        SetVolume1
 *
 * Description: Change the Volume Slider based on midi input.
 *---------------------------------------------*/
int SetVolume1(int Value) {

    gtk_adjustment_set_value(Adjustment1, Value);
    printd(LogDebug, "Slider 1\n");

    if (Value > 98) {
        Value = 100;
    }

    gMyInfo.AnalogVolume = Value;
    gtk_range_set_adjustment(VScale1, Adjustment1);

    return (Value);
}

/*--------------------------------------------
 * Function:        SetVolume2
 *
 * Description: Change the Volume Slider based on midi input.
 *---------------------------------------------*/
int SetVolume2(int Value) {
    printd(LogDebug, "Slider 2 %x %d\n", Adjustment2, Value);

    if (Value > 98) {
        Value = 100;
    }

    gtk_adjustment_set_value(Adjustment2, Value);
    gMyInfo.MidiVolume = (Value * 1.27);
    gtk_range_set_adjustment(VScale2, Adjustment2);
    return (Value);
}

/*--------------------------------------------
 * Function:        SetVolume3
 *
 * Description:
 *---------------------------------------------*/
int SetVolume3(int Value) {
    printd(LogDebug, "Slider 3 %x %d\n", Adjustment2, Value);

    if (Value > 98) {
        Value = 100;
    }

    gtk_adjustment_set_value(Adjustment3, Value);
    gMyInfo.V3Volume = Value;
    gtk_range_set_adjustment(VScale3, Adjustment3);
    return (Value);
}

/*--------------------------------------------
 * Function:        SetVolume4
 *
 * Description:
 *---------------------------------------------*/
int SetVolume4(int Value) {
    printd(LogTest, "SetVol Slider 4 %x %d\n", Adjustment2, Value);

    if (Value > 98) {
        Value = 100;
    }

    gtk_adjustment_set_value(Adjustment4, Value);
    gMyInfo.V4Volume = Value;
    gtk_range_set_adjustment(VScale4, Adjustment4);
    return (Value);
}

/*--------------------------------------------
 * Function:        SetUpMainButtons
 *
 * Description:     Set the title of all of the buttons
 *
 *---------------------------------------------*/
void SetUpMainButtons(PatchInfo *myPatchInfo) {
    GtkWidget *myButton;
    GtkWidget *myChild;
    tPatchIndex Loop;
    tPatchIndex PatchIndex;
    //    GdkColor color;
    char String[PatchNameSize];
    int StringLen;

#ifdef UsingImageButtons

    for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
        PatchIndex = LayoutSwitchPatch(Loop, FALSE);
        printd(LogInfo, "SetUpMainButtons1:IMG %d %d \n", Loop, PatchIndex);

        if (PatchIndex >= 0 && PatchIndex < Max_Patches) {
            MyImageButtonSetText2(&MainButtons[Loop], Loop + 1,
                                  gMyInfo.MyPatchInfo[PatchIndex].Name);
        }
    }

#else

    for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
        myButton = MainButtons[Loop];
        PatchIndex = LayoutSwitchPatch(Loop, FALSE);

        if (PatchIndex >= 0 && PatchIndex < Max_Patches) {
            sprintf(String, "%02d-%s", Loop + 1,
                    gMyInfo.MyPatchInfo[PatchIndex].Name);
            printd(LogInfo, "SetUpMainButtons %d %s \n", PatchIndex,
                   gMyInfo.MyPatchInfo[PatchIndex].Name);
            myChild = gtk_bin_get_child(GTK_BIN(myButton));
            gtk_label_set_text((myChild), String);
        }
    }
#endif
}

/*--------------------------------------------
 * Function:        DoPatch
 *
 * Description:     Trigger the patch by structure.
 *
 *---------------------------------------------*/
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

            if (NextCommand != -1) {
                usleep(150000);
            }

        } else {
            NextCommand = -1;
        }

    } while (NextCommand != -1);

    return (0);
}

/*--------------------------------------------
 * Function:        RaiseWindowsNum
 *
 * Description:     Bring the App/Window into the forground.
 *
 *---------------------------------------------*/
void RaiseWindowsNum(int AppNumber) {
    printd(LogInfo, "Raise Window %s\n", gMyInfo.Apps[AppNumber].Name);
    RaiseWindows(&gMyInfo.Apps[AppNumber].Name);
}

/*--------------------------------------------
 * Function:        GetModePreset
 *
 * Description:     Based on the current mode convert an index.
 *
 *---------------------------------------------*/
tPatchIndex GetModePreset(tPatchIndex Value) {
    tPatchIndex NewValue;

    NewValue = FindString(fsPatchNames,
                          gMyInfo.LayoutPresets[CurrentLayout].Presets[Value]);

    printd(LogInfo, "GetModePreset %d %d %d\n", Value, NewValue, CurrentLayout);
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
    //  printd(LogInfo, "Get Mode Preset Old %d New %d\n", Value, NewValue);
    return (NewValue);
}

/*--------------------------------------------
 * Function:        IncrementLayoutMode
 *
 * Description:     Based on the current mode do a different.
 *
 *---------------------------------------------*/
void IncrementLayoutMode(void) {

    // Check for wrap around.
    if (gMyInfo.LayoutPresets[++CurrentLayout].Name[0] == 0) {
        CurrentLayout = 0;
    }

    //  printd(LogInfo, "IncrementLayoutMode %d %s", CurrentLayout,
    //      gMyInfo.LayoutPresets[CurrentLayout].Name);
    //  gtk_widget_override_font(CurrentLayoutWid,
    //      pango_font_description_from_string("Sans Bold 16"));
    //  gtk_label_set_text(CurrentLayoutWid, LayoutPresets[CurrentLayout].Name);
    SetUpMainButtons(&gMyInfo.MyPatchInfo);

    printd(LogDebug, "Increment Mode %d\n", CurrentLayout);
}

/*--------------------------------------------
 * Function:        LayoutSwitchPatch
 *
 * Description:     Based on the current mode do a different.
 *
 *---------------------------------------------*/
tPatchIndex LayoutSwitchPatch(tPatchIndex MidiIn, char DoAction) {
    tPatchIndex Preset;
    tPatchIndex RetVal;
    tPatchIndex FinalRetVal;
    int Loop;

    Preset = GetModePreset(MidiIn);
    printd(LogInfo, "In LayoutSwitchPatch Mid In %d %d %d\n", MidiIn, Preset,
           &gMyInfo.MyPatchInfo[(char)Preset]);

    if (MidiIn >= Max_Patches) {
        printd(LogError, "MidiIn %d >= %d\n", MidiIn, Max_Patches);
        return (Patch_Not_Found);
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
        return (Patch_Not_Found);
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

            /* Find which button it is. Check to find the right 
            preset number. If it is then go to WebPresets.
             */
            if (gMyInfo.MyPatchInfo[RetVal].Patch == (Loop + 1))
                if (gMyInfo.WebPresets.thePreset[Loop] != -1) {
                    FinalRetVal = gMyInfo.WebPresets.thePreset[Loop];
                }
        }
    }

    // Make sure we don't overwrite anything.
    if (FinalRetVal > Max_Patches) {
        FinalRetVal = RetVal;
    }

    /* If we should act on it then call for a change.
     */
    if (DoAction) {
        if (FinalRetVal >= 0 && FinalRetVal < Max_Patches) {
            DoPatch(&gMyInfo.MyPatchInfo[(int)FinalRetVal]);
        }
    }

    // ejk event_ptr->data.control.value > 127 || event_ptr->data.control.value
    // < 0 ? "???": gm_get_instrument_name(event_ptr->data.control.value));
    return (FinalRetVal);
}

/*--------------------------------------------
 * Function:        GuitarMidiPreset
 *
 * Description:     Wait for a preset from the Midi Guitar.
 *
 *---------------------------------------------*/
int GuitarMidiPreset(char Wait) {

    printd(LogTest, "GuitarMidiPreset Start Wait=%d\n", Wait);

    /* Switch to the patch tab so we can see
    what we want to select.
    */
    SwitchToTab(0);

    // MyOSCJackVol(NewValue);
    MyOSCJackMute(1, typeOSCVolMaster);
    WaitingforMidi = 1;

    /* If we are wait for a specific release queue.
     */
    if (Wait) {
        WaitingforMidiHold = 1;
    }

    return (0);
}

/*--------------------------------------------
 * Function:        GuitarMidiPresetComplete
 *
 * Description:     Wait for a preset from the Midi Guitar.
 *
 *---------------------------------------------*/
int GuitarMidiPresetComplete(tPatchIndex MidiNote) {
    tPatchIndex PatchChange;

    printd(LogTest, "GuitarMidiPresetComplete Start %d\n", MidiNote);

    if (MidiNote < Max_Patches) {
        PatchChange = MidiNote - gMyInfo.MidiBaseNote;

        if (PatchChange >= 0 && PatchChange < Max_Patches) {
            LayoutSwitchPatch(PatchChange, TRUE);
        }

        //      DoPatch(&gMyInfo.MyPatchInfo[PatchChange]);
    }

    printd(LogTest, "GuitarMidiPresetComplete end Patch %d %d %d\n",
           gMyInfo.MidiBaseNote, PreviousTab, PatchChange);

    /* Switch back to what ever we were looking at.
     */
    SwitchToTab(PreviousTab);

    if (WaitingforMidiHold == 0) {
        WaitingforMidi = 0;
        printd(LogTest, "WaitingforMidiHold \n");
        /* In case the note rings.
         */
        RemoveMuteCount = MuteCountDelay;

    } else if (MidiNote >= Max_Patches) {
        printd(LogTest, "WaitingforMidiHold >Max_Patches \n");
        WaitingforMidi = 0;
        MyOSCJackMute(0, 0xff);
        WaitingforMidiHold = 0;
    }

    return (0);
}

/*--------------------------------------------
 * Function:        FindString
 *
 * Description: Find the offset into a list
 * of strings.
 *---------------------------------------------*/
int FindString(int StringList, char *String) {
    int Loop;

    printd(LogDebug, "FindString %d %s\n", StringList, String);

    // Look for Patch Names
    if (StringList == fsPatchNames) {
        for (Loop = 0; Loop < Max_Patches; Loop++) {
            if (!strcmp(gMyInfo.MyPatchInfo[Loop].Name, String)) {
                return (Loop);
            }
        }
        return (-1);
    }

    // Look for Port Names
    if (StringList == fsPortNames) {
        for (Loop = 0; Loop < MaxOutPorts; Loop++) {
            gMyInfo.OutPortName[Loop];
        }
    }

    // Look for external App Names
    if (StringList == fsAppNames) {
        for (Loop = 0; Loop < MaxApps; Loop++) {
            gMyInfo.Apps[Loop].Name;
        }
    }
    return (0);
}

/*--------------------------------------------
 * Function:        InitHistoryFile
 *
 * Description:     When the sliders are changed on the main screen.
 *
 *---------------------------------------------*/
int InitHistoryFile(void) {

    FileHistory = fopen(GetResourceDir(HistoryFileName, FileLocUser), "a+");
    //  FileHistory = fopen(GetResourceDir(HistoryFileName, FileLocConfig) ,
    //  "a+");
    printd(LogDebug, "File History %x\n", FileHistory);

    if (FileHistory) {
        fseek(FileHistory, 0, SEEK_END);
        printd(LogDebug, FileHistory, "*****************\n");
        fflush(FileHistory);
    }
}

/*-----------------------------------------------
 * Function: WriteToHistory
 *
 * Description:  Write what I am doing to history.
 * My memory sucks.
 *
 *----------------------------------------------*/
int WriteToHistory(char *str) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if (FileHistory) {
        fprintf(FileHistory, "%d-%d-%d %d:%d:%d->", tm.tm_year + 1900,
                tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        fprintf(FileHistory, "%s\n", str);
        fflush(FileHistory);
    }
}

/*-----------------------------------------------
 * Function: CloseHistoryFile
 *
 * Description:  Close the history file.
 *
 *----------------------------------------------*/
int CloseHistoryFile(void) {
    fflush(FileHistory);
    fclose(FileHistory);
}

/*--------------------------------------------
 * Function:        SetExpressionControl
 *
 * Description:     Control multiple volumes/expression
 *
 *---------------------------------------------*/
int SetExpressionControl(int Controller, int Value) {
    int ReturnVal = 0;
    int LogValue = 0;
    float NormValue = 0.0;

    /* Check values
     */
    if (Value < 3) {
        Value = 0;
    } else if (Value > 124) {
        Value = 127;
    }

    // Convert to Audio (log)-ish
    //    LogValue = (int)(pow(Value, 0.61) * 7.4) - 10;
    //    LogValue = (int)(24*log2(Value));


//    LogValue = (int)(pow(Value, 0.675) * 4.8) - 1;
    NormValue = (float)Value/127;
    LogValue = (NormValue / (1 + (1 - NormValue) * 2) * 127);

    if (LogValue < 3) {
        LogValue = 0;
    } else if (LogValue > 124) {
        LogValue = 127;
    }

    //    LogValue=Value;
    printd(LogInfo, "SetExpressionControl %d %d %d\n", Controller, Value,
           LogValue);

    switch (Controller) {

    // Guitar volume, currently goes to mixed
    case ecGuitarVolume:
        // Guitar Volume
        ReturnVal = gMyInfo.AnalogVolume;
        gMyInfo.SliderGUINumber = Slider1;
        gMyInfo.SliderGUIUpdate = GuiUpdateCount;
        gMyInfo.SliderGUIValue = LogValue;
        gMyInfo.AnalogVolume = LogValue;
        MyOSCJackVol(LogValue, typeOSCVolGuitarL);
        MyOSCJackVol(LogValue, typeOSCVolGuitarR);
        break;

    // Master midi volume, both metronome and instrument
    case ecMidiVolume:
        // Midi Volume
        ReturnVal = gMyInfo.MidiVolume;
        gMyInfo.SliderGUINumber = Slider2;
        gMyInfo.SliderGUIUpdate = GuiUpdateCount;
        gMyInfo.SliderGUIValue = Value;
        //      SetVolume2(Value);
        MyOSCJackVol(LogValue, typeOSCVolMidi);
        break;

    // everything volume, currently goes to mixed
    case ecMasterVolume:
        // Master Volume (OSC)
        ReturnVal = gMyInfo.V3Volume;
        gMyInfo.SliderGUINumber = Slider3;
        gMyInfo.SliderGUIValue = LogValue;
        gMyInfo.SliderGUIUpdate = GuiUpdateCount;
        //      SetVolume3(Value/1.27);
        MyOSCJackVol(LogValue, typeOSCVolMaster);
        break;

    // Change the tempo for metronome and looper sync
    case ecTempChange:
        // Tempo
        ReturnVal = gMyInfo.Tempo;
        gMyInfo.Tempo = Value + (MinTempoValue / 1.2);
        break;

    // Move the slider on the player page when changing volume.
    case ecMP3Volume:
        // MPS volume
        ReturnVal = gMyInfo.SetMP3PlayVolBool;
        gMyInfo.SetMP3PlayVolBool = LogValue;
        MyOSCJackVol(LogValue, typeOSCVolMP3);
        break;

    // Unused
    case ecExpress6:
        ReturnVal = gMyInfo.V6Volume;
        gMyInfo.V6Volume = Value;
        break;

    // The input pedal controls whatever is set by the software.
    case ecPedalControl:
        // Pedal Volume
        ReturnVal = gMyInfo.V4Volume;
        gMyInfo.SliderGUINumber = Slider4;
        gMyInfo.SliderGUIUpdate = GuiUpdateCount;
        gMyInfo.SliderGUIValue = Value;
        //      SetVolume4(Value);
        break;

    // Midi cut off to avoid ghost notes.
    case ecMidiThreshold:
        ReturnVal = gMyInfo.MidiThresholdLevel;
        gMyInfo.MidiThresholdLevel = Value;
        break;

    // Looper volume
    case ecLooperVolume:
        //      ReturnVal = gMyInfo.MidiThresholdLevel;
        MyOSCJackVol(Value, typeOSCVolLooper);
        break;

    // Not used
    case ecMidiAnaMix:
        ReturnVal = gMyInfo.MidiAnalMixLevel;
        gMyInfo.MidiAnalMixLevel = Value;
        ReturnVal = gMyInfo.AnalogVolume;
        ReturnVal = gMyInfo.MidiVolume;

        if (Value > 64) {
        } else {
        }

        //      SetExpressionControl
        break;

    case ecDistorion:
        SendMidi(SND_SEQ_EVENT_CONTROLLER, GuitarixPort, 1, 2, Value);
        break;

    case ecChorus:
        SendMidi(SND_SEQ_EVENT_CONTROLLER, GuitarixPort, 1, 12, Value);
        break;

    case ecWAH:
        SendMidi(SND_SEQ_EVENT_CONTROLLER, GuitarixPort, 1, 11, Value);
        break;
    }

    return (ReturnVal);
}

/*--------------------------------------------
 * Function:        GetExpressionControl
 *
 * Description:     Control multiple volumes/expression
 *
 *---------------------------------------------*/
int GetExpressionControl(int Controller) {
    int ReturnVal = 0;

    switch (Controller) {
    case ecGuitarVolume:
        ReturnVal = gMyInfo.AnalogVolume;
        break;

    case ecMidiVolume:
        ReturnVal = gMyInfo.MidiVolume;
        break;

    case ecMasterVolume:
        ReturnVal = gMyInfo.V3Volume;
        break;

    case ecTempChange:
        ReturnVal = gMyInfo.Tempo;
        break;

    case ecMP3Volume:
        ReturnVal = gMyInfo.SetMP3PlayVolBool;
        break;

    case ecExpress6:
        ReturnVal = gMyInfo.V6Volume;
        break;

    case ecPedalControl:
        ReturnVal = gMyInfo.V4Volume;
        break;

    case ecMidiThreshold:
        ReturnVal = gMyInfo.MidiThresholdLevel;
        break;
    }

    return (ReturnVal);
}

/*--------------------------------------------
 * Function:        LEDControl
 *
 * Description:
 *
 *---------------------------------------------*/
void LEDControl(char Beat, char State) {

    // DAW Lights Off
    SetDAWLed(dLEDBeat1, 0);
    SetDAWLed(dLEDBeat2, 0);
    SetDAWLed(dLEDBeat3, 0);
    SetDAWLed(dLEDBeat4, 0);

    switch (Beat) {
    case dLEDCnt_Beat1:

        // DAW Lights On
        SetDAWLed(dLEDBeat1, 1);

        // LPD8
        SendMidi(SND_SEQ_EVENT_CONTROLLER, DAWPort, 1,
                 MIDI_CTL_GENERAL_PURPOSE5, (int)1);

        // Click Track
        if (gMyInfo.MetronomeOn)
            SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort, DrumMidiChannel, 00,
                     (int)gMyInfo.Drum1);

        // Floor Pedal
        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort, DrumMidiChannel, 04,
                 (int)PedalLED4On);

        break;

    case dLEDCnt_Beat2:

        // DAW Lights On
        SetDAWLed(dLEDBeat2, 1);

        // LPD8
        SendMidi(SND_SEQ_EVENT_CONTROLLER, DAWPort, 1,
                 MIDI_CTL_GENERAL_PURPOSE6, (int)1);

        // Click Track
        if (gMyInfo.MetronomeOn)
            SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort, DrumMidiChannel, 00,
                     (int)gMyInfo.DrumRest);

        // Floor Pedal
        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort, DrumMidiChannel, 04,
                 (int)PedalLED3On);
        break;

    case dLEDCnt_Beat3:
        SetDAWLed(dLEDBeat3, 1);

        // LPD8
        SendMidi(SND_SEQ_EVENT_CONTROLLER, DAWPort, 1,
                 MIDI_CTL_GENERAL_PURPOSE7, (int)1);

        // Click Track
        if (gMyInfo.MetronomeOn)
            SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort, DrumMidiChannel, 00,
                     (int)gMyInfo.DrumRest);

        // Floor Pedal
        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort, DrumMidiChannel, 04,
                 (int)PedalLED3On);
        break;

    case dLEDCnt_Beat4:
        SetDAWLed(dLEDBeat4, 1);

        // LPD8
        SendMidi(SND_SEQ_EVENT_CONTROLLER, DAWPort, 1,
                 MIDI_CTL_GENERAL_PURPOSE8, (int)1);

        // Click Track
        if (gMyInfo.MetronomeOn)
            SendMidi(SND_SEQ_EVENT_NOTEON, ClickPort, DrumMidiChannel, 00,
                     (int)gMyInfo.DrumRest);

        // Floor Pedal
        SendMidi(SND_SEQ_EVENT_CONTROLLER, PedalPort, DrumMidiChannel, 04,
                 (int)PedalLED3On);
        break;

    case dLEDCnt_AllOff:
        // DAW Lights Off
        SendMidi(dLEDMIDI, DAWPort, dLEDChan, (int)dLEDBeat6, 0);
        SendMidi(dLEDMIDI, DAWPort, dLEDChan, (int)dLEDBeat7, 0);
        SendMidi(dLEDMIDI, DAWPort, dLEDChan, (int)dLEDBeat8, 0);
        break;

    case dLEDCnt_CountD1:
        // DAW Lights On
        SendMidi(dLEDMIDIO, DAWPort, dLEDChan, (int)dLEDBeat6, 127);
        break;

    case dLEDCnt_CountD2:
        // DAW Lights On
        SendMidi(dLEDMIDIO, DAWPort, dLEDChan, (int)dLEDBeat7, 127);
        break;

    case dLEDCnt_CountD3:
        // DAW Lights On
        SendMidi(dLEDMIDIO, DAWPort, dLEDChan, (int)dLEDBeat8, 127);
        break;
    }
}

#if 0
/*--------------------------------------------
 * Function:        show_motion_notify_cb
 *
 * Description: CallBack function for motion.
 *---------------------------------------------*/
gint button_press_notify_cb (GtkWidget *entries, GdkEventKey *event,
                             GtkWidget *widget) {

#if 0

    switch (event->keyval) {
    case GDK_p:
        printd (LogInfo, "key pressed: %s\n", "p");
        break;

    case GDK_s:
        if (MyState & GDK_SHIFT_MASK) {
            printd (LogInfo, "key pressed: %s\n", "shift + s");

        }
        else
            if (MyState & GDK_CONTROL_MASK) {
                printd (LogInfo, "key pressed: %s\n", "ctrl + s");

            }
            else {
                printd (LogInfo, "key pressed: %s\n", "s");
            }

        break;

    case GDK_m:
        if (MyState & GDK_SHIFT_MASK) {
            printd (LogInfo, "key pressed: %s\n", "shift + m");

        }
        else
            if (MyState & GDK_CONTROL_MASK) {
                printd (LogInfo, "key pressed: %s\n", "ctrl + m");

            }
            else {
                printd (LogInfo, "key pressed: %s\n", "m");
            }

        break;

    default:
        return FALSE;
    }

#endif
    return (FALSE);
}

/*--------------------------------------------
 * Function:        CreateHTMLGuide
 *
 * Description:     Writes and HTML file for the presets.
 *
 *---------------------------------------------*/
void CreateHTMLGuide (LiveMusicInfo *myInfo) {
    FILE *MyFile;
    char FileName[255];
    int Loop;
    int Loop1;

    strcpy (FileName, myInfo->BasePath);
    strcat (FileName, "/aaFootSwitch.html");

    MyFile = fopen (FileName, "w");

    if (!MyFile) {
        return;
    }

    fprintf (MyFile,
             "<html><head>\n<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\n<title>FootSwitchController\n</title>\n</head><body><h3>Foot Switch Controller&nbsp; - Elias<br> </h3>");
    fprintf (MyFile,
             "<table style=\"text-align: left; width: 849px; height: 83px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n<tbody>\n<tr>\n");
    Loop1 = 0;

    for (Loop = 0; Loop < Max_Patches; Loop++) {
        printd (LogInfo, "Create HTML %d %d\n", Loop, Loop1);

        if (Loop1 == 0) {
            fprintf (MyFile,
                     "<td style=\"vertical-align: top; font-weight: bold; width: 75px;\"><big><big><big>%d</big></big></big><br>\n</td>\n",
                     Loop);
            fprintf (MyFile,
                     "<td style=\"vertical-align: top; font-weight: bold; width: 75px;\">\n");
            fprintf (MyFile,
                     "<table style=\"text-align: left; width: 716px; height: 32px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n<tbody>\n<tr>\n");
        }

        fprintf (MyFile,
                 "<td style=\"vertical-align: top; font-weight: bold; width: 75px;\">%d - %s<br>\n</td>\n",
                 Loop, myInfo->MyPatchInfo[Loop].Name);

        if (++Loop1 == 5) {
            printd (LogInfo, "Loop %d %d\n", Loop, Loop1);
            fprintf (MyFile, "</td>\n</tr>\n</tbody>\n</table>\n</td>\n</tr>\n");
            Loop1 = 0;

            if ( ( (Loop + 1) % 10) == 0)
                fprintf (MyFile,
                         "</tbody>\n</table>\n<table style=\"text-align: left; width: 849px; height: 83px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\n<tbody>\n<tr>");
        }

        //      myInfo->OutPortName[Loop]
    }

    fclose (MyFile);
}

/*--------------------------------------------
 * Function:            create_pixbuf
 *
 * Description:     Load an icon file for the application
 *
 *---------------------------------------------*/
GdkPixbuf *create_pixbuf (const gchar *filename) {
    GdkPixbuf *pixbuf;
    GError *error = NULL;
#if 0
    pixbuf = gdk_pixbuf_new_from_file (filename, &error);

    if (!pixbuf) {
        fprintf (stderr, "%s\n", error->message);
    }

    g_error_free (error);
}
return pixbuf;
#endif
return (NULL);
}


/*--------------------------------------------
 * Function:        CheckConnectionData
 *
 * Description:     <Description/Comments>
 *
 *---------------------------------------------*/
void CheckConnectionData (LiveMusicInfo *myInfo) {

}
/*--------------------------------------------
 * Function:            apply_font_to_widget
 *
 * Description:     <Description/Comments>
 *
 *---------------------------------------------*/
void apply_font_to_widget (GtkWidget *widget, gchar *fload) {
    PangoFontDescription *pfd;
    pfd = pango_font_description_from_string (fload);

    if (GTK_IS_LABEL (widget)) {
        gtk_widget_override_font (widget, pfd);
    }
    else
        gtk_widget_override_fo (GTK_WIDGET (gtk_bin_get_child (GTK_BIN (widget))),
                                pfd);

    pango_font_description_free (pfd);
}


/*--------------------------------------------
 * Function:        on_hscale1_value_changed
 *
 * Description:     When the sliders are changed on the main screen.
 *
 *---------------------------------------------*/
void on_hscale1_value_changed (GtkWidget *widget, gpointer user_data) {
    gdouble val;

    val = gtk_range_get_value (GTK_RANGE (widget));
    /* print to screen */
    printd (LogInfo, "Range value: %d\n", (guint) val);
    SendMidi (SND_SEQ_EVENT_CONTROLLER, 0, DefaultMidiChannel, 07, (int) val);
}
#endif

#if 0
GtkCssProvider *provider = gtk_css_provider_new();
//  gtk_css_provider_load_from_data(provider, ".frame{border:10px solid red;}", -1, NULL);
gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (provider),
                                 "* {border-color: #CC0000}",
                                 -1, NULL);
gtk_css_provider_load_from_data (provider,
                                 "}\n", -1, NULL);
FaderControl.png
.scale.slider,
.scale.slider.horizontal {
background - image:
    -gtk - scaled (url ("assets/scale-slider-horz-dark.png"), url ("assets/scale-slider-horz-dark@2.png"));
}

/*---------------- CSS ----------------------------------------------------------------------------*/
provider = gtk_css_provider_new();
display = gdk_display_get_default();
screen = gdk_display_get_default_screen (display);
gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

gsize bytes_written, bytes_read;

const gchar *home = "YourPathHere";  // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

GError *error = 0;

gtk_css_provider_load_from_path (provider,
                                 g_filename_to_utf8 (home, strlen (home), &bytes_read, &bytes_written, &error),
                                 NULL);
g_object_unref (provider);
/*-------------------------------------------------------------------------------------------------*/

gtk_style_context_add_provider_for_screen (gdk_screen_get_default(), provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#endif
