#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <webkit/webkitwebview.h>
#include <glade/glade.h>
#include "GTKMidiUI.h"

int Search_in_File(const char *fname, WebLoadPresets *thePresets);
int ScalePage(void);
tPatchIndex	AssignPreset(int PresetNum, char *String);
void SetPatchTitles(GtkWidget *MyButton, char *Text);

static WebKitWebView* web_view;

GtkWidget *Preset1Button;
GtkWidget *Preset2Button;
GtkWidget *scrolled_window;

/*--------------------------------------------------------------------
 * Function:		on_Back_clicked
 *
 * Description:	Web browser back button
 *
 *---------------------------------------------------------------------*/
void on_Back_clicked(GtkButton * button, gpointer user_data) {
//		webkit_web_view_set_editable( web_view, false);
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

    webkit_web_view_go_forward(web_view);
    g_print("Forward:\n");
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
    WebKitViewportAttributes* attributes;

//	 webkit_web_view_reload(web_view);
    attributes = webkit_web_view_get_viewport_attributes(web_view);

    Adjust = gtk_scrolled_window_get_hadjustment(scrolled_window);
    UpperH = gtk_adjustment_get_upper(Adjust);

    Adjust = gtk_scrolled_window_get_vadjustment(scrolled_window);
    UpperV = gtk_adjustment_get_upper(Adjust);

    g_object_get(G_OBJECT(attributes), "available-width", &Horiz, NULL);
    g_object_get(G_OBJECT(attributes), "available-height", &Vert, NULL);

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
return(0);
}

#if 0
gint WebPageHeight;
gint WebPageWidth;
gint ScreenHeight;
gint ScreenWidth;
gint AvailHeight;
gint AvailWidth;
gfloat ScaleSizeW;
gfloat ScaleSizeH;
gboolean UserScale;
gint WindowX,WindowY;
GtkAdjustment *Adjust;
GtkAllocation allocation;
gint Upper;
gtk_widget_get_allocation(GTK_WIDGET(scrolled_window), &allocation);
printd(LogInfo, "Allocation %d %d %d %d\n",
       allocation.x, allocation.y, allocation.width, allocation.height);

//	gtk_widget_get_size_request(scrolled_window, &WindowX, &WindowY);
//	printd(LogInfo, "****************Widget %d %d\n", WindowX, WindowY);
Adjust = gtk_scrolled_window_get_hadjustment(scrolled_window);

Upper = gtk_adjustment_get_upper(Adjust);
printd(LogInfo, "****************Widget %x %x %f %f %f  %f  %f\n", scrolled_window,
       Adjust, Adjust->page_size, Adjust->upper, Adjust->page_increment,
       Adjust, Adjust->lower, Adjust->step_increment);

WebKitViewportAttributes* attributes =
    webkit_web_view_get_viewport_attributes (web_view);
g_object_get (G_OBJECT (attributes), "available-height", &AvailHeight, NULL);
g_object_get (G_OBJECT (attributes), "available-width", &AvailWidth, NULL);
printd(LogInfo, "****************InitHTMLA %d %d\n", AvailHeight, AvailWidth);

#if 1
g_object_get (G_OBJECT (attributes), "device-height", &ScreenHeight, NULL);
g_object_get (G_OBJECT (attributes), "device-width", &ScreenWidth, NULL);
printd(LogInfo, "****************InitHTMLD %d %d\n", ScreenHeight, ScreenWidth);
g_object_get (G_OBJECT (attributes), "height", &WebPageHeight, NULL);
g_object_get (G_OBJECT (attributes), "width", &WebPageWidth, NULL);
printd(LogInfo, "****************InitHTMLW %d %d\n", WebPageHeight, WebPageWidth);
#endif

#if 0
g_object_set (G_OBJECT (attributes), "available-height", WebPageHeight, NULL);
printd(LogInfo, "Page Loaded %d\n", WebPageHeight);
g_object_set (G_OBJECT (attributes), "available-width", WebPageWidth, NULL);
printd(LogInfo, "Page Loaded %d\n", WebPageWidth);
#endif

if (AvailWidth > 200 && Upper != 0)
    ScaleSizeW = ((gfloat)(ScreenWidth - 150)/(gfloat)Upper);
else
    ScaleSizeW = 1;

if (AvailHeight > 200 )
    ScaleSizeH = (gfloat)ScreenHeight/(gfloat)AvailHeight;
else
    ScaleSizeH = 1;

printd(LogInfo, "Setting Scale %f W%d S%d D%d\n", ScaleSizeW, WebPageWidth, ScreenWidth, AvailWidth);
printd(LogInfo, "Setting Scale %f W%d S%d D%d\n", ScaleSizeH, WebPageHeight, ScreenHeight, AvailHeight);
//	ScaleSize =  (gfloat)WebPageHeight/(gfloat)ScreenHeight;
printd(LogInfo, "Current Zoom %f\n", webkit_web_view_get_zoom_level(web_view));
webkit_web_view_set_zoom_level (web_view, ScaleSizeW );
//	webkit_web_view_set_zoom_level (web_view,MIN(ScaleSizeW,ScaleSizeH) );
//	webkit_web_view_set_zoom_level (web_view,MIN(ScaleSizeW,.85) );

g_object_get (G_OBJECT (attributes), "user-scalable", &UserScale, NULL);
printd(LogInfo, "User-scable %d\n", UserScale);
//	webkit_web_view_set_viewport_attributes (web_view, attributes);
//	webkit_viewport_attributes_recompute (attributes);
#endif

/*--------------------------------------------------------------------
 * Function:		on_Forward_clicked
 *
 * Description:	Web browser forward button
 *
 *---------------------------------------------------------------------*/
void on_toolbutton3_clicked(GtkWidget *widget, gpointer data) {
    const gchar *CurrentURI;

//	 webkit_web_view_reload(web_view);
    CurrentURI = webkit_web_view_get_uri(web_view);
    g_print("Current %s\n", CurrentURI);
    ScalePage();
}

/*--------------------------------------------------------------------
 * Function:		Patch 1 Selected
 *
 * Description:	The users patch 1 was selected.
 *
 *---------------------------------------------------------------------*/
void on_patch1_clicked(GtkWidget *widget, gpointer data) {
    char Preset;

    printd(LogInfo, "In Button Preset1 %d %s\n", Preset,
           gMyInfo.MyPatchInfo[Preset].Name);

    Preset = gMyInfo.WebPresets.thePreset1;
    if (Preset != -1)
        DoPatch(&gMyInfo.MyPatchInfo[Preset]);
}

/*--------------------------------------------------------------------
 * Function:		Patch 2 Selected
 *
 * Description:	The users patch 2 was selected.
 *
 *---------------------------------------------------------------------*/
void on_patch2_clicked(GtkWidget *widget, gpointer data) {
    char Preset;

    printd(LogInfo, "In Button Preset2 %d %s\n", Preset,
           gMyInfo.MyPatchInfo[Preset].Name);

    Preset = gMyInfo.WebPresets.thePreset2;
    if (Preset != -1)
        DoPatch(&gMyInfo.MyPatchInfo[Preset]);
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
    WebKitWebFrame *theFrame;
    FILE *fp;

    CurrentURI = webkit_web_view_get_uri(web_view);
    g_print("Save %s\n", CurrentURI);
    webkit_web_view_execute_script(web_view,
                                   "document.title=document.documentElement.innerHTML;");
    theFrame = webkit_web_view_get_main_frame(web_view);
    Buffer = webkit_web_frame_get_title(theFrame);
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
    else if  (strstr(CurrentURI, ".mp3")) {
    	printd(LogInfo, "*** MP3 file.\n");
    	return;
    }
    else {
    	printd(LogInfo, "Not HTML File\n");
        return;
    }

    printf("******** WebKit Set Edit %d\n",webkit_web_view_get_editable(web_view));
    webkit_web_view_set_zoom_level(web_view, 1);

}
void load_finished_cb(WebKitWebView *web_view, WebKitWebFrame *web_frame,
                      gpointer data) {
    printd(LogInfo, "Finished downloading %s\n", webkit_web_view_get_uri(web_view));
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
#if 0
gboolean
DownloadRequestcb (WebKitWebView  *web_view,
               WebKitDownload *download,
               gpointer        user_data) {

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
gboolean
NavigationPolicy (WebKitWebView             *web_view,
               WebKitWebFrame            *frame,
               WebKitNetworkRequest      *request,
               WebKitWebNavigationAction *navigation_action,
               WebKitWebPolicyDecision   *policy_decision,
               gpointer                   user_data) {
char	*theURI;
char	string[150];

	theURI = webkit_web_navigation_action_get_original_uri(navigation_action);

//	uri=networkRequest.get_uri()

    printd(LogInfo, "NavigationPolicy %s \n", theURI);
    if  (strstr(theURI, ".mp3")) {
    	webkit_web_policy_decision_ignore(policy_decision);
    	sprintf(string,"/usr/bin/playitslowly --sink=jackaudiosink %s", theURI);
    	system(string);
    	printd(LogInfo, "*** systemcall %s\n", string);
    	return(true);
    }
    webkit_web_policy_decision_use(policy_decision);
 //   webkit_web_policy_decision_ignore(policy_decision);
return(false);
}

/*--------------------------------------------------------------------
 * Function:		InitHTML
 *
 * Description:	Set up the WebKit environment.
 *
 *---------------------------------------------------------------------*/
void InitHTML(GladeXML *gxml) {
    char FileName[255];
    GtkWidget *Widget;

    /* Load the buttons and set the callbacks for them.
     */
    Preset1Button = GTK_WIDGET(gtk_builder_get_object(gxml, "Patch1"));
    g_signal_connect(G_OBJECT (Preset1Button), "clicked",
                     G_CALLBACK (on_patch1_clicked), NULL);

    Preset2Button = GTK_WIDGET(gtk_builder_get_object(gxml, "Patch2"));
    g_signal_connect(G_OBJECT (Preset2Button), "clicked",
                     G_CALLBACK (on_patch2_clicked), NULL);

    scrolled_window = GTK_WIDGET(
                          gtk_builder_get_object(gxml, "scrolledwindow1"));

    //	gtk_widget_set_name (scrolled_window, "GtkLauncher");
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    /* Create a new webkit view to display our data.
     */
    web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(web_view));

    /* Make the view editable.
     */
    webkit_web_view_set_editable(web_view, TRUE);

    /* We should scale everything.
     */
    webkit_web_view_set_full_content_zoom(web_view, TRUE);

    Widget = GTK_WIDGET(gtk_builder_get_object(gxml, "BackButton"));
    g_signal_connect(G_OBJECT (Widget), "clicked", G_CALLBACK (on_Back_clicked),
                     NULL);

    Widget = GTK_WIDGET(gtk_builder_get_object(gxml, "ForwardButton"));
    g_signal_connect(G_OBJECT (Widget), "clicked",
                     G_CALLBACK (on_Forward_clicked), NULL);

    Widget = GTK_WIDGET(gtk_builder_get_object(gxml, "ProcessButton"));
    g_signal_connect(G_OBJECT (Widget), "clicked",
                     G_CALLBACK (on_toolbutton3_clicked), NULL);

    Widget = GTK_WIDGET(gtk_builder_get_object(gxml, "SaveWeb"));
    g_signal_connect(G_OBJECT (Widget), "clicked",
                     G_CALLBACK (on_SaveWeb_clicked), NULL);

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
    g_signal_connect(web_view, "navigation-policy-decision-requested", G_CALLBACK(NavigationPolicy), NULL);



    strncpy(FileName, gMyInfo.BasePath, 254);
 //   strcat(FileName, "/indexCharts.html");
    webkit_web_view_open(web_view, FileName);
#if 0

    WebKitWebSettings *settings = webkit_web_settings_new ();
    g_object_set (G_OBJECT(settings), "auto-shrink-images", FALSE, NULL);

    /* Apply the result */
    webkit_web_view_set_settings (WEBKIT_WEB_VIEW(web_view), settings);
#endif
  create_Popup_view(web_view);

//	gtk_widget_grab_focus (GTK_WIDGET (web_view));
    gtk_widget_show_all(scrolled_window);

}


#define ParseValue "Preset"
#define MAXLINE 250

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
    char	*tokenizer;
    int Count = 0;

    /* Get passed the file://
     */
    fname += 7;
    if ((fp = fopen(fname, "r")) == NULL) {
        return (-1);
    }

    thePresets->thePreset1 = -1;
    thePresets->thePreset2 = -1;
    thePresets->theTempo = -1;

//printd(LogInfo, "Have file %x %s\n", fp, fname);
    while (fgets(temp, MAXLINE - 1, fp) != NULL && (++Count < 150)) {
        temp[MAXLINE] = 0;

        strncpy(Copy, temp, MAXLINE);
        /* Set up Preset 1 button.
         */
        Found = strstr(temp, "Preset1");
        if (Found != NULL) {
            /* skip 8  "preset1 "	*/
            Found += 8;
            thePresets->thePreset1 = AssignPreset(1, Found);
            strncpy(temp, Copy, MAXLINE);

        }

        /* Set up Preset 2 Button.
         */
        Found = strstr(temp, "Preset2");
        if (Found != NULL) {
            Found += 8;
            thePresets->thePreset2 = AssignPreset(2, Found);
            strncpy(temp, Copy, MAXLINE);
        }

        /* Set the Tempo for this tune.
         */
        Found = strstr(temp, "Tempo");
        if (Found != NULL) {
            Found += 6;
            Value = atoi(Found);
            thePresets->theTempo = Value;
            SetTempo(Value);
            printd(LogInfo, "Tempo %d\n", Value);
        }

        /* Set the Tempo for this tune.
         */
        Found = strstr(temp, "LMA_Time");
        if (Found != NULL) {
            Found += 9;
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
            Found += 7;
            printd(LogInfo, "SetNow %d\n", Value);

            AssignPreset(0, Found);
        }

    }
    //Close the file if still open.
    if (fp) {
        fclose(fp);
    }
    return (0);
}

/*--------------------------------------------------------------------
 * Function:		AssignPreset
 *
 * Description:	Parse for the value of the preset given.
 *
 *---------------------------------------------------------------------*/
tPatchIndex	AssignPreset(int PresetNum, char *String) {
    int	Value;
    char	*tokenizer;

    printd(LogInfo,"AssignPreset %d %s\n\n",PresetNum, String);


    /* If we Start with a quote it's a name lookup.
     */
    if (*String == '"') {
        String++;
        tokenizer = strtok(String,"\"");//break up by spaces
        printd(LogInfo, "Token1 %s\n", tokenizer);
        for (Value = 0; Value < Max_Patches; Value++) {
            if ( !strcmp(gMyInfo.MyPatchInfo[Value].Name, tokenizer) )
                break;
        }

    } else {

        Value = atoi(String);
        printd(LogInfo,"***** Assign Value %s %d\n",String, Value);
        if (Value >= Max_Patches)
            Value = 0;
    }

    printd(LogInfo,"Preset %d %d %s\n",PresetNum, Value, gMyInfo.MyPatchInfo[Value].Name);

    if (Value < 1  || Value >= Max_Patches)
        return(Value);


    /* If it's a preset button or a set now.	*/
    switch(PresetNum) {
    case 0:
        DoPatch(&gMyInfo.MyPatchInfo[Value]);
        break;

    case 1:
        SetPatchTitles(Preset1Button, gMyInfo.MyPatchInfo[Value].Name);
        break;

    case 2:
        SetPatchTitles(Preset2Button, gMyInfo.MyPatchInfo[Value].Name);
        break;

    default:
        break;
    }

    return(Value);
}

/*--------------------------------------------------------------------
 * Function:		SetPatchTitles
 *
 * Description:	When we load patched, rename the buttons.
 *
 *---------------------------------------------------------------------*/
void SetPatchTitles(GtkWidget *MyButton, char *Text) {

    printd(LogInfo,"SetPatchTitles %x %s\n",MyButton, Text);

    gtk_button_set_label(MyButton, Text);
}
