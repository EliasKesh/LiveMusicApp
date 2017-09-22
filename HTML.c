#include <gtk/gtk.h>

//#define WebKit2 1
#ifdef WebKit2
#include <webkit2/webkit2.h>
#else
#include <webkit/webkit.h>
#include <webkit/webkitwebview.h>
#endif

#include "GTKMidiUI.h"
#include "MyWidgets.h"
#define _GNU_SOURCE
#include <string.h>

int Search_in_File(const char *fname, WebLoadPresets *thePresets);
int ScalePage(void);
tPatchIndex AssignPreset(int PresetNum, char *String);
void SetPatchTitles(theImageButtons *MyButton, char *Text, int Value);
gboolean Play_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static WebKitWebView* web_view;
theImageButtons PresetButtons[MaxPresetButtons];
theImageButtons SaveWebButton;
theImageButtons ScaleButton;
theImageButtons ForwardButton;
theImageButtons BackButton;
theImageButtons SetListButton;
theImageButtons PlayPauseButton;

GtkWidget *scrolled_window;
// char		SetListFileName[FileNameMaxLength];
char SetListFileName[250];
FILE *SetListFile;
#define ParseValue "Preset"
#define MAXLINE 250

/*--------------------------------------------------------------------
 * Function:		on_Back_clicked
 *
 * Description:	Web browser back button
 *
 *---------------------------------------------------------------------*/
void on_Back_clicked(GtkButton * button, gpointer user_data) {
//		webkit_web_view_set_editable( web_view, false);
	gtk_image_set_from_pixbuf(GTK_IMAGE(BackButton.Image),
		BackButton.ButtonDownImage);
	webkit_web_view_go_back(web_view);
	g_print("Back:\n");
}

/*--------------------------------------------------------------------
 * Function:		on_Forward_clicked
 *
 * Description:	Web browser forward button
 *
 *---------------------------------------------------------------------*/
void on_Forward_clicked(GtkButton * button, gpointer user_data) {

	gtk_image_set_from_pixbuf(GTK_IMAGE(ForwardButton.Image),
		ForwardButton.ButtonDownImage);
	webkit_web_view_go_forward(web_view);
	g_print("Forward:\n");
}

/*--------------------------------------------------------------------
 * Function:		ScrollDown
 *
 * Description:	Scroll the music
 *
 *---------------------------------------------------------------------*/
int ScrollDown(int Amount) {
	GtkAdjustment *Adjust;
	gint Value, UpperV, VIncrement;

	Adjust = gtk_scrolled_window_get_vadjustment(scrolled_window);
	UpperV = gtk_adjustment_get_upper(Adjust);
	VIncrement = gtk_adjustment_get_page_increment(Adjust);
	Value = gtk_adjustment_get_value(Adjust);
	printd(LogInfo, "In ScrollDown %d  %d %d\n", UpperV, VIncrement, Value);
	if (((VIncrement + Value) + (UpperV / 10)) >= UpperV) {
		gtk_adjustment_set_value(Adjust, 0);
		printd(LogInfo, "ScrollDown Rolling Over\n");
	}
	else {
		gtk_adjustment_set_value(Adjust, VIncrement + Value);
	}
return(0);
}
/*--------------------------------------------------------------------
 * Function:		Scale the page to fit with scroll bars
 *
 * Description:	Web browser forward button
 *
 *---------------------------------------------------------------------*/
int ScalePage(void) {
	gint UpperH, UpperV;
	GtkAdjustment *Adjust;
	gint Horiz, Vert;
	gfloat ScaleH, ScaleV;

#ifndef WebKit2 // FIX THIS
	WebKitViewportAttributes* attributes;
	attributes = webkit_web_view_get_viewport_attributes(web_view);
#endif

	Adjust = gtk_scrolled_window_get_hadjustment(scrolled_window);
	UpperH = gtk_adjustment_get_upper(Adjust);

	Adjust = gtk_scrolled_window_get_vadjustment(scrolled_window);
	UpperV = gtk_adjustment_get_upper(Adjust);

#ifndef WebKit2 // FIX THIS
	g_object_get(G_OBJECT(attributes), "available-width", &Horiz, NULL);
	g_object_get(G_OBJECT(attributes), "available-height", &Vert, NULL);
#endif
	if (UpperH != 0)
		ScaleH = ((gfloat) Horiz) / (gfloat) UpperH;
	else
		ScaleH = 1.0;

	if (UpperV != 0)
		ScaleV = ((gfloat) Vert) / (gfloat) UpperV;
	else
		ScaleV = 1.0;

	g_print("Button3  %d %d %f\n", UpperH, Horiz, ScaleH);
	g_print("Button3  %d %d %f\n", UpperV, Vert, ScaleV);
	webkit_web_view_set_zoom_level(web_view, MIN(ScaleV, ScaleH) + 0.001);
	return (0);
}

/*--------------------------------------------------------------------
 * Function:		on_scalebutton_clicked
 *
 * Description:	Scale the graphics to fit the page
 *
 *---------------------------------------------------------------------*/
void on_scalebutton_clicked(GtkWidget *widget, gpointer data) {
	const gchar *CurrentURI;

//	 webkit_web_view_reload(web_view);
	gtk_image_set_from_pixbuf(GTK_IMAGE(ScaleButton.Image),
		ScaleButton.ButtonDownImage);

	CurrentURI = webkit_web_view_get_uri(web_view);
	g_print("Current %s\n", CurrentURI);
	ScalePage();
}

/*--------------------------------------------------------------------
 * Function:		Patch Selected
 *
 * Description:	The users patch 1 was selected.
 *
 *---------------------------------------------------------------------*/
gboolean on_patch_clicked(GtkWidget *widget, GdkEvent *event, gpointer user_data) {

	tPatchIndex Preset;
	int CPatch;

	CPatch = (int) user_data;

	if (CPatch >= 0 && CPatch < MaxPresetButtons) {
		Preset = gMyInfo.WebPresets.thePreset[CPatch];
	}
	else
		return (false);

	printd(LogInfo, "In Button Preset %d %d\n", CPatch, Preset);
	printd(LogInfo, "In Button Preset %s\n", gMyInfo.MyPatchInfo[Preset].Name);

	if (Preset != -1)
		DoPatch(&gMyInfo.MyPatchInfo[Preset]);

	gtk_image_set_from_pixbuf(GTK_IMAGE(PresetButtons[CPatch].Image),
		PresetButtons[CPatch].ButtonDownImage);

	return (1);
}

gboolean on_patch__release_handler(GtkWidget *widget,
	GdkEvent *event,
	gpointer user_data)
{
	theImageButtons *theButton;
	theButton = (theImageButtons *) user_data;
	//	PatchIndex = LayoutSwitchPatch(user_data, true);

	gtk_image_set_from_pixbuf(GTK_IMAGE(PresetButtons[(int )user_data].Image),
		PresetButtons[(int) user_data].ButtonUpImage);
	return TRUE; /* stop event propagation */
}
/*--------------------------------------------------------------------
 * Function:		on_SaveWeb_clicked
 *
 * Description:	Save the changes to the file
 *
 *---------------------------------------------------------------------*/
void on_SaveWeb_clicked(GtkWidget *widget, gpointer data) {
	const gchar *CurrentURI;
	const char *Buffer;
	GString* theBuffer;
#ifndef WebKit2
	WebKitWebFrame *theFrame;
#endif
	FILE *fp;

	/*
	 * Draw the button
	 */
	gtk_image_set_from_pixbuf(GTK_IMAGE(SaveWebButton.Image),
		SaveWebButton.ButtonDownImage);


	CurrentURI = webkit_web_view_get_uri(web_view);
	g_print("Save %s\n", CurrentURI);
#ifdef WebKit2
	Buffer = webkit_web_view_get_title(web_view);
#else
	webkit_web_view_execute_script(web_view,
		"document.title=document.documentElement.innerHTML;");
	theFrame = webkit_web_view_get_main_frame(web_view);
	Buffer = webkit_web_frame_get_title(theFrame);
#endif

	printd(LogInfo, "Len = %d\n %s\n", (int) strlen(Buffer), Buffer);
	/* Get passed the file://
	 */
	CurrentURI += 7;
	if ((fp = fopen(CurrentURI, "w")) == NULL) {
		g_print("File open Failed %s\n", CurrentURI);
		return;
	}

	fwrite(Buffer, strlen(Buffer), 1, fp);
	fclose(fp);
}

/*--------------------------------------------------------------------
 * Function:		PageLoaded
 *
 * Description:	Called after the page is loaded, we can then parse
 * 		for the user selections.
 *
 *---------------------------------------------------------------------*/
void PageLoaded(GtkWidget *widget, gpointer data) {
	const gchar *CurrentURI;
	char *Pointer;

	CurrentURI = webkit_web_view_get_uri(web_view);
	printd(LogInfo, "load_status_cb %s\n", CurrentURI);

	Pointer = strstr(CurrentURI, ".html");
//	printd(LogInfo, "Pointer %x\n",(unsigned int)Pointer);

	if (strstr(CurrentURI, ".html"))
		Search_in_File(CurrentURI, &gMyInfo.WebPresets);
	else if (strstr(CurrentURI, ".mp3")) {
		printd(LogInfo, "*** MP3 file.\n");
		return;
	}
	else {
		printd(LogInfo, "Not HTML File\n");
		return;
	}

#ifndef WebKit2
//	printd(LogInfo, "WebKit Set Edit %d\n",
	webkit_web_view_get_editable(web_view);
#endif
	webkit_web_view_set_zoom_level(web_view, 1);

}

#if 0
void load_finished_cb(WebKitWebView *web_view, WebKitWebFrame *web_frame,
	gpointer data) {
	printd(LogInfo, "Finished downloading %s\n",
		webkit_web_view_get_uri(web_view));
	ScalePage();
}

/* testing		*/

/* Handle the viewport-attributes-recompute-requested signal to override the device width */
static void viewport_recompute_cb(WebKitWebView* web_view,
	WebKitViewportAttributes* attributes, GtkWidget* window) {
	gint override_available_width = 700;

	g_object_get(G_OBJECT(attributes), "width", &override_available_width,
		NULL);
	g_object_set(G_OBJECT(attributes), "available-width",
		override_available_width, NULL);

//	g_object_get (G_OBJECT (attributes), "height", &override_available_width, NULL);
//	g_object_set (G_OBJECT(attributes), "available-height", override_available_width, NULL);
}

/* Handle the viewport-attributes-changed signal to recompute the initial scale factor */
static void viewport_changed_cb(WebKitWebView* web_view,
	WebKitViewportAttributes* attributes, gpointer data) {
	gfloat initialScale;
	g_object_get(G_OBJECT(attributes), "initial-scale-factor", &initialScale,
		NULL);
	webkit_web_view_set_zoom_level(web_view, initialScale);
}

/* Handle the notify::valid signal to initialize the zoom level */
static void viewport_valid_changed_cb(WebKitViewportAttributes* attributes,
	GParamSpec* pspec, WebKitWebView* web_view) {
	gboolean is_valid;
	g_object_get(attributes, "valid", &is_valid, NULL);
	if (!is_valid)
	webkit_web_view_set_zoom_level(web_view, 1.0);
}
#endif
#if 0
gboolean
DownloadRequestcb (WebKitWebView *web_view,
	WebKitDownload *download,
	gpointer user_data) {

	printd(LogInfo, "DownloadRequestcb %s\n", download);

}

gboolean
PolicyRequestCD (WebKitWebView* view, WebKitWebFrame* frame,
	WebKitNetworkRequest* request, const char* mime_type,
	WebKitWebPolicyDecision* decision, gpointer data)
{
//    char* type = (char*)data;

	printd(LogInfo, "PolicyRequestCD %s %s\n", mime_type, data);
#if 0

	if (g_str_equal(type, "pdf")) {
		g_assert_cmpstr(mime_type, ==, "application/pdf");
		g_assert(!webkit_web_view_can_show_mime_type(view, mime_type));
	} else
	if (g_str_equal(type, "html")) {
		g_assert_cmpstr(mime_type, ==, "text/html");
		g_assert(webkit_web_view_can_show_mime_type(view, mime_type));
	} else
	if (g_str_equal(type, "text")) {
		WebKitNetworkResponse* response = webkit_web_frame_get_network_response(frame);
		SoupMessage* message = webkit_network_response_get_message(response);
		char* disposition;

		g_assert(message);
		soup_message_headers_get_content_disposition(message->response_headers,
			&disposition, NULL);
		g_object_unref(response);

		g_assert_cmpstr(disposition, ==, "attachment");
		g_free(disposition);

		g_assert_cmpstr(mime_type, ==, "text/plain");
		g_assert(webkit_web_view_can_show_mime_type(view, mime_type));
	} else
#endif

	if (g_str_equal(mime_type, "audio/mpeg")) {
		g_assert(webkit_web_view_can_show_mime_type(view, mime_type));
		printd(LogInfo, "PolicyRequestCD MP3 Found %s \n", mime_type);
	}
	//   g_free(type);

	return FALSE;
}

#endif
int ishex(int x)
{
	return	(x >= '0' && x <= '9')	||
		(x >= 'a' && x <= 'f')	||
		(x >= 'A' && x <= 'F');
}

int decode(const char *s, char *dec)
{
	char *o;
	char *end = s + strlen(s);
	int c;

	for (o = dec; s <= end; o++) {
		c = *s++;
		if (c == '+') c = ' ';
		else if (c == '%' && (	!ishex(*s++)	||
					!ishex(*s++)	||
					!sscanf(s - 2, "%2x", &c)))
			return -1;

		if (dec) *o = c;
	}
#if 0
	size_t		Length;
	int		Loop;
	Length = o - dec;
	o = dec;

	for (Loop = 0; Loop < Length; Loop++)
		if ( dec[Loop] == 0x20)
			dec[Loop] = 255;
#endif

	return o - dec;
}

#ifndef WebKit2

gboolean NavigationPolicy(WebKitWebView *web_view,
	WebKitWebFrame *frame,
	WebKitNetworkRequest *request,
	WebKitWebNavigationAction *navigation_action,
	WebKitWebPolicyDecision *policy_decision,
	gpointer user_data) {
	char *theOrgURI;
	char theURI[250];
	char string[150];
	int Loop;

#ifdef WebKit2
	theOrgURI = webkit_web_view_get_uri(web_view);
#else
	theOrgURI = webkit_web_navigation_action_get_original_uri(navigation_action);
#endif
	//	uri=networkRequest.get_uri()
	decode(theOrgURI, theURI);
	printd(LogInfo, "NavigationPolicy %s \n", theURI);

	/* If we find an MP3 file then handle it ourselves and tell WebKit
	 * not to deal with it.
	 */
	if (strstr(theURI, ".mp3")) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		SetPlayerFile((theURI + 7));
#if 0
#ifndef WebKit2
		webkit_web_policy_decision_ignore(policy_decision);
#endif
		sprintf(SysCallString, "/usr/bin/smplayer %s &",
			theURI);
		printd(LogInfo, "***Before  systemcall %s\n", SysCallString);
		system(SysCallString);
		printd(LogInfo, "*** After systemcall %s\n", SysCallString);
#endif
		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}

	if (strstr(theURI, ".tg") || strstr(theURI, ".gp") || strstr(theURI, ".ptb") ) {
		/*
		 * Tell web kit not to o anything with it.
		 */
#ifndef WebKit2
		webkit_web_policy_decision_ignore(policy_decision);
#endif
		sprintf(string, "/home/Dropbox/LiveEffects/MyTuxGuitar \"%s\" &", theURI);
		system(string);
		printd(LogInfo, "*** systemcall %s\n", string);

		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}

	if (strstr(theURI, ".pdf")) {
	char *PageIndex;
	int	PageNumber;

		/*
		 * Tell web kit not to do anything with it.
		 */
#ifndef WebKit2
		webkit_web_policy_decision_ignore(policy_decision);
#endif
		PageIndex=strstr(theURI, "#page=");
		if (PageIndex) {
			*PageIndex=0;
			PageIndex += 6;
			PageNumber = atoi(PageIndex);
		}
		printf("PAGE %x %s %d\n", &PageIndex, PageIndex, PageNumber);

		sprintf(string, "/usr/bin/okular \"%s\" --page=%d &", theURI, PageNumber);
#if 0
		for (Loop = 0; Loop < strlen(theURI); Loop++) {
			String[Loop] = theURI[Loop];
		}

		for (Loop = 0; Loop < strlen(string); Loop++) {
			if (string[Loop] == '%') {
				string[Loop++] = 0x20;
				string[Loop++] = 0x20;
				string[Loop] = 0x20;
			}

			/*
			 * If it's a reference to page terminate the string
			 */
			if (string[Loop] == '#') {
				string[Loop] = '\"';
				string[Loop++] = 0x20;
				string[Loop] = 0x20;
			}
#endif
		system(string);
		printd(LogInfo, "*** systemcall %s\n", string);

		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}

	//   webkit_web_policy_decision_use(policy_decision);
	return (false);
}
#endif

/*--------------------------------------------------------------------
 * Function:		OpenSetListSong
 *
 * Description:	If we have a set list saved then open a song from it.
 *
 *---------------------------------------------------------------------*/
void OpenSetListSong(int SongNumber) {
	int Loop;
	char temp[MAXLINE];
	char Copy[MAXLINE];
	char *tokenizer;
	char *Found;
	int Count = 0;
	int SongCount = 0;

	printd(LogInfo, "Made it to OpenSetListSong\n");

	if (SongNumber < 1) {
		printd(LogInfo, "Invalid SongNumber %d\n", SongNumber);
		return;
	}

	SetListFile = fopen(&SetListFileName[7], "r");

	if (!SetListFile) {
		printd(LogInfo, "Can't open Setlist file  %s\n", SetListFileName);
		return;
	}

	/*
	 * Walk thru the file and find the HREF links.
	 */
	while (fgets(temp, MAXLINE - 1, SetListFile) != NULL && (++Count < 150)) {
		temp[MAXLINE] = 0;

		/*
		 * Look for the Links and .Count the number.
		 */
		Found = strstr(temp, "href=\"");
		if (Found != NULL) {
			SongCount++;
			Found += 6;
			tokenizer = strtok(Found, "\"");
			printd(LogInfo, "Parser found %d %s\n", SongCount, tokenizer);

			/*
			 * If the is the SongNumber HREF count then grab the file name.
			 */
			if (SongCount == SongNumber) {
				CurrentSetListSong = SongNumber;
				strcpy(Copy, SetListFileName);
//				dirname(SetListFileName);
				dirname(Copy);
				printd(LogInfo, "After  %s\n", Copy);
				strcat(Copy, "/");
				strcat(Copy, tokenizer);
				printd(LogInfo, "Final  %s\n", Copy);
				webkit_web_view_open(web_view, Copy);
				break;
			}
		}
	}

	// Close the file if still open.
	if (SetListFile) {
		fclose(SetListFile);
	}

}

/*--------------------------------------------------------------------
 * Function:		on_SaveWeb_clicked
 *
 * Description:	Save the changes to the file
 *
 *---------------------------------------------------------------------*/
void on_SetList_clicked(GtkWidget *widget, gpointer data) {
	const gchar *CurrentURI;
	char *BasePtr;
	int Length;

	CurrentURI = webkit_web_view_get_uri(web_view);
	strcpy(SetListFileName, CurrentURI);
	BasePtr = basename(SetListFileName);
	Length = strlen(BasePtr);
	BasePtr[Length - 5] = 0;
	MyImageButtonSetText(&SetListButton, BasePtr);
	strcpy(SetListFileName, CurrentURI);

}

/*--------------------------------------------------------------------
 * Function:		InitHTML
 *
 * Description:	Set up the WebKit environment.
 *
 *---------------------------------------------------------------------*/
void InitHTML(GtkBuilder *gxml) {
	char FileName[255];
	GtkWidget *Widget;
	int Loop;
	char Buffer[40];
	GtkWidget *EventBox;
	/* Load the buttons and set the callbacks for them.
	 */
	CurrentSetListSong = 0;

	for (Loop = 0; Loop < MaxPresetButtons - 1; Loop++) {
		sprintf(Buffer, "Patch%d", Loop + 1);
		EventBox = GTK_WIDGET(gtk_builder_get_object((GtkBuilder * )gxml, Buffer));
//		gtk_widget_get_usize(EventBox);

		MyImageButtonInit(&PresetButtons[Loop], EventBox, PatchButtonOnImage,
			PatchButtonOffImage);
		MyImageButtonSetText(&PresetButtons[Loop], Buffer);

		g_signal_connect(G_OBJECT(EventBox),
			"button-press-event",
			G_CALLBACK(on_patch_clicked),
			Loop);

		g_signal_connect(G_OBJECT(EventBox),
			"button-release-event",
			G_CALLBACK(on_patch__release_handler),
			Loop);

	}

	scrolled_window = GTK_WIDGET(
		gtk_builder_get_object(gxml, "scrolledwindow1"));

	//	gtk_widget_set_name (scrolled_window, "GtkLauncher");
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	/* Create a new webkit view to display our data.
	 */
	web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
	gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(web_view));

#ifndef WebKit2
	/* Make the view editable.
	 */
	webkit_web_view_set_editable(web_view, TRUE);
	/* We should scale everything.
	 */
	webkit_web_view_set_full_content_zoom(web_view, TRUE);
#endif

	EventBox = GTK_WIDGET(
		gtk_builder_get_object((GtkBuilder * )gxml, "BackButton"));
	MyImageButtonInit(&BackButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&BackButton, "Back");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
		G_CALLBACK(on_Back_clicked), &BackButton);
	g_signal_connect(G_OBJECT(EventBox), "button-release-event",
		G_CALLBACK(normal_release_handler), &BackButton);

	EventBox = GTK_WIDGET(
		gtk_builder_get_object(gxml, "ForwardButton"));
	MyImageButtonInit(&ForwardButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&ForwardButton, "Forward");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
		G_CALLBACK(on_Forward_clicked), &ForwardButton);
	g_signal_connect(G_OBJECT(EventBox), "button-release-event",
		G_CALLBACK(normal_release_handler), &ForwardButton);

	EventBox = GTK_WIDGET(
		gtk_builder_get_object(gxml, "ScaleButton"));
	MyImageButtonInit(&ScaleButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&ScaleButton, "Scale");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
		G_CALLBACK(on_scalebutton_clicked), &ScaleButton);
	g_signal_connect(G_OBJECT(EventBox), "button-release-event",
		G_CALLBACK(normal_release_handler), &ScaleButton);

	EventBox = GTK_WIDGET(
		gtk_builder_get_object(gxml, "SaveWeb"));
	MyImageButtonInit(&SaveWebButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&SaveWebButton, "Save");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
		G_CALLBACK(on_SaveWeb_clicked), &SaveWebButton);
	g_signal_connect(G_OBJECT(EventBox), "button-release-event",
		G_CALLBACK(normal_release_handler), &SaveWebButton);

	EventBox = GTK_WIDGET(
		gtk_builder_get_object(gxml, "SetList"));
	MyImageButtonInit(&SetListButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&SetListButton, "SetList");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
		G_CALLBACK(on_SetList_clicked), &SetListButton);
	g_signal_connect(G_OBJECT(EventBox), "button-release-event",
		G_CALLBACK(normal_release_handler), &SetListButton);

	EventBox = GTK_WIDGET(
		gtk_builder_get_object(gxml, "PlayPause"));
	MyImageButtonInit(&PlayPauseButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&PlayPauseButton, "Play");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
		G_CALLBACK(Play_click_handler), &PlayPauseButton);
//	g_signal_connect(G_OBJECT(EventBox), "button-release-event",
//		G_CALLBACK(normal_release_handler), &PlayPauseButton);

//    g_signal_connect(web_view, "load-finished", G_CALLBACK(load_finished_cb), NULL);

#if 0
	/* Connect to the viewport-attributes-changes signal */
	WebKitViewportAttributes* attributes = webkit_web_view_get_viewport_attributes (web_view);
	g_signal_connect (web_view, "viewport-attributes-recompute-requested", G_CALLBACK (viewport_recompute_cb), scrolled_window);
	g_signal_connect (web_view, "viewport-attributes-changed", G_CALLBACK (viewport_changed_cb), scrolled_window);
	g_signal_connect (attributes, "notify::valid", G_CALLBACK (viewport_valid_changed_cb), web_view);
#endif

	/* Register a callback that gets invoked each time that a page is finished downloading */

	g_signal_connect(web_view, "load-finished", G_CALLBACK(PageLoaded), NULL);
#if 0
	g_signal_connect(web_view, "download-requested", G_CALLBACK(DownloadRequestcb), NULL);
	g_signal_connect(web_view, "mime-type-policy-decision-requested", G_CALLBACK(PolicyRequestCD), NULL);
#endif
#ifndef WebKit2
	g_signal_connect(web_view, "navigation-policy-decision-requested",
		G_CALLBACK(NavigationPolicy), NULL);
#endif
	strncpy(FileName, gMyInfo.BasePath, 254);
	//   strcat(FileName, "/indexCharts.html");
#ifndef WebKit2
	webkit_web_view_open(web_view, FileName);
#else
	strncpy(FileName, "file:///home/Dropbox/FusionBlue/index.html", 254);
	webkit_web_view_load_uri (web_view, FileName);
#endif
#if 1

	WebKitWebSettings *settings = webkit_web_settings_new();
	g_object_set(G_OBJECT(settings), "auto-shrink-images", FALSE, NULL);
	g_object_set(G_OBJECT(settings), "enable-page-cache", FALSE, NULL);

	if (ScreenSize == 0) {
		g_object_set(G_OBJECT(settings), "default-font-size", 12, NULL);
		g_object_set(G_OBJECT(settings), "default-monospace-font-size", 12, NULL);
	}

	if (ScreenSize == 1) {
		g_object_set(G_OBJECT(settings), "default-font-size", 18, NULL);
		g_object_set(G_OBJECT(settings), "default-monospace-font-size", 18, NULL);
	}

	if (ScreenSize == 2) {
		g_object_set(G_OBJECT(settings), "default-font-size", 24, NULL);
		g_object_set(G_OBJECT(settings), "default-monospace-font-size", 24, NULL);
	}

	/* Apply the result */
	webkit_web_view_set_settings(WEBKIT_WEB_VIEW(web_view), settings);
#endif
//	create_Popup_view(web_view);

//	gtk_widget_grab_focus (GTK_WIDGET (web_view));
	gtk_widget_show_all(scrolled_window);

}
#define ContentTagLen	9
/*--------------------------------------------------------------------
 * Function:		Search_in_File
 *
 * Description:	Look thru the file and find presets.
 *
 *---------------------------------------------------------------------*/
int Search_in_File(const char *fname, WebLoadPresets *thePresets) {
	FILE *fp;
	char temp[MAXLINE];
	char Copy[MAXLINE];
	char *Found;
	char Value;
	char *tokenizer;
	char *String;
	int Count = 0;
	char DrumFile[FileNameMaxLength];
	char LoopFile[FileNameMaxLength];

	/* Get passed the file://
	 */
	fname += 7;
	if ((fp = fopen(fname, "r")) == NULL) {
		return (-1);
	}

	for (Count = 0; Count < MaxPresetButtons; Count++)
		thePresets->thePreset[Count] = -1;

	DrumFile[0] = 0;
	LoopFile[0] = 0;
	Count = 0;

//printd(LogInfo, "Have file %x %s\n", fp, fname);
	while (fgets(temp, MAXLINE - 1, fp) != NULL && (++Count < 150)) {
		temp[MAXLINE] = 0;

		strncpy(Copy, temp, MAXLINE);
		/* Set up Preset 1 button.
		 */
		Found = strstr(temp, "Preset1");
		if (Found != NULL) {
			/* skip 8  "preset1 "	*/
			Found += (8 + ContentTagLen);
			thePresets->thePreset[0] = AssignPreset(1, Found);
			strncpy(temp, Copy, MAXLINE);

		}

		/* Set up Preset 2 Button.
		 */
		Found = strstr(temp, "Preset2");
		if (Found != NULL) {
			Found +=  (8 + ContentTagLen);
			thePresets->thePreset[1] = AssignPreset(2, Found);
			strncpy(temp, Copy, MAXLINE);
		}

		Found = strstr(temp, "Preset3");
		if (Found != NULL) {
			Found +=  (8 + ContentTagLen);
			thePresets->thePreset[2] = AssignPreset(3, Found);
			strncpy(temp, Copy, MAXLINE);
		}
		Found = strstr(temp, "Preset4");
		if (Found != NULL) {
			Found +=  (8 + ContentTagLen);
			thePresets->thePreset[3] = AssignPreset(4, Found);
			strncpy(temp, Copy, MAXLINE);
		}
		Found = strstr(temp, "Preset5");
		if (Found != NULL) {
			Found +=  (8 + ContentTagLen);
			thePresets->thePreset[4] = AssignPreset(5, Found);
			strncpy(temp, Copy, MAXLINE);
		}
		Found = strstr(temp, "Preset6");
		if (Found != NULL) {
			Found += (8 + ContentTagLen);
			thePresets->thePreset[5] = AssignPreset(6, Found);
			strncpy(temp, Copy, MAXLINE);
		}
		/* Set the Tempo for this tune.
		 */
		Found = strstr(temp, "Tempo");
		if (Found != NULL) {
			Found += 7 + ContentTagLen;
			Value = atoi(Found);
			if (Value > 60 && Value < 160)
				thePresets->theTempo = Value;

			SetTempo(Value);
			printd(LogInfo, "Tempo %d\n", Value);
		}

		/* Set the Tempo for this tune.
		 */
		Found = strstr(temp, "LMA_Time");
		if (Found != NULL) {
			Found += 10 + ContentTagLen;
			Value = atoi(Found);
			/* Since we flash we need twice as many counts.
			 */
			gMyInfo.TempoMax = (2 * Value);
//			SetTempo(Value);
			printd(LogInfo, "Time %d\n", Value);
		}

		/* Set the current patch to this one.
		 */
		Found = strstr(temp, "SetNow");
		if (Found != NULL) {
			Found +=  (7 + ContentTagLen);
			printd(LogInfo, "SetNow %s\n", Found);
			AssignPreset(0, Found);
			strncpy(temp, Copy, MAXLINE);

		}

		/* Get the Loop file patch..
		 */
		Found = strstr(temp, "LoopFile");
		if (Found != NULL) {
			Found += 10 + ContentTagLen;
			String = Found;
			tokenizer = strtok(String, "\""); //break up by spaces
			printd(LogInfo, "LoopFile %s\n", tokenizer);
			strcpy(LoopFile, tokenizer);
		}

		/* Get the Drum File patch.
		 */
		Found = strstr(temp, "DrumFile");
		if (Found != NULL) {
			Found += 10 + ContentTagLen;
			String = Found;
			tokenizer = strtok(String, "\""); //break up by spaces
			printd(LogInfo, "DrumFile %s\n", tokenizer);
			strncpy(temp, Copy, MAXLINE);
			strcpy(DrumFile, tokenizer);

		}

		/* Set the current patch to this one.
		 */
		Found = strstr(temp, "IntroCount");
		if (Found != NULL) {
			Found += 12 + ContentTagLen;
			Value = atoi(Found);
			printd(LogInfo, "IntroCount %d\n", Value);
			strncpy(temp, Copy, MAXLINE);
			gMyInfo.CountInBeats = Value;

		}

		/* Set the current patch to this one.
		 */
		Found = strstr(temp, "LoopLength");
		if (Found != NULL) {
			Found += 12 + ContentTagLen;
			Value = atoi(Found);
			printd(LogInfo, "LoopLength %d\n", Value);
			strncpy(temp, Copy, MAXLINE);
			gMyInfo.LoopRecBeats = Value;
		}

	}
	//Close the file if still open.
	if (fp) {
		fclose(fp);
	}

	/*
	 * Check to see if we have requested a new file for the drum or the looper.
	 */
	if (DrumFile[0] || LoopFile[0]) {
		/*
		 * Make sure the the looper is the second argument even of the drum file is the same.
		 */
		if (DrumFile[0] == 0)
			DrumFile[0] = 'A';

		sprintf(Copy, "/home/Dropbox/LiveEffects/ReloadLivesFiles %s %s & ", DrumFile, LoopFile);
		printd(LogInfo, "Calling System with %s\n", Copy);
		system(Copy);
	}

	return (0);
}

/*--------------------------------------------------------------------
 * Function:		AssignPreset
 *
 * Description:	Parse for the value of the preset given.
 *
 *---------------------------------------------------------------------*/
tPatchIndex AssignPreset(int PresetNum, char *String) {
	int Value;
	char *tokenizer;

	printd(LogInfo, "AssignPreset %d %s\n\n", PresetNum, String);

	/* If we Start with a quote it's a name lookup.
	 */
	if (*String == '"') {
		String++;
		tokenizer = strtok(String, "\""); //break up by spaces
		printd(LogInfo, "Token1 %s\n", tokenizer);
		Value = FindString(fsPatchNames, tokenizer);

	} else {
		Value = atoi(String);
		printd(LogInfo, "***** Assign Value %s %d\n", String, Value);
		if (Value >= Max_Patches)
			Value = 0;
	}

	printd(LogInfo, "Preset %d %d %s\n", PresetNum, Value,
		gMyInfo.MyPatchInfo[Value].Name);

	if (Value < 0 || Value >= Max_Patches)
		return (Value);

	/* If it's a preset button or a set now.	*/
	switch (PresetNum) {
		case 0:
			DoPatch(&gMyInfo.MyPatchInfo[Value]);
			break;

		case 1:
			printd(LogInfo, "*********PresetNum Case 1 ");
			SetPatchTitles(&PresetButtons[0], gMyInfo.MyPatchInfo[Value].Name, 1);
			break;

		case 2:
			SetPatchTitles(&PresetButtons[1], gMyInfo.MyPatchInfo[Value].Name, 2);
			break;

		case 3:
			SetPatchTitles(&PresetButtons[2], gMyInfo.MyPatchInfo[Value].Name, 3);
			break;

		case 4:
			SetPatchTitles(&PresetButtons[3], gMyInfo.MyPatchInfo[Value].Name, 4);
			break;

		case 5:
			SetPatchTitles(&PresetButtons[4], gMyInfo.MyPatchInfo[Value].Name, 5);
			break;

		case 6:
			SetPatchTitles(&PresetButtons[5], gMyInfo.MyPatchInfo[Value].Name, 6);
			break;

		default:
			break;
	}

	return (Value);
}

/*--------------------------------------------------------------------
 * Function:		SetPatchTitles
 *
 * Description:	When we load patched, rename the buttons.
 *
 *---------------------------------------------------------------------*/
void SetPatchTitles(theImageButtons *MyButton, char *Text, int Value) {
	char String[PatchNameSize];
	int StringLen;

	printd(LogInfo, "SetPatchTitles %x %s\n", MyButton, Text);
	StringLen = strlen(Text);
	sprintf(String, "       %02d       \n%*s", Value, 7 + StringLen / 2,
		Text);
	MyImageButtonSetText(MyButton, String);
}
