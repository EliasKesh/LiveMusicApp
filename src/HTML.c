/*---------------------------------------------------------------------
|
|	File: 	HTML.c
|
|	Contains: Routines for reading charts
|		and webkit interactions.
|
|
|	Written By: 	Elias Keshishoglou
|
|	Copyright �: 	2014 Elias Keshishoglou all rights reserved.
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
|
|---------------------------------------------------------------------*/

#define HTML_c

/*
 * Place Includes Here
 */

#include <gtk/gtk.h>

#define WebKit2 1
#include <webkit2/webkit2.h>
#include "LiveMusicApp.h"
#include "MyWidgets.h"

#define _GNU_SOURCE
#include <string.h>
#include "Player.h"
#include <unistd.h>
#include "SooperOSC.h"
#include <sys/time.h>

/*
 * Place defines and Typedefs here
 */

#define ParseValue "Preset"
#define MAXLINE 250

/*
 * Place Local prototypes here
 */

int Search_in_File(const char *fname, WebLoadPresets *thePresets);
int ScalePage(void);
tPatchIndex AssignPreset(int PresetNum, char *String);
void SetPatchTitles(theImageButtons *MyButton, char *Text, int Value);
gboolean Play_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);

/*
 * Place Static variables here
 */

static WebKitWebView* web_view;
theImageButtons PresetButtons[MaxPresetButtons];
theImageButtons SaveWebButton;
theImageButtons ScaleButton;
theImageButtons ForwardButton;
theImageButtons BackButton;
theImageButtons SetListButton;
theImageButtons PlayPauseButton;
theImageButtons TapTempoButton;

GtkWidget *scrolled_window;
char		SetListFileName[FileNameMaxLength];
FILE *SetListFile;
char  BasePathName[250];

// ~/.config/mimeapps.list

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
	SetPatchTitles(&PresetButtons[0], "Preset 1", 1);
	SetPatchTitles(&PresetButtons[1], "Preset 2", 2);
	SetPatchTitles(&PresetButtons[2], "Preset 3", 3);
	SetPatchTitles(&PresetButtons[3], "Preset 4", 4);
	SetPatchTitles(&PresetButtons[4], "Preset 5", 5);
	SetPatchTitles(&PresetButtons[5], "Preset 6", 6);
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

	Adjust = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *)scrolled_window);
	UpperV = gtk_adjustment_get_upper(Adjust);
	VIncrement = gtk_adjustment_get_page_increment(Adjust);
	Value = gtk_adjustment_get_value(Adjust);
	printd(LogDebug, "In ScrollDown %d  %d %d\n", UpperV, VIncrement, Value);
	if (((VIncrement + Value) + (UpperV / 10)) >= UpperV) {
		gtk_adjustment_set_value(Adjust, 0);
		printd(LogDebug, "ScrollDown Rolling Over\n");
	} else {
		gtk_adjustment_set_value(Adjust, VIncrement + Value);
	}
	return (0);
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

	Adjust = gtk_scrolled_window_get_hadjustment((GtkScrolledWindow *)scrolled_window);
	UpperH = gtk_adjustment_get_upper(Adjust);

	Adjust = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *)scrolled_window);
	UpperV = gtk_adjustment_get_upper(Adjust);

	if (UpperH != 0)
		ScaleH = ((gfloat) Horiz) / (gfloat) UpperH;
	else
		ScaleH = 1.0;

	if (UpperV != 0)
		ScaleV = ((gfloat) Vert) / (gfloat) UpperV;
	else
		ScaleV = 1.0;

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
//	g_print("Current %s\n", CurrentURI);
	ScalePage();
}

unsigned int CurTapTempo;
struct timeval Time0;
/*--------------------------------------------------------------------
 * Function:		Tap Tempo
 *
 * Description:	Tap Tempo
 *
 *---------------------------------------------------------------------*/
gboolean on_TapTempo_clicked(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
	char String[20];
	struct timeval Time1;
	double elapsedTime;

	// Get the new time
	gettimeofday(&Time1, NULL);

	// Subtract the old time.
	elapsedTime = (Time1.tv_sec - Time0.tv_sec) * 1000.0;      // sec to ms
	elapsedTime += (Time1.tv_usec - Time0.tv_usec) / 1000.0;   // us to ms
#if 0
	printf("%ld:%ld->", Time0.tv_sec, Time0.tv_usec);
	printf("%ld:%ld->\n", Time1.tv_sec, Time1.tv_usec);
	printf("Elasped %lf\n", elapsedTime);
#endif

	// Calculate BPM
	if (elapsedTime != 0)
		CurTapTempo = (.35 * CurTapTempo) + (.65 * (60 * 1000) / elapsedTime);
	else
		CurTapTempo = 30;

	// Make this time the old time
	Time0 = Time1;

	// Update the button.
	sprintf(String, "Tap=%d", CurTapTempo);
	MyImageButtonSetText(&TapTempoButton, String);
//	webkit_web_view_set_editable(web_view, TRUE);

	/*
	 * Draw the button
	 */
	gtk_image_set_from_pixbuf(GTK_IMAGE(TapTempoButton.Image), TapTempoButton.ButtonDownImage);

}

/*--------------------------------------------------------------------
 * Function:		Patch Selected
 *
 * Description:	The users patch 1 was selected.
 *
 *---------------------------------------------------------------------*/
gboolean on_patch_clicked(GtkWidget *widget,
                          GdkEvent *event,
                          gpointer user_data) {

	tPatchIndex Preset;
	int CPatch;
	GdkEvent *theEvent;

	theEvent = gtk_get_current_event();

	CPatch = (int) user_data;
	printd(LogDebug, "In Button Preset %d\n", CPatch);

	/* Check to make sure the preset value is valid.	*/
	if (CPatch >= 0 && CPatch < MaxPresetButtons) {
		Preset = gMyInfo.WebPresets.thePreset[CPatch];
	} else
		return (false);

	if (theEvent->button.state & GDK_CONTROL_MASK) {
		printd(LogInfo, "We have Control Down\n");
/* FINISH, need to make the assignment in Patch_Popup_CB
*/

		ShowPatchListSelect(GTK_WINDOW(widget), CPatch);

	}

	/* Make sure the preset is at least in the correct range.
	*/
	if (Preset < 0  ||  Preset > Max_Patches)
		return (false);

	printd(LogDebug, "In Button Preset %d %d\n", CPatch, Preset);
	printd(LogDebug, "In Button Preset %s\n", gMyInfo.MyPatchInfo[Preset].Name);

	/* Execute the patch change.
	*/
	DoPatch(&gMyInfo.MyPatchInfo[Preset]);

	/* Redraw the button.	*/
	gtk_image_set_from_pixbuf(GTK_IMAGE(PresetButtons[CPatch].Image), PresetButtons[CPatch].ButtonDownImage);

	return (1);
}

gboolean on_patch__release_handler(GtkWidget *widget,
                                   GdkEvent *event,
                                   gpointer user_data) {
	theImageButtons *theButton;
	theButton = (theImageButtons *) user_data;
	//	PatchIndex = LayoutSwitchPatch(user_data, true);

	gtk_image_set_from_pixbuf(GTK_IMAGE(PresetButtons[(int )user_data].Image),
	                          PresetButtons[(int) user_data].ButtonUpImage);
	return TRUE; /* stop event propagation */
}

#if 0
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
	FILE *fp;

	if (webkit_web_view_is_editable(web_view) ) {
		MyImageButtonSetText(&SaveWebButton, "Edit");
		webkit_web_view_set_editable(web_view, FALSE);
		return;
	}

	MyImageButtonSetText(&SaveWebButton, "Save");
	webkit_web_view_set_editable(web_view, TRUE);

	/*
	 * Draw the button
	 */
	gtk_image_set_from_pixbuf(GTK_IMAGE(SaveWebButton.Image),
	                          SaveWebButton.ButtonDownImage);


	CurrentURI = webkit_web_view_get_uri(web_view);
	g_print("Save %s\n", CurrentURI);
	Buffer = webkit_web_view_get_title(web_view);

	printd(LogDebug, "Len = %d\n %s\n", (int) strlen(Buffer), Buffer);

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
#else
/*--------------------------------------------------------------------
 * Function:		on_SaveWeb_clicked
 *
 * Description:	Save the changes to the file
 *
 *---------------------------------------------------------------------*/
void on_SaveWeb_clicked(GtkWidget *widget, gpointer data) {
	const gchar *CurrentURI;
	char ExecuteString[200];

	/*
	 * Draw the button
	 */
	gtk_image_set_from_pixbuf(GTK_IMAGE(SaveWebButton.Image), SaveWebButton.ButtonDownImage);

	// Get the current URI of the viewable file.
	CurrentURI = webkit_web_view_get_uri(web_view);
	// g_print("Save %s\n", CurrentURI);

	// Call the text editor.
	sprintf(ExecuteString, "%s/LiveEditor %s &\n",
	        GetResourceDir("", FileLocConfig), CurrentURI);
	printd(LogDebug, "Edit: %s\n", ExecuteString);
	system(ExecuteString);
}
#endif
/*--------------------------------------------------------------------
 * Function:		PageLoaded
 *
 * Description:	Called after the page is loaded, we can then parse
 * 		for the user selections.
 *
 *---------------------------------------------------------------------*/
void PageLoaded (WebKitWebView  *web_view,
                 WebKitLoadEvent load_event,
                 gpointer        user_data) {
	gchar *CurrentURI;
	char *Pointer;
	int   Loop;

	/* We only care about the completed event.
	*/
	if (load_event != WEBKIT_LOAD_FINISHED)
		return;

	/* Get the URL that was selected.
	*/
	CurrentURI = webkit_web_view_get_uri(web_view);
	printd(LogDebug, "load_status_cb %s event %d \n", CurrentURI, load_event);

#if 1
	strcpy(BasePathName, CurrentURI);
	for (Loop = strlen(BasePathName); Loop >= 0; Loop--)
		if (BasePathName[Loop] == '/') {
			BasePathName[Loop + 1] = 0;
			Loop = -1;
		}

//	printd(LogDebug, "Base Name %s \n", BasePathName);
#endif
	printd(LogDebug, "After Basename %s \n", BasePathName);

	/* Keep a record
	*/
	WriteToHistory(CurrentURI);

	Pointer = strstr(CurrentURI, ".html");

	/* Let's check to see if this is an HTML file.
	*/
	if (strstr(CurrentURI, ".html"))
		Search_in_File(CurrentURI, &gMyInfo.WebPresets);
	else if (strstr(CurrentURI, ".mp3")) {
		printd(LogDebug, "*** MP3 file.\n");
		return;
	} else {
		printd(LogDebug, "Not HTML File\n");
		return;
	}

//	webkit_web_view_set_editable(web_view, 1);
	webkit_web_view_set_zoom_level(web_view, 1);
}

int ishex(int x) {
	return	(x >= '0' && x <= '9')	||
	        (x >= 'a' && x <= 'f')	||
	        (x >= 'A' && x <= 'F');
}

int DecodeURI(char *s, char *dec) {
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

gboolean NavigationPolicy(WebKitWebView * web_view,
                          WebKitPolicyDecision    * decision,
                          WebKitPolicyDecisionType decision_type,
                          gpointer user_data) {
	char *theOrgURI;
	char theURI[250];
	char string[150];
	int Loop;
	char *mimeType;

	if (decision_type != WEBKIT_POLICY_DECISION_TYPE_RESPONSE)
		return FALSE;

	WebKitResponsePolicyDecision *responseDecision = WEBKIT_RESPONSE_POLICY_DECISION(decision);

	WebKitWebResource *mainResource = webkit_web_view_get_main_resource(web_view);
	WebKitURIRequest *request = webkit_response_policy_decision_get_request(responseDecision);
	const char *requestURI = webkit_uri_request_get_uri(request);
	printd(LogDebug, "*** requestURI %s %s\n", requestURI, webkit_web_resource_get_uri(mainResource) );

//   webkit_policy_decision_download(decision);
//   return TRUE;

	theOrgURI = requestURI;
	DecodeURI(theOrgURI, theURI);
	printd(LogDebug, "NavigationPolicy2 %s %s \n", theURI, theOrgURI);

	/* If we find an MP3 file then handle it ourselves and tell WebKit
	 * not to deal with it.
	 */
	if (strstr(theURI, ".mp3")) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		SetPlayerFile((theURI + 7));
		printd(LogDebug, "Call SetPlayer %s \n", theURI);

		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));
		printd(LogDebug, "*** After systemcall %s\n", SysCallString);
		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}

	if (strstr(theURI, ".mp4") || strstr(theURI, ".webm") || strstr(theURI, ".mpg") ) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));

		sprintf(string, "smplayer \'%s\' &", &theURI[7]);
		system(string);
		printf("**----** systemcall %s\n", string);
		printd(LogDebug, "*** systemcall %s\n", string);

		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}

	if (strstr(theURI, ".mid") || strstr(theURI, ".med") ) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));

		sprintf(string, "muse \'%s\' &", &theURI[7]);
//		sprintf(string, "/usr/bin/rosegarden \'%s\' &", &theURI[7]);
		system(string);
		printf("**----** systemcall %s\n", string);
		printd(LogDebug, "*** systemcall %s\n", string);

		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}

	if (strstr(theURI, ".mscz")) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));

		sprintf(string, "musescore \'%s\' &", &theURI[7]);
		system(string);
		printf("**----** systemcall %s\n", string);
		printd(LogDebug, "*** systemcall %s\n", string);

		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}


	if (strstr(theURI, ".tg") || strstr(theURI, ".gp") || strstr(theURI, ".ptb") ) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));

//		sprintf(string, "/home/Dropbox/LiveEffects/MyTuxGuitar \'%s\' &", &theURI[7]);
		sprintf(string, "tuxguitar \'%s\' &", &theURI[7]);
		system(string);
		printd(LogDebug, "*** systemcall %s\n", string);

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
		PageIndex = strstr(theURI, "#page=");
		if (PageIndex) {
			webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));
			*PageIndex = 0;
			PageIndex += 6;
			PageNumber = atoi(PageIndex);
//			sprintf(string, "/usr/bin/okular \'%s\'' --page=%d &", theURI, PageNumber);
			sprintf(string, "okular \'%s\'' --page=%d &", theURI, PageNumber);
		} else {
			return (false);
//		sprintf(string, "/usr/bin/okular \"%s\" &", theURI);
		}

//		printd(LogDebug, "PAGE %x %s %d\n", &PageIndex, PageIndex, PageNumber);
		system(string);
		printd(LogDebug, "*** systemcall %s\n", string);

		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}

	//   webkit_web_policy_decision_use(policy_decision);
	// Tell Webkit to handle the URI
	return (false);
}

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

	printd(LogDebug, "Made it to OpenSetListSong %d\n", SongNumber);

	if (SongNumber < 1) {
		printd(LogDebug, "Invalid SongNumber %d\n", SongNumber);
		return;
	}

	SetListFile = fopen(&SetListFileName[7], "r");

	if (!SetListFile) {
		printd(LogDebug, "Can't open Setlist file  %s\n", SetListFileName);
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
		Found = strstr(temp, "href=");

		if (Found != NULL) {
			SongCount++;
			Found += 6;
			tokenizer = strtok(Found, ">");
			printd(LogDebug, "Parser found %d %s\n", SongCount, tokenizer);

			/*
			 * If the is the SongNumber HREF count then grab the file name.
			 */
			if (SongCount == SongNumber) {
				CurrentSetListSong = SongNumber;
				strcpy(Copy, SetListFileName);
//				dirname(SetListFileName);
				dirname(Copy);
				printd(LogDebug, "After  %s\n", Copy);
				strcat(Copy, "/");
				strcat(Copy, tokenizer);
				printd(LogDebug, "Final  %s\n", Copy);
// ejk				webkit_web_view_open(web_view, Copy);
				webkit_web_view_load_uri(web_view, Copy);

				break;
			}
		}
	}

	printd(LogDebug, "Leaving OpenSetListSong\n");

	// Close the file if still open.
	if (SetListFile) {
		fclose(SetListFile);
	}
}

/*--------------------------------------------------------------------
 * Function:		on_SetList_clicked
 *
 * Description:	Save the changes to the file
 *
 *---------------------------------------------------------------------*/
void on_SetList_clicked(GtkWidget * widget, gpointer data) {
	const gchar *CurrentURI;
	char *BasePtr;
	int Length;
	int Loop;

	printd(LogDebug, "on_SetList_clicked  1\n");

	CurrentURI = webkit_web_view_get_uri(web_view);
	strcpy(SetListFileName, CurrentURI);
	Length =  strlen(SetListFileName);
	for (Loop = Length; Loop; Loop--)
		if (SetListFileName[Loop] == '/')
			break;

//	BasePtr = basename(SetListFileName);
	BasePtr = &SetListFileName[Loop + 1];
	Length = strlen(BasePtr);
	BasePtr[Length - 5] = 0;
	MyImageButtonSetText(&SetListButton, BasePtr);
	strcpy(SetListFileName, CurrentURI);
	printd(LogDebug, "on_SetList_clicked  4\n");
}

// /usr/lib/mozilla/plugins
// libevbrowserplugin.so
/*--------------------------------------------------------------------
 * Function:		InitHTML
 *
 * Description:	Set up the WebKit environment.
 *
 *---------------------------------------------------------------------*/
void InitHTML(GtkBuilder * gxml) {
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


#if 1
// Scroll bar fix to add GtkViewport
	GtkWidget *viewport;

	viewport =
	    gtk_viewport_new (gtk_scrolled_window_get_hadjustment (scrolled_window),
	                      gtk_scrolled_window_get_vadjustment (scrolled_window));
	gtk_container_set_focus_hadjustment (GTK_CONTAINER (viewport),
	                                     gtk_scrolled_window_get_hadjustment (GTK_SCROLLED_WINDOW (scrolled_window)));
	gtk_container_set_focus_vadjustment (GTK_CONTAINER (viewport),
	                                     gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (scrolled_window)));
//      gtk_container_add (GTK_CONTAINER (scrolled_window), viewport);
	gtk_scrolled_window_add_with_viewport (GTK_CONTAINER (scrolled_window), viewport);


#endif

	/* Create a new webkit view to display our data.
	 */
	web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
	gtk_container_add(GTK_CONTAINER(viewport), GTK_WIDGET(web_view));
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
	g_signal_connect(G_OBJECT(EventBox), "button-release-event", G_CALLBACK(normal_release_handler),
	                 &ForwardButton);

	EventBox = GTK_WIDGET(
	               gtk_builder_get_object(gxml, "ScaleButton"));
	MyImageButtonInit(&ScaleButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&ScaleButton, "Scale");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
	                 G_CALLBACK(on_scalebutton_clicked), &ScaleButton);
	g_signal_connect(G_OBJECT(EventBox), "button-release-event", G_CALLBACK(normal_release_handler),
	                 &ScaleButton);

	EventBox = GTK_WIDGET(
	               gtk_builder_get_object(gxml, "TapTempo"));
	MyImageButtonInit(&TapTempoButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&TapTempoButton, "Tap");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
	                 G_CALLBACK(on_TapTempo_clicked), &TapTempoButton);
	g_signal_connect(G_OBJECT(EventBox), "button-release-event", G_CALLBACK(normal_release_handler), &TapTempoButton);

	EventBox = GTK_WIDGET(
	               gtk_builder_get_object(gxml, "SaveWeb"));
	MyImageButtonInit(&SaveWebButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&SaveWebButton, "Edit");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
	                 G_CALLBACK(on_SaveWeb_clicked), &SaveWebButton);
	g_signal_connect(G_OBJECT(EventBox), "button-release-event", G_CALLBACK(normal_release_handler), &SaveWebButton);

	EventBox = GTK_WIDGET(
	               gtk_builder_get_object(gxml, "SetList"));
	MyImageButtonInit(&SetListButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&SetListButton, "SetList");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
	                 G_CALLBACK(on_SetList_clicked), &SetListButton);
	g_signal_connect(G_OBJECT(EventBox), "button-release-event", G_CALLBACK(normal_release_handler), &SetListButton);

	EventBox = GTK_WIDGET(
	               gtk_builder_get_object(gxml, "PlayPause"));
	MyImageButtonInit(&PlayPauseButton, EventBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&PlayPauseButton, "Play");
	g_signal_connect(G_OBJECT(EventBox), "button-press-event",
	                 G_CALLBACK(Play_click_handler), &PlayPauseButton);

#if 0
	g_signal_connect(G_OBJECT(EventBox), "button-release-event",
	                 G_CALLBACK(normal_release_handler), &PlayPauseButton);

	g_signal_connect(web_view, "load-finished", G_CALLBACK(load_finished_cb), NULL);

	/* Connect to the viewport-attributes-changes signal */
	WebKitViewportAttributes* attributes = webkit_web_view_get_viewport_attributes (web_view);
	g_signal_connect (web_view, "viewport-attributes-recompute-requested", G_CALLBACK (viewport_recompute_cb), scrolled_window);
	g_signal_connect (web_view, "viewport-attributes-changed", G_CALLBACK (viewport_changed_cb), scrolled_window);
	g_signal_connect (attributes, "notify::valid", G_CALLBACK (viewport_valid_changed_cb), web_view);
#endif

	/* Register a callback that gets invoked each time that a page is finished downloading */

	g_signal_connect(web_view, "load-changed", G_CALLBACK(PageLoaded), NULL);
#if 0
	g_signal_connect(web_view, "load-finished", G_CALLBACK(PageLoaded), NULL);

	g_signal_connect(web_view, "download-requested", G_CALLBACK(DownloadRequestcb), NULL);
	g_signal_connect(web_view, "mime-type-policy-decision-requested", G_CALLBACK(PolicyRequestCD), NULL);
#endif

	g_signal_connect(web_view, "decide-policy",
	                 G_CALLBACK(NavigationPolicy), NULL);

	strncpy(FileName, "file://", 7 );
	strncpy(&FileName[7], gMyInfo.BasePath, sizeof (FileName) - 7);
//	strncpy(FileName, gMyInfo.BasePath, sizeof (FileName));
	printf("OK from here\n");
	printf("Path %s %s\n", gMyInfo.BasePath, FileName);
	//   strcat(FileName, "/indexCharts.html");
//	strncpy(FileName, "file:///home/Dropbox/FusionBlue/index.html", 254);
	webkit_web_view_load_uri (web_view, FileName);

	WebKitSettings *settings = webkit_settings_new ();
	g_object_set(G_OBJECT(settings), "auto-shrink-images", FALSE, NULL);
	g_object_set(G_OBJECT(settings), "enable-page-cache", FALSE, NULL);
//	g_object_set(G_OBJECT(settings), "enable-frame-flattening", FALSE, NULL);

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

	printd(LogDebug, "Settings for webkit\n");
	webkit_settings_set_enable_media_stream(G_OBJECT(settings), FALSE);
	webkit_settings_set_enable_mediasource(G_OBJECT(settings), FALSE);
	webkit_settings_set_enable_fullscreen(G_OBJECT(settings), TRUE);
//	webkit_web_view_set_editable(G_OBJECT(settings), FALSE);
#if 0
	webkit_settings_set_enable_accelerated_2d_canvas(G_OBJECT(settings), TRUE);
	webkit_settings_set_draw_compositing_indicators(G_OBJECT(settings), FALSE);
	g_object_set(settings,
	             "enable-webgl", FALSE,
	             "enable-java", FALSE,
	             "enable-javascript", FALSE,
	             "enable-mock-capture-devices", FALSE,
	             "enable-offline-web-application-cache", FALSE,
	             "enable-page-cache", FALSE,
	             "enable-smooth-scrolling", FALSE,
	             "hardware-acceleration-policy", WEBKIT_HARDWARE_ACCELERATION_POLICY_ALWAYS
	            );
#endif

	/* Apply the result */
	webkit_web_view_set_settings(WEBKIT_WEB_VIEW(web_view), settings);

	gtk_widget_show_all(scrolled_window);
}

#define ContentTagLen	9
/*--------------------------------------------------------------------
 * Function:		Search_in_File
 *
 * Description:	Look thru the file and find presets.
 *
 *---------------------------------------------------------------------*/
int Search_in_File(const char *fname, WebLoadPresets * thePresets) {
	FILE *fp;
	char temp[MAXLINE];
	char Copy[MAXLINE];
	char *Found;
	int Value;
	char *tokenizer;
	char *String;
	int Count = 0;
	char DrumFile[FileNameMaxLength];
	char LoopFile[FileNameMaxLength];
	char StatusString[40];


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

	printd(LogDebug, "Have file %x %s\n", fp, fname);
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

			printd(LogInfo, "Tempo %d\n", Value);
			SetTempo(Value);
			sprintf(StatusString, "Tempo %d", Value);
			UpdateStatus(StatusString);
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
//			String = Found;
//			tokenizer = strtok(String, "\""); //break up by spaces
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
			printd(LogDebug, "LoopFile %s\n", tokenizer);

#if 1
			strcpy(gMyInfo.LoopFileName, BasePathName);
			strcat(gMyInfo.LoopFileName, tokenizer);
#else
			strcpy(LoopFile, tokenizer);
			strcpy(gMyInfo.LoopFileName, tokenizer);
#endif
			printd(LogDebug, "LoopFile name %s\n", gMyInfo.LoopFileName);
			MyOSCLoadFile(gMyInfo.LoopFileName);
		}

		/* Get the Drum File patch.
		 */
		Found = strstr(temp, "DrumFile");
		if (Found != NULL) {
			Found += 10 + ContentTagLen;
			String = Found;
			tokenizer = strtok(String, "\""); //break up by spaces
			printd(LogDebug, "DrumFile %s\n", tokenizer);
			strncpy(temp, Copy, MAXLINE);
			strcpy(DrumFile, tokenizer);
		}

		/* Set the current patch to this one.
		 */
		Found = strstr(temp, "IntroCount");
		if (Found != NULL) {
			Found += 12 + ContentTagLen;
			Value = atoi(Found);
			printd(LogDebug, "IntroCount %d\n", Value);
			strncpy(temp, Copy, MAXLINE);
			gMyInfo.CountInBeats = Value;
			sprintf(StatusString, "Intro Count %d", Value);
			UpdateStatus(StatusString);
		}

		/* Set the current patch to this one.
		 */
		Found = strstr(temp, "BeatsPerMeasure");
		if (Found != NULL) {
			Found += 17 + ContentTagLen;
			Value = atoi(Found);
			printd(LogDebug, "BeatsPerMeasure %d\n", Value);
			strncpy(temp, Copy, MAXLINE);
			gMyInfo.BeatsPerMeasure = Value;
			sprintf(StatusString, "Beats  %d", Value);
			UpdateStatus(StatusString);
		}

		/* Set the current patch to this one.
		 */
		Found = strstr(temp, "LoopLength");
		if (Found != NULL) {
			Found += 12 + ContentTagLen;
			Value = atoi(Found);
			printd(LogDebug, "LoopLength %d\n", Value);
			strncpy(temp, Copy, MAXLINE);
			gMyInfo.LoopRecBeats = Value;
			sprintf(StatusString, "Loop Len  %d", Value);
			UpdateStatus(StatusString);

		}
	}

	//Close the file if still open.
	if (fp) {
		fclose(fp);
	}


#if 1
	/*
	 * Check to see if we have requested a new file for the drum or the looper.
	 */
	if (DrumFile[0] || LoopFile[0]) {
		/*
		 * Make sure the the looper is the second argument even of the drum file is the same.
		 */
		if (DrumFile[0] == 0)
			DrumFile[0] = 'A';

		sprintf(Copy, "/home/Dropbox/LiveEffects/LoadDrumFile %s & ", DrumFile);
		printd(LogDebug, "Calling System with %s\n", Copy);
		system(Copy);
	}
#endif
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
		printd(LogDebug, "Token1 %s\n", tokenizer);
		Value = FindString(fsPatchNames, tokenizer);

	} else {
		Value = atoi(String);
		printd(LogDebug, "***** Assign Value %s %d\n", String, Value);
		if (Value >= Max_Patches)
			Value = 0;
	}

	printd(LogDebug, "Preset %d %d %s\n", PresetNum, Value,
	       gMyInfo.MyPatchInfo[Value].Name);

	if (Value < 0 || Value >= Max_Patches)
		return (Value);

	/* If it's a preset button or a set now.	*/
	switch (PresetNum) {
	case 0:
		DoPatch(&gMyInfo.MyPatchInfo[Value]);
		break;

	case 1:
		printd(LogDebug, "*********PresetNum Case 1 ");
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
void SetPatchTitles(theImageButtons * MyButton, char *Text, int Value) {
	char String[PatchNameSize];
	int StringLen;

	printd(LogDebug, "SetPatchTitles %x %s\n", MyButton, Text);
	StringLen = strlen(Text);
	sprintf(String, "       %02d       \n%*s", Value, 7 + StringLen / 2,
	        Text);

	printd(LogDebug, "SetPatchTitles %s\n", String);
	MyImageButtonSetText(MyButton, String);
}

#if 0
void load_finished_cb(WebKitWebView *web_view, WebKitWebFrame *web_frame,
                      gpointer data) {
	printd(LogDebug, "Finished downloading %s\n",
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

	printd(LogDebug, "DownloadRequestcb %s\n", download);

}

gboolean
PolicyRequestCD (WebKitWebView* view, WebKitWebFrame* frame,
                 WebKitNetworkRequest* request, const char* mime_type,
                 WebKitWebPolicyDecision* decision, gpointer data) {
//    char* type = (char*)data;

	printd(LogDebug, "PolicyRequestCD %s %s\n", mime_type, data);
#if 0

	if (g_str_equal(type, "pdf")) {
		g_assert_cmpstr(mime_type, ==, "application/pdf");
		g_assert(!webkit_web_view_can_show_mime_type(view, mime_type));
	} else if (g_str_equal(type, "html")) {
		g_assert_cmpstr(mime_type, ==, "text/html");
		g_assert(webkit_web_view_can_show_mime_type(view, mime_type));
	} else if (g_str_equal(type, "text")) {
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
			printd(LogDebug, "PolicyRequestCD MP3 Found %s \n", mime_type);
		}
	//   g_free(type);

	return FALSE;
}

#endif