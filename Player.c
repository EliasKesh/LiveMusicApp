/*---------------------------------------------------------------------
|
|	File: 	Player
|
|	Contains: 	
|
|
|	Written By: 	Elias Keshishoglou on Sun Mar 22 14:18:29 PDT 2015
|
|	Copyright �: 	2015 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#define Player_c

/*
 * Place Includes Here
 */

#include <gtk/gtk.h>
#include "GTKMidiUI.h"

#include "stdio.h"
#include "Player.h"



/*
 * Place defines and Typedefs here
 */


/*
 * Place Local prototypes here
 */
void PositionSlider_Changed(GtkAdjustment *adj);
gboolean SetA_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean SetB_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
void SetASlider_Changed(GtkAdjustment *adj);
void SetBSlider_Changed(GtkAdjustment *adj);
void SetAFineSlider_Changed(GtkAdjustment *adj);
void SetBFineSlider_Changed(GtkAdjustment *adj);
gboolean Play_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean Stop_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean Loop_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);

/*
 * Place Static variables here
 */
static FILE	*InPipe;
static FILE	*OutPipe;
int			InPipeFD;
int			OutPipeFD;
int		PlayPauseState = 0;
#define InPipeName "/tmp/LiveMusicIn"
#define OutPipeName "/tmp/LiveMusicOut"
GtkAdjustment *PositionAdjustment;
GtkAdjustment *StartAdjustment;
GtkAdjustment *EndAdjustment;
GtkAdjustment *FineStartAdjustment;
GtkAdjustment *FineEndAdjustment;
GtkWidget *PositionSlider;
GtkWidget *StartSpin, *EndSpin;
GtkWidget *FineStartSpin, *FineEndSpin;
float		TotalLength;
float		CurrentLength;
theImageButtons SetA;
theImageButtons SetB;
theImageButtons PlayPause;
theImageButtons StopButton;
theImageButtons LoopButton;
char 	DontUpDateSlider;
/*--------------------------------------------------------------------
 * Function:		LivePlayerInit
 *
 * Description:		Set up the Mplayer pipe and controls.
 *
 *---------------------------------------------------------------------*/
int LivePlayerInit(GtkWidget *MainWindow, GtkWidget *window) {
	GtkWidget *vbox;
	GtkWidget *PositionBox;
	GtkWidget *PlayControlBox;
	GtkWidget *PositionStartBox;
	GtkWidget *PositionEndBox;
	GtkWidget *EventBox;
	GtkWidget *EventBox1;
	GtkWidget *EventBox2;
	GtkWidget *EventBox3;
	GtkWidget *EventBox4;
	GtkWidget *FineABox;
	GtkWidget *FineBBox;
    int result;
    char		theString[300];

    system("killall mplayer");
     system("killall mplayer");
 	sprintf(theString, " rm  %s",OutPipeName);
	system(theString);
 	sprintf(theString, " rm  %s",InPipeName);
	system(theString);


	result = mkfifo(InPipeName, 0666);
	chmod(InPipeName, 0666);
	if (result < 0) {
		printf("Before fopen in %d %s\n", result,strerror(result));
	}

	OutPipe = popen("mplayer -slave -quiet -idle  -af scaletempo -idle >/tmp/LiveMusicIn " , "w");

	/*
	 * Start laying out the controls
	 */
//	GTK_ORIENTATION_VERTICAL
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	PositionBox= gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	PositionStartBox= gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	PositionEndBox= gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	FineABox= gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	FineBBox= gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	PlayControlBox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);

	/*
	 * Main Box
	 */
printf(" Init Player %x %x\n", vbox, window);
	PositionAdjustment =gtk_adjustment_new (0,0,400,1,20,0);
	PositionSlider = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(PositionAdjustment));
  //  gtk_scale_set_digits (PositionSlider, 1);
//    gtk_scale_set_value_pos (PositionSlider, GTK_POS_TOP);
    gtk_scale_set_draw_value (PositionSlider, TRUE);
	g_signal_connect(G_OBJECT (PositionSlider), "change_value",
		G_CALLBACK (PositionSlider_Changed), NULL);

	/*
	 * AB Set
	 */
	EventBox = gtk_event_box_new();
	MyImageButtonInit(&SetA, EventBox, MainButtonOnImage,	MainButtonOffImage);
	MyImageButtonSetText(&SetA, "Set A");
	g_signal_connect(G_OBJECT(EventBox),
		"button-press-event",
		G_CALLBACK(SetA_click_handler),
		&SetA);
	g_signal_connect(G_OBJECT(EventBox),
		"button-release-event",
		G_CALLBACK(normal_release_handler),
		&SetA);


	StartAdjustment = gtk_adjustment_new  ( 0.0,-1, 1, 0.001, 0.1, 0);
	StartSpin = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(StartAdjustment));
	g_signal_connect(G_OBJECT (StartSpin), "value_changed",
		G_CALLBACK (SetASlider_Changed), NULL);

	FineStartAdjustment = gtk_adjustment_new  (0,0,400,0.1,1,0);
	FineStartSpin = gtk_spin_button_new (FineStartAdjustment, 0.1,3);
	g_signal_connect(G_OBJECT (FineStartSpin), "value_changed",
		G_CALLBACK (SetAFineSlider_Changed), NULL);

	EventBox1 = gtk_event_box_new();
	MyImageButtonInit(&SetB, EventBox1, MainButtonOnImage,	MainButtonOffImage);
	MyImageButtonSetText(&SetB, "Set B");
	g_signal_connect(G_OBJECT(EventBox1),
		"button-press-event",
		G_CALLBACK(SetB_click_handler),
		&SetB);
	g_signal_connect(G_OBJECT(EventBox1),
		"button-release-event",
		G_CALLBACK(normal_release_handler),
		&SetB);


	EndAdjustment = gtk_adjustment_new ( 0.0,-1, 1, 0.001, 0.1, 0);
	EndSpin = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(EndAdjustment));
	g_signal_connect(G_OBJECT (EndSpin), "value_changed",
		G_CALLBACK (SetBSlider_Changed), NULL);

	FineEndAdjustment = gtk_adjustment_new (0,0,400,0.1,1,0);
	FineEndSpin = gtk_spin_button_new (FineEndAdjustment, 0.1,3);
	g_signal_connect(G_OBJECT (FineEndSpin), "value_changed",
		G_CALLBACK (SetBFineSlider_Changed), NULL);

	EventBox2 = gtk_event_box_new();
	MyImageButtonInit(&PlayPause, EventBox2 ,MainButtonOffImage, MainButtonOnImage);
	MyImageButtonSetText(&PlayPause, "Play");
	g_signal_connect(G_OBJECT(EventBox2),
		"button-press-event",
		G_CALLBACK(Play_click_handler),
		&PlayPause);


	EventBox3 = gtk_event_box_new();
	MyImageButtonInit(&StopButton, EventBox3, MainButtonOnImage,	MainButtonOffImage);
	MyImageButtonSetText(&StopButton, "Stop");
	g_signal_connect(G_OBJECT(EventBox3),
		"button-press-event",
		G_CALLBACK(Stop_click_handler),
		&StopButton);
	g_signal_connect(G_OBJECT(EventBox3),
		"button-release-event",
		G_CALLBACK(normal_release_handler),
		&StopButton);

	EventBox4 = gtk_event_box_new();
	MyImageButtonInit(&LoopButton, EventBox4, MainButtonOnImage,MainButtonOffImage);
	MyImageButtonSetText(&LoopButton, "Loop");
	g_signal_connect(G_OBJECT(EventBox3),
		"button-press-event",
		G_CALLBACK(Loop_click_handler),
		&LoopButton);
	g_signal_connect(G_OBJECT(EventBox3),
		"button-release-event",
		G_CALLBACK(normal_release_handler),
		&LoopButton);

	gtk_box_set_homogeneous(GTK_BOX (FineABox), TRUE);
	gtk_box_pack_start(GTK_BOX(FineABox), EventBox, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(FineABox), FineStartSpin, TRUE, TRUE, 5);

	gtk_box_set_homogeneous(GTK_BOX (FineBBox), TRUE);
	gtk_box_pack_start(GTK_BOX(FineBBox), EventBox1, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(FineBBox), FineEndSpin, TRUE, TRUE, 5);

	gtk_box_set_homogeneous(GTK_BOX (PositionStartBox), TRUE);
	gtk_box_pack_start(GTK_BOX(PositionStartBox), FineABox, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(PositionStartBox), StartSpin, TRUE, TRUE, 5);

	gtk_box_set_homogeneous(GTK_BOX  (PositionEndBox), TRUE);
	gtk_box_pack_start(GTK_BOX(PositionEndBox), FineBBox, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(PositionEndBox), EndSpin, TRUE, TRUE, 5);

	gtk_box_set_homogeneous(GTK_BOX (PositionBox), TRUE);
	gtk_box_pack_start(GTK_BOX(PositionBox), PositionStartBox, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(PositionBox), PositionEndBox, TRUE, TRUE, 5);

	gtk_box_pack_start(GTK_BOX(PlayControlBox), EventBox2, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), EventBox3, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), EventBox4, TRUE, TRUE, 5);


	gtk_box_set_homogeneous(GTK_BOX (vbox), TRUE);
	gtk_box_pack_start(GTK_BOX(vbox), PositionSlider, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), PositionBox, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), PlayControlBox, TRUE, TRUE, 5);


	gtk_container_add(GTK_CONTAINER(window), vbox);
//		  gtk_widget_set_size_request(window, 100, 100);
//	gtk_window_set_default_size(window, 200, 200);
	gtk_widget_show_all(window);


	/*
	 * Init the Pipe for communications with MPlayer
	 */
	InPipeFD=open(InPipeName,O_RDONLY | O_NONBLOCK);
	if (InPipeFD < 1) {
		int err = errno;
		printf("Error in open In pipe %d\n", errno);
		exit(1);
	}
}

/*--------------------------------------------------------------------
 * Function:		LivePlayerInit
 *
 * Description:		Set up the Mplayer pipe and controls.
 *
 *---------------------------------------------------------------------*/
int LivePlayerClose(void) {

	PlayerWrite("quit\n");

}
/*--------------------------------------------------------------------
 * Function:		SetPlayerFile
 *
 * Description:		Check for any data from MPlayer..
 *---------------------------------------------------------------------*/
void SetPlayerFile(char	*FileName) {
char		theString[300];

sprintf(theString, "load %s\n", FileName);
	PlayerWrite(theString);
	PlayerWrite("stream_time_pos\n");
	DontUpDateSlider = FALSE;
}
/*--------------------------------------------------------------------
 * Function:		Check info from MPlayer
 *
 * Description:		Check for any data from MPlayer..
 *---------------------------------------------------------------------*/
int	PlayerWrite(char *String) {
int	Val;

	Val =fputs(String, OutPipe);
	fflush(OutPipe);
//	printf("Player Write %x %d  [%s]\n", OutPipe, Val, String);

	if (Val < 0)
		printf("Player Write %d  [%s]\n", Val, String);
}

/*--------------------------------------------------------------------
 * Function:		Check info from MPlayer
 *
 * Description:		Check for any data from MPlayer..
 *---------------------------------------------------------------------*/
void PlayerPoll(void) {
char		Buffer[1000];
int		ReturnCount;
float		FValue;
int		Offset;
char		*Found;
char		*Current;
char		CommandsDone = 0;
	PlayerWrite("get_time_pos\n");

	ReturnCount = read(InPipeFD, Buffer, sizeof(Buffer));
	Current = Buffer;
	if (ReturnCount >0) {
//		printf("**V**  %d  %s\n",ReturnCount,  Current);

		while(CommandsDone == 0) {
			CommandsDone = 1;

		// Total Length   get_time_length
		Found = strstr(Current, "ANS_LENGTH");
		if (Found != NULL) {
			CommandsDone = 0;
			Found += 11;
			Current = Found;
			FValue = atof(Found);
			TotalLength =FValue;
//			printf("Found ANS_LENGTH %f\n", FValue);
			gtk_adjustment_set_upper(PositionAdjustment, FValue);
			gtk_adjustment_set_upper(FineStartAdjustment, FValue);
			gtk_adjustment_set_upper(FineEndAdjustment, FValue);

		}

		// Current Length get_time_pos
		Found = strstr(Current, "ANS_TIME_POSITION");
		if (Found != NULL) {
			CommandsDone = 0;
			Found += 18;
			Current = Found;
			FValue = atof(Found);
			CurrentLength =FValue;
//			printf("Found ANS_TIME_POSITION %f\n", FValue);
			if (!DontUpDateSlider) {
				gtk_adjustment_set_value(PositionAdjustment, FValue);
			}
			else
				DontUpDateSlider--;

			}
		}
	}
}

/*
 * CALBACKS.
 */

/*--------------------------------------------------------------------
 * Function:		PositionSlider_Changed
 *
 * Description:		Position Sliders changed.
 *---------------------------------------------------------------------*/
void PositionSlider_Changed(GtkAdjustment *adj) {
char		theString[100];
float		NewValue;

	NewValue = gtk_adjustment_get_value(PositionAdjustment);
	if ( NewValue  != CurrentLength) {
		DontUpDateSlider = 5;
		sprintf(theString,"set_property time_pos %f\n",  gtk_adjustment_get_value(PositionAdjustment) );
		PlayerWrite(theString);
	}
	else {
		DontUpDateSlider = 0;
	}
	//	printf("\nPositionSlider_Changed %f\n", gtk_adjustment_get_value(PositionAdjustment));
}

/*--------------------------------------------------------------------
 * Function:		SetASlider_Changed
 *
 * Description:		.
 *---------------------------------------------------------------------*/
void SetASlider_Changed(GtkAdjustment *adj) {
	   printf("SetASlider_Changed %f\n", gtk_adjustment_get_value(StartAdjustment));
}
/*--------------------------------------------------------------------
 * Function:		SetBSlider_Changed
 *
 * Description:		.
 *---------------------------------------------------------------------*/
void SetBSlider_Changed(GtkAdjustment *adj) {
	   printf("SetBSlider_Changed %f\n", gtk_adjustment_get_value(EndAdjustment));
}
/*--------------------------------------------------------------------
 * Function:		SetAFineSlider_Changed
 *
 * Description:		.
 *---------------------------------------------------------------------*/
void SetAFineSlider_Changed(GtkAdjustment *adj) {
	   printf("SetAFineSlider_Changed %f\n", gtk_adjustment_get_value(FineStartAdjustment));
}
/*--------------------------------------------------------------------
 * Function:		SetBFineSlider_Changed
 *
 * Description:		.
 *---------------------------------------------------------------------*/
void SetBFineSlider_Changed(GtkAdjustment *adj) {
	   printf("SetAFineSlider_Changed %f\n", gtk_adjustment_get_value(FineEndAdjustment));
}

/*--------------------------------------------------------------------
 * Function:		normal_release_handler
 *
 * Description:		Position Sliders changed.
 *---------------------------------------------------------------------*/
gboolean SetA_click_handler(GtkWidget *widget,GdkEvent *event,gpointer user_data) {
theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	printf("SetA  %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
		theButton->ButtonDownImage);

	gtk_adjustment_set_value(StartAdjustment, 0.0);
//	gtk_adjustment_set_value(FineStartAdjustment, 0.0);
	gtk_adjustment_set_value(FineStartAdjustment, CurrentLength);

	return TRUE; /* stop event propagation */
}

gboolean SetB_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	printf("SetB %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
		theButton->ButtonDownImage);
	gtk_adjustment_set_value( EndAdjustment, 0.0);
//	gtk_adjustment_set_value(FineEndAdjustment, 0.0);
	gtk_adjustment_set_value(FineEndAdjustment, CurrentLength);

	return TRUE; /* stop event propagation */
}

gboolean Loop_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	printf("SetB %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
		theButton->ButtonDownImage);

	return TRUE; /* stop event propagation */
}


gboolean Play_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
theImageButtons *theButton;
char		theString[40];
	theButton = (theImageButtons *) user_data;
	if (PlayPauseState) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
			theButton->ButtonUpImage);
		MyImageButtonSetText(&PlayPause, "Play");
		PlayerWrite("pause\n");

		PlayPauseState = 0;
	}
		else {
			gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
				theButton->ButtonDownImage);
			MyImageButtonSetText(&PlayPause, "Pause");

			PlayerWrite("pause\n");
			PlayPauseState = 1;
		}

	return TRUE; /* stop event propagation */
}

gboolean Stop_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
		theButton->ButtonDownImage);
	return TRUE; /* stop event propagation */
}


