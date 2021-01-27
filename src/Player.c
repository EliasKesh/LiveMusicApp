/*------------------------------------------------
|
|	File: 	Player
|
|	Contains:	The user interface and control code for MPlayer
|
|
|	Written By: 	Elias Keshishoglou on Sun Mar 22 14:18:29 PDT 2015
|
|	Copyright : 	2015 Elias Keshishoglou all rights reserved.
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
|------------------------------------------------*/

#define Player_c

/*
 * Place Includes Here
 */

//#include <gtk/gtk.h>
#include "LiveMusicApp.h"

#include "HTML.h"
#include "stdio.h"
#include "Player.h"
#include <unistd.h>
#include <errno.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <sys/stat.h>
/*
 * Place defines and Typedefs here
 */
#define InPipeName "/tmp/LiveMusicIn"
#define OutPipeName "/tmp/LiveMusicOut"
#define RestartPlayerValue		7

/*
 * Place Local prototypes here
 */
void PositionSlider_Changed(GtkAdjustment *adj);
gboolean SetA_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean SetB_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
void SetASlider_Changed(GtkAdjustment *adj);
void SetBSlider_Changed(GtkAdjustment *adj);
void SetAFineTickBox_Changed(GtkAdjustment *adj);
void SetBFineTickBox_Changed(GtkAdjustment *adj);
gboolean Play_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean Stop_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean Loop_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
void SpeedSlider_Changed(GtkAdjustment *adj);
gboolean PrevSeg_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean NextSeg_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean Speed_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
void VolumeSlider_Changed(GtkAdjustment *adj);
static void SaveLoopPopup_cb(GtkWidget *widget, GtkWidget *entry);
gboolean NewLoop_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean EnterLoop_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean NewMarker_click_handler(GtkWidget *widget, GdkEvent *event, gpointer user_data);
// void SaveSongMarkers(char *FileName);
void SaveLoopFile(char *FileName);

void OpenSavedLoopFile(char *FileName);
int PlayerWrite(char *String);

/*
 * Place Static variables here
 */
FILE *SavedLoopFD;
FILE *SaveMarksFD;
static FILE *OutPipe;
int InPipeFD;
int InPlayingState = 0;
GtkAdjustment *PositionAdjustment;
GtkAdjustment *StartAdjustment;
GtkAdjustment *EndAdjustment;
GtkAdjustment *FineStartAdjustment;
GtkAdjustment *FineEndAdjustment;
GtkAdjustment *SpeedAdjustment;
GtkAdjustment *VolumeAdjustment;
GtkWidget *PositionSlider;
GtkWidget *StartSpin, *EndSpin;
GtkWidget *FineStartSpin, *FineEndSpin;
GtkWidget *SpeedSpin, *VolumeSpin;
float TotalLength;
float CurrentSongPosition;
theImageButtons SetA;
theImageButtons SetB;
theImageButtons PlayPause;
theImageButtons StopButton;
theImageButtons LoopButton;
theImageButtons SpeedButton;
theImageButtons PrevSegButton;
theImageButtons NextSegButton;
extern theImageButtons PlayPauseButton;

theImageButtons EnterSaveLoop;
theImageButtons NewSaveLoop;
theImageButtons NewMarker;
SavedLoopType mySavedLoops[MaxSavedLoops];
GtkWidget *SaveCombo;
int NumSavedLoops;
GtkWidget *ImageWidget;
float PlayerCurrentTime;
char DontUpDateSlider;
char WeAreLooping;
char CurrentFile[FileNameMaxLength];
char CurrentFileSpec[FileNameMaxLength];
char InPlayerTimer;
char RestartPlayer;
char PlayerString[2000];
char PlayerAsk = 0;
float CurrentSpeed;

/*-----------------------------------------------
 * Function:		LivePlayerInit
 *
 * Description:		Set up the Mplayer pipe and controls.
 *
 *------------------------------------------------*/
int LivePlayerInit(GtkWidget *MainWindow, GtkWidget *window) {
	GtkWidget *MainBox, *PositionBox, *PlayControlBox, *PositionStartBox, *PositionEndBox, *SaveLoopBox, *SpeedBox;
	GtkWidget *SetABox, *SetBBox, *SetBPlay, *ResetBox, *LoopBox, *NormalBox, *PrevSegBox, *NextSegBox;
	GtkWidget *EditLoopBox, *NewLoopBox, *FineABox, *FineBBox;
	GtkWidget *NewSongMarkBox, *SpeedInBox;
	GtkWidget *theFrame, *SavedFrame, *SaveFixed, *SavedLabel, *PlayerFrame;
	GtkWidget *PositionSlicerBox;
	int result;

	InPlayerTimer = 0;
	InPlayingState = 0;
	RestartPlayer = 0;
	PlayerDisTime = 0.0;
	CurrentSpeed = 1.0;
	strcpy(PlayerDisSection, "-----");

	sprintf(PlayerString, " rm  %s -rf", InPipeName);
	system(PlayerString);
	CurrentFile[0] = 0;

	/*
	 * Create the fifo for communications with MPlayer.
	 */
	result = mkfifo(InPipeName, S_IRUSR | S_IWUSR);
	chmod(InPipeName, 0666);
	if (result < 0) {
		printd(LogPlayer, "Before fopen in %d %s\n", result, strerror(result));
	}

	/*
	 * Start laying out the control boxs
	 */
	MainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	PositionBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
	PositionStartBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	PositionEndBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	SpeedBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	FineABox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	FineBBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	SpeedInBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	PlayControlBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	SaveLoopBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);

	/*
	 * Get the image generated of the music..
	 */
	ImageWidget = gtk_image_new_from_file(ImageWidget);

	/*
	 * Main Box
	 */
	printd(LogPlayer, " Init Player %x %x\n", MainBox, window);
	PositionAdjustment = gtk_adjustment_new(0, 0, 400, 1, 20, 0);
	PositionSlider = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL,
	                               GTK_ADJUSTMENT(PositionAdjustment));
	//  gtk_scale_set_digits (PositionSlider, 1);
//    gtk_scale_set_value_pos (PositionSlider, GTK_POS_TOP);
	gtk_scale_set_draw_value(PositionSlider, TRUE);
	gtk_scale_set_value_pos(PositionSlider, GTK_POS_BOTTOM);
	g_signal_connect(G_OBJECT (PositionSlider), "change_value",
	                 G_CALLBACK (PositionSlider_Changed), NULL);

	/*
	 * A Set Controls, Button
	 */
	SetABox = gtk_event_box_new();
	MyImageButtonInit(&SetA, SetABox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&SetA, "Start");
	g_signal_connect(G_OBJECT(SetABox),
	                 "button-press-event",
	                 G_CALLBACK(SetA_click_handler),
	                 &SetA);
	g_signal_connect(G_OBJECT(SetABox),
	                 "button-release-event",
	                 G_CALLBACK(normal_release_handler),
	                 &SetA);
 	gtk_widget_set_tooltip_text(SetA.EventBox, "Start of the loop.");

	/*
	 * A Set Controls, Large Controls
	 */
	StartAdjustment = gtk_adjustment_new(0.0, -1, 1, 0.02, 0.1, 0);
	StartSpin = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL,
	                          GTK_ADJUSTMENT(StartAdjustment));
	g_signal_connect(G_OBJECT (StartSpin), "value_changed",
	                 G_CALLBACK (SetASlider_Changed), NULL);

	/*
	 * A Set Controls, Fine Controls
	 */
	FineStartAdjustment = gtk_adjustment_new(0, 0, 400, 0.05, .1, 0);
	FineStartSpin = gtk_spin_button_new(FineStartAdjustment, 0.05, 3);
	g_signal_connect(G_OBJECT (FineStartSpin), "value_changed",
	                 G_CALLBACK (SetAFineTickBox_Changed), NULL);

	/*
	 * Length Set Controls, Button
	 */
	SetBBox = gtk_event_box_new();
	MyImageButtonInit(&SetB, SetBBox, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&SetB, "Length");
	g_signal_connect(G_OBJECT(SetBBox),
	                 "button-press-event",
	                 G_CALLBACK(SetB_click_handler),
	                 &SetB);
	g_signal_connect(G_OBJECT(SetBBox),
	                 "button-release-event",
	                 G_CALLBACK(normal_release_handler),
	                 &SetB);
 	gtk_widget_set_tooltip_text(SetB.EventBox, "Hit at the B point of the loop to set the length.");

	/*
	 * Length Set Controls, Course control
	 */
	EndAdjustment = gtk_adjustment_new(0.0, -1, 1, 0.001, 1, 0);
	EndSpin = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL,
	                        GTK_ADJUSTMENT(EndAdjustment));
	g_signal_connect(G_OBJECT (EndSpin), "value_changed",
	                 G_CALLBACK (SetBSlider_Changed), NULL);

	/*
	 * Length Set Controls, Fine control
	 */
	FineEndAdjustment = gtk_adjustment_new(0, 0, 400, 0.05, .1, 0);
	FineEndSpin = gtk_spin_button_new(FineEndAdjustment, 0.05, 3);
	g_signal_connect(G_OBJECT (FineEndSpin), "value_changed",
	                 G_CALLBACK (SetBFineTickBox_Changed), NULL);

	/*
	 * Play/Pause Button
	 */
	SetBPlay = gtk_event_box_new();
	MyImageButtonInit(&PlayPause, SetBPlay, MainButtonOnImage, MainButtonOffImage);
	MyImageButtonSetText(&PlayPause, "Play");
	g_signal_connect(G_OBJECT(SetBPlay),
	                 "button-press-event",
	                 G_CALLBACK(Play_click_handler),
	                 &PlayPause);

	/*
	 * Loop Reset
	 */
	ResetBox = gtk_event_box_new();
	MyImageButtonInit(&StopButton, ResetBox, MainButtonOnImage,
	                  MainButtonOffImage);
	MyImageButtonSetText(&StopButton, "Reset");
	g_signal_connect(G_OBJECT(ResetBox),
	                 "button-press-event",
	                 G_CALLBACK(Stop_click_handler),
	                 &StopButton);
	g_signal_connect(G_OBJECT(ResetBox),
	                 "button-release-event",
	                 G_CALLBACK(normal_release_handler),
	                 &StopButton);
 	gtk_widget_set_tooltip_text(StopButton.EventBox, "Reset Volume, loop, speed and position.");


	/*
	 * Loop on/off
	 */
	LoopBox = gtk_event_box_new();
	MyImageButtonInit(&LoopButton, LoopBox, MainButtonOnImage,
	                  MainButtonOffImage);
	MyImageButtonSetText(&LoopButton, "Loop");
	g_signal_connect(G_OBJECT(LoopBox),
	                 "button-press-event",
	                 G_CALLBACK(Loop_click_handler),
	                 &LoopButton);
 	gtk_widget_set_tooltip_text(LoopButton.EventBox, "Toggle looping and playing.");

	/*
	 * Playback speed control
	 */
	SpeedAdjustment = gtk_adjustment_new(1.0, 0.5, 1.5, 0.05, 1, 0);
	SpeedSpin = gtk_spin_button_new(SpeedAdjustment, 0.05, 2);
	g_signal_connect(G_OBJECT (SpeedSpin), "value_changed",
	                 G_CALLBACK (SpeedSlider_Changed), NULL);
#if 0
	gtk_widget_set_size_request(SpeedSpin, 50, 50);
	gtk_widget_set_margin_top(SpeedSpin, 20);
	gtk_widget_set_margin_bottom(SpeedSpin, 20);
#endif
	/*
	 * Normal speed set
	 */
	NormalBox = gtk_event_box_new();
	MyImageButtonInit(&SpeedButton, NormalBox, MainButtonOnImage,
	                  MainButtonOffImage);
	MyImageButtonSetText(&SpeedButton, "1X Speed");
	g_signal_connect(G_OBJECT(NormalBox),
	                 "button-press-event",
	                 G_CALLBACK(Speed_click_handler),
	                 &SpeedButton);
	g_signal_connect(G_OBJECT(NormalBox),
	                 "button-release-event",
	                 G_CALLBACK(normal_release_handler),
	                 &SpeedButton);
 	gtk_widget_set_tooltip_text(SpeedButton.EventBox, "Reset to normal speed of the file.");

	/*
	 * Playback volume slider
	 */
	VolumeAdjustment = gtk_adjustment_new(75, 0, 100, 1, 10, 0);
	VolumeSpin = gtk_scale_new(GTK_ORIENTATION_VERTICAL,
	                           GTK_ADJUSTMENT(VolumeAdjustment));
//    gtk_scale_set_value_pos (GTK_SCALE (VolumeSpin), GTK_POS_BOTTOM);
//	   gtk_widget_set_vexpand (VolumeSpin, TRUE);

	gtk_widget_set_margin_bottom(VolumeSpin, 15);
	gtk_range_set_range (GTK_RANGE (VolumeSpin), 0, 100);
	gtk_range_set_inverted (GTK_RANGE (VolumeSpin), TRUE);

	g_signal_connect(G_OBJECT (VolumeSpin), "value_changed",
	                 G_CALLBACK (VolumeSlider_Changed), NULL);
	gtk_scale_set_draw_value((GtkScale *)VolumeSpin, TRUE);
	gtk_scale_set_has_origin((GtkScale *)VolumeSpin, TRUE);
	gtk_scale_set_digits((GtkScale *)VolumeSpin, TRUE);
	gtk_widget_set_margin_top(VolumeSpin, 20);

//	gtk_widget_set_margin_right(VolumeSpin, 20);
//	gtk_widget_set_margin_left(VolumeSpin, 20);

	gtk_widget_set_margin_end(VolumeSpin, 20);
	gtk_widget_set_margin_start(VolumeSpin, 20);


	/*
	 * Loop Segment prev
	 */
	PrevSegBox = gtk_event_box_new();
	MyImageButtonInit(&PrevSegButton, PrevSegBox, MainButtonOnImage,
	                  MainButtonOffImage);
	MyImageButtonSetText(&PrevSegButton, "Prev Loop");
	g_signal_connect(G_OBJECT(PrevSegBox),
	                 "button-press-event",
	                 G_CALLBACK(PrevSeg_click_handler),
	                 &PrevSegButton);
	g_signal_connect(G_OBJECT(PrevSegBox),
	                 "button-release-event",
	                 G_CALLBACK(normal_release_handler),
	                 &PrevSegButton);
 	gtk_widget_set_tooltip_text(PrevSegButton.EventBox, "Reset the start point back by the current length.");

	/*
	 * Loop Segment next
	 */
	NextSegBox = gtk_event_box_new();
	MyImageButtonInit(&NextSegButton, NextSegBox, MainButtonOnImage,
	                  MainButtonOffImage);
	MyImageButtonSetText(&NextSegButton, "Next Loop");
	g_signal_connect(G_OBJECT(NextSegBox),
	                 "button-press-event",
	                 G_CALLBACK(NextSeg_click_handler),
	                 &NextSegButton);
	g_signal_connect(G_OBJECT(NextSegBox),
	                 "button-release-event",
	                 G_CALLBACK(normal_release_handler),
	                 &NextSegButton);
 	gtk_widget_set_tooltip_text(NextSegButton.EventBox, "Reset the start point forward by the current length.");

	/*
	 * Edit an existing saved loop
	 */
	EditLoopBox = gtk_event_box_new();
	MyImageButtonInit(&EnterSaveLoop, EditLoopBox, MainButtonOnImage,
	                  MainButtonOffImage);
	MyImageButtonSetText(&EnterSaveLoop, "Edit Loop");
	g_signal_connect(G_OBJECT(EditLoopBox),
	                 "button-press-event",
	                 G_CALLBACK(EnterLoop_click_handler),
	                 &EnterSaveLoop);
	g_signal_connect(G_OBJECT(EditLoopBox),
	                 "button-release-event",
	                 G_CALLBACK(normal_release_handler),
	                 &EnterSaveLoop);
 	gtk_widget_set_tooltip_text(EnterSaveLoop.EventBox, "Reset Start Length for the current loop.");


	/*
	 * Create a new saved loop
	 */
	NewLoopBox = gtk_event_box_new();
	MyImageButtonInit(&NewSaveLoop, NewLoopBox, MainButtonOnImage,
	                  MainButtonOffImage);
	MyImageButtonSetText(&NewSaveLoop, "Add Loop");
	g_signal_connect(G_OBJECT(NewLoopBox),
	                 "button-press-event",
	                 G_CALLBACK(NewLoop_click_handler),
	                 &NewSaveLoop);
	g_signal_connect(G_OBJECT(NewLoopBox),
	                 "button-release-event",
	                 G_CALLBACK(normal_release_handler),
	                 &NewSaveLoop);
 	gtk_widget_set_tooltip_text(NewSaveLoop.EventBox, "Create a named loop to recall later.");

	/*
	 * Create a new saved loop
	 */
	NewSongMarkBox = gtk_event_box_new();
	MyImageButtonInit(&NewMarker, NewSongMarkBox, MainButtonOnImage,
	                  MainButtonOffImage);
	MyImageButtonSetText(&NewMarker, "Add Marker");
	g_signal_connect(G_OBJECT(NewSongMarkBox),
	                 "button-press-event",
	                 G_CALLBACK(NewMarker_click_handler),
	                 &NewMarker);
	g_signal_connect(G_OBJECT(NewSongMarkBox),
	                 "button-release-event",
	                 G_CALLBACK(normal_release_handler),
	                 &NewMarker);
 	gtk_widget_set_tooltip_text(NewMarker.EventBox, "Create a marker which gets a countdown during playback.");


	/*
	 * Saved Loops popup box
	 */
	SaveFixed = gtk_fixed_new();
	SaveCombo = gtk_combo_box_text_new();

	SavedLabel = gtk_label_new("Saved Loops ");
	gtk_fixed_put(GTK_FIXED(SaveFixed), SavedLabel, 0, 0);
	gtk_fixed_put(GTK_FIXED(SaveFixed), SaveCombo, 0, 25);
	g_signal_connect(G_OBJECT(SaveCombo), "changed",
	                 G_CALLBACK(SaveLoopPopup_cb), (gpointer ) SavedLabel);
	gtk_widget_set_size_request(SaveCombo, 130, 60);
 	gtk_widget_set_tooltip_text(SaveCombo, "Select a saved loop.");

	/*
	 * Now that everything has been created let's pack them together.
	 */
//	gtk_box_set_homogeneous(GTK_BOX(FineABox), TRUE);
	gtk_box_pack_start(GTK_BOX(FineABox), SetABox, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(FineABox), FineStartSpin, TRUE, FALSE,  5);

//	gtk_box_set_homogeneous(GTK_BOX(FineBBox), TRUE);
	gtk_box_pack_start(GTK_BOX(FineBBox), SetBBox, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(FineBBox), FineEndSpin, TRUE, FALSE,  5);

//	gtk_box_set_homogeneous(GTK_BOX(PositionStartBox), TRUE);
	gtk_box_pack_start(GTK_BOX(PositionStartBox), FineABox, TRUE, FALSE,  5);
//	gtk_box_pack_start(GTK_BOX(PositionStartBox), StartSpin, TRUE, FALSE,  5);

	gtk_box_set_homogeneous(GTK_BOX(PositionEndBox), TRUE);
	gtk_box_pack_start(GTK_BOX(PositionEndBox), FineBBox, TRUE, FALSE,  5);
//	gtk_box_pack_start(GTK_BOX(PositionEndBox), EndSpin, TRUE, FALSE,  5);



	gtk_box_pack_start(GTK_BOX(SpeedInBox), NormalBox, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(SpeedInBox), SpeedSpin, TRUE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(SpeedBox), SpeedInBox, TRUE, FALSE, 5);

//	gtk_box_set_homogeneous(GTK_BOX(PositionBox), TRUE);
	theFrame = gtk_frame_new("Start Position");
	gtk_frame_set_label_align((GtkFrame *)theFrame, 0.5, 0.5);
	gtk_container_add(GTK_CONTAINER(theFrame), PositionStartBox);
	gtk_frame_set_shadow_type(GTK_FRAME(theFrame), GTK_SHADOW_ETCHED_OUT);
	gtk_box_pack_start(GTK_BOX(PositionBox), theFrame, TRUE, FALSE,  5);

	theFrame = gtk_frame_new("Length");
	gtk_frame_set_label_align((GtkFrame *)theFrame, 0.5, 0.5);
	gtk_container_add(GTK_CONTAINER(theFrame), PositionEndBox);
	gtk_frame_set_shadow_type(GTK_FRAME(theFrame), GTK_SHADOW_ETCHED_OUT);
	gtk_box_pack_start(GTK_BOX(PositionBox), theFrame, TRUE, FALSE,  5);

	theFrame = gtk_frame_new("Speed");
	gtk_frame_set_label_align((GtkFrame *)theFrame, 0.5, 0.5);
	gtk_container_add(GTK_CONTAINER(theFrame), SpeedBox);
	gtk_frame_set_shadow_type(GTK_FRAME(theFrame), GTK_SHADOW_ETCHED_OUT);
	gtk_box_pack_start(GTK_BOX(PositionBox), theFrame, TRUE, FALSE,  5);

	theFrame = gtk_frame_new("Volume");
	gtk_frame_set_label_align((GtkFrame *)theFrame, 0.5, 0.5);
	gtk_container_add(GTK_CONTAINER(theFrame), VolumeSpin);
	gtk_frame_set_shadow_type(GTK_FRAME(theFrame), GTK_SHADOW_ETCHED_OUT);
	gtk_box_pack_start(GTK_BOX(PositionBox), theFrame, TRUE, FALSE,  5);

#if 1
	PlayerFrame = gtk_frame_new("Player Control");
	gtk_frame_set_label_align((GtkFrame *)PlayerFrame, 0.5, 0.5);
	gtk_container_add(GTK_CONTAINER(PlayerFrame), PlayControlBox);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), SetBPlay, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), ResetBox, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), LoopBox, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), PrevSegBox, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), NextSegBox, TRUE, FALSE,  5);

#else
	gtk_box_pack_start(GTK_BOX(PlayControlBox), SetBPlay, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), ResetBox, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), LoopBox, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), PrevSegBox, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(PlayControlBox), NextSegBox, TRUE, FALSE,  5);
#endif


	SavedFrame = gtk_frame_new("Saved Loops");
	gtk_frame_set_label_align((GtkFrame *)SavedFrame, 0.5, 0.5);
	gtk_frame_set_shadow_type(GTK_FRAME(SavedFrame), GTK_SHADOW_ETCHED_OUT);
	gtk_container_add(GTK_CONTAINER(SavedFrame), SaveLoopBox);
	gtk_box_pack_start(GTK_BOX(SaveLoopBox), SaveFixed, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(SaveLoopBox), EditLoopBox, TRUE, FALSE,  5);
	gtk_box_pack_start(GTK_BOX(SaveLoopBox), NewLoopBox, TRUE, FALSE,  5);

	gtk_box_pack_start(GTK_BOX(SaveLoopBox), NewSongMarkBox, TRUE, FALSE,  5);

//	gtk_misc_set_padding(ImageWidget, 55,10);
//	gtk_misc_set_alignment(ImageWidget, 0,0);

//	gtk_widget_set_margin_left(ImageWidget, 28);
//	gtk_widget_set_margin_right(ImageWidget, 28);
	gtk_widget_set_margin_start(ImageWidget, 28);
	gtk_widget_set_margin_end(ImageWidget, 28);
	gtk_widget_set_margin_top(ImageWidget, 15);
	gtk_widget_set_margin_bottom(ImageWidget, 15);

//	gtk_widget_set_margin_left(PositionSlider, 25);
//	gtk_widget_set_margin_right(PositionSlider, 25);
	gtk_widget_set_margin_start(PositionSlider, 25);
	gtk_widget_set_margin_end(PositionSlider, 25);

//	gtk_box_pack_start(GTK_BOX(MainBox), ImageWidget, TRUE, FALSE, 5);

	PositionSlicerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

	theFrame = gtk_frame_new("Song Position");
	gtk_frame_set_label_align((GtkFrame *)theFrame, 0.5, 0.5);

	gtk_container_add(GTK_CONTAINER(PositionSlicerBox), ImageWidget);
	gtk_container_add(GTK_CONTAINER(PositionSlicerBox), PositionSlider);
	// gtk_box_pack_start(GTK_BOX(PositionSlicerBox), ImageWidget, TRUE, FALSE, 5);
	// gtk_box_pack_start(GTK_BOX(PositionSlicerBox), PositionSlider, TRUE, FALSE,  5);
	gtk_widget_set_margin_bottom(PositionSlicerBox, 30);

	gtk_container_add(GTK_CONTAINER(theFrame), PositionSlicerBox);

//	gtk_container_add(GTK_CONTAINER(theFrame), PositionSlider);

	gtk_frame_set_shadow_type(GTK_FRAME(theFrame), GTK_SHADOW_ETCHED_OUT);

	// Pack everything together
	gtk_box_pack_start(GTK_BOX(MainBox), theFrame, TRUE, TRUE, 15);
	gtk_box_pack_start(GTK_BOX(MainBox), PositionBox, TRUE, TRUE,  15);

	gtk_box_pack_start(GTK_BOX(MainBox), PlayerFrame, TRUE, TRUE,  15);
//	gtk_box_pack_start(GTK_BOX(MainBox), PlayControlBox, TRUE, FALSE,  15);
	gtk_box_pack_start(GTK_BOX(MainBox), SavedFrame, TRUE, TRUE,  15);

	// Add this to the main window.
	gtk_container_add(GTK_CONTAINER(window), MainBox);
//	gtk_widget_set_size_request(window, 100, 100);
//	gtk_window_set_default_size(window, 200, 200);
	gtk_widget_show_all(window);

	StartPlayer();

	/*
	 * Init the Pipe for communications with MPlayer
	 */
	InPipeFD = open(InPipeName, O_RDONLY | O_NONBLOCK);
//	printf("FIFO %d\n", InPipeFD);
	if (InPipeFD < 1) {
		int err = errno;
		printd(LogPlayer, "Error in open In pipe %d\n", errno);
		exit(1);
	}
	return (0);
}

/*-----------------------------------------------
 * Function:		SetMP3PlayVol
 *
 * Description:		Set the playback volume
 *------------------------------------------------*/
int SetMP3PlayVol(int Value) {
	gtk_adjustment_set_value(VolumeAdjustment, Value);
	printd(LogPlayer, "Player Volume %d\n", Value);
	gtk_range_set_adjustment(VolumeSpin, VolumeAdjustment);

	return (Value);
}

/*-----------------------------------------------
 * Function:		SaveLoopPopup_cb
 *
 * Description:		Enter loop info after set by user..
 *------------------------------------------------*/
static void SaveLoopPopup_cb(GtkWidget *widget, GtkWidget *entry) {
	int CurrentSavedLoop;

	CurrentSavedLoop = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
	gtk_adjustment_set_value(FineStartAdjustment,
	                         mySavedLoops[CurrentSavedLoop].Start);
	gtk_adjustment_set_value(FineEndAdjustment,
	                         mySavedLoops[CurrentSavedLoop].Length);
}

/*-----------------------------------------------
 * Function:		ResetPlayer
 *
 * Description:
 *
 *------------------------------------------------*/
int ResetPlayer(void) {


	// Clear the Marker display.
	PlayerDisSection[0] = 0;
	PlayerDisTime = 0;

//	system("killall mplayer");
	printd(LogPlayer, "ResetPlayer\n");

	gtk_adjustment_set_value(PositionAdjustment, 0);
	gtk_adjustment_set_value(FineStartAdjustment, 0);
	gtk_adjustment_set_value(FineEndAdjustment, 0);
	gtk_adjustment_set_value(SpeedAdjustment, 1.0);

	// gtk_image_set_from_pixbuf(GTK_IMAGE(LoopButton.Image),
	//              LoopButton.ButtonUpImage);
//	plLoopToggle();

//	WeAreLooping = 1;
	CurrentSpeed = 1.0;
	CurrentSongPosition = 0.0;
//	plLoopToggle();
//	usleep(150);
	PlayerWrite("set_property stream_pos 0.000\n");
//	usleep(150);
	PlayerWrite("set_property time_pos 0.000\n");
//	usleep(150);
	return (0);
}

/*-----------------------------------------------
 * Function:		LivePlayerInit
 *
 * Description:		Set up the Mplayer pipe and controls.
 *
 *------------------------------------------------*/
int LivePlayerClose(void) {

	printd(LogPlayer, "LivePlayerClose\n");

	system("killall mplayer");

	PlayerDisTime = 0.0;
	strcpy(PlayerDisSection, "-----");

	SaveLoopFile(CurrentFile);
//	SaveSongMarkers(CurrentFile);

//	PlayerWrite("quit\n");
	system("killall mplayer");
	return (0);
}
/*-----------------------------------------------
 * Function:		SetPlayerFile
 *
 * Description:		Check for any data from MPlayer..
 *------------------------------------------------*/
void SetPlayerFile(char *FileName) {

	/* In case we are still player when a song is selected.
	*/
	plStop();
	ResetPlayer();

	if (CurrentFile[0]) {
		SaveLoopFile(CurrentFile);
//		SaveSongMarkers(CurrentFile);
	}

	LEDControl(7,1);

	strncpy(CurrentFile, FileName, FileNameMaxLength);
	strncpy(CurrentFileSpec, FileName, FileNameMaxLength);
	strcat(CurrentFileSpec, ".spec.png");

	OpenSavedLoopFile(FileName);

	// sprintf(PlayerString,
	//         "sox \"%s\"  -n spectrogram  -x 1400 -Y 130 -c 1  -r -a -o %s\n",
	//         FileName,CurrentFileSpec);
	// printd(LogPlayer, "%s", PlayerString);
	// system(PlayerString);
	gtk_image_clear((GtkImage *)ImageWidget);
	gtk_image_set_from_file((GtkImage *)ImageWidget, CurrentFileSpec);

	RestartPlayer = RestartPlayerValue;
	sprintf(PlayerString, "load \"%s\"\n", FileName);

#if 0
	PlayerWrite(PlayerString);
	WeAreLooping = 0;
	ResetPlayer();
	PlayerWrite("get_time_length\n");
	PlayerPoll(0);
	plStop();
#endif

#if 0
	sprintf(PlayerString, "load \"%s\"\n", FileName);
	PlayerWrite(PlayerString);
//	PlayerWrite("stream_time_pos\n");
	DontUpDateSlider = 0;
	PlayerAsk = 0;
	InPlayingState = 0;
	plPausePlay();
	ResetPlayer();
	PlayerPoll(0);

	PlayerWrite("get_time_length\n");
#endif
}

/*-----------------------------------------------
 * Function:		OpenSavedLoopFile
 *
 * Description:		Open/and or create new Saved Position file.
 *------------------------------------------------*/
void OpenSavedLoopFile(char *FileName) {
	char SaveLoopName[300];

	if (SavedLoopFD) {
		fclose(SavedLoopFD);
		SavedLoopFD = 0;
	}

	memset(mySavedLoops, 0, sizeof(mySavedLoops));

	NumSavedLoops = 0;
	/*
	 * Let's open the Saved Looped file.
	 */
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX(SaveCombo));
	sprintf(SaveLoopName, "%s.Loops", FileName);
	printd(LogPlayer, " OpenSavedLoopFile %s\n", SaveLoopName);
	SavedLoopFD = fopen(SaveLoopName, "r");
	printd(LogPlayer, " OpenSavedLoopFile FD %d %s\n", SavedLoopFD, strerror(errno));
	if (SavedLoopFD) {
		while (!feof(SavedLoopFD)) {
			fscanf(SavedLoopFD, "%s %f, %f, %f\n",
			       (char *)&mySavedLoops[NumSavedLoops].LoopName,
			       &mySavedLoops[NumSavedLoops].Start,
			       &mySavedLoops[NumSavedLoops].Length,
			       &mySavedLoops[NumSavedLoops].Position);
#if 1
			printd(LogPlayer, "Reading Loop %d %s %f %f %f\n", NumSavedLoops,
			       mySavedLoops[NumSavedLoops].LoopName,
			       mySavedLoops[NumSavedLoops].Start,
			       mySavedLoops[NumSavedLoops].Length,
			       mySavedLoops[NumSavedLoops].Position);
#endif
			gtk_combo_box_text_append_text(GTK_COMBO_BOX(SaveCombo),
			                               mySavedLoops[NumSavedLoops].LoopName);

			// Check for Zero entries and don't save
			if ( !((mySavedLoops[NumSavedLoops].LoopName[0] == 0) &&
				(mySavedLoops[NumSavedLoops].Start == 0) && 
			(mySavedLoops[NumSavedLoops].Length == 0) && 
			(mySavedLoops[NumSavedLoops].Position == 0)) ) 
				NumSavedLoops++;
			/*
			 * Check for errors.
			 */
			if (NumSavedLoops >= MaxSavedLoops) {
				printd(LogPlayer, "Error, too many Loops\n");
				return;
			}

			if (NumSavedLoops > 0) {
//				CurrentSavedLoop = 1;
				gtk_combo_box_set_active(GTK_COMBO_BOX(SaveCombo), 1);
			}
		}
		fclose(SavedLoopFD);
		SavedLoopFD = 0;
	}
}

/*-----------------------------------------------
 * Function:		SaveLoopFile
 *
 * Description:		Save the Loop points.
 *------------------------------------------------*/
void SaveLoopFile(char *FileName) {
	char SaveLoopName[FileNameMaxLength];
	int Loop = 0;
	FILE *SavedLoopFD;
	int i;
	int j;
	SavedLoopType temp;
	/*
	 * Let's open the Saved Looped file.
	 */
	sprintf(SaveLoopName, "%s.Loops", FileName);
	printd(LogPlayer, " SavedLoopFile [%s] %d\n", SaveLoopName, strlen(SaveLoopName));
	errno = 0;
	SavedLoopFD = fopen(SaveLoopName, "w+");
//	sprintf(SaveLoopName, "MyTest.loop");
	printd(LogPlayer, " SavedLoopFile FD %d %s\n", SavedLoopFD, strerror(errno) );

	// Save the list in Time order.
	for (i = NumSavedLoops - 2; i >= 0; i--) {
		for (j = 0; j <= i; j++) {
			if (mySavedLoops[j].Start > mySavedLoops[j + 1].Start) {
				temp = mySavedLoops[j];
				mySavedLoops[j] = mySavedLoops[j + 1];
				mySavedLoops[j + 1] = temp;
			}
		}
	}

	if (SavedLoopFD) {
		while (Loop < NumSavedLoops) {
			fprintf(SavedLoopFD, "%20s %0.2f, %0.2f, %0.2f\n",
			        mySavedLoops[Loop].LoopName,
			        mySavedLoops[Loop].Start,
			        mySavedLoops[Loop].Length,
			        mySavedLoops[Loop].Position);
#if 1
			printd(LogPlayer, "Saving Loop %d %s %f %f %f\n", NumSavedLoops,
			       mySavedLoops[NumSavedLoops].LoopName,
			       mySavedLoops[NumSavedLoops].Start,
			       mySavedLoops[NumSavedLoops].Length,
			       mySavedLoops[NumSavedLoops].Position);
#endif


			Loop++;
		}
		fclose(SavedLoopFD);
		SavedLoopFD = 0;
	}
}

/*-----------------------------------------------
 * Function:		Check info from MPlayer
 *
 * Description:		Check for any data from MPlayer..
 *------------------------------------------------*/
int PlayerWrite(char *String) {
	int Val;

	Val = fputs(String, OutPipe);
	fflush(OutPipe);

	printd(LogPlayer, "Player Write %x %d  %s", OutPipe, Val, String);

	if (Val < 0)
		printd(LogPlayer, "Error **Player Write %d  %s", Val, String);

	return (0);
}

/*-----------------------------------------------
 * Function:		Check info from MPlayer
 *
 * Description:		Check for any data from MPlayer..
 *------------------------------------------------*/
void PlayerPoll(char How) {
	char Buffer[1000];
	int ReturnCount;
	float FValue;
	int Offset;
	char *Found;
	char *Current;
	char CommandsDone = 0;
	int Loop;

	/* If we are not playing then don't bother.
	*/
	if (!InPlayingState)
		return;

	printd(LogPlayer, "PlayerPoll State %d\n", InPlayingState);

	/*
	 * If we were called via a timer interrupt and we were also called directly just leave.
	 */
	if (How && InPlayerTimer) {
		InPlayerTimer--;
		return;
	}
	printd(LogPlayer, "PlayerPoll After InPlayerTimer\n");

	/*
	 * If we were called directly then lock out the interrupt call
	 */
	if (!How) {
		InPlayerTimer = 2;
	}

	/*
	 * Don't update the slider.
	 */
	if (DontUpDateSlider) {
		printd(LogPlayer, "Decrement Slider %d\n", DontUpDateSlider);
		DontUpDateSlider--;
	}

	/*
	 * Wait a few timer cycles before restarting MPlayer.
	 */
	if (RestartPlayer == 3) {
		StartPlayer();
	}

	if (RestartPlayer) {
		RestartPlayer--;
		return;
	}

	/*
	 * Ask the player for the current playback position.
	 */
	PlayerWrite("get_time_pos\n");
	PlayerAsk++;
	printd(LogPlayer, "get_time_pos %d\n", PlayerAsk);

	/*
	 * If we are asking for a value wait until Mplayer responds.
	 */
	if (InPlayerTimer) {
		usleep(25);
	}

	/*
	 * See what has come back from the player.
	 */
	ReturnCount = read(InPipeFD, Buffer, sizeof(Buffer));
	Current = Buffer;

	if (ReturnCount > 0) {
		printd(LogPlayer, "**V**  %d  %s\n", ReturnCount,  Current);

		while (CommandsDone == 0) {
			CommandsDone = 1;

			/*
			 * Get the current song length
			 */
			Found = strstr(Current, "ANS_LENGTH");
			if (Found != NULL) {
				CommandsDone = 0;
				Found += 11;
				Current = Found;
				FValue = atof(Found) + .5;
				TotalLength = FValue;
				printd(LogPlayer, "Found TOTAL LENGTH ANS_LENGTH %f\n", FValue);
				gtk_adjustment_set_upper(PositionAdjustment, FValue);
				gtk_adjustment_set_upper(FineStartAdjustment, FValue);
				gtk_adjustment_set_upper(FineEndAdjustment, FValue);
			}

			/*
			 * Get the current position in the playing tune.
			 */
			Found = strstr(Current, "ANS_TIME_POSITION");
			if (Found != NULL) {
				CommandsDone = 0;
				Found += 18;
				Current = Found;
				FValue = atof(Found);
				CurrentSongPosition = FValue;
				PlayerAsk--;
				printd(LogPlayer, "Mpl Time %f - %s\n", FValue, Found );

				/* Set the variables for the
				SongMarkers.
				*/
#if 1
				PlayerCurrentTime = FValue;
				if (NumSavedLoops)
					for (Loop = 0; Loop < NumSavedLoops; Loop++) {
						if (FValue < mySavedLoops[Loop].Start) {
							strcpy(PlayerDisSection, mySavedLoops[Loop].LoopName);
							PlayerDisTime = mySavedLoops[Loop].Start - FValue;

							if (PlayerDisTime < 10) {
								if (PlayerDisTime < 1)
									LEDControl(7,1);
								else
								if (PlayerDisTime < 4)
									LEDControl(8,0);
								else
								if (PlayerDisTime < 7)
									LEDControl(9,0);
								else
								if (PlayerDisTime < 10)
									LEDControl(10,0);
							
							}

							if (mySavedLoops[Loop].Position >= 0 )
								ScrollCtrl(mySavedLoops[Loop].Position);

							if (mySavedLoops[Loop].Position == -2)
								ScrollCtrl(ScrollHome);

							if (mySavedLoops[Loop].Position == -3)
								ScrollCtrl(ScrollPgUp);

							if (mySavedLoops[Loop].Position == -4)
								ScrollCtrl(ScrollKeyUp);

							if (mySavedLoops[Loop].Position == -5)
								ScrollCtrl(ScrollKeyDn);

							if (mySavedLoops[Loop].Position == -6)
								ScrollCtrl(ScrollPgDn);

							if (mySavedLoops[Loop].Position == -7)
								ScrollCtrl(ScrollEnd);

							break;
						}
					}
#else
				if (NumberSongMarks)
					for (Loop = 0; Loop < NumberSongMarks; Loop++) {
						if (FValue < SongMarkers[Loop].SongMark) {
							strcpy(PlayerDisSection, SongMarkers[Loop].SongSection);
							PlayerDisTime = SongMarkers[Loop].SongMark - FValue;
							break;
						}
					}
#endif
				/* Set the Global.
				*/
#if 1
				if ((TotalLength > 0) && (CurrentSongPosition + 0.5 >= TotalLength))  {
					printd(LogPlayer, "Player Stopping\n");
					plSetPosition(0.0);
				}
#endif
				/*
				 * If someone is moving the position slider don't update it.
				 */
				if (!DontUpDateSlider) {
					DontUpDateSlider = 3;
					gtk_adjustment_set_value(PositionAdjustment, FValue);
				}
			}
		}
	}

	if (How && InPlayerTimer) {
		printd(LogPlayer, "Cleaning InPlayerTimer\n");
		InPlayerTimer = 0;
	}
}

/*
 * CALBACKS.
 */

/*-----------------------------------------------
 * Function:		PositionSlider_Changed
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
void PositionSlider_Changed(GtkAdjustment *adj) {
	float NewValue;

	LEDControl(7,1);

	NewValue = gtk_adjustment_get_value(PositionAdjustment);
	printd(LogPlayer, "PositionSlider_Changed %f %f\n", NewValue, CurrentSongPosition);
	/* If we change the current position while stopped.
	*/
	CurrentSongPosition = NewValue;
	plSetPosition(gtk_adjustment_get_value(PositionAdjustment));
}

/*-----------------------------------------------
 * Function:		SetASlider_Changed
 *
 * Description:		.
 *------------------------------------------------*/
void SetASlider_Changed(GtkAdjustment *adj) {
	float NewValue;

	NewValue = gtk_adjustment_get_value(StartAdjustment);
	printd(LogPlayer, "SetASlider_Changed\n");
}

/*-----------------------------------------------
 * Function:		SetBSlider_Changed
 *
 * Description:		.
 *------------------------------------------------*/
void SetBSlider_Changed(GtkAdjustment *adj) {
	float NewValue;

	NewValue = gtk_adjustment_get_value(EndAdjustment);
	printd(LogPlayer, "SetBSlider_Changed\n");
}

/*-----------------------------------------------
 * Function:		SetAFineTickBox_Changed
 *
 * Description:		.
 *------------------------------------------------*/
void SetAFineTickBox_Changed(GtkAdjustment *adj) {
	printd(LogPlayer, "SetBFineTickBox_Changed\n");
	if (WeAreLooping)
		RestartPlayer = RestartPlayerValue;
}

/*-----------------------------------------------
 * Function:		SetBFineTickBox_Changed
 *
 * Description:		.
 *------------------------------------------------*/
void SetBFineTickBox_Changed(GtkAdjustment *adj) {
	printd(LogPlayer, "SetBFineTickBox_Changed\n");
	if (WeAreLooping)
		RestartPlayer = RestartPlayerValue;
}

/*-----------------------------------------------
 * Function:		SpeedSlider_Changed
 *
 * Description:		.
 *------------------------------------------------*/
void SpeedSlider_Changed(GtkAdjustment *adj) {

//	printd(LogPlayer, "SpeedSlider_Changed %f\n", gtk_adjustment_get_value(SpeedAdjustment));
	CurrentSpeed = gtk_adjustment_get_value(SpeedAdjustment);
	sprintf(PlayerString, "speed_set %f\n",
	        CurrentSpeed);
	printd(LogPlayer, "%s \n", PlayerString);

	if (!WeAreLooping)
		PlayerWrite(PlayerString);
	else
		StartPlayer();
}

/*-----------------------------------------------
 * Function:		VolumeSlider_Changed
 *
 * Description:		.
 *------------------------------------------------*/
void VolumeSlider_Changed(GtkAdjustment *adj) {
	printd(LogPlayer, "VolumeSlider_Changed %f\n",
	       gtk_adjustment_get_value(VolumeAdjustment));
	sprintf(PlayerString, "volume %f 1 \n",
	        gtk_adjustment_get_value(VolumeAdjustment));
	PlayerWrite(PlayerString);
}

/*-----------------------------------------------
 * Function:		normal_release_handler
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
gboolean SetA_click_handler(GtkWidget *widget, GdkEvent *event,
                            gpointer user_data) {
	theImageButtons *theButton;
	PlayerPoll(FALSE);
	plSetA();
	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "SetA_click_handler  %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonDownImage);

	return TRUE; /* stop event propagation */
}

/*-----------------------------------------------
 * Function:		SetB_click_handler
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
gboolean SetB_click_handler(GtkWidget *widget, GdkEvent *event,
                            gpointer user_data) {
	theImageButtons *theButton;
	PlayerPoll(FALSE);
	plSetB();

	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "SetB_click_handler %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonDownImage);
	return TRUE; /* stop event propagation */
}

/*-----------------------------------------------
 * Function:		Speed_click_handler
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
gboolean Speed_click_handler(GtkWidget *widget, GdkEvent *event,
                             gpointer user_data) {
	theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "Speed_click_handler %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonDownImage);

	gtk_adjustment_set_value(SpeedAdjustment, 1.0);

	CurrentSpeed = 1.0;
	PlayerWrite("speed_set 1.0\n");
//	gtk_adjustment_set_value(FineEndAdjustment, CurrentSongPosition - gtk_adjustment_get_value(FineStartAdjustment));

	return TRUE; /* stop event propagation */
}

/*-----------------------------------------------
 * Function:		NewLoop_click_handler
 *
 * Description:		Create a new saved Loop.
 *------------------------------------------------*/
gboolean NewLoop_click_handler(GtkWidget *widget, GdkEvent *event,
                               gpointer user_data) {
	theImageButtons *theButton;
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;
	char *entry_line;

	/*
	 * Check for errors.
	 */
	if (NumSavedLoops >= MaxSavedLoops) {
		printd(LogPlayer, "Error, too many Loops\n");
		return (FALSE);
	}
	dialog = gtk_dialog_new();
	gtk_dialog_add_button(GTK_DIALOG(dialog), "OK", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "CANCEL", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	switch (result) {
	case 0:
		entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));
		printd(LogPlayer, "Entry Value %s\n", entry_line);
		strcpy(mySavedLoops[NumSavedLoops].LoopName, entry_line);
		mySavedLoops[NumSavedLoops].Start = gtk_adjustment_get_value(
		                                        FineStartAdjustment);
		mySavedLoops[NumSavedLoops].Length = gtk_adjustment_get_value(
		        FineEndAdjustment);
		mySavedLoops[NumSavedLoops].Position = ScrollGetPosition();

		gtk_combo_box_text_append_text(GTK_COMBO_BOX(SaveCombo),
		                               mySavedLoops[NumSavedLoops].LoopName);
		printd(LogPlayer, "Reading Loop %d %s %f %f %f\n", NumSavedLoops,
		       mySavedLoops[NumSavedLoops].LoopName,
		       mySavedLoops[NumSavedLoops].Start,
		       mySavedLoops[NumSavedLoops].Length,
		       mySavedLoops[NumSavedLoops].Position);

		// gtk_combo_box_text_append_text(GTK_COMBO_BOX(SaveCombo),
		//                                mySavedLoops[NumSavedLoops].LoopName);
		NumSavedLoops++;

		SaveLoopFile(CurrentFile);
		break;

	case 1:
		break;

	default:
		break;
	}

	gtk_widget_destroy(dialog);
	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "NextSeg_click_handler %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonUpImage);


	SaveLoopFile(CurrentFile);
	return TRUE; /* stop event propagation */
}


/*-----------------------------------------------
 * Function:		NewMarker_click_handler
 *
 * Description:		Create a new saved Loop.
 *------------------------------------------------*/
gboolean NewMarker_click_handler(GtkWidget *widget, GdkEvent *event,
                                 gpointer user_data) {
	theImageButtons *theButton;
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;
	char *entry_line;
	float theTime;


	PlayerPoll(FALSE);
//	PlayerDisTime
	theTime = PlayerCurrentTime;

	dialog = gtk_dialog_new();
	gtk_dialog_add_button(GTK_DIALOG(dialog), "OK", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "CANCEL", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "NewMarker_click_handler %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonUpImage);
	switch (result) {
	case 0:
		entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));
		printd(LogPlayer, "Entry Value %s\n", entry_line);
#if 1
		strcpy(mySavedLoops[NumSavedLoops].LoopName, entry_line);
		mySavedLoops[NumSavedLoops].Start = theTime;
		mySavedLoops[NumSavedLoops].Length = -1;
		mySavedLoops[NumSavedLoops].Position = ScrollGetPosition();
		SaveLoopFile(CurrentFile);

		gtk_combo_box_text_append_text(GTK_COMBO_BOX(SaveCombo),
		                               mySavedLoops[NumSavedLoops].LoopName);
		gtk_combo_box_set_active(GTK_COMBO_BOX(SaveCombo), 1);
		NumSavedLoops++;
#else
		// Add the new tag to the list.
		strcpy(SongMarkers[NumberSongMarks].SongSection, entry_line);
		SongMarkers[NumberSongMarks++].SongMark = theTime;
#endif
		break;

	case 1:
		break;
	default:
		break;
	}

	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "NewMarker_click_handler %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonUpImage);

	gtk_widget_destroy(dialog);
	return TRUE; /* stop event propagation */
}

#if 0
/*-----------------------------------------------
 * Function:		Save SongMarkers
 *
 * Description:
 *------------------------------------------------*/
void SaveSongMarkers(char *FileName) {
	char SaveMarksName[FileNameMaxLength];
	int Loop;

	/*
	 * Let's open the Saved Looped file.
	 */
	sprintf(SaveMarksName, "%s.SongMarks", FileName);
	printd(LogPlayer, " SaveSongMarkers %s\n", SaveMarksName);
	SaveMarksFD = fopen(SaveMarksName, "w+");
	printd(LogPlayer, " SaveSongMarkers FD %d %s\n", SaveMarksFD, strerror(errno));
	if (SaveMarksFD) {
		for (Loop = 0; Loop < NumberSongMarks; Loop++) {
			fprintf(SaveMarksFD, "<meta name=\"SongMark\" content=\"%.2f,%s\"> %f->%s\n",
			        SongMarkers[Loop].SongMark,
			        SongMarkers[Loop].SongSection);
		}

		fclose(SaveMarksFD);
		SaveMarksFD = 0;
	}
}

#endif


/*-----------------------------------------------
 * Function:		EnterLoop_click_handler
 *
 * Description:		Modify a saved Loop.
 *------------------------------------------------*/
gboolean EnterLoop_click_handler(GtkWidget *widget, GdkEvent *event,
                                 gpointer user_data) {
	theImageButtons *theButton;
	int CurrentSavedLoop;

	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "EnterLoop_click_handler %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonDownImage);

	CurrentSavedLoop = gtk_combo_box_get_active(GTK_COMBO_BOX(SaveCombo));
	mySavedLoops[CurrentSavedLoop].Start = gtk_adjustment_get_value(
	        FineStartAdjustment);
	mySavedLoops[CurrentSavedLoop].Length = gtk_adjustment_get_value(
	        FineEndAdjustment);
	mySavedLoops[CurrentSavedLoop].Position = ScrollGetPosition();

	printd(LogPlayer, "Reading Loop %d %s %f %f\n", CurrentSavedLoop,
	       mySavedLoops[CurrentSavedLoop].LoopName,
	       mySavedLoops[CurrentSavedLoop].Start,
	       mySavedLoops[CurrentSavedLoop].Length);

	return TRUE; /* stop event propagation */
}

/*-----------------------------------------------
 * Function:		NextSeg_click_handler
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
gboolean NextSeg_click_handler(GtkWidget *widget, GdkEvent *event,
                               gpointer user_data) {
	theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "NextSeg_click_handler %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonDownImage);

	plNextSeg();

	return TRUE; /* stop event propagation */
}

/*-----------------------------------------------
 * Function:		PrevSeg_click_handler
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
gboolean PrevSeg_click_handler(GtkWidget *widget, GdkEvent *event,
                               gpointer user_data) {
	theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "PrevSeg_click_handler %x\n", theButton);
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonDownImage);

	plPrevSeg();
	return TRUE; /* stop event propagation */
}

/*-----------------------------------------------
 * Function:		Loop_click_handler
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
gboolean Loop_click_handler(GtkWidget *widget, GdkEvent *event,
                            gpointer user_data) {
	theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	printd(LogPlayer, "Loop %x\n", theButton);

	plLoopToggle();
	gtk_adjustment_set_value(StartAdjustment, 0.0);
	gtk_adjustment_set_value(EndAdjustment, 0.0);

	return TRUE; /* stop event propagation */
}

/*-----------------------------------------------
 * Function:		StartPlayer
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
int StartPlayer(void) {

	system("killall mplayer &>/dev/null");
	sleep(.3);
	system("killall mplayer &>/dev/null");
	sleep(.3);
	system("killall mplayer &>/dev/null");

	sleep(.3);
	system("killall mplayer &>/dev/null");

	printd(LogPlayer, "StartPlayer:After Kill\n");

#if 1
	if (OutPipe) {
		printd(LogPlayer, "About to close pipe %x\n", OutPipe);
		pclose(OutPipe);
		OutPipe = NULL;
	}
#endif
	/*
	--idle Keeps mplayer from quitting
	--quiet prevents console output
	--nocache used for seeking
	--hr-mp3-seek Helps with exact position for seek
	for stereo use two output system:playback_(21|22)
	*/

	if (WeAreLooping) {
		sprintf(PlayerString,
//				"-use-filedir-conf=./Prefs/mplayer/
		        "mplayer -nocache -ao jack:port=input_3:name=MPlayer -slave -hr-mp3-seek -fixed-vo -quiet -idle -af scaletempo -loop 0 -ss %f -endpos %f  -volume %3.1f -speed %0.2f \"%s\" >/tmp/LiveMusicIn 2>/dev/null" ,
//		        "mplayer -ao jack:port=jack-volume:name=MPlayer -slave -hr-mp3-seek -quiet -idle -af scaletempo -loop 0 -ss %f -endpos %f  -volume %3.1f -speed %0.2f \"%s\" >/tmp/LiveMusicIn &>/dev/null" ,
		        gtk_adjustment_get_value(FineStartAdjustment),
		        gtk_adjustment_get_value(FineEndAdjustment),
		        gtk_adjustment_get_value(VolumeAdjustment),
//		        gtk_adjustment_get_value(SpeedAdjustment),
		        CurrentSpeed, CurrentFile);
		printd(LogPlayer, "calling  Loop %s\n", PlayerString);
		printf("Looping %s\n", PlayerString);

	} else {
		sprintf(PlayerString,
		        "mplayer -nocache -ao jack:port=input_3:name=MPlayer -slave -hr-mp3-seek -fixed-vo -quiet -idle -af scaletempo -ss %f -volume %f -speed %0.2f -idle \"%s\" >/tmp/LiveMusicIn 2>/dev/null",
		        CurrentSongPosition,
		        gtk_adjustment_get_value(VolumeAdjustment),
		        CurrentSpeed, (char *)CurrentFile);
		printd(LogPlayer, "calling %s\n", PlayerString);
	}

	OutPipe = popen(PlayerString, "w");
	if (!OutPipe) {
		RestartPlayer = RestartPlayerValue;
	}
	printd(LogPlayer, "After Loop open %x\n", OutPipe);

	return (0);
}

/*-----------------------------------------------
 * Function:		plScrub
 *
 * Description:		Scrub
 *------------------------------------------------*/
void plScrub(float Amount) {

	sprintf(PlayerString, "seek %4.2f\n", Amount);
	PlayerWrite(PlayerString);
	printd(LogPlayer, "plScrub\n");
}

/*-----------------------------------------------
 * Function:		Play_click_handler
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
gboolean Play_click_handler(GtkWidget *widget, GdkEvent *event,
                            gpointer user_data) {
	theImageButtons *theButton;
	theButton = (theImageButtons *) user_data;
	plPausePlay();
	printd(LogPlayer, "Play_click_handler\n");
	return TRUE; /* stop event propagation */
}

/*-----------------------------------------------
 * Function:		Stop_click_handler
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
gboolean Stop_click_handler(GtkWidget *widget, GdkEvent *event,
                            gpointer user_data) {
	theImageButtons *theButton;

	theButton = (theImageButtons *) user_data;
	gtk_image_set_from_pixbuf(GTK_IMAGE(theButton->Image),
	                          theButton->ButtonDownImage);

//	PlayerWrite("stop \n");
	ResetPlayer();
	printd(LogPlayer, "Stop_click_handler\n");
	return TRUE; /* stop event propagation */
}

/*
 * Remote control Commands.
 */

/*-----------------------------------------------
 * Function:		plSetA
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
void plSetA(void) {
	gtk_adjustment_set_value(StartAdjustment, 0.0);
//	gtk_adjustment_set_value(FineStartAdjustment, 0.0);
	gtk_adjustment_set_value(FineStartAdjustment, CurrentSongPosition);
	printd(LogPlayer, "plSetA\n");
}

/*-----------------------------------------------
 * Function:		plSetB
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
void plSetB(void) {
	gtk_adjustment_set_value(EndAdjustment, 0.0);
//	gtk_adjustment_set_value(FineEndAdjustment, 0.0);
	gtk_adjustment_set_value(FineEndAdjustment,
	                         CurrentSongPosition - gtk_adjustment_get_value(FineStartAdjustment));
	printd(LogPlayer, "plSetB\n");
}

/*-----------------------------------------------
 * Function:		plLoopToggle
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
void plLoopToggle(void) {

	if (!WeAreLooping) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(LoopButton.Image),
		                          LoopButton.ButtonDownImage);
		WeAreLooping = 1;
		printd(LogPlayer, "After Close\n");
		StartPlayer();

	} else {
		gtk_image_set_from_pixbuf(GTK_IMAGE(LoopButton.Image),
		                          LoopButton.ButtonUpImage);
		WeAreLooping = 0;

		/* If we turn off loops and not playing then
		don't start.
		*/
		if (InPlayingState)
			StartPlayer();
	}
}

/*-----------------------------------------------
 * Function:		plPlay
 *
 * Description:		Start Player.
 *------------------------------------------------*/
void plPlay(void) {

	// Set current state to no-playing.
	InPlayingState = 0;
	plPausePlay();
	printd(LogPlayer, "plPlay\n");
}

/*-----------------------------------------------
 * Function:		plStop
 *
 * Description:		Stop Player.
 *------------------------------------------------*/
void plStop(void) {

	// Set current state to no-playing.
	InPlayingState = 1;
	plPausePlay();
	printd(LogPlayer, "plStop\n");
}

/*-----------------------------------------------
 * Function:		plPausePlay
 *
 * Description:		Position Sliders changed.
 *------------------------------------------------*/
void plPausePlay(void) {

	if (InPlayingState) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(PlayPause.Image),
		                          PlayPause.ButtonUpImage);
		MyImageButtonSetText(&PlayPause, "Play");
		gtk_image_set_from_pixbuf(GTK_IMAGE(PlayPauseButton.Image),
		                          PlayPauseButton.ButtonUpImage);
		MyImageButtonSetText(&PlayPauseButton, "Play");
		PlayerWrite("stop\n");
		printd(LogPlayer, "Mplayer Pause\n");
		InPlayingState = 0;
	} else {
		gtk_image_set_from_pixbuf(GTK_IMAGE(PlayPause.Image),
		                          PlayPause.ButtonDownImage);
		MyImageButtonSetText(&PlayPause, "Pause");
		gtk_image_set_from_pixbuf(GTK_IMAGE(PlayPauseButton.Image),
		                          PlayPauseButton.ButtonDownImage);
		MyImageButtonSetText(&PlayPauseButton, "Pause");

//		PlayerWrite("pause\n");
		StartPlayer();
		PlayerWrite("get_time_length\n");

		printd(LogPlayer, "Mplayer Play\n");
		InPlayingState = 1;
	}
}

/*-----------------------------------------------
 * Function:		plNextSeg
 *
 * Description:		.
 *------------------------------------------------*/
void plNextSeg(void) {
	gtk_adjustment_set_value(FineStartAdjustment,
	                         gtk_adjustment_get_value(FineStartAdjustment)
	                         + gtk_adjustment_get_value(FineEndAdjustment));

	printd(LogPlayer, "plNextSeg\n");
	if (WeAreLooping)
		RestartPlayer = RestartPlayerValue;
}

/*-----------------------------------------------
 * Function:		plPrevSeg
 *
 * Description:		.
 *------------------------------------------------*/
void plPrevSeg(void) {
	gtk_adjustment_set_value(FineStartAdjustment,
	                         gtk_adjustment_get_value(FineStartAdjustment)
	                         - gtk_adjustment_get_value(FineEndAdjustment));

	printd(LogPlayer, "plPrevSeg\n");
	if (WeAreLooping)
		RestartPlayer = RestartPlayerValue;
}

/*-----------------------------------------------
 * Function:		plSpeedUp
 *
 * Description:		.
 *------------------------------------------------*/
void plSpeedUp(void) {

	gtk_adjustment_set_value(SpeedAdjustment,
	                         gtk_adjustment_get_value(SpeedAdjustment) + 0.05);
	sprintf(PlayerString, "speed_set %f\n",
	        gtk_adjustment_get_value(SpeedAdjustment));
	printd(LogPlayer, "%s \n", PlayerString);

	if (!WeAreLooping)
		PlayerWrite(PlayerString);
	else
		StartPlayer();

}

/*-----------------------------------------------
 * Function:		plSpeedDown
 *
 * Description:		.
 *------------------------------------------------*/
void plSpeedDown(void) {

	gtk_adjustment_set_value(SpeedAdjustment,
	                         gtk_adjustment_get_value(SpeedAdjustment) - 0.05);
	sprintf(PlayerString, "speed_set %f\n",
	        gtk_adjustment_get_value(SpeedAdjustment));
	printd(LogPlayer, "%s \n", PlayerString);

	if (!WeAreLooping)
		PlayerWrite(PlayerString);
	else
		StartPlayer();

}

/*-----------------------------------------------
 * Function:		plSeekFw
 *
 * Description:		.
 *------------------------------------------------*/
void plSetPosition(float position) {
	sprintf(PlayerString, "set_property time_pos %4.2f\n", position);
	PlayerWrite(PlayerString);
	printd(LogPlayer, "plSetPosition\n");
}

/*-----------------------------------------------
 * Function:		plSeekFw
 *
 * Description:		.
 *------------------------------------------------*/
void plSeekFw(void) {

	plSetPosition(gtk_adjustment_get_value(PositionAdjustment) + 10);
	printd(LogPlayer, "plSeekFw\n");
}

/*-----------------------------------------------
 * Function:		plSeekBk
 *
 * Description:		.
 *------------------------------------------------*/
void plSeekBk(void) {

	plSetPosition(gtk_adjustment_get_value(PositionAdjustment) - 10);
	printd(LogPlayer, "plSeekBk\n");
}
