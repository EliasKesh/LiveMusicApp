#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <webkit/webkitwebview.h>
#include <glade/glade.h>
#include "GTKMidiUI.h"

int Search_in_File(char *fname, WebLoadPresets *thePresets);
void SetPatchTitles(GtkWidget	*myButton, char *Text );

static WebKitWebView* web_view;

GtkWidget	*Preset1Button;
GtkWidget	*Preset2Button;
GtkWidget	*scrolled_window;


void on_Back_clicked(GtkButton * 	button, gpointer 	user_data )
   {
		webkit_web_view_set_editable( web_view, false);
	   webkit_web_view_go_back(web_view);
	   g_print("Back:\n");

   }

void on_Forward_clicked(GtkButton * 	button, gpointer 	user_data ) {

	   webkit_web_view_go_forward(web_view);
	   g_print("Forward:\n");
   }

#if 1
void on_toolbutton3_clicked( GtkWidget *widget, gpointer data ) {
 gchar *CurrentURI; 
 
 //	 webkit_web_view_reload(web_view);
		CurrentURI = webkit_web_view_get_uri(web_view);
	   g_print("Current %s\n",CurrentURI);

   }

void on_patch1_clicked( GtkWidget *widget,gpointer   data ) {
char	Preset;

printf("In Button Preset1 %d %s\n",Preset, gMyInfo.MyPatchInfo[Preset].Name );

	Preset = gMyInfo.WebPresets.thePreset1;
	if (Preset != -1)
		DoPatch(&gMyInfo.MyPatchInfo[Preset]);
}

void on_patch2_clicked( GtkWidget *widget,gpointer   data ) {
char	Preset;

printf("In Button Preset2 %d %s\n",Preset, gMyInfo.MyPatchInfo[Preset].Name );

	Preset = gMyInfo.WebPresets.thePreset2;
	if (Preset != -1)
		DoPatch(&gMyInfo.MyPatchInfo[Preset]);
}

void on_SaveWeb_clicked( GtkWidget *widget, gpointer data ) {
 gchar *CurrentURI; 
 char	*Buffer;
 GString* theBuffer;
WebKitWebFrame *theFrame;
WebKitWebDataSource *theData;

		CurrentURI = webkit_web_view_get_uri(web_view);
//		Buffer = webkit_web_data_source_get_data(web_view);
	   g_print("Save %s\n",CurrentURI);
	   g_print("WebKit %x %d\n",web_view->parent_instance.widget.window,
			gtk_text_get_length(web_view->parent_instance.widget.window ));
//	   g_print("Save %d\n",gtk_text_get_length(web_view->parent_instance.window ));
//		webkit_web_frame_get_global_context(theFrame);
		webkit_web_view_execute_script(web_view, "document.title=document.documentElement.innerHTML;");
		theFrame = webkit_web_view_get_main_frame (web_view);
//		theBuffer = webkit_web_view_get_title (web_view);

		theBuffer = webkit_web_frame_get_title(theFrame);
//		theBuffer = webkit_web_frame_get_uri(theFrame);
//		theData = webkit_web_frame_get_data_source(theFrame);
//		theBuffer = webkit_web_data_source_get_data(theData);


		printf("Web %x %x %x\n", theFrame, theData, Buffer);
//		printf("%d %s\n",theBuffer->len, theBuffer->str);
		printf("%s\n",theBuffer);
// web_view->priv.backingstore.m_ptr
// web_view->parent_instance.widget
// web_view->parent_instance.window
#if 0
		webkit_web_view_save_to_file( web_view,
					CurrentURI,
					WEBKIT_SAVE_MODE_MHTML,
					NULL,
					NULL,
					NULL );
#endif
   }

void PageLoaded( GtkWidget *widget,gpointer   data ) {
gchar *CurrentURI; 
char *Pointer;

	CurrentURI = webkit_web_view_get_uri(web_view);
	printf("load_status_cb %s\n",CurrentURI);
	
	Pointer = strstr(CurrentURI, ".html");
	printf("Pointer %x\n",Pointer);

	if ( strstr(CurrentURI, ".html") )
		Search_in_File(CurrentURI, &gMyInfo.WebPresets);
	else
		printf("Not HTML File\n");

}

#endif
#if 0
int main (int argc, char *argv[])
{
	GtkBuilder *builder;
	GtkWidget  *window;
	GtkWidget  *scrolled_window;
	GtkWidget  *statusbar1;
	GtkWidget  *toolbutton1;
	GtkWidget  *toolbutton2;
	GtkWidget  *toolbutton3;
	GtkWidget  *toolbutton4;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "browser.glade", NULL);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
	scrolled_window = GTK_WIDGET (gtk_builder_get_object (builder, "scrolledwindow1"));
	statusbar1 = GTK_WIDGET (gtk_builder_get_object (builder, "statusbar1"));
	toolbutton1 = GTK_WIDGET (gtk_builder_get_object (builder, "toolbutton1"));
	toolbutton2 = GTK_WIDGET (gtk_builder_get_object (builder, "toolbutton2"));
	toolbutton3 = GTK_WIDGET (gtk_builder_get_object (builder, "toolbutton3"));
	toolbutton4 = GTK_WIDGET (gtk_builder_get_object (builder, "toolbutton4"));

	gtk_window_set_title(GTK_WINDOW(window), "Example Web Browser");

	web_view = WEBKIT_WEB_VIEW (webkit_web_view_new ());
	gtk_container_add (GTK_CONTAINER (scrolled_window), GTK_WIDGET (web_view));

//   gtk_builder_connect_signals (builder, NULL);

	g_object_unref (G_OBJECT (builder));

	gchar* uri = (gchar*) (argc > 1 ? argv[1] : "http://www.google.com");
	webkit_web_view_open (web_view, uri);

//	webkit_web_view_execute_script(WEBKIT_WEB_VIEW (web_view), "var t=setTimeout(\"alert('5 seconds!')\",5000);");

	gtk_statusbar_push (GTK_STATUSBAR(statusbar1), 0, "Done");

	g_signal_connect (G_OBJECT (toolbutton1), "clicked",
		G_CALLBACK (on_toolbutton1_clicked), NULL);

	g_signal_connect (G_OBJECT (toolbutton2), "clicked",
		G_CALLBACK (on_toolbutton2_clicked), NULL);

	g_signal_connect (G_OBJECT (toolbutton3), "clicked",
		G_CALLBACK (on_toolbutton3_clicked), NULL);

	g_signal_connect (G_OBJECT (toolbutton4), "clicked",
		G_CALLBACK (on_toolbutton4_clicked), NULL);

	gtk_widget_grab_focus (GTK_WIDGET (web_view));
	gtk_widget_show_all (window);

	gtk_main ();

	return 0;
}
#endif

void InitHTML(GladeXML *gxml) {
char	FileName[255];
GtkWidget *Widget;


	Preset1Button = GTK_WIDGET (gtk_builder_get_object (gxml, "Patch1") );
			g_signal_connect (G_OBJECT (Preset1Button), "clicked", 
			G_CALLBACK (on_patch1_clicked), NULL);
			
	Preset2Button = GTK_WIDGET (gtk_builder_get_object (gxml, "Patch2") );
			g_signal_connect (G_OBJECT (Preset2Button), "clicked", 
			G_CALLBACK (on_patch2_clicked), NULL);
			
	printf("HTML %x %x\n", Preset1Button, Preset2Button);
	scrolled_window = GTK_WIDGET (gtk_builder_get_object  (gxml, "scrolledwindow1") );

	//	gtk_widget_set_name (scrolled_window, "GtkLauncher");
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	web_view = WEBKIT_WEB_VIEW (webkit_web_view_new ());
	gtk_container_add (GTK_CONTAINER (scrolled_window), GTK_WIDGET (web_view));
	
	webkit_web_view_set_editable(web_view, TRUE);
	
	Widget = GTK_WIDGET (gtk_builder_get_object (gxml, "BackButton") );
			g_signal_connect (G_OBJECT (Widget), "clicked", 
			G_CALLBACK (on_Back_clicked), NULL);

	Widget = GTK_WIDGET (gtk_builder_get_object (gxml, "ForwardButton") );
			g_signal_connect (G_OBJECT (Widget), "clicked", 
			G_CALLBACK (on_Forward_clicked), NULL);

	Widget = GTK_WIDGET (gtk_builder_get_object (gxml, "ProcessButton") );
			g_signal_connect (G_OBJECT (Widget), "clicked", 
			G_CALLBACK (on_toolbutton3_clicked), NULL);

	Widget = GTK_WIDGET (gtk_builder_get_object (gxml, "SaveWeb") );
			g_signal_connect (G_OBJECT (Widget), "clicked", 
			G_CALLBACK (on_SaveWeb_clicked), NULL);

    /* Register a callback that gets invoked each time that a page is finished downloading */
    g_signal_connect(web_view, "load-finished", G_CALLBACK(PageLoaded), NULL);

//	gchar* uri = (gchar*) ("/home/Dropbox/FusionBlue/ChartsHTML/indexCharts.html");

	strncpy(FileName, gMyInfo.BasePath, 254 );
	strcat(FileName, "/indexCharts.html");
	webkit_web_view_open (web_view, FileName);
//	gtk_widget_grab_focus (GTK_WIDGET (web_view));
	gtk_widget_show_all (scrolled_window);

}


void SetPatchTitles(GtkWidget	*myButton, char *Text ) {
	gtk_button_set_label(myButton,Text);
}

#define ParseValue "Preset"
#define MAXLINE 250

int Search_in_File(char *fname, WebLoadPresets *thePresets) {
	FILE *fp;
	char temp[MAXLINE];
	char *Found;
	char	Value;
	int		Count = 0;
	/* Get passed the file://
	 */
	fname+=7;
	if((fp = fopen( fname, "r")) == NULL) {
		return(-1);
	}
	
	thePresets->thePreset1 = -1;
	thePresets->thePreset2 = -1;
	thePresets->theTempo = -1;

printf("Have file %x %s\n", fp, fname);
	while(fgets(temp, MAXLINE - 1, fp) != NULL && (++Count < 150) ) {
		temp[MAXLINE] = 0;

		/* Set up Preset 1 button.
		 */
		Found = strstr(temp, "Preset1");
		if(Found != NULL) {
			Found += 8;
			Value = atoi(Found);
			if (Value >= Max_Patches)
				Value = 0;
				
			thePresets->thePreset1 = Value;
			SetPatchTitles(Preset1Button, gMyInfo.MyPatchInfo[Value].Name);
			printf("Preset1 %d %s\n",Value, gMyInfo.MyPatchInfo[Value].Name);
		}

		/* Set up Preset 2 Button.
		 */
		Found = strstr(temp, "Preset2");
		if(Found != NULL) {
			Found += 8;
			Value = atoi(Found);
			if (Value >= Max_Patches)
				Value = 0;
			thePresets->thePreset2 = Value;
			SetPatchTitles(Preset2Button, gMyInfo.MyPatchInfo[Value].Name);
			printf("Preset2 %d %s\n",Value, gMyInfo.MyPatchInfo[Value].Name);
		}

		/* Set the Tempo for this tune.
		 */
		Found = strstr(temp, "Tempo");
		if(Found != NULL) {
			Found += 6;
			Value = atoi(Found);
			thePresets->theTempo = Value;
			SetTempo(Value);
			printf("Tempo %d\n",Value);
		}

		/* Set the current patch to this one.
		 */
		Found = strstr(temp, "SetNow");
		if(Found != NULL) {
			Found += 7;
			Value = atoi(Found);
			if (Value >= Max_Patches)
				Value = 0;
			printf("SetNow %d\n",Value);
			DoPatch(&gMyInfo.MyPatchInfo[Value]);
		}

	}
	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
   	return(0);
}

