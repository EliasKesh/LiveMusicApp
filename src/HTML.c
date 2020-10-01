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
#include "HTML.h"
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
#define AllowInternalMP4 1

/*
 * Place Local prototypes here
 */

int Search_in_File(const char *fname, WebLoadPresets *thePresets);
int ScalePage(void);
tPatchIndex AssignPreset(int PresetNum, char *String);
void SetPatchTitles(theImageButtons *MyButton, char *Text, int Value);
gboolean Play_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
static void
scroll_js_finished_cb (GObject      *object,
                       GAsyncResult *result,
                       gpointer      user_data);

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

//GtkWidget *scrolled_window;

GtkWidget *ChartGTKView;
char  SetListFileName[FileNameMaxLength];
FILE *SetListFile;
char  BasePathName[250];
int   ScrollPosition;
int   JavaScrollPosition;
int  WaitForCallBack;

// ~/.config/mimeapps.list

/*--------------------------------------------------------------------
 * Function:		SavePresetChanges
 *
 * Description:	write any changes to the file.
 * Cheesy way of doing this.
 *---------------------------------------------------------------------*/
char SavePresetChanges(char *FileName) {
	FILE *InFile;
	FILE *OutFile;
	char FileLine[200];
	tPatchIndex thePatch;
	DIR* directory;

	/* Check to see if this is a directory.
	If it is return, don't try and modify it.
	*/
	directory = opendir(FileName);
	if (directory != NULL) {
		closedir(directory);
		return 0;
	}

	/* Open the current html file.
	*/
	InFile = fopen(FileName, "r");

	/* Open the output file.
	*/
	sprintf(FileLine, "%s.Test", FileName);
	OutFile = fopen(FileLine, "w+");

	if (!InFile || !OutFile) {
		printd(LogError, "Html File Error %x %x\n", InFile, OutFile);
		return(1);
	}

	/* Read the infile line by line and replace the meta
	Data with the current settings.
	*/
	while (!feof(InFile)) {
		FileLine[0] = 0;

		/* Read a line of file.
		*/
		fgets(FileLine, 200, InFile);

		if (strstr(FileLine, "Preset1")) {
			thePatch = gMyInfo.WebPresets.thePreset[0];
			if (thePatch >= 0 && thePatch < Max_Patches)
				sprintf(FileLine, "<meta name=\"Preset1\" content=\"%s\" >\n",
				        gMyInfo.MyPatchInfo[thePatch].Name);
		}

		if (strstr(FileLine, "Preset2")) {
			thePatch = gMyInfo.WebPresets.thePreset[1];
			if (thePatch >= 0 && thePatch < Max_Patches)
				sprintf(FileLine, "<meta name=\"Preset2\" content=\"%s\" >\n",
				        gMyInfo.MyPatchInfo[thePatch].Name);
		}

		if (strstr(FileLine, "Preset3")) {
			thePatch = gMyInfo.WebPresets.thePreset[2];
			if (thePatch >= 0 && thePatch < Max_Patches)
				sprintf(FileLine, "<meta name=\"Preset3\" content=\"%s\" >\n",
				        gMyInfo.MyPatchInfo[thePatch].Name);
		}

		if (strstr(FileLine, "Preset4")) {
			thePatch = gMyInfo.WebPresets.thePreset[3];
			if (thePatch >= 0 && thePatch < Max_Patches)
				sprintf(FileLine, "<meta name=\"Preset4\" content=\"%s\" >\n",
				        gMyInfo.MyPatchInfo[thePatch].Name);
		}

		if (strstr(FileLine, "Preset5")) {
			thePatch = gMyInfo.WebPresets.thePreset[4];
			if (thePatch >= 0 && thePatch < Max_Patches)
				sprintf(FileLine, "<meta name=\"Preset5\" content=\"%s\" >\n",
				        gMyInfo.MyPatchInfo[thePatch].Name);
		}

		if (strstr(FileLine, "Preset6")) {
			thePatch = gMyInfo.WebPresets.thePreset[5];
			if (thePatch >= 0 && thePatch < Max_Patches)
				sprintf(FileLine, "<meta name=\"Preset6\" content=\"%s\" >\n",
				        gMyInfo.MyPatchInfo[thePatch].Name);
		}

		if (strstr(FileLine, "Tempo")) {
			sprintf(FileLine, "<meta name=\"Tempo\" content=\"%d\" >\n", gMyInfo.Tempo);
		}

//		printf("%s", FileLine);

		/* Now write the line back to the output file.
		*/
		fprintf(OutFile, "%s", FileLine);
	}

	// Close both files
	fclose(InFile);
	fclose(OutFile);

	sprintf(FileLine, "%s.Test", FileName);

	// Rename the temp file back to the main file.
	rename(FileLine, FileName);

	return(0);
}


/*--------------------------------------------------------------------
 * Function:		on_Back_clicked
 *
 * Description:	Web browser back button
 *
 *---------------------------------------------------------------------*/
void on_Back_clicked(GtkButton * button, gpointer user_data) {
	const gchar *CurrentURI;

	ScrollPosition = 0;
//	webkit_web_view_set_editable( web_view, FALSE);
	gtk_image_set_from_pixbuf(GTK_IMAGE(BackButton.Image),
	                          BackButton.ButtonDownImage);
	/* Get the currently loaded page.
	*/
	CurrentURI = webkit_web_view_get_uri(web_view);

	SavePresetChanges((char *)&CurrentURI[7]);

	webkit_web_view_go_back(web_view);

	// Reset the patches
	SetPatchTitles(&PresetButtons[0], "Preset 1", 1);
	SetPatchTitles(&PresetButtons[1], "Preset 2", 2);
	SetPatchTitles(&PresetButtons[2], "Preset 3", 3);
	SetPatchTitles(&PresetButtons[3], "Preset 4", 4);
	SetPatchTitles(&PresetButtons[4], "Preset 5", 5);
//	SetPatchTitles(&PresetButtons[5], "Preset 6", 6);
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
}

//#define MaxChartRepeat 10

/*--------------------------------------------------------------------
 * Function:		web_view_javascript_finished
 *
 * Description:	JavaScript callback
 *
 *---------------------------------------------------------------------*/
static void
web_view_javascript_finished (GObject      *object,
                              GAsyncResult *result,
                              gpointer      user_data) {
	WebKitJavascriptResult *js_result;
	JSCValue               *value;
	GError                 *error = NULL;

	WaitForCallBack = -100;

//	printf("web_view_javascript_finished\n");
	js_result = webkit_web_view_run_javascript_finish (WEBKIT_WEB_VIEW (object), result, &error);
	if (!js_result) {
//		printf ("Error running javascript: %s", error->message);
		g_error_free (error);
		return;
	}

	value = webkit_javascript_result_get_js_value (js_result);
//	printf(" Return0 %x %s\n", value, value);
	if (jsc_value_is_string (value)) {
		JSCException *exception;
		gchar        *str_value;

		str_value = jsc_value_to_string (value);
		exception = jsc_context_get_exception (jsc_value_get_context (value));
		if (user_data == 1) {
			JavaScrollPosition = atoi(str_value);
//			printf("Java Scroll %s %d\n", str_value, JavaScrollPosition);
		}
	}
	webkit_javascript_result_unref (js_result);
}

static void
scroll_js_finished_cb (GObject      *object,
                       GAsyncResult *result,
                       gpointer      user_data) {
  WebKitJavascriptResult *js_result;
  GError *error = NULL;

  js_result = webkit_web_view_run_javascript_finish (WEBKIT_WEB_VIEW (object), result, &error);
  if (error != NULL) {
    g_print ("Error running scroll script: %s", error->message);
    g_error_free (error);
    return;
	}
} 

/*--------------------------------------------------------------------
 * Function:		ScrollCtrl
 *
 * Description:	Scroll the music
 *
 *---------------------------------------------------------------------*/
int ScrollCtrl(float Amount) {
	GtkAdjustment *Adjust;
	gdouble  UpperV, VIncrement;
	gdouble Value;
	char Script[500];

	ScrollPosition = ScrollGetPosition();

	if (Amount == ScrollPgDn) {
		ScrollPosition += 200;
	}

	if (Amount == ScrollPgUp) {
		ScrollPosition -= 200;
	}

	if (Amount == ScrollEnd) {
		ScrollPosition = 100000;
	}

	if (Amount == ScrollHome) {
		ScrollPosition = 0;
	}

	if (Amount == ScrollKeyDn) {
		ScrollPosition += 25;
	}

	if (Amount == ScrollKeyUp) {
		ScrollPosition -= 25;
	}

	if (Amount > 20)
		ScrollPosition = Amount;

	if (ScrollPosition < 0)
		ScrollPosition = 0;

//	sprintf(Script,"window.getSelection().getRangeAt(0).toString()");
	printf("Scroll Control %f %d\n", Amount, ScrollPosition);
	sprintf(Script, "window.scrollTo(%d,%d);", 0, ScrollPosition);
	webkit_web_view_run_javascript (web_view,
	                                Script,
	                                NULL,
	                                scroll_js_finished_cb,
	                                NULL);
	return (0);

}

/*--------------------------------------------------------------------
 * Function:		ScrollGetPosition
 *
 * Description:	Get the Current Scolling position
 *
 *---------------------------------------------------------------------*/
float ScrollGetPosition(void) {

	WaitForCallBack = 10000;
	webkit_web_view_run_javascript (web_view,
	                                "window.pageYOffset.toString();",
	                                NULL,
	                                web_view_javascript_finished,
	                                1);

	while (--WaitForCallBack > 0);
//	printf("Scroll J %d-> S %d  Wait %d\n",
//	       JavaScrollPosition, ScrollPosition, WaitForCallBack);
	return (JavaScrollPosition);
}

/*--------------------------------------------------------------------
 * Function:		Scale the page to fit with scroll bars
 *
 * Description:	Scale to fit routine. Doesn't work well.
 *
 *---------------------------------------------------------------------*/
int ScalePage(void) {
	gfloat UpperH, UpperV;
	GtkAdjustment *Adjust;
	gint Horiz, Vert;
	gfloat ScaleH, ScaleV;

	Adjust = gtk_scrolled_window_get_hadjustment((GtkScrolledWindow *)ChartGTKView);
	UpperH = gtk_adjustment_get_upper(Adjust);

	Adjust = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *)ChartGTKView);
	UpperV = gtk_adjustment_get_upper(Adjust);
	printf("ScalePage %x %f\n", Adjust, UpperV );

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

//	 webkit_web_view_reload(web_view);
	gtk_image_set_from_pixbuf(GTK_IMAGE(ScaleButton.Image),
	                          ScaleButton.ButtonDownImage);

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
long long elapsedTime;
int 	ReturnVal;

	ReturnVal = clock_gettime(CLOCK_REALTIME, &Time1);

#if 0
	long NowTime,OldTime;
	// Get the new time
	// ReturnVal = clock_getres(CLOCK_REALTIME, &Time1);
	// printf("TimeRes %ld %ld %d\n", Time1.tv_sec,Time1.tv_usec, ReturnVal);

	NowTime = (long) ( ( ((long)(Time1.tv_sec)*1000000) + ((long)(Time1.tv_usec)/1000)))  ;
	printf("Now %18ld \n", NowTime);
	OldTime = (long) ( ( ((long)(Time0.tv_sec)*1000000) + ((long)(Time0.tv_usec)/1000)))  ;
	printf("Old %18ld \n", OldTime);
	printf("Dif %18ld  %d \n", NowTime - OldTime,60000000/(NowTime - OldTime) );
	printf("msec %18ld %18ld \n", (Time1.tv_sec-Time0.tv_sec), Time1.tv_usec - Time0.tv_usec);

	NowTime = Time1.tv_usec - Time0.tv_usec;
	if (NowTime < 0) {
		NowTime = ((1000000000 + Time1.tv_usec) - Time0.tv_usec);
		printf("Neg %ld\n", NowTime);
	}
	else
		printf("Nor %ld\n", NowTime);

#endif

	if (Time1.tv_usec-Time0.tv_usec)
		elapsedTime = (long) (60000000 / ( ((long long)(Time1.tv_sec-Time0.tv_sec)*1000000) + ((long long)(Time1.tv_usec-Time0.tv_usec)/1000)))  ;
	else
		elapsedTime = 0;

	Time0 = Time1;

	/* If it's a ctrl click then assign the value.
	*/
	if ((event->button.state & GDK_CONTROL_MASK) ||
	(event->button.button == 3)) {
		gMyInfo.Tempo = CurTapTempo;
		g_idle_add(GTKIdel_cb, theMainWindow);
		return(CurTapTempo);
	}

	// Calculate BPM
	if (elapsedTime > 0 && elapsedTime < 220) {
		CurTapTempo = (0.50 * CurTapTempo) + (0.50 * elapsedTime);
	}

	sprintf(String, "Tap=%d", CurTapTempo);
	MyImageButtonSetText(&TapTempoButton, String);

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
		CurrentPreset = CPatch + Max_Patches + 1;
		ShowPatchListSelect(GTK_WINDOW(widget), CPatch);

		return (TRUE);
	}

	printd(LogDebug, "In Button Preset %d %d\n", CPatch, Preset);

	/* Make sure the preset is at least in the correct range.
	*/
	if (Preset < 0  ||  Preset > Max_Patches)
		return (false);

	printd(LogDebug, "In Button Preset %s\n", gMyInfo.MyPatchInfo[Preset].Name);

	/* Execute the patch change.
	*/
	DoPatch(&gMyInfo.MyPatchInfo[Preset]);

	/* Redraw the button.	*/
	gtk_image_set_from_pixbuf(GTK_IMAGE(PresetButtons[CPatch].Image), PresetButtons[CPatch].ButtonDownImage);

	return (1);
}

/*--------------------------------------------------------------------
 * Function:		on_patch__release_handler
 *
 * Description:	The release of a preset click.
 *
 *---------------------------------------------------------------------*/
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

	// Call the text editor.
//	sprintf(ExecuteString, "gedit %s &\n",
//	        CurrentURI);
	// sprintf(ExecuteString, "%s %s &\n",
	//         gMyInfo.Apps[HTMLEditor].Name,
	//         &CurrentURI[7]);
	sprintf(ExecuteString, "MusicApps.sh html %s &\n",
	        &CurrentURI[7]);
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
	char  FileName[200];

	/* Get the URL that was selected.
	*/
	CurrentURI = webkit_web_view_get_uri(web_view);
	printd(LogDebug, "load_status_cb %s event %d \n", CurrentURI, load_event);
	ScrollPosition = 0;

	/* We only care about the completed event.
	WEBKIT_LOAD_STARTED, WEBKIT_LOAD_REDIRECTED,
	WEBKIT_LOAD_COMMITTED, WEBKIT_LOAD_FINISHED
	*** Use WEBKIT_LOAD_FINISHED to reload after ever click.
	*/
	if (load_event != WEBKIT_LOAD_COMMITTED)
		return;
#if 1
	/* We need an absolue path. For files in the current
	song folder add the full path before the name.
	*/
	strcpy(BasePathName, CurrentURI);
	for (Loop = strlen(BasePathName); Loop >= 0; Loop--)
		if (BasePathName[Loop] == '/') {
			strcpy(FileName, &BasePathName[Loop + 1]);
			BasePathName[Loop + 1] = 0;
			Loop = -1;
		}


//	printd(LogDebug, "Base Name %s \n", BasePathName);
#endif
	printd(LogDebug, "After Basename %s \n", BasePathName);

#if 0
	if (strcmp("index.html", FileName ) ) {
		printf("match Basename1 %s \n", FileName);
//		UpdateStatus(FileName);
	}
#endif

	/* Keep a record
	*/
	WriteToHistory(CurrentURI);
//	Pointer = strstr(CurrentURI, ".html");

	/* Let's check to see if this is an HTML file.
	*/
	if (strstr(CurrentURI, ".html")) {
		Search_in_File(CurrentURI, &gMyInfo.WebPresets);

		// Pointer = webkit_web_view_get_title(web_view);
		// if (Pointer)
		// 	printf("%s\n", Pointer);

		/* Add the file to the status.
		*/
		if (!strstr(CurrentURI, "index"))
			UpdateStatus("---Load---");
	}
	else if (strstr(CurrentURI, ".mp3")) {
		printd(LogDebug, "*** MP3 file.\n");
//		UpdateStatus(FileName);

		return;
	} else {
		printd(LogDebug, "Not HTML File\n");
		return;
	}

//	webkit_web_view_set_editable(web_view, FALSE);
	webkit_web_view_set_zoom_level(web_view, 1);
}

int ishex(int x) {
	return	(x >= '0' && x <= '9')	||
	        (x >= 'a' && x <= 'f')	||
	        (x >= 'A' && x <= 'F');
}

/* Get rid of URL funky characters, like %20
*/
int DecodeURI(char *s, char *dec) {
	char *o;
	char *end = s + strlen(s);
	int c;

	for (o = dec; s <= end; o++) {
		c = *s++;
		if (c == '+')
			c = ' ';
		else if (c == '%' && (	!ishex(*s++)	||
		                        !ishex(*s++)	||
		                        !sscanf(s - 2, "%2x", &c)))
			return -1;

		if (dec)
			*o = c;
	}
	return o - dec;
}

/*--------------------------------------------------------------------
 * Function:		NavigationPolicy
 *
 * Description:	Return TRUE means we are handling the URL
 * FALSE means that webkit has to handle it.
 *
 *---------------------------------------------------------------------*/
gboolean NavigationPolicy(WebKitWebView * web_view,
                          WebKitPolicyDecision    * decision,
                          WebKitPolicyDecisionType decision_type,
                          gpointer user_data) {
	char *theOrgURI;
	char theURI[250];
	char string[150];
	int Loop;
	char *mimeType;
	char *ext;
	int 	SysRet;
	char *PageIndex;
	int	PageNumber;
	char *FileName;

	if (decision_type != WEBKIT_POLICY_DECISION_TYPE_RESPONSE)
		return FALSE;

	WebKitResponsePolicyDecision *responseDecision =
	    WEBKIT_RESPONSE_POLICY_DECISION(decision);
	WebKitWebResource *mainResource =
	    webkit_web_view_get_main_resource(web_view);
	WebKitURIRequest *request =
	    webkit_response_policy_decision_get_request(responseDecision);
	char *requestURI =
	    webkit_uri_request_get_uri(request);

	printd(LogDebug, "*** requestURI %s %s\n", requestURI, webkit_web_resource_get_uri(mainResource) );

//   webkit_policy_decision_download(decision);
//   return TRUE;

	theOrgURI = requestURI;
	DecodeURI(theOrgURI, theURI);
	printd(LogDebug, "NavigationPolicy2 %s %s \n", theURI, theOrgURI);

	for (Loop = strlen(requestURI); Loop >= 0; Loop--)
		if (requestURI[Loop] == '/') {
			FileName = &requestURI[Loop + 1];
			Loop = -1;
		}


	/* get the extension of the file.
	*/
	ext = strrchr(theURI, '.');

	/* If it's a web page we want to display,
	Let the WebKit handle it.
	*/
	if (strstr(theURI, ".html") || (ext == NULL)) {
		return (FALSE);
	}

#if 1
	/* If we find an MP3 file then handle it ourselves and tell WebKit
	 * not to deal with it.
	 */
//	if (strstr(theURI, ".mp3") || strstr(theURI, ".mp4")) {
	if (strstr(theURI, ".mp3")) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		SetPlayerFile((theURI + 7));
		printd(LogInfo, "Call SetPlayer %s \n", theURI);

		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));
		printd(LogDebug, "*** After systemcall %s\n", SysCallString);
		printf("*** MP3 file. %s\n", FileName);

		UpdateStatus(FileName);

		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}
#ifdef AllowInternalMP4
	if (strstr(theURI, ".mp4")) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		SetPlayerFile((theURI + 7));
		printd(LogInfo, "Call SetPlayer %s \n", theURI);

		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));
		printd(LogDebug, "*** After systemcall %s\n", SysCallString);
		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}
#endif

	/* Depending on the PDF call there may be a page number, so
	we will handle it.
	*/
	if (strstr(theURI, ".pdf")) {
		PageIndex = strstr(theURI, "#page=");
		if (PageIndex) {
			webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));
			*PageIndex = 0;
			PageIndex += 6;
			PageNumber = atoi(PageIndex);
			sprintf(string, "MusicApps.sh %s \'%s\' %d", ext + 1, &theURI[7], PageNumber);
		} else
			sprintf(string, "MusicApps.sh %s \'%s\' ", ext + 1, &theURI[7]);
	} else
		sprintf(string, "MusicApps.sh %s \'%s\' ", ext + 1, &theURI[7]);

	/* Call the bash app and see if there is a handler for the media-type.
	*/
	SysRet = system(string);
	printd(LogInfo, "*** systemcall %d %s\n", SysRet, string);

	/*
	 * This tells webkit we are dealing with it.
	 */
	if (!SysRet) {
		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));
		printd(LogInfo, "NavPol return true\n");
		return (true);
	} else {
		printd(LogInfo, "NavPol return false\n");
		return (false);
	}
#else
	/* If we find an MP3 file then handle it ourselves and tell WebKit
	 * not to deal with it.
	 */
	if (strstr(theURI, ".mp3")) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		SetPlayerFile((theURI + 7));
		printd(LogInfo, "Call SetPlayer %s \n", theURI);

		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));
		printd(LogDebug, "*** After systemcall %s\n", SysCallString);
		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}

#ifdef AllowInternalMP4

	if (strstr(theURI, ".mp4") || strstr(theURI, ".webm") || strstr(theURI, ".mpg") ) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		SetPlayerFile((theURI + 7));
		printd(LogInfo, "Call SetPlayer %s \n", theURI);

		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));
		printd(LogDebug, "*** After systemcall %s\n", SysCallString);
		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}
#else
	if (strstr(theURI, ".mp4") || strstr(theURI, ".webm") || strstr(theURI, ".mpg") ) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));

//		sprintf(string, "smplayer \'%s\' &", &theURI[7]);
		sprintf(string, "MusicApps.sh  mp4 \'%s\' ", &theURI[7]);
		system(string);
		printd(LogInfo, "*** systemcall %s\n", string);

		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}
#endif
	if (strstr(theURI, ".mid") || strstr(theURI, ".med") ) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));

//		sprintf(string, "%s \'%s\' &", gMyInfo.Apps[MidiPlayer].Name, &theURI[7]);
		sprintf(string, "MusicApps.sh med \'%s\' ", &theURI[7]);
//		sprintf(string, "muse \'%s\' &", &theURI[7]);
//		sprintf(string, "/usr/bin/rosegarden \'%s\' &", &theURI[7]);
		SysRet = system(string);
		printd(LogInfo, "*** systemcall %s\n", string);

		printf("Type %s returns %d\n", ext, SysRet);
		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}

	if (strstr(theURI, ".mscz")  || strstr(theURI, ".gp") || strstr(theURI, ".ptb") ) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));

//		sprintf(string, "musescore \'%s\' &", &theURI[7]);
		sprintf(string, "MusicApps.sh %s \'%s\' ", ext + 1, &theURI[7]);
//		sprintf(string, "%s \'%s\' &",gMyInfo.Apps[MidiPlayer].Name, &theURI[7]);
		SysRet = system(string);
		printd(LogInfo, "*** systemcall %s\n", string);
		printf("Type %s returns %d\n", ext, SysRet);

		/*
		 * This tells webkit we are dealing with it.
		 */
		if (!SysRet)
			return (true);
		else
			return (false);
	}


	if (strstr(theURI, ".tg") ) {
		/*
		 * Tell web kit not to o anything with it.
		 */
		webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));

		sprintf(string, "%s \'%s\' &", gMyInfo.Apps[TabPlayer].Name, &theURI[7]);
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
		PageIndex = strstr(theURI, "#page=");
		if (PageIndex) {
			webkit_policy_decision_ignore (WEBKIT_POLICY_DECISION (decision));
			*PageIndex = 0;
			PageIndex += 6;
			PageNumber = atoi(PageIndex);
//			sprintf(string, "/usr/bin/okular \'%s\'' --page=%d &", theURI, PageNumber);
//			sprintf(string, "okular \'%s\' --page=%d &", &theURI[7], PageNumber);
			sprintf(string, "MusicApps.sh pdf \'%s\' --page=%d ", &theURI[7], PageNumber);
		} else {
			return (false);
		}

		system(string);
		printd(LogInfo, "*** systemcall %s\n", string);

		/*
		 * This tells webkit we are dealing with it.
		 */
		return (true);
	}
#endif
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

	ChartGTKView = GTK_WIDGET(
	                   gtk_builder_get_object(gxml, "scrolledwindow1"));

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(ChartGTKView),
	                               GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	/* Create a new webkit view to display our data.
	 */
	web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
	gtk_container_add(GTK_CONTAINER(ChartGTKView), GTK_WIDGET(web_view));

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

	/* Register a callback that gets invoked each time that a page is finished downloading */

	g_signal_connect(web_view, "load-changed", G_CALLBACK(PageLoaded), NULL);
	g_signal_connect(web_view, "decide-policy",
	                 G_CALLBACK(NavigationPolicy), NULL);

	strncpy(FileName, "file://", 7 );
	strncpy(&FileName[7], gMyInfo.BasePath, sizeof (FileName) - 7);
	printd (LogDebug, "Path %s %s\n", gMyInfo.BasePath, FileName);
	webkit_web_view_load_uri (web_view, FileName);

	WebKitSettings *settings = webkit_settings_new ();
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
		g_object_set(G_OBJECT(settings), "default-font-size", 28, NULL);
		g_object_set(G_OBJECT(settings), "default-monospace-font-size", 28, NULL);
	}

	printd(LogDebug, "Settings for webkit\n");
	webkit_settings_set_enable_media_stream(G_OBJECT(settings), FALSE);
	webkit_settings_set_enable_mediasource(G_OBJECT(settings), FALSE);
	webkit_settings_set_enable_fullscreen(G_OBJECT(settings), TRUE);
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
	gtk_widget_show_all(ChartGTKView);
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
	float FValue;

	/* Start at zero song sections.
	*/
	NumberSongMarks = 0;

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

		/* Look for Presets.
		 */
		Found = strstr(temp, "Preset");
		if (Found != NULL) {
			Value = Found[6] - '1';
//			printf("We are in Presets [%d]\n", Value);

			/* skip 8  "preset1 "	*/
			Found += (8 + ContentTagLen);

			/* Validate the index before using it.
			*/
			if (Value >= 0 && Value < 10) {
				thePresets->thePreset[Value] = AssignPreset(Value + 1, Found);
//				printf("Preset%d %d\n", Value, thePresets->thePreset[Value]);
				printd(LogDebug, "Preset%d %d\n", Value, thePresets->thePreset[Value]);
				strncpy(temp, Copy, MAXLINE);
			}
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
			gMyInfo.Tempo = Value;
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
			printd(LogDebug, "LoopFile %s\n", tokenizer);

			strcpy(gMyInfo.LoopFileName, BasePathName);
			strcat(gMyInfo.LoopFileName, tokenizer);

			printd(LogDebug, "LoopFile name %s\n", gMyInfo.LoopFileName);
			MyOSCLoadFile(gMyInfo.LoopFileName);
			strncpy(temp, Copy, MAXLINE);

		}

		/* Get the Drum File patch.
		 */
		Found = strstr(temp, "DrumFile");
		if (Found != NULL) {
			Found += 10 + ContentTagLen;
			String = Found;
			tokenizer = strtok(String, "\""); //break up by spaces
			printd(LogDebug, "DrumFile %s\n", tokenizer);
			strcpy(DrumFile, tokenizer);
			strncpy(temp, Copy, MAXLINE);
			if (DrumFile[0] == 62)
				DrumFile[0] = 0;

			if (!strcmp(DrumFile, "/dev/null")) {
				DrumFile[0] = 0;
			}
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


	/*
	 * Check to see if we have requested a new file for the drum or the looper.
	 */
	if (DrumFile[0] || LoopFile[0]) {
		/*
		 * Make sure the the looper is the second argument even of the drum file is the same.
		 */
		sprintf(Copy, "MusicApp.sh DrumFile %s & ", DrumFile);
		printd(LogDebug, "Calling System with %s\n", Copy);
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

	printd(LogDebug, "AssignPreset %d %s\n", PresetNum, String);

	/* If we Start with a quote it's a name lookup.
	 */
	if (*String == '"') {
		String++;
		tokenizer = strtok(String, "\""); //break up by spaces
		printd(LogDebug, "Token1 [%s]\n", tokenizer);
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


	if (PresetNum == 0)
		DoPatch(&gMyInfo.MyPatchInfo[Value]);

	if (PresetNum > 0 && PresetNum < 9) {
		printd(LogDebug, "*********PresetNum Case %d\n", PresetNum);
		SetPatchTitles(&PresetButtons[PresetNum-1], gMyInfo.MyPatchInfo[Value].Name, PresetNum);
	}

#if 0
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
#endif
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
#if 0
if (Amount == ScrollPgDn)
	if ((VIncrement + Value) >= UpperV) {
		gtk_adjustment_set_value(Adjust, 0);
		printd(LogDebug, "ScrollDown Rolling Over\n");
	} else {
		gtk_adjustment_set_value(Adjust, VIncrement + Value);
	}

if (Amount == ScrollPgUp)
	if ((Value - VIncrement) < 0) {
		gtk_adjustment_set_value(Adjust, 0);
		printd(LogDebug, "ScrollDown Rolling Over\n");
	} else {
		gtk_adjustment_set_value(Adjust, Value - VIncrement);
	}

if (Amount >= 0) {
	gtk_adjustment_set_value(Adjust, Amount);

}

gtk_adjustment_changed(Adjust);
gtk_scrolled_window_set_vadjustment (GTK_SCROLLED_WINDOW(ChartGTKView), Adjust);
#endif
#if 0
// Scroll bar fix to add GtkViewport
GtkWidget *viewport;
printf("Adjustment %d %d\n", gtk_widget_get_margin_left (scrolled_window),
       gtk_widget_get_margin_right (scrolled_window));
viewport =
    gtk_viewport_new (gtk_scrolled_window_get_hadjustment (scrolled_window),
                      gtk_scrolled_window_get_vadjustment (scrolled_window));
gtk_container_set_focus_hadjustment (GTK_CONTAINER (viewport),
                                     gtk_scrolled_window_get_hadjustment (GTK_SCROLLED_WINDOW (scrolled_window)));
gtk_container_set_focus_vadjustment (GTK_CONTAINER (viewport),
                                     gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (scrolled_window)));
//      gtk_container_add (GTK_CONTAINER (scrolled_window), viewport);
//	gtk_scrolled_window_set_max_content_width()
gtk_scrolled_window_add_with_viewport (GTK_CONTAINER (scrolled_window), viewport);


#endif


#if 0
GtkAdjustment *Adjust;
gdouble Value;
Adjust = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *)ChartGTKView);
gtk_adjustment_set_page_size(Adjust, 100);
Value = gtk_adjustment_get_value(Adjust);
printf("Value %f\n", Value);
return ((float)Value);
#endif

// xdotool windowactivate 100663307 ; xdotool key Page_Up
//web_view
//ChartGTKView
#if 0
Adjust = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *)ChartGTKView);
gtk_adjustment_set_page_size(Adjust, 100);

UpperV = gtk_adjustment_get_upper(Adjust);
gtk_adjustment_set_page_size(Adjust, 100);
gtk_adjustment_set_page_increment(Adjust, UpperV / 4);
VIncrement = gtk_adjustment_get_page_increment(Adjust);
Value = gtk_adjustment_get_value(Adjust);
printd(LogDebug, "In ScrollDown %f  %f %f\n", UpperV, VIncrement, Value);
printf("In ScrollDown %f  %f %f\n", UpperV, VIncrement, Value);
printf("Page Size %f\n", gtk_adjustment_get_value(Adjust));
#endif
