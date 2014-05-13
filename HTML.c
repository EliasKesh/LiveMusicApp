#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <webkit/webkitwebview.h>
#include <glade/glade.h>
#include "GTKMidiUI.h"

int Search_in_File(const char *fname, WebLoadPresets *thePresets);
void SetPatchTitles(GtkWidget	*myButton, char *Text );

static WebKitWebView* web_view;

GtkWidget	*Preset1Button;
GtkWidget	*Preset2Button;
GtkWidget	*scrolled_window;

/*--------------------------------------------------------------------
* Function:		on_Back_clicked
*
* Description:	Web browser back button
*
*---------------------------------------------------------------------*/
void on_Back_clicked(GtkButton * 	button, gpointer 	user_data )
   {
		webkit_web_view_set_editable( web_view, false);
	   webkit_web_view_go_back(web_view);
	   g_print("Back:\n");

   }

/*--------------------------------------------------------------------
* Function:		on_Forward_clicked
*
* Description:	Web browser forward button
*
*---------------------------------------------------------------------*/
void on_Forward_clicked(GtkButton * 	button, gpointer 	user_data ) {

	   webkit_web_view_go_forward(web_view);
	   g_print("Forward:\n");
   }


/*--------------------------------------------------------------------
* Function:		on_Forward_clicked
*
* Description:	Web browser forward button
*
*---------------------------------------------------------------------*/
void on_toolbutton3_clicked( GtkWidget *widget, gpointer data ) {
 const gchar *CurrentURI; 
 
 //	 webkit_web_view_reload(web_view);
		CurrentURI = webkit_web_view_get_uri(web_view);
	   g_print("Current %s\n",CurrentURI);

   }

/*--------------------------------------------------------------------
* Function:		Patch 1 Selected
*
* Description:	The users patch 1 was selected.
*
*---------------------------------------------------------------------*/
void on_patch1_clicked( GtkWidget *widget,gpointer   data ) {
char	Preset;

printf("In Button Preset1 %d %s\n",Preset, gMyInfo.MyPatchInfo[Preset].Name );

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
void on_patch2_clicked( GtkWidget *widget,gpointer   data ) {
char	Preset;

printf("In Button Preset2 %d %s\n",Preset, gMyInfo.MyPatchInfo[Preset].Name );

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
void on_SaveWeb_clicked( GtkWidget *widget, gpointer data ) {
const gchar *CurrentURI; 
const char	*Buffer;
GString* theBuffer;
WebKitWebFrame *theFrame;
FILE *fp;

	CurrentURI = webkit_web_view_get_uri(web_view);
	g_print("Save %s\n",CurrentURI);
	webkit_web_view_execute_script(web_view, "document.title=document.documentElement.innerHTML;");
	theFrame = webkit_web_view_get_main_frame (web_view);
	Buffer = webkit_web_frame_get_title(theFrame);
	printf("Len = %d\n %s\n", (int)strlen(Buffer), Buffer);
	/* Get passed the file://
	 */
	CurrentURI+=7;
	if((fp = fopen( CurrentURI, "w")) == NULL) {
		g_print("File open Failed %s\n",CurrentURI);
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
void PageLoaded( GtkWidget *widget,gpointer   data ) {
const gchar *CurrentURI; 
char *Pointer;

	CurrentURI = webkit_web_view_get_uri(web_view);
	printf("load_status_cb %s\n",CurrentURI);
	
	Pointer = strstr(CurrentURI, ".html");
//	printf("Pointer %x\n",(unsigned int)Pointer);

	if ( strstr(CurrentURI, ".html") )
		Search_in_File(CurrentURI, &gMyInfo.WebPresets);
	else
		printf("Not HTML File\n");

}

/*--------------------------------------------------------------------
* Function:		InitHTML
*
* Description:	Set up the WebKit environment.
*
*---------------------------------------------------------------------*/
void InitHTML(GladeXML *gxml) {
char	FileName[255];
GtkWidget *Widget;

	/* Load the buttons and set the callbacks for them.
	 */
	Preset1Button = GTK_WIDGET (gtk_builder_get_object (gxml, "Patch1") );
			g_signal_connect (G_OBJECT (Preset1Button), "clicked", 
			G_CALLBACK (on_patch1_clicked), NULL);
			
	Preset2Button = GTK_WIDGET (gtk_builder_get_object (gxml, "Patch2") );
			g_signal_connect (G_OBJECT (Preset2Button), "clicked", 
			G_CALLBACK (on_patch2_clicked), NULL);
			
	scrolled_window = GTK_WIDGET (gtk_builder_get_object  (gxml, "scrolledwindow1") );

	//	gtk_widget_set_name (scrolled_window, "GtkLauncher");
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), 
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	/* Create a new webkit view to display our data.
	 */
	web_view = WEBKIT_WEB_VIEW (webkit_web_view_new ());
	gtk_container_add (GTK_CONTAINER (scrolled_window), GTK_WIDGET (web_view));

	/* Make the view editable.
	 */
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

	strncpy(FileName, gMyInfo.BasePath, 254 );
	strcat(FileName, "/indexCharts.html");
	webkit_web_view_open (web_view, FileName);

//	gtk_widget_grab_focus (GTK_WIDGET (web_view));
	gtk_widget_show_all (scrolled_window);

}

/*--------------------------------------------------------------------
* Function:		SetPatchTitles
*
* Description:	When we load patched, rename the buttons.
*
*---------------------------------------------------------------------*/
void SetPatchTitles(GtkWidget	*myButton, char *Text ) {
	gtk_button_set_label(myButton,Text);
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

//printf("Have file %x %s\n", fp, fname);
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

